// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2018 Lars Droegemueller
// See LICENSE file for details

#include <stdio.h>

#include "program.hpp"
#include "parser.hpp"
#include "ast.hpp"
#include "optimizations.hpp"

size_t getline(FILE *f, char **buffer, size_t buffer_size) {
	size_t i = 0;

	while (i < buffer_size) {
		int c;
		c = fgetc(f);
		if (c == EOF || c == '\n') {
			(*buffer)[i] = '\0';
			return i;
		}
		else if (c <= 0xFF) {
			(*buffer)[i++] = c;
		}
	}
	return -1;
}

bool startswith(const char *str1, const char *str2) {
	if (str1 == nullptr || str2 == nullptr)
		return false;
	size_t idx = 0;
	while (str1[idx] != '\0' && str2[idx] != '\0') {
		if (str1[idx] != str2[idx])
			return false;
		idx++;
	}
	return true;
}

int main(int argc, char **argv) {
	size_t buffer_size = 1024;
	char *buffer = new char[buffer_size];
	char *expr = new char[buffer_size];
	*expr = '\0';
	Ast *ast = nullptr;
	Program *prog = nullptr;

	for (;;) {
		printf("> ");
		fflush(stdout);
		size_t len = getline(stdin, &buffer, buffer_size);
		if (len == 0) {
			continue;
		}
		if (strcmp(buffer, "exit") == 0) {
			break;
		}
		else if (strcmp(buffer, "hello") == 0) {
			printf("Hello, World!\n");
		}
		else if (startswith(buffer, "set ")) {
			strcpy_s(expr, buffer_size, &buffer[4]);
		}
		else if (strcmp(buffer, "print expr") == 0) {
			printf("%s\n", expr);
		}
		else if (strcmp(buffer, "make ast") == 0) {
			Parser parser(expr);
			if (parser.parse()) {
				printf("Error: %s\n", parser.getError());
				printf("%s\n", expr);
				printf("%*s\n", parser.getLastToken().pos + 1, "^");
			}
			else {
				if (ast) delete ast;
				ast = new Ast();
				*ast = parser.getAst();
			}
		}
		else if (strcmp(buffer, "print ast") == 0) {
			if (ast) {
				print(*ast);
			}
		}
		else if (strcmp(buffer, "optimize powi") == 0) {
			if (ast) {
				Optimizer optimizer;
				optimizer.optimizePowersToIntegerExponents(ast);
			}
		}
		else if (strcmp(buffer, "optimize constants") == 0) {
			if (ast) {
				Optimizer optimizer;
				optimizer.collapseConstants(ast);
			}
		}
		else if (strcmp(buffer, "optimize sign") == 0) {
			if (ast) {
				Optimizer optimizer;
				optimizer.collapseSigns(ast);
			}
		}
		else if (strcmp(buffer, "optimize stack") == 0) {
			if (ast) {
				Optimizer optimizer;
				optimizer.compressStack(ast);
			}
		}
		else if (strcmp(buffer, "make prog") == 0) {
			if (ast) {
				if (prog) delete prog;
				prog = new Program(*ast);
			}
		}
		else if (strcmp(buffer, "print prog") == 0) {
			if (prog) {
				prog->print();
			}
		}
		else {
			printf("invalid input\n");
		}
	}

	if (ast) delete ast;
	if (prog) delete prog;
	delete[] buffer;
	delete[] expr;
	return 0;
}
