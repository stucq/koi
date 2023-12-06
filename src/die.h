#ifndef DIE_H
#define DIE_H

typedef enum {
  MAIN,
  LEXER,
  PARSER,
} CompilerPhase;

/* reports one error on the client's end */
void die_report(CompilerPhase phase, char *reason, int line);

/* reports a fatal error and calls die_exit() */
void die_fatal(CompilerPhase phase, char *reason, int line);

/* reports a fatal internal error and calls die_exit() */
void die_internal(CompilerPhase phase, char *reason);

/* exits after a fatal error is encountered or a compiler phase is completed */
void die_exit(CompilerPhase phase);

extern unsigned int error_count;

#endif
