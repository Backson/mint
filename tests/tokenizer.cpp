#include <gtest/gtest.h>

#include "tokenizer.hpp"

TEST(TokenizerTests, Empty) {
	Tokenizer tokenizer("");
	Token tok = tokenizer.getNextToken();

	EXPECT_EQ(TOK_EOF, tok.id);
}