#include <stdio.h>

#include "exec.h"
#include "memory.h"
#include "parser.h"

static int eval(Expr e) {
  int left, right;

  if (e.left)
    left = eval(*e.left);
  if (e.right)
    right = eval(*e.right);

  switch (e.type) {
  case E_INT:
    return e.value.ival;
  case E_ADD:
    return left + right;
  case E_SUB:
    return left - right;
  case E_MULT:
    return left * right;
  case E_DIV:
    return left / right;
  case E_EXPR:
    return left;
  default:
    return -1;
  }
}

int main() {
  Parser p;
  Memory m;

  int r;

  parse_init(&p, "(1 + 2) * 3\n");
  memory_init(&m);

  while ((r = parse_expr(&p, &m)) != -1)
    ;

  parse_free(&p);
}
