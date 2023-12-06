#ifndef LEXER_H
#define LEXER_H

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
	// TODO: use char *, not FILE *
	FILE *file;

	unsigned int line;
	int putback;
} Lexer;

void lexer_init(Lexer *l, FILE *f);
Token lexer_scan(Lexer *l);
void lexer_free(Lexer *l);

#endif
