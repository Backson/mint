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
		default:
			is_operator = false;
			break;
		case OP_PI:
			ast->d = 3.14159265358979323846264338327950288419716939937510;
			is_operator = false;
			break;
		case OP_E:
			ast->d = 2.71828182845904523536028747135266249775724709369995;
			is_operator = false;
			break;
		case OP_NEG:
			d = - ast->children[0].d;
			break;
		case OP_INV:
			d = 1 / ast->children[0].d;
			break;
		case OP_SQ:
			d = ast->children[0].d * ast->children[0].d;
			break;
		case OP_SQRT:
			d = sqrt(ast->children[0].d);
			break;
		case OP_SIN:
			d = sin(ast->children[0].d);
			break;
		case OP_COS:
			d = cos(ast->children[0].d);
			break;
		case OP_LOG:
			d = log(ast->children[0].d);
			break;
		case OP_EXP:
			d = exp(ast->children[0].d);
			break;
		case OP_ADD:
			d = ast->children[0].d + ast->children[1].d;
			break;
		case OP_SUB:
			d = ast->children[0].d - ast->children[1].d;
			break;
		case OP_MUL:
			d = ast->children[0].d * ast->children[1].d;
			break;
		case OP_DIV:
			d = ast->children[0].d / ast->children[1].d;
			break;
		case OP_POW:
			d = pow(ast->children[0].d, ast->children[1].d);
			break;
		case OP_POWI:
			d = pow(ast->children[0].d, ast->i);
			break;
		}
		if (is_operator) {
			ast->children.clear();
			ast->op = OP_PSHC;
			ast->d = d;
		}
	};
	visitor_lambda(ast);
}

void Optimizer::optimizeDefaults(Ast *ast) {
	optimizePowersToIntegerExponents(ast);
	optimizeConstantFolding(ast);
}