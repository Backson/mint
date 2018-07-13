// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#include "tokens.hpp"

#include "ops.hpp"

#define LEFT  0
#define RIGHT 1
#define NONE  0

static const struct {
	int tokenId;
	char canBeValue;
	char canBePrefix;
	char canBeInfix;
	char canBeFunction;
	char canBeOperation;
	int precedence;
	char associativity;
	Op op;
} TOKEN_DATA_TABLE[] = {
	{ TOK_EOF,     0, 0, 0, 0, 0, -1, NONE,  OP_HLT },

	{ TOK_ARG,     1, 0, 0, 0, 0, -1, NONE,  OP_ARG },
	{ TOK_LIT,     1, 0, 0, 0, 0, -1, NONE,  OP_CONST },
	{ TOK_IDENT,   1, 0, 0, 0, 0, -1, NONE,  OP_NOOP },

	{ TOK_LPAREN,  0, 0, 0, 0, 0, -1, NONE,  OP_NOOP },
	{ TOK_RPAREN,  1, 0, 0, 0, 0, -1, NONE,  OP_NOOP },
	{ TOK_COMMA,   0, 0, 0, 0, 0, -1, NONE,  OP_NOOP },

	{ TOK_PLUS,    0, 0, 0, 0, 0, -1, NONE,  OP_NOOP },
	{ TOK_MINUS,   0, 0, 0, 0, 0, -1, NONE,  OP_NOOP },

	{ TOK_UN_NEG,  0, 1, 0, 0, 1,  1, RIGHT, OP_NEG },
	{ TOK_UN_PLUS, 0, 1, 0, 0, 1,  1, RIGHT, OP_NOOP },

	{ TOK_F_PI,    1, 0, 0, 1, 0, 10, NONE,  OP_PI },
	{ TOK_F_E,     1, 0, 0, 1, 0, 10, NONE,  OP_E },

	{ TOK_F_SQRT,  0, 1, 0, 1, 1, 10, RIGHT, OP_SQRT },
	{ TOK_F_SIN,   0, 1, 0, 1, 1, 10, RIGHT, OP_SIN },
	{ TOK_F_COS,   0, 1, 0, 1, 1, 10, RIGHT, OP_COS },
	{ TOK_F_TAN,   0, 1, 0, 1, 1, 10, RIGHT, OP_TAN },
	{ TOK_F_ASIN,  0, 1, 0, 1, 1, 10, RIGHT, OP_ASIN },
	{ TOK_F_ACOS,  0, 1, 0, 1, 1, 10, RIGHT, OP_ACOS },
	{ TOK_F_ATAN,  0, 1, 0, 1, 1, 10, RIGHT, OP_ATAN },
	{ TOK_F_SINH,  0, 1, 0, 1, 1, 10, RIGHT, OP_SINH },
	{ TOK_F_COSH,  0, 1, 0, 1, 1, 10, RIGHT, OP_COSH },
	{ TOK_F_TANH,  0, 1, 0, 1, 1, 10, RIGHT, OP_TANH },
	{ TOK_F_ASINH, 0, 1, 0, 1, 1, 10, RIGHT, OP_ASINH },
	{ TOK_F_ACOSH, 0, 1, 0, 1, 1, 10, RIGHT, OP_ACOSH },
	{ TOK_F_ATANH, 0, 1, 0, 1, 1, 10, RIGHT, OP_ATANH },
	{ TOK_F_EXP,   0, 1, 0, 1, 1, 10, RIGHT, OP_EXP },
	{ TOK_F_LOG,   0, 1, 0, 1, 1, 10, RIGHT, OP_LOG },
	{ TOK_F_ERF,   0, 1, 0, 1, 1, 10, RIGHT, OP_ERF },
	{ TOK_F_ERFC,  0, 1, 0, 1, 1, 10, RIGHT, OP_ERFC },
	{ TOK_F_ABS,   0, 1, 0, 1, 1, 10, RIGHT, OP_ABS },
	{ TOK_F_FLOOR, 0, 1, 0, 1, 1, 10, RIGHT, OP_FLOOR },
	{ TOK_F_CEIL,  0, 1, 0, 1, 1, 10, RIGHT, OP_CEIL },
	{ TOK_F_ROUND, 0, 1, 0, 1, 1, 10, RIGHT, OP_ROUND },
	{ TOK_F_TRUNC, 0, 1, 0, 1, 1, 10, RIGHT, OP_TRUNC },

	{ TOK_F_POW,   0, 0, 0, 1, 1, 10, NONE,  OP_POW },

	{ TOK_OP_ADD,  0, 0, 1, 0, 1,  1, LEFT,  OP_ADD },
	{ TOK_OP_SUB,  0, 0, 1, 0, 1,  1, LEFT,  OP_SUB },
	{ TOK_OP_MUL,  0, 0, 1, 0, 1,  2, LEFT,  OP_MUL },
	{ TOK_OP_DIV,  0, 0, 1, 0, 1,  2, LEFT,  OP_DIV },
	{ TOK_OP_POW,  0, 0, 1, 0, 1,  3, RIGHT, OP_POW },

	{ TOK_ERROR,   0, 0, 0, 0, 0, -1, NONE,  OP_NOOP },
	{ TOK_NONE,    0, 0, 0, 0, 0, -1, NONE,  OP_NOOP },
};

bool canBeValue(int id) {
	return TOKEN_DATA_TABLE[id].canBeValue != 0;
}

bool canBePrefix(int id) {
	return TOKEN_DATA_TABLE[id].canBePrefix != 0;
}

bool canBeInfix(int id) {
	return TOKEN_DATA_TABLE[id].canBeInfix != 0;
}

bool canBeFunction(int id) {
	return TOKEN_DATA_TABLE[id].canBeFunction != 0;
}

bool canBeOperation(int id) {
	return TOKEN_DATA_TABLE[id].canBeOperation != 0;
}

int getPrecedence(int id) {
	return TOKEN_DATA_TABLE[id].precedence;
}

bool isRightAssociative(int id) {
	return TOKEN_DATA_TABLE[id].associativity == RIGHT;
}

Op getOperator(int id) {
	return TOKEN_DATA_TABLE[id].op;
}
