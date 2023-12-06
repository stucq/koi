#include <stdio.h>
#include <stdlib.h>

#include "die.h"

unsigned int error_count = 0;

static char *to_str(CompilerPhase phase) {
  switch (phase) {
  case MAIN:
    return "main";
  case LEXER:
    return "lexer";
  case PARSER:
    return "parser";
  case EXEC:
    return "vm";
  }
}

static void die_exit_code(CompilerPhase phase, int code) {
  if (!error_count)
    return;

  fprintf(stderr,

          "\n%d error%s encountered in phase: %s\n"
          "cannot continue, exiting (code %d)...\n",

          error_count, error_count == 1 ? "" : "s", to_str(phase), code);

  exit(code);
}

void die_report(CompilerPhase phase, char *reason, int line) {
  printf("[%s] line %d: %s\n", to_str(phase), line, reason);
  error_count++;
}

void die_fatal(CompilerPhase phase, char *reason, int line) {
  die_report(phase, reason, line);
  die_exit(phase);
}

void die_internal(CompilerPhase phase, char *reason) {
  printf("[%s] internal: %s\n", to_str(phase), reason);
  error_count++;
  die_exit_code(phase, -1);
}

void die_exit(CompilerPhase phase) { die_exit_code(phase, phase + 1); }
