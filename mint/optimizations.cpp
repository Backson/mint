// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Dammann
// See LICENSE file for details

#include "optimizations.hpp"

#include "ast.hpp"
#include "impl.hpp"

#include <functional>
#include <climits>

using std::move;
using std::swap;

template <typename Visitor>
static void matchAll(Visitor visitor, Ast *ast) {
	struct AstTraversalState {
		Ast *ast;
		size_t index;
	};
	std::vector<AstTraversalState> stack;
	stack.push_back({ ast, 0 });
	while (stack.size() > 0) {
		AstTraversalState &state = stack.back();
		size_t index = state.index;
		if (index < state.ast->children.size()) {
			state.index++;
			stack.push_back({ &state.ast->children[index], 0 });
		}
		else {
			visitor(state.ast);
			stack.pop_back();
		}
	}
}

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

void Optimizer::foldConstants(Ast *ast) {
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

void Optimizer::foldDoubleMinus(Ast *ast) {
	matchAll([](Ast *ast) {
		if (ast->op != OP_NEG)
			return;
		Ast *child = &ast->children[0];
		if (child->op != OP_NEG)
			return;
		Ast *childchild = &child->children[0];
		// undefined execution order forces us to use a temp
		// otherwise childchild might get destroyed before it gets moved
		Ast tmp = move(*childchild);
		*ast = move(tmp);
	}, ast);
}

void Optimizer::SubtractionToSum(Ast *ast) {
	matchAll([](Ast *ast) {
		if (ast->op == OP_SUB && ast->children.size() == 2) {
			Ast lhs = move(ast->children[0]);
			Ast rhs(OP_NEG);
			rhs.children.emplace_back(move(ast->children[1]));
			Ast sum(OP_ADD);
			sum.children.reserve(2);
			sum.children.emplace_back(move(lhs));
			sum.children.emplace_back(move(rhs));
			*ast = move(sum);
		}
	}, ast);
}

void Optimizer::FlattenSum(Ast *ast) {
	matchAll([](Ast *ast) {
		if (ast->op == OP_ADD) {
			// TODO: As an optimization, only copy the children to temp and move back to ast,
			// if there is actually at least one child that is OP_ADD
			Ast temp(OP_ADD);
			for (Ast &child : ast->children) {
				if (child.op == OP_ADD) {
					for (Ast &grandchild : child.children) {
						temp.children.emplace_back(move(grandchild));
					}
				}
				else {
					temp.children.emplace_back(move(child));
				}
			}
			*ast = move(temp);
		}
	}, ast);
}

void Optimizer::compressStack(Ast *ast) {
	matchAll([](Ast *ast) {
		if (ast->op == OP_ADD || ast->op == OP_MUL) {
			auto &lhs = ast->children[0];
			auto &rhs = ast->children[1];
			if (lhs.stack_size_needed < rhs.stack_size_needed)
				swap(lhs, rhs);
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
