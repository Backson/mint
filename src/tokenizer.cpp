// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2022 Lars Dammann
// See LICENSE file for details

#include "tokenizer.hpp"

#include <cctype>
#include <cstring>
#include <cstdlib>

enum State {
	TERM,
	START,
	DOLLAR,
	IDENT,
	CONST,
};

Token Tokenizer::getNextToken() {
	tok = { TOK_ERROR, nullptr, -1, -1 };
	State state = START;
	while (state != TERM) {
		switch (state) {
		case START:
			// eat whitespace
			while (std::isspace(*next))
				++next;
			tok.start = next;
			tok.pos = (int)(next - str);
			// select next state
			if (*next == '\0') {
				tok.id = TOK_EOF;
				state = TERM;
			}
			else if (*next == '$') {
				++next;
				state = DOLLAR;
			}
			else if (*next == '(') {
				++next;
				tok.id = TOK_LPAREN;
				state = TERM;
			}
			else if (*next == ')') {
				++next;
				tok.id = TOK_RPAREN;
				state = TERM;
			}
			else if (*next == ',') {
				++next;
				tok.id = TOK_COMMA;
				state = TERM;
			}
			else if (*next == '+') {
				++next;
				tok.id = TOK_PLUS;
				state = TERM;
			}
			else if (*next == '-') {
				++next;
				tok.id = TOK_MINUS;
				state = TERM;
			}
			else if (*next == '*') {
				++next;
				tok.id = TOK_OP_MUL;
				state = TERM;
			}
			else if (*next == '/') {
				++next;
				tok.id = TOK_OP_DIV;
				state = TERM;
			}
			else if (*next == '^') {
				++next;
				tok.id = TOK_OP_POW;
				state = TERM;
			}
			else if (isalpha(*next)) {
				state = IDENT;
			}
			else if (isdigit(*next) || *next == '.') {
				state = CONST;
			}
			else {
				tok.err = "unexpected symbol";
				state = TERM;
			}
			continue;
		case DOLLAR:
			if (!isdigit(*next)) {
				tok.err = "Expected digit after '$'";
			}
			else {
				tok.i = strtoul(next, const_cast<char**>(&next), 10) - 1;
				tok.id = TOK_ARG;
			}
			state = TERM;
			continue;
		case IDENT: {
			while (isalpha(*next))
				++next;

			bool (*cmp)(const char*, const char*, const char*) =
				[](const char* word, const char* start, const char* end) -> bool {
				auto diff = end - start;
				if (strlen(word) != diff)
					return false;
				return strncmp(word, start, diff) == 0;
			};

			if (cmp("pi", tok.start, next)) {
				tok.id = TOK_F_PI;
			}
			else if (cmp("e", tok.start, next)) {
				tok.id = TOK_F_E;
			}
			else if (cmp("sqrt", tok.start, next)) {
				tok.id = TOK_F_SQRT;
			}
			else if (cmp("sin", tok.start, next)) {
				tok.id = TOK_F_SIN;
			}
			else if (cmp("cos", tok.start, next)) {
				tok.id = TOK_F_COS;
			}
			else if (cmp("tan", tok.start, next)) {
				tok.id = TOK_F_TAN;
			}
			else if (cmp("arcsin", tok.start, next)) {
				tok.id = TOK_F_ASIN;
			}
			else if (cmp("arccos", tok.start, next)) {
				tok.id = TOK_F_ACOS;
			}
			else if (cmp("arctan", tok.start, next)) {
				tok.id = TOK_F_ATAN;
			}
			else if (cmp("sinh", tok.start, next)) {
				tok.id = TOK_F_SINH;
			}
			else if (cmp("cosh", tok.start, next)) {
				tok.id = TOK_F_COSH;
			}
			else if (cmp("tanh", tok.start, next)) {
				tok.id = TOK_F_TANH;
			}
			else if (cmp("arsinh", tok.start, next)) {
				tok.id = TOK_F_ASINH;
			}
			else if (cmp("arcosh", tok.start, next)) {
				tok.id = TOK_F_ACOSH;
			}
			else if (cmp("artanh", tok.start, next)) {
				tok.id = TOK_F_ATANH;
			}
			else if (cmp("exp", tok.start, next)) {
				tok.id = TOK_F_EXP;
			}
			else if (cmp("log", tok.start, next)) {
				tok.id = TOK_F_LOG;
			}
			else if (cmp("erf", tok.start, next)) {
				tok.id = TOK_F_ERF;
			}
			else if (cmp("erfc", tok.start, next)) {
				tok.id = TOK_F_ERFC;
			}
			else if (cmp("abs", tok.start, next)) {
				tok.id = TOK_F_ABS;
			}
			else if (cmp("floor", tok.start, next)) {
				tok.id = TOK_F_FLOOR;
			}
			else if (cmp("ceil", tok.start, next)) {
				tok.id = TOK_F_CEIL;
			}
			else if (cmp("round", tok.start, next)) {
				tok.id = TOK_F_ROUND;
			}
			else if (cmp("trunc", tok.start, next)) {
				tok.id = TOK_F_TRUNC;
			}
			else if (cmp("pow", tok.start, next)) {
				tok.id = TOK_F_POW;
			}
			else if (cmp("x", tok.start, next)) {
				tok.id = TOK_ARG;
				tok.i = 0;
			}
			else if (cmp("y", tok.start, next)) {
				tok.id = TOK_ARG;
				tok.i = 1;
			}
			else if (cmp("z", tok.start, next)) {
				tok.id = TOK_ARG;
				tok.i = 2;
			}
			else if (cmp("w", tok.start, next)) {
				tok.id = TOK_ARG;
				tok.i = 3;
			}
			else if (cmp("a", tok.start, next)) {
				tok.id = TOK_ARG;
				tok.i = 0;
			}
			else if (cmp("b", tok.start, next)) {
				tok.id = TOK_ARG;
				tok.i = 1;
			}
			else if (cmp("c", tok.start, next)) {
				tok.id = TOK_ARG;
				tok.i = 2;
			}
			else if (cmp("d", tok.start, next)) {
				tok.id = TOK_ARG;
				tok.i = 3;
			}
			else {
				tok.id = TOK_IDENT;
			}
			state = TERM;
			continue;
		}
		case CONST:
			char* end = nullptr;
			tok.d = strtod(next, const_cast<char**>(&end));
			if (end == next) {
				tok.err = "decimal conversion failed";
			}
			else {
				tok.id = TOK_LIT;
				next = end;
			}
			state = TERM;
			continue;
		}
	}
	tok.len = (int)(next - tok.start);
	return tok;
}
