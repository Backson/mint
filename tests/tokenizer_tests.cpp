#include <gtest/gtest.h>

#include "tokenizer.hpp"

TEST(TokenizerTests, Empty) {
	Tokenizer tokenizer("");
	Token tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_EOF, tok.id);
}

TEST(TokenizerTests, OnlyWhitespace) {
	Tokenizer tokenizer("  \t  ");
	Token tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_EOF, tok.id);
}

TEST(TokenizerTests, IntConstant) {
	Tokenizer tokenizer("1234");
	Token tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_LIT, tok.id);
	EXPECT_EQ(1234, tok.d);
	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_EOF, tok.id);
}

TEST(TokenizerTests, IntConstantWithWhitespace) {
	Tokenizer tokenizer("   1234   ");
	Token tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_LIT, tok.id);
	EXPECT_EQ(1234, tok.d);
	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_EOF, tok.id);
}

TEST(TokenizerTests, RealConstant) {
	Tokenizer tokenizer("1234.56789");
	Token tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_LIT, tok.id);
	EXPECT_EQ(1234.56789, tok.d);
	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_EOF, tok.id);
}

TEST(TokenizerTests, X) {
	Tokenizer tokenizer("x");
	Token tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_ARG, tok.id);
	EXPECT_EQ(0, tok.i);
	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_EOF, tok.id);
}

TEST(TokenizerTests, Y) {
	Tokenizer tokenizer("y");
	Token tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_ARG, tok.id);
	EXPECT_EQ(1, tok.i);
	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_EOF, tok.id);
}

TEST(TokenizerTests, Z) {
	Tokenizer tokenizer("z");
	Token tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_ARG, tok.id);
	EXPECT_EQ(2, tok.i);
	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_EOF, tok.id);
}

TEST(TokenizerTests, Arg1) {
	Tokenizer tokenizer("$1");
	Token tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_ARG, tok.id);
	EXPECT_EQ(0, tok.i);
	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_EOF, tok.id);
}

TEST(TokenizerTests, Arg2) {
	Tokenizer tokenizer("$2");
	Token tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_ARG, tok.id);
	EXPECT_EQ(1, tok.i);
	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_EOF, tok.id);
}

TEST(TokenizerTests, Arg3) {
	Tokenizer tokenizer("$3");
	Token tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_ARG, tok.id);
	EXPECT_EQ(2, tok.i);
	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_EOF, tok.id);
}

TEST(TokenizerTests, Parens) {
	Tokenizer tokenizer("(((0)))");
	Token tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_LPAREN, tok.id);
	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_LPAREN, tok.id);
	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_LPAREN, tok.id);
	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_LIT, tok.id);
	EXPECT_EQ(0, tok.d);
	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_RPAREN, tok.id);
	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_RPAREN, tok.id);
	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_RPAREN, tok.id);
	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_EOF, tok.id);
}

TEST(TokenizerTests, CallCos) {
	Tokenizer tokenizer("cos( 2*pi * x )");

	Token tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_F_COS, tok.id);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_LPAREN, tok.id);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_LIT, tok.id);
	EXPECT_EQ(2, tok.d);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_OP_MUL, tok.id);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_F_PI, tok.id);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_OP_MUL, tok.id);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_ARG, tok.id);
	EXPECT_EQ(0, tok.i);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_RPAREN, tok.id);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_EOF, tok.id);
}

TEST(TokenizerTests, CallPow) {
	Tokenizer tokenizer("pow(x,2)");

	Token tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_F_POW, tok.id);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_LPAREN, tok.id);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_ARG, tok.id);
	EXPECT_EQ(0, tok.i);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_COMMA, tok.id);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_LIT, tok.id);
	EXPECT_EQ(2, tok.d);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_RPAREN, tok.id);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_EOF, tok.id);
}

TEST(TokenizerTests, TwoPi) {
	Tokenizer tokenizer("2pi");

	Token tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_LIT, tok.id);
	EXPECT_EQ(2, tok.d);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_F_PI, tok.id);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_EOF, tok.id);
}

TEST(TokenizerTests, SignPlus) {
	Tokenizer tokenizer("+1.0");

	Token tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_PLUS, tok.id);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_LIT, tok.id);
	EXPECT_EQ(1, tok.d);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_EOF, tok.id);
}

TEST(TokenizerTests, SignMinus) {
	Tokenizer tokenizer("-1.0");

	Token tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_MINUS, tok.id);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_LIT, tok.id);
	EXPECT_EQ(1, tok.d);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_EOF, tok.id);
}

TEST(TokenizerTests, ExpNotation) {
	Tokenizer tokenizer("123.456e+089");

	Token tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_LIT, tok.id);
	EXPECT_EQ(123.456e89, tok.d);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_EOF, tok.id);
}

TEST(TokenizerTests, TenPowMinusTwo) {
	Tokenizer tokenizer("10^-2");

	Token tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_LIT, tok.id);
	EXPECT_EQ(10, tok.d);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_OP_POW, tok.id);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_MINUS, tok.id);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_LIT, tok.id);
	EXPECT_EQ(2, tok.d);

	tok = tokenizer.getNextToken();
	EXPECT_EQ(TOK_EOF, tok.id);
}
