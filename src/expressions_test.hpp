// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#ifndef EXPRESSIONS_TEST_HPP_
#define EXPRESSIONS_TEST_HPP_

#include <string>
#include <vector>

struct NativeEntry {
	std::string expr;
	double(*native)(const double *);
	size_t i;
};

extern std::vector<NativeEntry> arithmetic_expressions_3_entries;
extern std::vector<NativeEntry> selection_entries;

#endif // EXPRESSIONS_TEST_HPP_
