// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#ifndef PROGRAM_HPP_
#define PROGRAM_HPP_

#include <exception>

class Program {
public:
	Program(const char * src);
	~Program();

	void print();

	double run(const double * arguments);

private:
	unsigned char * program = nullptr;
	double * constants = nullptr;
	double stack[100];
};

#endif
