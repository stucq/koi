#ifndef LEXER_H
#define LEXER_H

#include <stdio.h>

#include "value.h"

typedef enum {
  T_EOF,

  T_PLUS,
  T_MINUS,
  T_STAR,
  T_SLASH,

  T_INTLIT,

  T_LPAREN,
  T_RPAREN,
} TokenType;

typedef struct {
  TokenType type;
  InternalValue value;

  /* debugging */
  unsigned int line;
} Token;

typedef struct {
  char *input;

  unsigned int line;
  char putback;
} Lexer;

void lexer_init(Lexer *l, char *input);
Token lexer_scan(Lexer *l);

#endif
