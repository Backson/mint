// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#include "parser.hpp"

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <stack>

#include "ast.hpp"

enum State {
	TERM,
	START,
	DOLLAR,
	IDENT,
	CONST,
};

Token Scanner::getNextToken() {
	tok = {TOK_ERROR, nullptr, -1, -1};
	State state = START;
	while (state != TERM) {
		switch (state) {
		case START:
			// eat whitespace
			while (isblank(*next))
				++next;
			tok.start = next;
			tok.pos = next - str;
			// select next state
			if (*next == '\0') {
				tok.id = TOK_EOF;
				state = TERM;
			} else if (*next == '$') {
				++next;
				state = DOLLAR;
			} else if (*next == '(') {
				++next;
				tok.id = TOK_LPAREN;
				state = TERM;
			} else if (*next == ')') {
				++next;
				tok.id = TOK_RPAREN;
				state = TERM;
			} else if (*next == ',') {
				++next;
				tok.id = TOK_COMMA;
				state = TERM;
			} else if (*next == '+') {
				++next;
				tok.id = TOK_PLUS;
				state = TERM;
			} else if (*next == '-') {
				++next;
				tok.id = TOK_MINUS;
				state = TERM;
			} else if (*next == '*') {
				++next;
				tok.id = TOK_OP_MUL;
				state = TERM;
			} else if (*next == '/') {
				++next;
				tok.id = TOK_OP_DIV;
				state = TERM;
			} else if (*next == '^') {
				++next;
				tok.id = TOK_OP_POW;
				state = TERM;
			} else if (isalpha(*next)) {
				state = IDENT;
			} else if (isdigit(*next) || *next == '.') {
				state = CONST;
			} else {
				tok.err = "unexpected symbol";
				state = TERM;
			}
			continue;
		case DOLLAR:
			if (!isdigit(*next)) {
				tok.err = "Expected digit after '$'";
			} else {
				tok.i = strtoul(next, const_cast<char **>(&next), 10) - 1;
				tok.id = TOK_ARG;
			}
			state = TERM;
			continue;
		case IDENT:
			while (isalpha(*next))
				++next;
			if (strncmp("pi", tok.start, next - tok.start) == 0) {
				tok.id = TOK_F_PI;
			} else if (strncmp("e", tok.start, next - tok.start) == 0) {
				tok.id = TOK_F_E;
			} else if (strncmp("sqrt", tok.start, next - tok.start) == 0) {
				tok.id = TOK_F_SQRT;
			} else if (strncmp("sin", tok.start, next - tok.start) == 0) {
				tok.id = TOK_F_SIN;
			} else if (strncmp("cos", tok.start, next - tok.start) == 0) {
				tok.id = TOK_F_COS;
			} else if (strncmp("exp", tok.start, next - tok.start) == 0) {
				tok.id = TOK_F_EXP;
			} else if (strncmp("log", tok.start, next - tok.start) == 0) {
				tok.id = TOK_F_LOG;
			} else if (strncmp("pow", tok.start, next - tok.start) == 0) {
				tok.id = TOK_F_POW;
			} else if (strncmp("x", tok.start, next - tok.start) == 0) {
				tok.id = TOK_ARG;
				tok.i = 0;
			} else if (strncmp("y", tok.start, next - tok.start) == 0) {
				tok.id = TOK_ARG;
				tok.i = 1;
			} else if (strncmp("z", tok.start, next - tok.start) == 0) {
				tok.id = TOK_ARG;
				tok.i = 2;
			} else {
				tok.id = TOK_IDENT;
			}
			state = TERM;
			continue;
		case CONST:
			char * end = nullptr;
			tok.d = strtod(next, const_cast<char **>(&end));
			if (end == next) {
				tok.err = "decimal conversion failed";
			} else {
				tok.id = TOK_LIT;
				next = end;
			}
			state = TERM;
			continue;
		}
	}
	tok.len = next - tok.start;
	return tok;
}

int Parser::parse() {
	std::stack<Token> stack;
	ast.op = OP_HLT;
	ast.i = 0;
	lastTokenId = TOK_NONE;
	lastOp = -1;
	tok = scanner.getNextToken();
	while (tok.id != TOK_EOF) {
		bool expectUnary = lastTokenId == TOK_LPAREN
			|| lastTokenId == TOK_NONE
			|| canBePrefix(lastTokenId)
			|| canBeInfix(lastTokenId);
		bool expectExpr = lastTokenId == TOK_NONE
			|| lastTokenId == TOK_LPAREN
			|| canBeInfix(lastTokenId);
		switch (tok.id) {

			default:
				raiseError("unknown token");
				return 1;

			case TOK_ERROR:
				raiseError(tok.err);
				return 1;

			case TOK_PLUS:
				if (expectUnary)
					tok.id = TOK_UN_PLUS;
				else
					tok.id = TOK_OP_ADD;
				continue;

			case TOK_MINUS:
				if (expectUnary)
					tok.id = TOK_UN_NEG;
				else
					tok.id = TOK_OP_SUB;
				continue;

			case TOK_ARG:
				if (canBeValue(lastTokenId)) {
					raiseError("unexpected primary value");
					return 1;
				}
				if (tok.i > UCHAR_MAX) {
					raiseError("argument number out of range");
					return 1;
				}
				emitOp(OP_PSHA, unsigned char(tok.i));
				break;

			case TOK_LIT:
				if (canBeValue(lastTokenId)) {
					raiseError("unexpected primary value");
					return 1;
				}
				emitOp(OP_PSHC, tok.d);
				break;

			case TOK_F_PI:
				if (canBeValue(lastTokenId)) {
					raiseError("unexpected primary value");
					return 1;
				}
				emitOp(OP_PI);
				break;

			case TOK_F_E:
				if (canBeValue(lastTokenId)) {
					raiseError("unexpected primary value");
					return 1;
				}
				emitOp(OP_E);
				break;

			case TOK_UN_PLUS:
				// unary plus is a noop
				break;

			case TOK_LPAREN:
				if (!canBeFunction(lastTokenId) && !expectExpr) {
					raiseError("unexpected open parenthesis");
					return 1;
				}
				stack.push(tok);
				break;

			case TOK_F_SQRT:
			case TOK_F_SIN:
			case TOK_F_COS:
			case TOK_F_EXP:
			case TOK_F_LOG:
			case TOK_F_POW:
			case TOK_UN_NEG:
				if (canBeValue(lastTokenId)) {
					raiseError("unexpected prefix operator");
					return 1;
				}
				stack.push(tok);
				break;

			case TOK_COMMA:
				while (stack.size() > 0 && stack.top().id != TOK_LPAREN) {
					emitOp(getOperator(stack.top().id));
					stack.pop();
				}
				if (stack.size() <= 0 || stack.top().id != TOK_LPAREN) {
					raiseError("misplaced comma");
					return 1;
				}
				break;

			case TOK_RPAREN:
				while (stack.size() > 0 && stack.top().id != TOK_LPAREN) {
					emitOp(getOperator(stack.top().id));
					stack.pop();
				}
				if (stack.size() <= 0 || stack.top().id != TOK_LPAREN) {
					raiseError("mismatched parenthesis");
					return 1;
				}
				stack.pop();
				while (stack.size() > 0 && canBePrefix(stack.top().id)) {
					emitOp(getOperator(stack.top().id));
					stack.pop();
				}
				break;
			
			case TOK_OP_ADD:
			case TOK_OP_SUB:
			case TOK_OP_MUL:
			case TOK_OP_DIV:
			case TOK_OP_POW:
				if (!canBeValue(lastTokenId)) {
					raiseError("unexpected operator");
					return 1;
				}
				while (stack.size() > 0 && canBeOperation(stack.top().id)) {
					if (isRightAssociative(tok.id)) {
						if (getPrecedence(tok.id) < getPrecedence(stack.top().id)) {
							emitOp(getOperator(stack.top().id));
							stack.pop();
						} else
							break;
					} else {
						if (getPrecedence(tok.id) <= getPrecedence(stack.top().id)) {
							emitOp(getOperator(stack.top().id));
							stack.pop();
						} else
							break;
					}
				}
				stack.push(tok);
				break;
		}

		lastTokenId = tok.id;
		tok = scanner.getNextToken();
	}

	if (!canBeValue(lastTokenId)) {
		raiseError("unexpected program end");
		return 1;
	}

	while (stack.size() > 0) {
		if (stack.top().id == TOK_LPAREN || stack.top().id == TOK_RPAREN) {
			raiseError("mismatched parenthesis");
			return 1;
		} else {
			emitOp(getOperator(stack.top().id));
			stack.pop();
		}
	}
	return 0;
}

void Parser::emitOp(int op, int i) {
	Ast ast;
	ast.op = op;
	ast.i = i;
	emitOp(ast);
}

void Parser::emitOp(int op, double d) {
	Ast ast;
	ast.op = op;
	ast.d = d;
	emitOp(ast);
}

void Parser::emitOp(Ast &ast) {
	if (ast.op == OP_HLT)
		return;
	int n = getOperandNumber(ast.op);
	ast.children.resize(n);
	for (int i = 0; i < n; ++i) {
		ast.children[n - i - 1] = this->ast.children.back();
		this->ast.children.pop_back();
	}
	this->ast.children.push_back(ast);
	lastOp = ast.op;
}

void Parser::raiseError(const char * reason) {
	error = reason;
}