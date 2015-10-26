// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#include "program.hpp"

#include "ops.hpp"
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
		printf("Error: %s\n", parser.getError());
		printf("%s\n", src);
		printf("%*s\n\n", parser.getLastToken().pos + 1, "^");
		getchar();
		throw std::runtime_error("parsing error");
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
		case OP_CONST:
			program_p->push_back(constants_p->size());
			constants_p->push_back(ast.d);
			break;
		case OP_ARG:
			program_p->push_back(unsigned char(ast.i));
			break;
		case OP_POWI:
			program_p->push_back(unsigned char(ast.i - SCHAR_MIN));
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
		switch (*ip++) {
		case OP_NOOP:
			break;
		case OP_CONST:
			*++sp = constants[*ip++];
			continue;
		case OP_ARG:
			*++sp = arguments[*ip++];
			continue;
		case OP_PI:
			*++sp = 3.14159265358979323846264338327950288419716939937510;
			continue;
		case OP_E:
			*++sp = 2.71828182845904523536028747135266249775724709369995;
			continue;
		case OP_NEG:
			sp[0] = - sp[0];
			continue;
		case OP_INV:
			sp[0] = 1 / sp[0];
			continue;
		case OP_SQ:
			sp[0] = sp[0] * sp[0];
			continue;
		case OP_CU:
			sp[0] = sp[0] * sp[0] * sp[0];
			continue;
		case OP_SQRT:
			sp[0] = sqrt(sp[0]);
			continue;
		case OP_SIN:
			sp[0] = sin(sp[0]);
			continue;
		case OP_COS:
			sp[0] = cos(sp[0]);
			continue;
		case OP_TAN:
			sp[0] = tan(sp[0]);
			continue;
		case OP_ASIN:
			sp[0] = asin(sp[0]);
			continue;
		case OP_ACOS:
			sp[0] = acos(sp[0]);
			continue;
		case OP_ATAN:
			sp[0] = atan(sp[0]);
			continue;
		case OP_SINH:
			sp[0] = sinh(sp[0]);
			continue;
		case OP_COSH:
			sp[0] = cosh(sp[0]);
			continue;
		case OP_TANH:
			sp[0] = tanh(sp[0]);
			continue;
		case OP_ASINH:
			sp[0] = asinh(sp[0]);
			continue;
		case OP_ACOSH:
			sp[0] = acosh(sp[0]);
			continue;
		case OP_ATANH:
			sp[0] = atanh(sp[0]);
			continue;
		case OP_LOG:
			sp[0] = log(sp[0]);
			continue;
		case OP_EXP:
			sp[0] = exp(sp[0]);
			continue;
		case OP_ERF:
			sp[0] = erf(sp[0]);
			continue;
		case OP_ERFC:
			sp[0] = erfc(sp[0]);
			continue;
		case OP_ABS:
			sp[0] = abs(sp[0]);
			continue;
		case OP_FLOOR:
			sp[0] = floor(sp[0]);
			continue;
		case OP_CEIL:
			sp[0] = ceil(sp[0]);
			continue;
		case OP_ROUND:
			sp[0] = round(sp[0]);
			continue;
		case OP_TRUNC:
			sp[0] = trunc(sp[0]);
			continue;
		case OP_ADD:
			sp[-1] += sp[0];
			--sp;
			continue;
		case OP_SUB:
			sp[-1] -= sp[0];
			--sp;
			continue;
		case OP_MUL:
			sp[-1] *= sp[0];
			--sp;
			continue;
		case OP_DIV:
			sp[-1] /= sp[0];
			--sp;
			continue;
		case OP_POW:
			sp[-1] = pow(sp[-1], sp[0]);
			--sp;
			continue;
		case OP_POWI:
			sp[0] = pow(sp[0], SCHAR_MIN + int(*ip++));
			continue;
		}
	}

	return stack[0];
}