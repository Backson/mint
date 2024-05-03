#include <gtest/gtest.h>

#include "optimizations.hpp"

#include <utility>

using std::move;
using std::swap;

class OptimizationsTests : public testing::Test {
protected:

	OptimizationsTests()
	{
		x_ast = Ast(OP_ARG);
		x_ast.i = 0;

		y_ast = Ast(OP_ARG);
		y_ast.i = 1;

		z_ast = Ast(OP_ARG);
		z_ast.i = 2;

		minus_y_ast = Ast(OP_NEG);
		minus_y_ast.children.emplace_back(y_ast);

		x_minus_y_ast = Ast(OP_SUB);
		x_minus_y_ast.children.emplace_back(x_ast);
		x_minus_y_ast.children.emplace_back(y_ast);

		x_plus_minus_y_ast = Ast(OP_ADD);
		x_plus_minus_y_ast.children.emplace_back(x_ast);
		x_plus_minus_y_ast.children.emplace_back(minus_y_ast);

		x_y_sum_ast = Ast(OP_ADD);
		x_y_sum_ast.children.emplace_back(x_ast);
		x_y_sum_ast.children.emplace_back(y_ast);

		x_y_z_sum_ast = Ast(OP_ADD);
		x_y_z_sum_ast.children.emplace_back(x_ast);
		x_y_z_sum_ast.children.emplace_back(y_ast);
		x_y_z_sum_ast.children.emplace_back(z_ast);
	}

	Ast x_ast;
	Ast y_ast;
	Ast z_ast;
	Ast minus_y_ast;
	Ast x_minus_y_ast;
	Ast x_plus_minus_y_ast;
	Ast x_y_sum_ast;
	Ast x_y_z_sum_ast;

	Optimizer optimizer;
};

TEST_F(OptimizationsTests, SubtractionToSum) {
	Ast ast = x_minus_y_ast;

	optimizer.SubtractionToSum(&ast);

	EXPECT_EQ(x_plus_minus_y_ast, ast);
}

TEST_F(OptimizationsTests, FlattenSum1) {
	Ast ast(OP_ADD);
	ast.children.emplace_back(x_y_sum_ast);
	ast.children.emplace_back(z_ast);

	optimizer.FlattenSum(&ast);

	EXPECT_EQ(x_y_z_sum_ast, ast);
}
