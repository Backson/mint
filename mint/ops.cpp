// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Dammann
// See LICENSE file for details

#include "ops.hpp"

static const struct {
	Op op;
	const char *name;
	char operandNumber;
	char isConstant;
} OPERATOR_DATA_TABLE[] = {
	{ OP_HLT,   "HLT",   0, 0 },
	{ OP_NOOP,  "NOOP",  0, 0 },

	{ OP_CONST, "CONST", 0, 1 },
	{ OP_ARG,   "ARG",   0, 0 },

	{ OP_PI,    "PI",    0, 1 },
	{ OP_E,     "E",     0, 1 },

	{ OP_NEG,   "NEG",   1, 0 },
	{ OP_INV,   "INV",   1, 0 },

	{ OP_SQ,    "SQ",    1, 0 },
	{ OP_CU,    "CU",    1, 0 },
	{ OP_SQRT,  "SQRT",  1, 0 },

	{ OP_SIN,   "SIN",   1, 0 },
	{ OP_COS,   "COS",   1, 0 },
	{ OP_TAN,   "TAN",   1, 0 },
	{ OP_ASIN,  "ASIN",  1, 0 },
	{ OP_ACOS,  "ACOS",  1, 0 },
	{ OP_ATAN,  "ATAN",  1, 0 },
	{ OP_SINH,  "SINH",  1, 0 },
	{ OP_COSH,  "COSH",  1, 0 },
	{ OP_TANH,  "TANH",  1, 0 },
	{ OP_ASINH, "ASINH", 1, 0 },
	{ OP_ACOSH, "ACOSH", 1, 0 },
	{ OP_ATANH, "ATANH", 1, 0 },

	{ OP_EXP,   "EXP",   1, 0 },
	{ OP_LOG,   "LOG",   1, 0 },

	{ OP_ERF,   "ERF",   1, 0 },
	{ OP_ERFC,  "ERFC",  1, 0 },

	{ OP_ABS,   "ABS",   1, 0 },
	{ OP_FLOOR, "FLOOR", 1, 0 },
	{ OP_CEIL,  "CEIL",  1, 0 },
	{ OP_ROUND, "ROUND", 1, 0 },
	{ OP_TRUNC, "TRUNC", 1, 0 },

	{ OP_POWI,  "POWI",  1, 0 },

	{ OP_ADD,   "ADD",   2, 0 },
	{ OP_SUB,   "SUB",   2, 0 },
	{ OP_MUL,   "MUL",   2, 0 },
	{ OP_DIV,   "DIV",   2, 0 },
	{ OP_POW,   "POW",   2, 0 },

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
