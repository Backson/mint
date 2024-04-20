#include <gtest/gtest.h>

#include "ast.hpp"

#include <utility>

using std::move;

class AstTests : public testing::Test {
protected:

	AstTests()
	{
		default_ast = Ast();

		default_copied_ast = default_ast;

		var_y_ast = Ast(OP_ARG);
		var_y_ast.i = 1;

		const_5_ast = Ast(OP_CONST);
		const_5_ast.d = 5.0;

		y_plus_5_ast = Ast(OP_ADD);
		y_plus_5_ast.children.emplace_back(var_y_ast);
		y_plus_5_ast.children.emplace_back(const_5_ast);

		y_plus_5_copied_ast = y_plus_5_ast;

		y_plus_5_moved_from_ast = y_plus_5_ast;

		y_plus_5_moved_to_ast = move(y_plus_5_moved_from_ast);
	}

	Ast default_ast;
	Ast default_copied_ast;
	Ast var_y_ast;
	Ast const_5_ast;
	Ast y_plus_5_ast;
	Ast y_plus_5_copied_ast;
	Ast y_plus_5_moved_from_ast;
	Ast y_plus_5_moved_to_ast;
};

TEST_F(AstTests, Default_OpIsInvalid) {
	EXPECT_EQ(OP_INVALID, default_ast.op);
}

TEST_F(AstTests, Default_IntIs0) {
	EXPECT_EQ(0, default_ast.i);
}

TEST_F(AstTests, Default_HasNoChildren) {
	EXPECT_EQ(0, default_ast.children.size());
}

TEST_F(AstTests, DefaultCopied_OpIsInvalid) {
	EXPECT_EQ(OP_INVALID, default_copied_ast.op);
}

TEST_F(AstTests, DefaultCopied_IntIs0) {
	EXPECT_EQ(0, default_copied_ast.i);
}

TEST_F(AstTests, DefaultCopied_HasNoChildren) {
	EXPECT_EQ(0, default_copied_ast.children.size());
}

TEST_F(AstTests, VarY_OpIsVar) {
	EXPECT_EQ(OP_ARG, var_y_ast.op);
}

TEST_F(AstTests, VarY_IntIs1) {
	EXPECT_EQ(1, var_y_ast.i);
}

TEST_F(AstTests, VarY_HasNoChildren) {
	EXPECT_EQ(0, var_y_ast.children.size());
}

TEST_F(AstTests, Const5_OpIsConst) {
	EXPECT_EQ(OP_CONST, const_5_ast.op);
}

TEST_F(AstTests, Const5_DoubleIs5) {
	EXPECT_EQ(5.0, const_5_ast.d);
}

TEST_F(AstTests, Const5_HasNoChildren) {
	EXPECT_EQ(0, const_5_ast.children.size());
}

TEST_F(AstTests, YPlus5_OpIsAdd) {
	EXPECT_EQ(OP_ADD, y_plus_5_ast.op);
}

TEST_F(AstTests, YPlus5_HasTwoChildren) {
	EXPECT_EQ(2, y_plus_5_ast.children.size());
}

TEST_F(AstTests, YPlus5Copy_OpIsAdd) {
	EXPECT_EQ(OP_ADD, y_plus_5_copied_ast.op);
}

TEST_F(AstTests, YPlus5Copy_HasTwoChildren) {
	EXPECT_EQ(2, y_plus_5_copied_ast.children.size());
}

TEST_F(AstTests, YPlus5Moved_OpIsAdd) {
	EXPECT_EQ(OP_ADD, y_plus_5_moved_to_ast.op);
}

TEST_F(AstTests, YPlus5Moved_HasTwoChildren) {
	EXPECT_EQ(2, y_plus_5_moved_to_ast.children.size());
}

TEST_F(AstTests, Default_eq_DefaultCopied) {
	EXPECT_TRUE(default_ast == default_copied_ast);
}

TEST_F(AstTests, YPlus5_eq_YPlus5Copy) {
	EXPECT_TRUE(y_plus_5_ast == y_plus_5_copied_ast);
	EXPECT_FALSE(y_plus_5_ast != y_plus_5_copied_ast);
}

TEST_F(AstTests, YPlus5_eq_YPlus5Moved) {
	EXPECT_TRUE(y_plus_5_ast == y_plus_5_moved_to_ast);
	EXPECT_FALSE(y_plus_5_ast != y_plus_5_moved_to_ast);
}

TEST_F(AstTests, VarY_neq_Const5) {
	EXPECT_TRUE(var_y_ast != const_5_ast);
	EXPECT_FALSE(var_y_ast == const_5_ast);
}
