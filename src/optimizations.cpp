// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#include "optimizations.hpp"

#include "ast.hpp"
#include "impl.hpp"

#include <functional>
#include <climits>

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

void Optimizer::collapseConstants(Ast *ast) {
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
					ast->d = op0_impl<double>(ast->op);
					is_operator = false;
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

void Optimizer::collapseSigns(Ast *ast) {
	std::function<void(Ast *ast)> visitor_lambda =
	[&visitor_lambda](Ast *ast) {
		if (ast->op == OP_NEG) {
			Ast *child = &ast->children[0];
			if (child->op == OP_NEG) {
				Ast *childchild = &child->children[0];
				// undefined execution order forces us to use a temp
				// otherwise childchild might get destroyed before it gets moved
				Ast tmp = std::move(*childchild);
				*ast = std::move(tmp);
				visitor_lambda(ast);
			}
		} else {
			for (Ast &child : ast->children) {
				visitor_lambda(&child);
			}
		}
	};
	visitor_lambda(ast);
}

void Optimizer::compressStack(Ast *ast) {
	std::function<int(Ast *ast)> visitor_lambda =
	[&visitor_lambda](Ast *ast) -> int {
		std::vector<int> sizes;
		sizes.resize(ast->children.size());
		for (unsigned i = 0; i < sizes.size(); ++i) {
			sizes[i] = visitor_lambda(&ast->children[i]);
		}
		if (ast->op == OP_ADD || ast->op == OP_MUL) {
			if (sizes[0] < sizes[1]) {
				std::swap(sizes[0], sizes[1]);
				std::swap(ast->children[0], ast->children[1]);
			}
		}
		int max_size = -1;
		for (unsigned i = 0; i < sizes.size(); ++i) {
			int cur_size = sizes[i] + i;
			if (max_size < cur_size)
				max_size = cur_size;
		}
		return max_size;
	};
	visitor_lambda(ast);
}

void Optimizer::optimizeDefaults(Ast *ast) {
	optimizeAll(ast);
}

void Optimizer::optimizeAll(Ast *ast) {
	optimizePowersToIntegerExponents(ast);
	collapseConstants(ast);
	collapseSigns(ast);
	compressStack(ast);
}
