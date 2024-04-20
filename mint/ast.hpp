// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Dammann
// See LICENSE file for details

#ifndef AST_HPP_
#define AST_HPP_

#include "ops.hpp"

#include <vector>

struct Ast {
	Op op = OP_INVALID;
	std::vector<Ast> children;
	union {
		double d;
		long i;
		char str[8];
	};
	size_t stack_size_needed = 0;

	Ast(Op op = OP_INVALID) :
		op(op)
	{
		memset(&str, 0, sizeof(str));
	}

	bool equals(const Ast &) const;
};

inline bool operator == (const Ast &lhs, const Ast &rhs) {
	return lhs.equals(rhs);
}

inline bool operator != (const Ast &lhs, const Ast &rhs) {
	return !lhs.equals(rhs);
}

void print(const Ast &ast);

#endif // AST_HPP_
