#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exec.h"
#include "memory.h"
#include "parser.h"

#define MAX_INPUT_LEN 256

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

  memory_init(&m);

  char *buf = malloc(MAX_INPUT_LEN);

  while (1) {
    fputs(">>> ", stdout);
    fgets(buf, MAX_INPUT_LEN, stdin);

    if (strcmp(buf, "exit\n") == 0)
      break;

    parse_init(&p, buf);

    while (parse_expr(&p, &m) != -1)
      ;
  }

  parse_free(&p);
  memory_free(&m);
}
