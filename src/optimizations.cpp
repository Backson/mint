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
				case 3: {
					double x = ast->children[0].d;
					double y = ast->children[1].d;
					double z = ast->children[2].d;
					d = op3_impl<double>(ast->op, x, y, z);
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

void Optimizer::contract(Ast *ast) {
	matchAll([](Ast *ast) {
		Ast a, b, c;
		bool can_contract = true;
		if (ast->op == OP_MUL && ast->children[0].op == OP_MUL) {
			ast->op = OP_FUSED_MM_L;
			a = std::move(ast->children[0].children[0]);
			b = std::move(ast->children[0].children[1]);
			c = std::move(ast->children[1]);
		} else
		if (ast->op == OP_ADD && ast->children[0].op == OP_MUL) {
			ast->op = OP_FUSED_MA_L;
			a = std::move(ast->children[0].children[0]);
			b = std::move(ast->children[0].children[1]);
			c = std::move(ast->children[1]);
		} else
		if (ast->op == OP_SUB && ast->children[0].op == OP_MUL) {
			ast->op = OP_FUSED_MS_L;
			a = std::move(ast->children[0].children[0]);
			b = std::move(ast->children[0].children[1]);
			c = std::move(ast->children[1]);
		} else
		if (ast->op == OP_DIV && ast->children[0].op == OP_MUL) {
			ast->op = OP_FUSED_MD_L;
			a = std::move(ast->children[0].children[0]);
			b = std::move(ast->children[0].children[1]);
			c = std::move(ast->children[1]);
		} else
		if (ast->op == OP_MUL && ast->children[0].op == OP_ADD) {
			ast->op = OP_FUSED_AM_L;
			a = std::move(ast->children[0].children[0]);
			b = std::move(ast->children[0].children[1]);
			c = std::move(ast->children[1]);
		} else
		if (ast->op == OP_ADD && ast->children[0].op == OP_ADD) {
			ast->op = OP_FUSED_AA_L;
			a = std::move(ast->children[0].children[0]);
			b = std::move(ast->children[0].children[1]);
			c = std::move(ast->children[1]);
		} else
		if (ast->op == OP_SUB && ast->children[0].op == OP_ADD) {
			ast->op = OP_FUSED_AS_L;
			a = std::move(ast->children[0].children[0]);
			b = std::move(ast->children[0].children[1]);
			c = std::move(ast->children[1]);
		} else
		if (ast->op == OP_DIV && ast->children[0].op == OP_ADD) {
			ast->op = OP_FUSED_AD_L;
			a = std::move(ast->children[0].children[0]);
			b = std::move(ast->children[0].children[1]);
			c = std::move(ast->children[1]);
		} else
		if (ast->op == OP_MUL && ast->children[0].op == OP_SUB) {
			ast->op = OP_FUSED_SM_L;
			a = std::move(ast->children[0].children[0]);
			b = std::move(ast->children[0].children[1]);
			c = std::move(ast->children[1]);
		} else
		if (ast->op == OP_ADD && ast->children[0].op == OP_SUB) {
			ast->op = OP_FUSED_SA_L;
			a = std::move(ast->children[0].children[0]);
			b = std::move(ast->children[0].children[1]);
			c = std::move(ast->children[1]);
		} else
		if (ast->op == OP_SUB && ast->children[0].op == OP_SUB) {
			ast->op = OP_FUSED_SS_L;
			a = std::move(ast->children[0].children[0]);
			b = std::move(ast->children[0].children[1]);
			c = std::move(ast->children[1]);
		} else
		if (ast->op == OP_DIV && ast->children[0].op == OP_SUB) {
			ast->op = OP_FUSED_SD_L;
			a = std::move(ast->children[0].children[0]);
			b = std::move(ast->children[0].children[1]);
			c = std::move(ast->children[1]);
		} else
		if (ast->op == OP_MUL && ast->children[0].op == OP_DIV) {
			ast->op = OP_FUSED_DM_L;
			a = std::move(ast->children[0].children[0]);
			b = std::move(ast->children[0].children[1]);
			c = std::move(ast->children[1]);
		} else
		if (ast->op == OP_ADD && ast->children[0].op == OP_DIV) {
			ast->op = OP_FUSED_DA_L;
			a = std::move(ast->children[0].children[0]);
			b = std::move(ast->children[0].children[1]);
			c = std::move(ast->children[1]);
		} else
		if (ast->op == OP_SUB && ast->children[0].op == OP_DIV) {
			ast->op = OP_FUSED_DS_L;
			a = std::move(ast->children[0].children[0]);
			b = std::move(ast->children[0].children[1]);
			c = std::move(ast->children[1]);
		} else
		if (ast->op == OP_DIV && ast->children[0].op == OP_DIV) {
			ast->op = OP_FUSED_DD_L;
			a = std::move(ast->children[0].children[0]);
			b = std::move(ast->children[0].children[1]);
			c = std::move(ast->children[1]);
		} else
			
		if (ast->op == OP_MUL && ast->children[1].op == OP_MUL) {
			ast->op = OP_FUSED_MM_R;
			a = std::move(ast->children[0]);
			b = std::move(ast->children[1].children[0]);
			c = std::move(ast->children[1].children[1]);
		} else
		if (ast->op == OP_MUL && ast->children[1].op == OP_ADD) {
			ast->op = OP_FUSED_MA_R;
			a = std::move(ast->children[0]);
			b = std::move(ast->children[1].children[0]);
			c = std::move(ast->children[1].children[1]);
		} else
		if (ast->op == OP_MUL && ast->children[1].op == OP_SUB) {
			ast->op = OP_FUSED_MS_R;
			a = std::move(ast->children[0]);
			b = std::move(ast->children[1].children[0]);
			c = std::move(ast->children[1].children[1]);
		} else
		if (ast->op == OP_MUL && ast->children[1].op == OP_DIV) {
			ast->op = OP_FUSED_MD_R;
			a = std::move(ast->children[0]);
			b = std::move(ast->children[1].children[0]);
			c = std::move(ast->children[1].children[1]);
		} else
		if (ast->op == OP_ADD && ast->children[1].op == OP_MUL) {
			ast->op = OP_FUSED_AM_R;
			a = std::move(ast->children[0]);
			b = std::move(ast->children[1].children[0]);
			c = std::move(ast->children[1].children[1]);
		} else
		if (ast->op == OP_ADD && ast->children[1].op == OP_ADD) {
			ast->op = OP_FUSED_AA_R;
			a = std::move(ast->children[0]);
			b = std::move(ast->children[1].children[0]);
			c = std::move(ast->children[1].children[1]);
		} else
		if (ast->op == OP_ADD && ast->children[1].op == OP_SUB) {
			ast->op = OP_FUSED_AS_R;
			a = std::move(ast->children[0]);
			b = std::move(ast->children[1].children[0]);
			c = std::move(ast->children[1].children[1]);
		} else
		if (ast->op == OP_ADD && ast->children[1].op == OP_DIV) {
			ast->op = OP_FUSED_AD_R;
			a = std::move(ast->children[0]);
			b = std::move(ast->children[1].children[0]);
			c = std::move(ast->children[1].children[1]);
		} else
		if (ast->op == OP_SUB && ast->children[1].op == OP_MUL) {
			ast->op = OP_FUSED_SM_R;
			a = std::move(ast->children[0]);
			b = std::move(ast->children[1].children[0]);
			c = std::move(ast->children[1].children[1]);
		} else
		if (ast->op == OP_SUB && ast->children[1].op == OP_ADD) {
			ast->op = OP_FUSED_SA_R;
			a = std::move(ast->children[0]);
			b = std::move(ast->children[1].children[0]);
			c = std::move(ast->children[1].children[1]);
		} else
		if (ast->op == OP_SUB && ast->children[1].op == OP_SUB) {
			ast->op = OP_FUSED_SS_R;
			a = std::move(ast->children[0]);
			b = std::move(ast->children[1].children[0]);
			c = std::move(ast->children[1].children[1]);
		} else
		if (ast->op == OP_SUB && ast->children[1].op == OP_DIV) {
			ast->op = OP_FUSED_SD_R;
			a = std::move(ast->children[0]);
			b = std::move(ast->children[1].children[0]);
			c = std::move(ast->children[1].children[1]);
		} else
		if (ast->op == OP_DIV && ast->children[1].op == OP_MUL) {
			ast->op = OP_FUSED_DM_R;
			a = std::move(ast->children[0]);
			b = std::move(ast->children[1].children[0]);
			c = std::move(ast->children[1].children[1]);
		} else
		if (ast->op == OP_DIV && ast->children[1].op == OP_ADD) {
			ast->op = OP_FUSED_DA_R;
			a = std::move(ast->children[0]);
			b = std::move(ast->children[1].children[0]);
			c = std::move(ast->children[1].children[1]);
		} else
		if (ast->op == OP_DIV && ast->children[1].op == OP_SUB) {
			ast->op = OP_FUSED_DS_R;
			a = std::move(ast->children[0]);
			b = std::move(ast->children[1].children[0]);
			c = std::move(ast->children[1].children[1]);
		} else
		if (ast->op == OP_DIV && ast->children[1].op == OP_DIV) {
			ast->op = OP_FUSED_DD_R;
			a = std::move(ast->children[0]);
			b = std::move(ast->children[1].children[0]);
			c = std::move(ast->children[1].children[1]);
		} else

		{
			can_contract = false;
		}
		if (can_contract) {
			ast->children.clear();
			ast->children.push_back(std::move(a));
			ast->children.push_back(std::move(b));
			ast->children.push_back(std::move(c));
		}
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
	contract(ast);
}
