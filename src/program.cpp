// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#include "program.hpp"

#include "ops.hpp"
#include "double_n.hpp"
#include "impl.hpp"
#include "tokens.hpp"
#include "parser.hpp"

#include "optimizations.hpp"

#include <cstring>
#include <cstdio>
#include <climits>
#include <cmath>
#include <functional>

Program::Program(const char * src, int optimize) {
	Parser parser(src);
	if (parser.parse()) {
		//printf("Error: %s\n", parser.getError());
		//printf("%s\n", src);
		//printf("%*s\n\n", parser.getLastToken().pos + 1, "^");
		throw std::invalid_argument("parsing error");
	}

	Ast ast = parser.getAst();

	Optimizer optimizer;
	switch (optimize) {
	default:
	case OPTIMIZE_NOTHING:
		break;
	case OPTIMIZE_MANDATORY:
		optimizer.optimizePowersToIntegerExponents(&ast);
		break;
	case OPTIMIZE_STRICT:
		optimizer.optimizePowersToIntegerExponents(&ast);
		optimizer.collapseConstants(&ast);
		optimizer.collapseSigns(&ast);
		optimizer.compressStack(&ast);
		break;
	case OPTIMIZE_PRECISE:
		optimizer.optimizePowersToIntegerExponents(&ast);
		optimizer.collapseConstants(&ast);
		optimizer.collapseSigns(&ast);
		optimizer.compressStack(&ast);
		optimizer.contract(&ast);
		break;
	}
	optimizer.optimizeDefaults(&ast);

	auto program_p = &program;
	auto constants_p = &constants;
	std::function<void(const Ast &ast)> visitor_lambda =
	[program_p, constants_p, &visitor_lambda](const Ast &ast) {
		for (const Ast &child : ast.children) {
			visitor_lambda(child);
		}
		if (ast.op != OP_NOOP)
			program_p->push_back(ast.op);
		switch (ast.op) {
		case OP_CONST: {
			// prevent the exact same constant from being stored twice
			int constant_index = -1;
			for (unsigned i = 0; i < constants_p->size(); ++i) {
				double c = (*constants_p)[i];
				if (ast.d == c) {
					constant_index = i;
					break;
				}
			}
			if (constant_index >= 0) {
				program_p->push_back((unsigned char)constant_index);
			} else {
				program_p->push_back((unsigned char)constants_p->size());
				constants_p->push_back(ast.d);
			}
			break;
		}
		case OP_ARG:
			program_p->push_back((unsigned char)(ast.i));
			break;
		case OP_POWI:
			program_p->push_back((unsigned char)(ast.i - SCHAR_MIN));
			break;
		default:
			break;
		}
	};

	visitor_lambda(ast);
}

void Program::print() {
	unsigned char const *ip = program.data(); // instruction pointer

	do {
		int op = *ip++;
		printf("%-6s", getOperatorName(op));
		switch (op) {
		case OP_CONST:
			printf("%-3i (%g)\n", (int) *ip, constants[*ip]);
			ip++;
			break;
		case OP_ARG:
			printf("%-3i\n", (int) *ip++);
			break;
		case OP_POWI:
			printf("%-3i\n", SCHAR_MIN + int(*ip++));
			break;
		default:
			printf("\n");
			break;
		}
	} while (*ip != OP_HLT);
}

double Program::run(const double *arguments) {
	unsigned char const *ip = program.data(); // instruction pointer
	double *sp = stack - 1; // stack pointer

	while (*ip != OP_HLT) {
		Op op = Op(*ip++);
		switch (op) {
		case OP_NOOP:
			break;
		case OP_CONST:
			*++sp = constants[*ip++];
			break;
		case OP_ARG:
			*++sp = arguments[*ip++];
			break;
		case OP_POWI:
			sp[0] = pow(sp[0], SCHAR_MIN + int(*ip++));
			break;
		default: {
			int num_operands = getOperandNumber(op);
			switch (num_operands) {
			case 0: {
				*++sp = op0_impl<double>(op);
				break;
			}
			case 1: {
				double x = *sp;
				*sp = op1_impl(op, x);
				break;
			}
			case 2: {
				double y = *sp--;
				double x = *sp;
				*sp = op2_impl<double>(op, x, y);
				break;
			}
			case 3: {
				double z = *sp--;
				double y = *sp--;
				double x = *sp;
				*sp = op3_impl<double>(op, x, y, z);
				break;
			}
			}
		} // default case
		} // switch (*ip++)
	} // while (*ip != OP_HLT)

	return stack[0];
}


void Program::run(double **arguments, double *result, size_t n) {
	std::vector<double_n<16>> stack;
	stack.resize(20);

	for (size_t i = 0; i < n / 16; ++i) {
		unsigned char const *ip = program.data(); // instruction pointer
		double_n<16> *sp = stack.data() - 1; // stack pointer

		while (*ip != OP_HLT) {
			Op op = Op(*ip++);
			switch (op) {
			case OP_NOOP:
				break;
			case OP_CONST:
				*++sp = double_n<16>::constant(constants[*ip++]);
				break;
			case OP_ARG:
				*++sp = double_n<16>::vals(&arguments[*ip++][i * 16]);
				break;
			case OP_POWI:
				sp[0] = pow_i(sp[0], SCHAR_MIN + int(*ip++));
				break;
			default: {
				int num_operands = getOperandNumber(op);
				switch (num_operands) {
				case 0: {
					*++sp = op0_impl<double_n<16>>(op);
					break;
				}
				case 1: {
					double_n<16> x = *sp;
					*sp = op1_impl<double_n<16>>(op, x);
					break;
				}
				case 2: {
					double_n<16> y = *sp--;
					double_n<16> x = *sp;
					*sp = op2_impl<double_n<16>>(op, x, y);
					break;
				}
				case 3: {
					double_n<16> z = *sp--;
					double_n<16> y = *sp--;
					double_n<16> x = *sp;
					*sp = op3_impl<double_n<16>>(op, x, y, z);
					break;
				}
				}
			} // default case
			} // switch (*ip++)
		} // while (*ip != OP_HLT)

		stack[0].unpack(result + 16 * i);
	}

	for (size_t i = 0; i < n % 16; ++i) {
		double params[4];
		params[0] = arguments[0][n / 16 * 16 + i];
		params[1] = arguments[1][n / 16 * 16 + i];
		params[2] = arguments[2][n / 16 * 16 + i];
		params[3] = arguments[3][n / 16 * 16 + i];
		result[n / 16 * 16 + i] = run(params);
	}
}
