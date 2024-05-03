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

	/// folds sub-expressions that do not depend on any arguments into a single constant
	void foldConstants(Ast *);

	/// -(-(x)) => x
	void foldDoubleMinus(Ast *);

	/// a-b => a+(-b)
	void SubtractionToSum(Ast *);

	/// a+(b+c) => a+b+c
	void FlattenSum(Ast *);

	/// rebalances the AST, so calculations which require lots of space are done first.  This
	/// can sometimes reduce the total necessary amount of space on the stack.
	void compressStack(Ast *);
};

#endif // OPTIMIZATIONS_HPP_
