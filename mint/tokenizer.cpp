// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2022 Lars Dammann
// See LICENSE file for details

#include "tokenizer.hpp"

#include <cctype>
#include <cstring>
#include <cstdlib>

static bool cmp(const char *word, const char *start, const char *end) {
	auto diff = end - start;
	if (strlen(word) != diff)
		return false;
	return strncmp(word, start, diff) == 0;
};

static void ident_helper(Token &tok, const char *next) {
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
}

static inline bool is_digit(char c) {
	return std::isdigit(c);
}

static inline bool is_alpha(char c) {
	return std::isalpha(c);
}

static inline bool is_space(char c) {
	return std::isspace(c);
}

static inline unsigned long parse_ulong(const char *begin, const char **end) {
	return std::strtoul(begin, (char **)end, 10);
}

static inline long parse_long(const char *begin, const char **end) {
	return std::strtol(begin, (char **)end, 10);
}

static inline double parse_double(const char *begin, const char **end) {
	return std::strtod(begin, (char **)end);
}

Token Tokenizer::getNextToken() {
	tok = { TOK_ERROR, nullptr, -1, -1 };

	// eat whitespace
	while (is_space(*next))
		++next;

	// start new token
	tok.start = next;
	tok.pos = (int)(next - str);

	// inspect first character of the token
	if (*next == '\0') {
		tok.id = TOK_EOF;
	}
	else if (*next == '$') {
		++next;
		if (!is_digit(*next)) {
			tok.err = "Expected digit after '$'";
		}
		else {
			tok.i = parse_ulong(next, &next) - 1;
			tok.id = TOK_ARG;
		}
	}
	else if (*next == '(') {
		++next;
		tok.id = TOK_LPAREN;
	}
	else if (*next == ')') {
		++next;
		tok.id = TOK_RPAREN;
	}
	else if (*next == ',') {
		++next;
		tok.id = TOK_COMMA;
	}
	else if (*next == '+') {
		++next;
		tok.id = TOK_PLUS;
	}
	else if (*next == '-') {
		++next;
		tok.id = TOK_MINUS;
	}
	else if (*next == '*') {
		++next;
		tok.id = TOK_OP_MUL;
	}
	else if (*next == '/') {
		++next;
		tok.id = TOK_OP_DIV;
	}
	else if (*next == '^') {
		++next;
		tok.id = TOK_OP_POW;
	}
	else if (is_alpha(*next)) {
		// consume all alphanumeric characters
		++next;
		while (is_alpha(*next))
			++next;

		// check if this is a known identifier, like "cos" or "pi"
		ident_helper(tok, next);
	}
	else if (is_digit(*next) || *next == '.') {
		const char *end = nullptr;
		tok.d = parse_double(next, &end);
		if (end == nullptr || end == next) {
			tok.err = "decimal conversion failed";
		}
		else {
			tok.id = TOK_LIT;
			next = end;
		}
	}
	else {
		tok.err = "unexpected symbol";
	}

	tok.len = (int)(next - tok.start);
	return tok;
}
