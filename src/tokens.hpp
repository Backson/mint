// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#ifndef TOKENS_HPP_
#define TOKENS_HPP_

// DON'T CHANGE THE ORDER OF THESE
enum TokenId {
	TOK_EOF,

	// technical tokens
	TOK_ARG, // arguments '$1' etc
	TOK_LIT, // literal double '0.5' etc
	TOK_IDENT, // identifier 'x' etc 

	TOK_LPAREN, // special tokens for grouping and function parameters
	TOK_RPAREN,
	TOK_COMMA,

	TOK_PLUS,
	TOK_MINUS,

	// unary operators
	TOK_UN_NEG,
	TOK_UN_PLUS,

	// constants (functions with zero arguments)
	TOK_F_PI,
	TOK_F_E,

	// functions with one argument
	TOK_F_SQRT,
	TOK_F_SIN,
	TOK_F_COS,
	TOK_F_TAN,
	TOK_F_ASIN,
	TOK_F_ACOS,
	TOK_F_ATAN,
	TOK_F_SINH,
	TOK_F_COSH,
	TOK_F_TANH,
	TOK_F_ASINH,
	TOK_F_ACOSH,
	TOK_F_ATANH,
	TOK_F_EXP,
	TOK_F_LOG,
	TOK_F_ERF,
	TOK_F_ERFC,
	TOK_F_ABS,
	TOK_F_FLOOR,
	TOK_F_CEIL,
	TOK_F_ROUND,
	TOK_F_TRUNC,

	// functions with two arguments
	TOK_F_POW,

	// infix operators
	TOK_OP_ADD,
	TOK_OP_SUB,
	TOK_OP_MUL,
	TOK_OP_DIV,
	TOK_OP_POW,

	// special tokens
	TOK_ERROR = -1,
	TOK_NONE = -2,
};

bool canBeValue(int);
bool canBePrefix(int);
bool canBeInfix(int);
bool canBeFunction(int);
bool canBeOperation(int);

int getPrecedence(int);
bool isRightAssociative(int);

unsigned char getOperator(int);

struct Token {
	TokenId id;
	const char * start;
	int pos;
	int len;
	union {
		long i;
		double d;
		const char * err;
	};
};

#endif
