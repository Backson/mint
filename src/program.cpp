// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#include "program.hpp"

#include "ops.hpp"
#include "impl.hpp"
#include "tokens.hpp"
#include "parser.hpp"

#include "optimizations.hpp"

#include <cstring>
#include <cstdio>
#include <climits>
#include <cmath>
#include <functional>

Program::Program(const char * src) {
	Parser parser(src);
	if (parser.parse()) {
		//printf("Error: %s\n", parser.getError());
		//printf("%s\n", src);
		//printf("%*s\n\n", parser.getLastToken().pos + 1, "^");
		throw std::invalid_argument("parsing error");
	}

	Ast ast = parser.getAst();

	Optimizer optimizer;
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
	unsigned char const * ip = program.data(); // instruction pointer

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

double Program::run(const double * arguments) {
	unsigned char const * ip = program.data(); // instruction pointer
	double * sp = stack - 1; // stack pointer

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
