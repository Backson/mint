// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#include "ast.hpp"

static void print(const Ast &ast, int indent) {
	if (indent > 0)
		printf("%*c", indent * 2, ' ');
	printf("%s", getOperatorName(ast.op));
	switch (ast.op) {
	case OP_PSHA:
	case OP_POWI:
		printf(" %d\n", ast.i);
		break;
	case OP_PSHC:
		printf(" %g\n", ast.d);
		break;
	default:
		printf("\n");
		break;
	}
	for (const Ast &a : ast.children) {
		print(a, indent + 1);
	}
}

void print(const Ast &ast) {
	print(ast, 0);
}
