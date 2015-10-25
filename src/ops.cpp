// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#include "ops.hpp"

int getOperandNumber(int op) {
	static const int data[] = {
		0, 0, 0, 0, 0, 0,
		1, 1,
		1, 1, 1, 1, 1, 1, 1,
		2, 2, 2, 2, 2
	};
	return data[op];
}

const char *getOperatorName(int op) {
	static const char *data[] = {
		"HLT",
		"NOOP",
		"PSHC",
		"PSHA",
		"PI",
		"E",
		"NEG",
		"INV",
		"SQ",
		"SQRT",
		"SIN",
		"COS",
		"EXP",
		"LOG",
		"POWI",
		"ADD",
		"SUB",
		"MUL",
		"DIV",
		"POW",
	};
	return data[op];
}