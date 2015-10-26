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
		int i;
	};
};

void print(const Ast &ast);

#endif // AST_HPP_
