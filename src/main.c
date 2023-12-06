#include <stdio.h>

#include "parser.h"

static int eval(Expr e) {
	int left, right;

	if (e.left) left = eval(*e.left);
	if (e.right) right = eval(*e.right);

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
	FILE *f = fopen("foo.koi", "r");

	Parser p;
	Expr e;

	printf("A\n"); // necessary or printf doesn't work 
	parse_init(&p, f);

	while (parse_expr(&p) != -1);

	printf("%d\n", eval(p.state.data[0]));

	parse_free(&p);
}