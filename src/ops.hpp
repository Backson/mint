// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#ifndef OPS_HPP_
#define OPS_HPP_

enum Op {
	// program flow
	OP_HLT,  // end program execution
	OP_NOOP, // do nothing

	// push values
	OP_PSHC, // push constant on stack
	OP_PSHA, // push argument on stack

	// constants
	OP_PI,   // push pi onto the stack
	OP_E,    // push e onto the stack

	// operations with one parameter
	OP_NEG,  // negate top value
	OP_INV,  // invert top value

	OP_SQ,   // square
	OP_SQRT, // square root
	OP_SIN,  // sine
	OP_COS,  // cosine
	OP_EXP,  // exponential function
	OP_LOG,  // natural logarithm

	OP_POWI, // power to a fixed integer

	// operations with two parameters
	OP_ADD,  // add top 2 values
	OP_SUB,  // subtract
	OP_MUL,  // multiply
	OP_DIV,  // divide
	OP_POW,  // first value to the second value's power
};

#endif
