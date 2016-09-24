// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#ifndef PROGRAM_HPP_
#define PROGRAM_HPP_

#include <exception>
#include <vector>

class Program {
public:
	Program(const char * src);
	~Program() = default;

	void print();

	double run(const double * arguments);

private:
	std::vector<unsigned char> program;
	std::vector<double> constants;
	double stack[100];
};

#endif
