// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#include "optimizations.hpp"

#include "ast.hpp"
#include "impl.hpp"

#include <functional>

void Optimizer::optimizePowersToIntegerExponents(Ast *ast) {
	std::function<void(Ast *ast)> visitor_lambda =
	[&visitor_lambda](Ast *ast) {
		if (ast->op == OP_POW && ast->children.size() == 2) {
			Ast *child = &ast->children[1];
			if (child->op == OP_CONST) {
				double constant = child->d;
				long i = long(constant);
				if (constant == constant && constant == i && SCHAR_MIN <= i && i <= SCHAR_MAX) {
					ast->children.pop_back();
					if (i == 1) {
						ast->op = OP_NOOP;
					} else if (i == 2) {
						ast->op = OP_SQ;
					} else if (i == 3) {
						ast->op = OP_CU;
					} else {
						ast->op = OP_POWI;
						ast->i = i;
					}
				}
			}
		}

		for (Ast &child : ast->children) {
			visitor_lambda(&child);
		}
	};
	visitor_lambda(ast);
}

void Optimizer::optimizeConstantFolding(Ast *ast) {
	std::function<void(Ast *ast)> visitor_lambda =
	[&visitor_lambda](Ast *ast) {
		for (Ast &child : ast->children) {
			visitor_lambda(&child);
		}
		if (getOperandNumber(ast->op) != ast->children.size())
			return;
		bool has_non_constant_child = false;
		for (Ast &child : ast->children) {
			if (!isOperatorConstant(child.op)) {
				has_non_constant_child = true;
				break;
			}
		}
		if (has_non_constant_child)
			return;
		double d;
		bool is_operator = true;
		switch (ast->op) {
		case OP_PI:
			ast->d = pi_impl<double>();
			is_operator = false;
			break;
		case OP_E:
			ast->d = e_impl<double>();
			is_operator = false;
			break;
		case OP_POWI:
			d = pow(ast->children[0].d, ast->i);
			break;
		case OP_ARG:
		case OP_CONST:
			is_operator = false;
			break;
		default:
			switch (getOperandNumber(ast->op)) {
				case 0: {
					d = op0_impl<double>(ast->op);
					break;
				}
				case 1: {
					double x = ast->children[0].d;
					d = op1_impl(ast->op, x);
					break;
				}
				case 2: {
					double x = ast->children[0].d;
					double y = ast->children[1].d;
					d = op2_impl<double>(ast->op, x, y);
					break;
				}
			}
		} // switch (ast->op)
		if (is_operator) {
			ast->children.clear();
			ast->op = OP_CONST;
			ast->d = d;
		}
	};
	visitor_lambda(ast);
}

void Optimizer::optimizeDefaults(Ast *ast) {
	optimizePowersToIntegerExponents(ast);
	optimizeConstantFolding(ast);
}