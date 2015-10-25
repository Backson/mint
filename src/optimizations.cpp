// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#include "optimizations.hpp"

#include "ast.hpp"

#include <functional>

void Optimizer::optimizePowersToIntegerExponents(Ast *ast) {
	std::function<void(Ast *ast)> visitor_lambda =
	[&visitor_lambda](Ast *ast) {
		if (ast->op == OP_POW && ast->children.size() == 2) {
			Ast *child = &ast->children[1];
			if (child->op == OP_PSHC) {
				double constant = child->d;
				long i = long(constant);
				if (constant == constant && constant == i && SCHAR_MIN <= i && i <= SCHAR_MAX) {
					ast->children.pop_back();
					ast->op = OP_POWI;
					ast->i = i;
				}
			}
		}

		for (Ast &child : ast->children) {
			visitor_lambda(&child);
		}
	};
	visitor_lambda(ast);
}
