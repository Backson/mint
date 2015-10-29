// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#ifndef OPS_HPP_
#define OPS_HPP_

enum Op {
	// program flow
	OP_HLT,   // end program execution
	OP_NOOP,  // do nothing

	// push values
	OP_CONST, // push constant on stack
	OP_ARG,   // push argument on stack

	// constants
	OP_PI,    // push pi onto the stack
	OP_E,     // push e onto the stack

	// operations with one parameter
	OP_NEG,   // negate top value
	OP_INV,   // invert top value

	OP_SQ,    // square
	OP_CU,    // cube
	OP_SQRT,  // square root
	
	OP_SIN,   // sine
	OP_COS,   // cosine
	OP_TAN,   // tangens
	OP_ASIN,  // arc sine
	OP_ACOS,  // arc cosine
	OP_ATAN,  // arc tangens
	OP_SINH,  // hyperbolic sine
	OP_COSH,  // hyperbolic cosine
	OP_TANH,  // hyperbolic tangens
	OP_ASINH, // inverse hyperbolic sine
	OP_ACOSH, // inverse hyperbolic cosine
	OP_ATANH, // inverse hyperbolic tangens

	OP_EXP,   // exponential function
	OP_LOG,   // natural logarithm

	OP_ERF,   // error function
	OP_ERFC,  // complementary error function

	OP_ABS,   // magnitude or absolute value of the given value
	OP_FLOOR, // nearest integer not greater than the given value
	OP_CEIL,  // nearest integer not less than the given value
	OP_ROUND, // nearest integer, rounding away from zero in halfway cases
	OP_TRUNC, // nearest integer not greater in magnitude than the given value

	OP_POWI,  // power to a fixed integer

	// operations with two parameters
	OP_ADD,   // add top 2 values
	OP_SUB,   // subtract
	OP_MUL,   // multiply
	OP_DIV,   // divide
	OP_POW,   // first value to the second value's power

	// contractions
	OP_FUSED_MM_L,
	OP_FUSED_MA_L,
	OP_FUSED_MS_L,
	OP_FUSED_MD_L,
	OP_FUSED_AM_L,
	OP_FUSED_AA_L,
	OP_FUSED_AS_L,
	OP_FUSED_AD_L,
	OP_FUSED_SM_L,
	OP_FUSED_SA_L,
	OP_FUSED_SS_L,
	OP_FUSED_SD_L,
	OP_FUSED_DM_L,
	OP_FUSED_DA_L,
	OP_FUSED_DS_L,
	OP_FUSED_DD_L,
	OP_FUSED_MM_R,
	OP_FUSED_MA_R,
	OP_FUSED_MS_R,
	OP_FUSED_MD_R,
	OP_FUSED_AM_R,
	OP_FUSED_AA_R,
	OP_FUSED_AS_R,
	OP_FUSED_AD_R,
	OP_FUSED_SM_R,
	OP_FUSED_SA_R,
	OP_FUSED_SS_R,
	OP_FUSED_SD_R,
	OP_FUSED_DM_R,
	OP_FUSED_DA_R,
	OP_FUSED_DS_R,
	OP_FUSED_DD_R,

	// technical nodes used by the AST
	OP_INVALID = 0x100, // invalid opcode

};

int getOperandNumber(int op);
bool isOperatorConstant(int op);
const char *getOperatorName(int op);

#endif
