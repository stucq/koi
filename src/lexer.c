#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "die.h"
#include "lexer.h"

void lexer_init(Lexer *l, char *input) {
  l->input = input;

  l->line = 1;
  l->putback = 0;
}

static int next(Lexer *l) {
  int c;

  if (!l->putback) {
    c = *(l->input++);

    if (c == '\n')
      l->line++;
  } else {
    c = *(--l->input);
    l->putback = 0;
  }

  return c;
}

static int skip(Lexer *l) {
  int c;

  c = next(l);

  while (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f')
    c = next(l);

  return c;
}

static int scan_int(Lexer *l, int c) {
  int k, val = 0;

  while ((k = c - 0x30) >= 0) {
    val *= 10;
    val += k;

    c = next(l);
  }

  l->putback = 1;
  return val;
}

Token lexer_scan(Lexer *l) {
  int c = skip(l);
  Token t;

  switch (c) {
  case '\0':
    t.type = T_EOF;
    break;
  case '+':
    t.type = T_PLUS;
    break;
  case '-':
    t.type = T_MINUS;
    break;
  case '*':
    t.type = T_STAR;
    break;
  case '/':
    t.type = T_SLASH;
    break;
  case '(':
    t.type = T_LPAREN;
    break;
  case ')':
    t.type = T_RPAREN;
    break;
  default:
    if (isdigit(c)) {
      t.value.ival = scan_int(l, c);
      t.type = T_INTLIT;

      break;
    }

    char *reason = calloc(27, 1);

    if (!reason)
      die_internal(LEXER, "calloc() returned null (could not allocate string)");

    snprintf(reason, 27, "unrecognized character '%c'", c);
    die_fatal(LEXER, reason, l->line);
  }

  return t;
}
