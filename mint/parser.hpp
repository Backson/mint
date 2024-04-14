// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Dammann
// See LICENSE file for details

#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "ops.hpp"
#include "tokens.hpp"
#include "ast.hpp"
#include "tokenizer.hpp"

#include <vector>

class Parser {
public:
	Parser(const char * str) : tokenizer(str) {}
	
	int parse();
	const char * getError() { return error; }
	Token getLastToken() { return tok; }

	Ast &getAst() { return ast; }

private:
	void raiseError(const char * reason);
	
	void emitOp(Op op, int i = 0);
	void emitOp(Op op, double d);
	void emitOp(Ast &ast);
	void emitOp(Ast &&ast);

	Tokenizer tokenizer;
	Token tok;
	Ast ast;
	int lastTokenId;
	int lastOp;
	const char * error = nullptr;
};

#endif
