// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Dammann
// See LICENSE file for details

#include "parser.hpp"

#include <stack>

#include "ast.hpp"
#include "ops.hpp"

int Parser::parse() {
	std::stack<Token> stack;
	ast.op = OP_HLT;
	ast.i = 0;
	lastTokenId = TOK_NONE;
	lastOp = -1;
	tok = tokenizer.getNextToken();
	while (tok.id != TOK_EOF) {
		bool expectUnary = lastTokenId == TOK_LPAREN
			|| lastTokenId == TOK_NONE
			|| canBePrefix(lastTokenId)
			|| canBeInfix(lastTokenId);
		bool expectExpr = lastTokenId == TOK_NONE
			|| lastTokenId == TOK_LPAREN
			|| canBePrefix(lastTokenId)
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
				emitOp(OP_ARG, (unsigned char)(tok.i));
				break;

			case TOK_LIT:
				if (canBeValue(lastTokenId)) {
					raiseError("unexpected primary value");
					return 1;
				}
				emitOp(OP_CONST, tok.d);
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
			case TOK_F_TAN:
			case TOK_F_ASIN:
			case TOK_F_ACOS:
			case TOK_F_ATAN:
			case TOK_F_SINH:
			case TOK_F_COSH:
			case TOK_F_TANH:
			case TOK_F_ASINH:
			case TOK_F_ACOSH:
			case TOK_F_ATANH:
			case TOK_F_EXP:
			case TOK_F_LOG:
			case TOK_F_ERF:
			case TOK_F_ERFC:
			case TOK_F_ABS:
			case TOK_F_FLOOR:
			case TOK_F_CEIL:
			case TOK_F_ROUND:
			case TOK_F_TRUNC:
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
		tok = tokenizer.getNextToken();
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

void Parser::emitOp(Op op, int i) {
	Ast &&ast = Ast::create(op);
	ast.i = i;
	emitOp(ast);
}

void Parser::emitOp(Op op, double d) {
	Ast &&ast = Ast::create(op);
	ast.d = d;
	emitOp(ast);
}

void Parser::emitOp(Ast &ast) {
	Ast copy = ast;
	emitOp(std::move(copy));
}

void Parser::emitOp(Ast &&ast) {
	ast.stack_size_needed = 0;
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
