// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2022 Lars Dammann
// See LICENSE file for details

#ifndef TOKENIZER_HPP_
#define TOKENIZER_HPP_

#include "tokens.hpp"

class Tokenizer {
public:
	Tokenizer(const char* str) : str(str), next(str) {};

	Token getNextToken();

private:
	const char* str = nullptr;
	const char* next = nullptr;

	Token tok;
};

#endif
