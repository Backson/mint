// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#ifndef PROGRAM_HPP_
#define PROGRAM_HPP_

#include <exception>
#include <vector>

class Program {
public:
	enum Optimizations {
		OPTIMIZE_NOTHING = 0,
		OPTIMIZE_MANDATORY = 1,
		OPTIMIZE_STRICT = 2,
		OPTIMIZE_PRECISE = 3,

	};
	Program(const char * src, int optimize = OPTIMIZE_STRICT);
	~Program() = default;

	void print();
	
	double run(const double *arguments);
	void run(double **arguments, double *result, size_t n);

private:
	std::vector<unsigned char> program;
	std::vector<double> constants;
	double stack[100];
};

#endif
