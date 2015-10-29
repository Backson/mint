// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#include "ops.hpp"

int getOperandNumber(int op) {
	static const int data[] = {
		0, 0, 0, 0, 0, 0,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		2, 2, 2, 2, 2,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
		3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	};
	return data[op];
}

bool isOperatorConstant(int op) {
	static const int data[] = {
		0, 0, 1, 0, 1, 1,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};
	return data[op] != 0;
}

const char *getOperatorName(int op) {
	static const char *data[] = {
		"HLT",
		"NOOP",
		"CONST",
		"ARG",
		"PI",
		"E",
		"NEG",
		"INV",
		"SQ",
		"CU",
		"SQRT",
		"SIN",
		"COS",
		"TAN",
		"ASIN",
		"ACOS",
		"ATAN",
		"SINH",
		"COSH",
		"TANH",
		"ASINH",
		"ACOSH",
		"ATANH",
		"EXP",
		"LOG",
		"ERF",
		"ERFC",
		"ABS",
		"FLOOR",
		"CEIL",
		"ROUND",
		"TRUNC",
		"POWI",
		"ADD",
		"SUB",
		"MUL",
		"DIV",
		"POW",
		"FMM_L",
		"FMA_L",
		"FMS_L",
		"FMD_L",
		"FAM_L",
		"FAA_L",
		"FAS_L",
		"FAD_L",
		"FSM_L",
		"FSA_L",
		"FSS_L",
		"FSD_L",
		"FDM_L",
		"FDA_L",
		"FDS_L",
		"FDD_L",
		"FMM_R",
		"FMA_R",
		"FMS_R",
		"FMD_R",
		"FAM_R",
		"FAA_R",
		"FAS_R",
		"FAD_R",
		"FSM_R",
		"FSA_R",
		"FSS_R",
		"FSD_R",
		"FDM_R",
		"FDA_R",
		"FDS_R",
		"FDD_R",
	};
	return data[op];
}
