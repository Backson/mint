// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#include "ops.hpp"

static const struct {
	Op op;
	const char *name;
	char operandNumber;
	char isConstant;
	int precedence;
} OPERATOR_DATA_TABLE[] = {
	{ OP_HLT,   "HLT",   0, 0, -1 },
	{ OP_NOOP,  "NOOP",  0, 0, -1 },

	{ OP_CONST, "CONST", 0, 1, 99 },
	{ OP_ARG,   "ARG",   0, 0, 99 },

	{ OP_PI,    "PI",    0, 1, 99 },
	{ OP_E,     "E",     0, 1, 99 },

	{ OP_NEG,   "NEG",   1, 0, 1 },
	{ OP_INV,   "INV",   1, 0, 2 },

	{ OP_SQ,    "SQ",    1, 0, 3 },
	{ OP_CU,    "CU",    1, 0, 3 },
	{ OP_SQRT,  "SQRT",  1, 0, 10 },

	{ OP_SIN,   "SIN",   1, 0, 10 },
	{ OP_COS,   "COS",   1, 0, 10 },
	{ OP_TAN,   "TAN",   1, 0, 10 },
	{ OP_ASIN,  "ASIN",  1, 0, 10 },
	{ OP_ACOS,  "ACOS",  1, 0, 10 },
	{ OP_ATAN,  "ATAN",  1, 0, 10 },
	{ OP_SINH,  "SINH",  1, 0, 10 },
	{ OP_COSH,  "COSH",  1, 0, 10 },
	{ OP_TANH,  "TANH",  1, 0, 10 },
	{ OP_ASINH, "ASINH", 1, 0, 10 },
	{ OP_ACOSH, "ACOSH", 1, 0, 10 },
	{ OP_ATANH, "ATANH", 1, 0, 10 },

	{ OP_EXP,   "EXP",   1, 0, 10 },
	{ OP_LOG,   "LOG",   1, 0, 10 },

	{ OP_ERF,   "ERF",   1, 0, 10 },
	{ OP_ERFC,  "ERFC",  1, 0, 10 },

	{ OP_ABS,   "ABS",   1, 0, 10 },
	{ OP_FLOOR, "FLOOR", 1, 0, 10 },
	{ OP_CEIL,  "CEIL",  1, 0, 10 },
	{ OP_ROUND, "ROUND", 1, 0, 10 },
	{ OP_TRUNC, "TRUNC", 1, 0, 10 },

	{ OP_POWI,  "POWI",  1, 0, 3 },

	{ OP_ADD,   "ADD",   2, 0, 1 },
	{ OP_SUB,   "SUB",   2, 0, 1 },
	{ OP_MUL,   "MUL",   2, 0, 2 },
	{ OP_DIV,   "DIV",   2, 0, 2 },
	{ OP_POW,   "POW",   2, 0, 3 },

	{ OP_INVALID, "", 0, 0 },
};

int getOperandNumber(int op) {
	return OPERATOR_DATA_TABLE[op].operandNumber;
}

bool isOperatorConstant(int op) {
	return OPERATOR_DATA_TABLE[op].isConstant != 0;
}

const char *getOperatorName(int op) {
	return OPERATOR_DATA_TABLE[op].name;
}

int getOperatorPrecedence(int op) {
	return OPERATOR_DATA_TABLE[op].precedence;
}
