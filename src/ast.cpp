// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#include "ast.hpp"

#include <cstdio>

static void printTree(const Ast &ast, int indent) {
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
		printTree(a, indent + 1);
	}
}

void printTree(const Ast &ast) {
	printTree(ast, 0);
}

static void printExprInternal(const Ast &ast) {
	switch (ast.op) {
	case OP_HLT:
		if (ast.children.size() > 0)
			printExprInternal(ast.children[0]);
		break;
		//case OP_NOOP:
	case OP_CONST:
		printf("%g", ast.d);
		break;
	case OP_ARG:
		if (ast.i == 0)
			printf("x");
		else if (ast.i == 1)
			printf("y");
		else if (ast.i == 2)
			printf("z");
		else if (ast.i == 3)
			printf("w");
		else
			printf("$%d", ast.i);
		break;
	case OP_PI:
		printf("pi");
		break;
	case OP_E:
		printf("e");
		break;
	case OP_NEG:
		printf("-(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_INV:
		printf("1/(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_SQ:
		printf("(");
		printExprInternal(ast.children[0]);
		printf(")^2");
		break;
	case OP_CU:
		printf("(");
		printExprInternal(ast.children[0]);
		printf(")^3");
		break;
	case OP_SQRT:
		printf("sqrt(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_SIN:
		printf("sin(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_COS:
		printf("cos(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_TAN:
		printf("tan(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_ASIN:
		printf("asin(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_ACOS:
		printf("acos(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_ATAN:
		printf("atan(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_SINH:
		printf("sinh(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_COSH:
		printf("cosh(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_TANH:
		printf("tanh(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_ASINH:
		printf("asinh(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_ACOSH:
		printf("acosh(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_ATANH:
		printf("atanh(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_EXP:
		printf("exp(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_LOG:
		printf("log(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_ERF:
		printf("erf(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_ERFC:
		printf("erfc(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_ABS:
		printf("abs(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_FLOOR:
		printf("floor(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_CEIL:
		printf("ceil(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_ROUND:
		printf("round(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_TRUNC:
		printf("trunc(");
		printExprInternal(ast.children[0]);
		printf(")");
		break;
	case OP_POWI:
		printf("(");
		printExprInternal(ast.children[0]);
		printf(")^%d", ast.i);
		break;
	case OP_ADD:
		printf("(");
		printExprInternal(ast.children[0]);
		printf(")+(");
		printExprInternal(ast.children[1]);
		printf(")");
		break;
	case OP_SUB:
		printf("(");
		printExprInternal(ast.children[0]);
		printf(")-(");
		printExprInternal(ast.children[1]);
		printf(")");
		break;
	case OP_MUL:
		printf("(");
		printExprInternal(ast.children[0]);
		printf(")*(");
		printExprInternal(ast.children[1]);
		printf(")");
		break;
	case OP_DIV:
		printf("(");
		printExprInternal(ast.children[0]);
		printf(")/(");
		printExprInternal(ast.children[1]);
		printf(")");
		break;
	case OP_POW:
		printf("(");
		printExprInternal(ast.children[0]);
		printf(")^(");
		printExprInternal(ast.children[1]);
		printf(")");
		break;
	default:
		printf("\nError: Invalid operator.");
		break;
	}
}

void printExpr(const Ast &ast) {
	printExprInternal(ast);
	printf("\n");
}
