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

	~Ast() = default;

	Ast(const Ast &other)
	{
		op = other.op;
		children = other.children;
		memcpy(&str, &other.str, sizeof(str));
		stack_size_needed = other.stack_size_needed;
	}

	Ast &operator=(const Ast &other)
	{
		op = other.op;
		children = other.children;
		memcpy(&str, &other.str, sizeof(str));
		stack_size_needed = other.stack_size_needed;
		return *this;
	}

	Ast(Ast &&other) noexcept
	{
		op = other.op;
		children = move(other.children);
		memcpy(&str, &other.str, sizeof(str));
		stack_size_needed = other.stack_size_needed;
	}

	Ast &operator=(Ast &&other) noexcept
	{
		op = other.op;
		children = move(other.children);
		memcpy(&str, &other.str, sizeof(str));
		stack_size_needed = other.stack_size_needed;
		return *this;
	}
};



void print(const Ast &ast);

#endif // AST_HPP_
