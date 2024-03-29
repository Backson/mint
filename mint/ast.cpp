// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Dammann
// See LICENSE file for details

#include "ast.hpp"

#include <cstdio>

static void print(const Ast &ast, int indent) {
	if (indent > 0)
		printf("%*c", indent * 2, ' ');
	printf("%s", getOperatorName(ast.op));
	switch (ast.op) {
	case OP_ARG:
	case OP_POWI:
		printf(" %d\n", ast.i);
		break;
	case OP_CONST:
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
