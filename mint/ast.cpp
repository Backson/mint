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

bool Ast::equals(const Ast &other) const
{
	if (this == &other)
		return true;
	if (op != other.op)
		return false;
	if (children.size() != other.children.size())
		return false;
	if (memcmp(&str, &other.str, sizeof(str)) != 0)
		return false;
	for (size_t i = 0; i < children.size(); ++i) {
		if (!children[i].equals(other.children[i]))
			return false;
	}
	return true;
}

void print(const Ast &ast) {
	print(ast, 0);
}
