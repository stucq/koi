#include "exec.h"
#include "die.h"
#include "parser.h"

int exec(Expr e) {
  int left, right;

  if (e.left)
    left = exec(*e.left);
  if (e.right)
    right = exec(*e.right);

  switch (e.type) {
  case E_INT:
    printf("%d", e.value.ival);
  case E_ADD:
    printf("%d", left + right);
  case E_SUB:
    printf("%d", left - right);
  case E_MULT:
    printf("%d", left * right);
  case E_DIV:
    printf("%d", left / right);
  case E_EXPR:
    printf("%d", left);
  default:
    die_fatal(EXEC, "unrecognized token", e.line);
    return -1;
  }

  return 0;
}
