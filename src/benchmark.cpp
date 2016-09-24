// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <random>
#include <chrono>
#include <algorithm>
#include <memory>

#include "program.hpp"

#include "expressions_test.hpp"

static const int MAXNARGS = 4;
static const int N = 100000;
static const double NATIVE_TIME_THRESHOLD = 0.100;
static const int NATIVE_TIME_MULTIPLIER = 4;

class Benchmark {
public:
	Benchmark();
	~Benchmark();
	
	void testCompilation(const NativeEntry &entry);
	void testResult(const NativeEntry &entry);

private:
	double *data[MAXNARGS];

	double *native_results;
	double *mint_results;
};

Benchmark::Benchmark() {
	// prepare some input data
	for (int i = 0; i < MAXNARGS; ++i) {
		data[i] = new double[N];
	}
	std::default_random_engine rng;
	rng.seed(42);
	std::uniform_real_distribution<double> distr(0.0, 100.0);
	for (int i = 0; i < MAXNARGS; ++i) {
		for (int j = 0; j < N; ++j) {
			data[i][j] = distr(rng);
		}
	}

	native_results = new double[N];
	mint_results = new double[N];
}

Benchmark::~Benchmark() {
	for (int i = 0; i < MAXNARGS; ++i) {
		delete[] data[i];
	}

	delete[] native_results;
	delete[] mint_results;
}

void Benchmark::testCompilation(const NativeEntry &entry) {
	const char *str = entry.expr.c_str();
	printf("===== %d =====\n", entry.i);
	printf(" %s\n", entry.expr.c_str());
	try {
		Program program(str);
		program.print();
	} catch (...) {
		printf("ERROR\n", str);
	}
	printf("\n");
}

void Benchmark::testResult(const NativeEntry &entry) {
	std::unique_ptr<Program> program;
	try {
		auto ptr = new Program(entry.expr.c_str());
		program = std::unique_ptr<Program>(ptr);
	} catch (...) {
		return;
	}

	using namespace std::chrono;
	high_resolution_clock::time_point t1, t2;
	double time_native, time_mint;
	int rounds = 1;

	// native
	RECOMPUTE:
	t1 = high_resolution_clock::now();
	for (int round = 0; round < rounds; ++round)
	for (int i = 0; i < N; ++i) {
		double params[4];
		params[0] = data[(0 + round) % 4][i];
		params[1] = data[(1 + round) % 4][i];
		params[2] = data[(2 + round) % 4][i];
		params[3] = data[(3 + round) % 4][i];
		native_results[i] = entry.native(params);
	}
	t2 = high_resolution_clock::now();
	time_native = duration_cast<duration<double>>(t2 - t1).count();
	if (time_native < NATIVE_TIME_THRESHOLD) {
		rounds *= NATIVE_TIME_MULTIPLIER;
		goto RECOMPUTE;
	}
	
	// mint
	t1 = high_resolution_clock::now();
	for (int round = 0; round < rounds; ++round) {
		double *params[4];
		params[0] = data[(0 + round) % 4];
		params[1] = data[(1 + round) % 4];
		params[2] = data[(2 + round) % 4];
		params[3] = data[(3 + round) % 4];
		program->run(params, mint_results, N);
	}
	t2 = high_resolution_clock::now();
	time_mint = duration_cast<duration<double>>(t2 - t1).count();

	// comparison
	int counter = 0;
	double error_sum = 0.0;
	double error_max = 0.0;
	for (int i = 0; i < N; ++i) {
		double native_res = native_results[i];
		double mint_res = mint_results[i];
		if (native_res == mint_res) {
			++counter;
		} else {
			double diff = mint_res - native_res;
			double rel_variance = (diff * diff) / (native_res * native_res);
			error_sum += rel_variance;
			error_max = std::max(error_max, rel_variance);
		}
	}

	char buffer[128];
	sprintf(buffer, "%4d %4d %12.6f %12.6f -- %8d %12.4e %12.4e", (int)entry.i, rounds,
		time_native, time_mint,
		counter,
		counter == N ? 0.0 : sqrt(error_sum / (N - counter)),
		sqrt(error_max)
	);
	printf("%s\n", buffer);
}


int main(int argc, char *argv[]) {
	Benchmark bm;
	
	for (const auto &entry : arithmetic_expressions_3_entries) bm.testCompilation(entry);
	for (const auto &entry : selection_entries) bm.testCompilation(entry);
	for (auto &entry : arithmetic_expressions_3_entries) bm.testResult(entry);
	for (auto &entry : selection_entries) bm.testResult(entry);

	printf("done.\n");
	getchar();
	return 0;
}