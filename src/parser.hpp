// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "ops.hpp"
#include "tokens.hpp"

#include <vector>

class Scanner {
public:
	Scanner(const char * str) : str(str), next(str) {};

	Token getNextToken();

private:
	const char * str = nullptr;
	const char * next = nullptr;

	Token tok;
};

class Parser {
public:
	Parser(const char * str) : scanner(str) {}
	
	int parse();
	const char * getError() { return error; }
	Token getLastToken() { return tok; }

	const unsigned char * getProgram() { return program.data(); }
	const double * getConstants() { return constants.data(); }

	int getProgramSize() const { return program.size(); }
	int getConstantCount() const { return constants.size(); }

private:
	void raiseError(const char * reason);

	void emitOp(int op);

	Scanner scanner;
	std::vector<unsigned char> program;
	std::vector<double> constants;
	Token tok;
	int lastTokenId;
	int lastOp;
	const char * error = nullptr;
};

#endif
