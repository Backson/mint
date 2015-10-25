// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#include "program.hpp"

#include "ops.hpp"
#include "tokens.hpp"
#include "parser.hpp"

#include <cstring>
#include <cstdio>
#include <climits>

Program::Program(const char * src) {
	Parser parser(src);
	if (parser.parse()) {
		printf("Error: %s\n", parser.getError());
		printf("%s\n", src);
		printf("%*s\n\n", parser.getLastToken().pos + 1, "^");
		getchar();
		throw std::runtime_error("parsing error");
	}
	/*
	program = new unsigned char[parser.getProgramSize()];
	memcpy(program, parser.getProgram(), parser.getProgramSize() * sizeof(unsigned char));
	constants = new double[parser.getConstantCount()];
	memcpy(constants, parser.getConstants(), parser.getConstantCount() * sizeof(double));
	*/
	::print(parser.getAst());
}

Program::~Program() {
	if (program) delete[] program;
	if (constants) delete[] constants;
}

void Program::print() {
	unsigned char const * ip = program; // instruction pointer

	while (*ip != OP_HLT) {
		switch (*ip++) {
		case OP_PSHC:
			printf("%-6s", "PSHC");
			printf("%-3i\n", (int) *ip++);
			continue;
		case OP_PSHA:
			printf("%-6s", "PSHA");
			printf("%-3i\n", (int) *ip++);
			continue;
		case OP_PI:
			printf("%-6s\n", "PI");
			continue;
		case OP_E:
			printf("%-6s\n", "E");
			continue;
		case OP_NEG:
			printf("%-6s\n", "NEG");
			continue;
		case OP_INV:
			printf("%-6s\n", "INV");
			continue;
		case OP_SQRT:
			printf("%-6s\n", "SQRT");
			continue;
		case OP_SIN:
			printf("%-6s\n", "SIN");
			continue;
		case OP_COS:
			printf("%-6s\n", "COS");
			continue;
		case OP_LOG:
			printf("%-6s\n", "LOG");
			continue;
		case OP_EXP:
			printf("%-6s\n", "EXP");
			continue;
		case OP_ADD:
			printf("%-6s\n", "ADD");
			continue;
		case OP_SUB:
			printf("%-6s\n", "SUB");
			continue;
		case OP_MUL:
			printf("%-6s\n", "MUL");
			continue;
		case OP_DIV:
			printf("%-6s\n", "DIV");
			continue;
		case OP_POW:
			printf("%-6s\n", "POW");
			continue;
		case OP_POWI:
			printf("%-6s", "POWI");
			printf("%-3i\n", SCHAR_MIN + int(*ip++));
			continue;
		default:
			printf("error decoding opcode!\n");
			return;
		}
	}
	printf("%-6s\n", "HLT");
}

double Program::run(const double * arguments) {
	unsigned char const * ip = program; // instruction pointer
	double * sp = stack - 1; // stack pointer

	while (*ip != OP_HLT) {
		switch (*ip++) {
		case OP_PSHC:
			*++sp = constants[*ip++];
			continue;
		case OP_PSHA:
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
		case OP_SQRT:
			sp[0] = sqrt(sp[0]);
			continue;
		case OP_SIN:
			sp[0] = sin(sp[0]);
			continue;
		case OP_COS:
			sp[0] = cos(sp[0]);
			continue;
		case OP_LOG:
			sp[0] = log(sp[0]);
			continue;
		case OP_EXP:
			sp[0] = exp(sp[0]);
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