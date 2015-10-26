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
	
	void optimizePowersToIntegerExponents(Ast *);
	void collapseConstants(Ast *);
	void collapseSigns(Ast *);
	void compressStack(Ast *);

	void optimizeDefaults(Ast *);
	void optimizeAll(Ast *);
private:

};

#endif // OPTIMIZATIONS_HPP_
