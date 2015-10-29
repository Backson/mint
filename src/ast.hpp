// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#ifndef AST_HPP_
#define AST_HPP_

#include "ops.hpp"

#include <vector>

struct Ast {
	Op op;
	std::vector<Ast> children;
	union {
		double d;
		long i;
		char str[8];
	};
	size_t stack_size_needed;

	static inline Ast create(Op op = OP_INVALID) {
		Ast result;
		memset(&result, 0, sizeof(Ast));
		result.op = op;
		result.stack_size_needed = 0;
		return result;
	}
};



void print(const Ast &ast);

#endif // AST_HPP_
