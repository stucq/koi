#include "exec.h"
#include "die.h"
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

void exec(Memory *m, Expr e) {
  int res = eval(e);

  if (res == -1)
    die_fatal(EXEC, "unrecognized token", e.line);
  else
    printf("%d\n", res);
}
