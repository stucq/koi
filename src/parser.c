#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "die.h"
#include "lexer.h"
#include "parser.h"
#include "value.h"

static void expr_array_init(ExprArray *arr) {
  arr->len = 0;

  arr->capacity = 16;
  arr->data = malloc(arr->capacity * sizeof(Expr));

  if (arr->data == NULL)
    die_internal(PARSER,
                 "malloc() returned null (could not allocate data for parser)");

  arr->parsing = 0;
}

static void expr_array_resize(ExprArray *arr) {
  arr->capacity *= 2;
  arr->data = realloc(arr->data, arr->capacity * sizeof(Expr));

  if (arr->data == NULL)
    die_internal(
        PARSER, "realloc() returned null (could not allocate data for parser)");
}

static void expr_array_remove(ExprArray *arr) {
  if (arr->len == 1 || arr->parsing)
    return;

  // TODO do something with arr->data[0]
  memmove(arr->data, &arr->data[1], --arr->len * sizeof(Expr));
}

static void expr_array_insert(ExprArray *arr, Expr *e, unsigned int line) {
  e->line = line;

  if (arr->len == arr->capacity)
    expr_array_resize(arr);

  arr->data[arr->len++] = *e;
  expr_array_remove(arr);
}

static void expr_array_free(ExprArray *arr) {
  for (int i = 0; i < arr->len; i++) {
    Expr e = arr->data[i];

    free(e.left);
    free(e.right);
  }

  free(arr->data);
}

static Expr *expr(ExprType type, InternalValue value, Expr *left, Expr *right) {
  Expr *e = malloc(sizeof(Expr));

  e->type = type;
  e->value = value;

  e->left = left;
  e->right = right;

  return e;
}

static Expr *binary(ExprType type, Expr *left, Expr *right) {
  InternalValue v;
  return expr(type, v, left, right);
}

static Expr *unary(ExprType type, InternalValue value, Expr *left) {
  return expr(type, value, left, NULL);
}

static Expr *unary_empty(ExprType type, Expr *left) {
  InternalValue v;
  return expr(type, v, left, NULL);
}

static Expr *leaf(ExprType type, InternalValue value) {
  return expr(type, value, NULL, NULL);
}

static Expr *leaf_empty(ExprType type) {
  InternalValue v;
  return expr(type, v, NULL, NULL);
}

void parse_init(Parser *p, FILE *f) {
  lexer_init(&p->lexer, f);
  expr_array_init(&p->state);
}

static Expr *expr_clone(Expr *e) {
  Expr *lhs = NULL;
  Expr *rhs = NULL;

  if (e->left)
    lhs = expr_clone(e->left);
  if (e->right)
    rhs = expr_clone(e->right);

  return expr(e->type, e->value, lhs, rhs);
}

static void parse_expr_binary(Parser *p, ExprType type) {
  p->state.parsing = 1;

  if (p->state.len == 0) {
    // TODO: handle unary operators

    die_report(PARSER, "no left hand side", p->lexer.line);
    return;
  }

  // This marks the copy of lhs in p->state.data as irrelevant...
  Expr *lhs = expr_clone(&p->state.data[--p->state.len]);

  /* in an expression, this will be set to the following:
   *   0 if lhs evaluates before rhs (e.g. 1 * 2 + 3)
   *   1 if rhs evaluates before lhs (e.g. 1 + 2 * 3) */
  char prec = type > lhs->type;

  // ...meaning it will be overwritten by rhs, which is read here...
  if (parse_expr(p) != 0) {
    die_report(PARSER, "no right hand side", p->lexer.line);
    return;
  }

  // ...which will be itself overwritten by the new expression.
  Expr *rhs = expr_clone(&p->state.data[--p->state.len]);

  if (prec) {
    rhs = binary(type, lhs->right, rhs);
    expr_array_insert(&p->state, binary(lhs->type, lhs->left, rhs),
                      p->lexer.line);
  }

  else
    expr_array_insert(&p->state, binary(type, lhs, rhs), p->lexer.line);

  p->state.parsing = 0;
}

static void parse_expr_nested(Parser *p) {
  p->state.parsing = 1;

  int res;
  int pos = p->state.len;

  while ((res = parse_expr(p)) == 0)
    ;

  if (res != T_RPAREN)
    die_report(PARSER, "syntax error: wrong closing token", p->lexer.line);

  int len = p->state.len - pos;
  p->state.len = pos;

  Expr *sequence = realloc(&p->state.data[pos], sizeof(Expr) * len);
  if (!sequence)
    die_internal(PARSER, "realloc() returned null (could not copy data)");

  for (int i = 0; i < len; i++) {
    Expr *src = expr_clone(&p->state.data[pos + i]);
    memcpy(&sequence[i], src, sizeof(Expr));
  }

  Expr *e = unary(E_EXPR, ival(len), sequence);
  expr_array_insert(&p->state, expr_clone(e), p->lexer.line);

  p->state.parsing = 0;
}

/* RETURN VALUE
 * ------------
 * 0 - OK
 * -1 - EOF
 *
 * otherwise (n): end block with token t
 */
int parse_expr(Parser *p) {
  Token t = lexer_scan(&p->lexer);

  switch (t.type) {
  case T_EOF:
    expr_array_insert(&p->state, leaf_empty(E_EOF), p->lexer.line);
    return -1;

  case T_INTLIT:
    expr_array_insert(&p->state, leaf(E_INT, t.value), p->lexer.line);
    break;

  case T_PLUS:
    parse_expr_binary(p, E_ADD);
    break;

  case T_MINUS:
    parse_expr_binary(p, E_SUB);
    break;

  case T_STAR:
    parse_expr_binary(p, E_MULT);
    break;

  case T_SLASH:
    parse_expr_binary(p, E_DIV);
    break;

  case T_LPAREN:
    parse_expr_nested(p);
    break;

  case T_RPAREN: // TODO: add other closing tokens
    return t.type;

  default:
    die_report(PARSER, "syntax error: unexpected token", p->lexer.line);
  }

  return 0;
}

void parse_free(Parser *p) {
  lexer_free(&p->lexer);
  expr_array_free(&p->state);
  die_exit(PARSER);
}
