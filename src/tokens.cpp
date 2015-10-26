// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#include "tokens.hpp"

#include "ops.hpp"

bool canBeValue(int id) {
	static const char data[] = {
		0, 1, 1, 1, 0, 1, 0, 0, 0,
		0, 0,
		1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		0, 0, 0, 0, 0,
		0, 0
	};
	return data[id] == 1;
}

bool canBePrefix(int id) {
	static const char data[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1,
		0, 0,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		0,
		0, 0, 0, 0, 0,
		0, 0
	};
	return data[id] == 1;
}

bool canBeInfix(int id) {
	static const char data[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0,
		0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0,
		1, 1, 1, 1, 1,
		0, 0
	};
	return data[id] == 1;
}

bool canBeFunction(int id) {
	static const char data[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0,
		1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1,
		0, 0, 0, 0, 0,
		0, 0
	};
	return data[id] == 1;
}

bool canBeOperation(int id) {
	static const char data[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0,
		1, 1,
		0, 0,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1,
		1, 1, 1, 1, 1,
		0, 0
	};
	return data[id] == 1;
}

int getPrecedence(int id) {
	static const char data[] = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1,
		10, 10,
		10, 10,
		10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
		10,
		1, 1, 2, 2, 3,
		-1, -1
	};
	return data[id];
}

bool isRightAssociative(int id) {
	if (canBePrefix(id))
		return true;
	switch (id) {
		case TOK_OP_POW:
			return true;
		default:
			return false;
	}
}

Op getOperator(int id) {
	static const Op data[] {
		OP_HLT,   // TOK_EOF
		OP_ARG,   // TOK_ARG
		OP_CONST, // TOK_LIT
		OP_NOOP,  // TOK_IDENT
		OP_NOOP,  // TOK_LPAREN
		OP_NOOP,  // TOK_RPAREN
		OP_NOOP,  // TOK_COMMA
		OP_NOOP,  // TOK_PLUS
		OP_NOOP,  // TOK_MINUS
		OP_NEG,   // TOK_UN_NEG,
		OP_NOOP,  // TOK_UN_PLUS
		OP_PI,    // TOK_F_PI
		OP_E,     // TOK_F_E
		OP_SQRT,  // TOK_F_SQRT
		OP_SIN,   // TOK_F_SIN
		OP_COS,   // TOK_F_COS
		OP_TAN,   // TOK_F_TAN
		OP_ASIN,  // TOK_F_ASIN
		OP_ACOS,  // TOK_F_ACOS
		OP_ATAN,  // TOK_F_ATAN
		OP_SINH,  // TOK_F_SINH
		OP_COSH,  // TOK_F_COSH
		OP_TANH,  // TOK_F_TANH
		OP_ASINH, // TOK_F_ASINH
		OP_ACOSH, // TOK_F_ACOSH
		OP_ATANH, // TOK_F_ATANH
		OP_EXP,   // TOK_F_EXP
		OP_LOG,   // TOK_F_LOG
		OP_ERF,   // TOK_F_ERF
		OP_ERFC,  // TOK_F_ERFC
		OP_ABS,   // TOK_F_ABS
		OP_FLOOR, // TOK_F_FLOOR
		OP_CEIL,  // TOK_F_CEIL
		OP_ROUND, // TOK_F_ROUND
		OP_TRUNC, // TOK_F_TRUNC
		OP_POW,   // TOK_F_POW
		OP_ADD,   // TOK_OP_ADD
		OP_SUB,   // TOK_OP_SUB
		OP_MUL,   // TOK_OP_MUL
		OP_DIV,   // TOK_OP_DIV
		OP_POW,   // TOK_OP_POW
		OP_NOOP,  // TOK_ERROR
		OP_NOOP,  // TOK_NONE
	};
	return data[id];
}
