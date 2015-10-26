// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#ifndef OPTIMIZATIONS_HPP_
#define OPTIMIZATIONS_HPP_

struct Ast;

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

	void optimizeDefaults(Ast *);
	void optimizeAll(Ast *);
private:

};

#endif // OPTIMIZATIONS_HPP_
