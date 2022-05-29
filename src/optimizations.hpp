// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Dammann
// See LICENSE file for details

#ifndef OPTIMIZATIONS_HPP_
#define OPTIMIZATIONS_HPP_

#include "ast.hpp"

#include <vector>

class Optimizer {
public:
	Optimizer() = default;
	~Optimizer() = default;
	
	/// Substitute calls to pow(double,double) with cheaper pow(double,int)
	void optimizePowersToIntegerExponents(Ast *);
	/// collapses sub-expressions that do not depend on any arguments into a single constant
	void collapseConstants(Ast *);
	/// -(-(x)) -> x
	void collapseSigns(Ast *);
	/// rebalances the AST, so calculations which require lots of space are done first.  This
	/// can sometimes reduce the total necessary amount of space on the stack.
	void compressStack(Ast *);
private:
	template <typename Visitor>
	void matchAll(Visitor visitor, Ast *ast) {
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
			} else {
				visitor(state.ast);
				stack.pop_back();
			}
		}
	}
};

#endif // OPTIMIZATIONS_HPP_
