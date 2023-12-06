#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>

#include "lexer.h"
#include "value.h"

typedef enum {
	E_EOF,

	E_ADD,
	E_SUB,
	E_MULT,
	E_DIV,

	E_EXPR,

	E_INT,
} ExprType;

struct Expr {
	ExprType type;

	struct Expr *left;
	struct Expr *right;

	InternalValue value;
};

typedef struct Expr Expr;

typedef struct {
	unsigned int len;
	unsigned int capacity;
	Expr *data;

	/**
	 * 0: not in an expression, new expression is executed
	 * 1: in an expression, new expression will be executed later
	 */
	int parsing;
} ExprArray;

typedef struct {
	Lexer lexer;
	ExprArray state;
} Parser;

void parse_init(Parser *p, FILE *f);
int parse_expr(Parser *p);
void parse_free(Parser *p);

#endif
