// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#include "optimizations.hpp"

#include "ast.hpp"
#include "impl.hpp"

#include <functional>
#include <climits>

void Optimizer::optimizePowersToIntegerExponents(Ast *ast) {
	matchAll([](Ast *ast) {
		if (ast->op != OP_POW || ast->children.size() != 2)
			return;
		Ast *exponent_node = &ast->children[1];
		if (exponent_node->op == OP_CONST) {
			double exponent = exponent_node->d;
			if (exponent != exponent)
				return;
			long i = long(exponent);
			if (exponent == (double)i && SCHAR_MIN <= i && i <= SCHAR_MAX) {
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
	}, ast);
}

void Optimizer::collapseConstants(Ast *ast) {
	matchAll([](Ast *ast) {
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
	}, ast);
}

void Optimizer::collapseSigns(Ast *ast) {
	matchAll([](Ast *ast) {
		if (ast->op != OP_NEG)
			return;
		Ast *child = &ast->children[0];
		if (child->op != OP_NEG)
			return;
		Ast *childchild = &child->children[0];
		// undefined execution order forces us to use a temp
		// otherwise childchild might get destroyed before it gets moved
		Ast tmp = std::move(*childchild);
		*ast = std::move(tmp);
	}, ast);
}

void Optimizer::compressStack(Ast *ast) {
	matchAll([](Ast *ast) {
		if (ast->op == OP_ADD || ast->op == OP_MUL) {
			auto &lhs = ast->children[0];
			auto &rhs = ast->children[1];
			if (lhs.stack_size_needed < rhs.stack_size_needed)
				std::swap(lhs, rhs);
		}
		size_t max_size = 1;
		for (unsigned i = 0; i < ast->children.size(); ++i) {
			auto &child = ast->children[i];
			size_t cur_size = child.stack_size_needed + i;
			if (max_size < cur_size)
				max_size = cur_size;
		}
		ast->stack_size_needed = max_size;
	}, ast);
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
