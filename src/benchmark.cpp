// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#include <cstdio>
#include <algorithm>
#include <chrono>
#include <random>

#include "program.hpp"

typedef double (*func_t)(const double *);

static const double pi = 3.14159265358979323846264338327950288419716939937510;

void test(int N, int NARGS, double ** data, const char * program_str, func_t native_f) {
	using namespace std::chrono;

	// build program
	printf("%s\n", program_str);
	Program p(program_str);
	p.print();

	double params[3];
	
	double *native_results = new double[N];
	double *mint_results = new double[N];

	// benchmarks
	high_resolution_clock::time_point t1, t2;

	// native
	t1 = high_resolution_clock::now();
	for (int i = 0; i < N; ++i) {
		params[0] = data[0][i];
		params[1] = data[1][i];
		params[2] = data[2][i];
		native_results[i] = native_f(params);
	}
	t2 = high_resolution_clock::now();
	double time_reference = duration_cast<duration<double>>(t2 - t1).count();
	
	// mint
	t1 = high_resolution_clock::now();
	for (int i = 0; i < N; ++i) {
		params[0] = data[0][i];
		params[1] = data[1][i];
		params[2] = data[2][i];
		mint_results[i] = p.run(params);
	}
	t2 = high_resolution_clock::now();
	double time_program = duration_cast<duration<double>>(t2 - t1).count();

	// tests
	int mint_counter = 0;
	double mint_error_sum = 0.0;
	double mint_error_max = 0.0;
	for (int i = 0; i < N; ++i) {
		double native_res = native_results[i];
		double mint_res = mint_results[i];
		if (native_res == mint_res) {
			++mint_counter;
		} else {
			double diff = mint_res - native_res;
			double rel_variance = (diff * diff) / (native_res * native_res);
			mint_error_sum += rel_variance;
			mint_error_max = std::max(mint_error_max, rel_variance);
		}
	}

	delete[] native_results;
	delete[] mint_results;

	printf("%12s %12s %12s\n", "",
		"native", "mint");
	printf("%12s %12f %12f\n", "time",
		time_reference, time_program);
	printf("%12s %11.1f%% %11.1f%%\n", "rel speed", 100.0,
		100.0 * time_reference / time_program);
	printf("%12s %12d %12d\n", "accuracy", N,
		mint_counter);
	printf("%12s %12.4f %12.4e\n", "avg error", 0.0,
		mint_counter == N ? 0.0 : sqrt(mint_error_sum / (N - mint_counter)));
	printf("%12s %12.4f %12.4e\n", "max error", 0.0,
		sqrt(mint_error_max));
	printf("\n");
}

double native_ex1(const double * d) {
	return (d[1] + d[0]);
}

double native_ex2(const double * d) {
	return 2 * (d[1] + d[0]);
}

double native_ex3(const double * d) {
	return ((1.23 * (d[0] * d[0])) / d[1]) - 123.123;
}

double native_ex4(const double * d) {
	return (d[1] + d[0] / d[1]) * (d[0] - d[1] / d[0]);
}

double native_ex5(const double * d) {
	return d[0] / ((d[0] + d[1]) + (d[0] - d[1])) / d[1];
}

double native_ex6(const double * d) {
	return 1 - ((d[0] * d[1]) + (d[1] / d[0])) - 3;
}

double native_ex7(const double * d) {
	return 1.1 * d[0] + 2.2 * (d[1] * d[1]) - 3.3 * (d[0] * d[0] * d[0]) + 4.4 * pow(d[1], 15) - 5.5 * pow(d[0], 23) + 6.6 * pow(d[1], 55);
}

double native_ex8(const double * d) {
	return sin(2 * d[0]) + cos(pi / d[1]);
}

double native_ex9(const double * d) {
	return 1 - sin(2 * d[0]) + cos(pi / d[1]);
}

double native_ex10(const double * d) {
	return sqrt(111.111 - sin(2 * d[0]) + cos(pi / d[1]) / 333.333);
}

double native_ex11(const double * d) {
	return (d[0] * d[0] / sin(2 * pi / d[1])) - d[0] / 2;
}

double native_ex12(const double * d) {
	return d[0] + (cos(d[1] - sin(2 / d[0] * pi)) - sin(d[0] - cos(2 * d[1] / pi))) - d[1];
}

double native_unity(const double * d) {
	return d[0];
}

double native_square(const double * d) {
	return d[0] * d[0];
}

double native_gaus(const double * d) {
	double x = (d[0] - d[2]) / d[1];
	return exp(-0.5 * x * x) / (sqrt(2 * pi) * d[1]);
}

double native_ex_functions(const double * d) {
	return tan(d[0]) + trunc(d[1]) + cos(abs(d[2])) - floor(d[0]) + atan(5) - acosh(pi);
}

double native_ex_plusses(const double * d) {
	return d[0] + d[0];
}

int main(int argc, char *argv[]) {
	static const int N = 1000000;
	static const int MAXNARGS = 3;

	// prepare some input data
	double *data[MAXNARGS];
	for (int i = 0; i < MAXNARGS; ++i) {
		data[i] = new double[N];
	}
	std::default_random_engine rng;
	rng.seed(42);
	std::uniform_real_distribution<double> distr(0.0, 10.0);
	for (int i = 0; i < MAXNARGS; ++i) {
		for (int j = 0; j < N; ++j) {
			data[i][j] = distr(rng);
		}
	}

	// benchmarks
	test(N, 2, &data[0], "(y + x)", &native_ex1);
	test(N, 2, &data[0], "2 * (y + x)", &native_ex2);
	test(N, 2, &data[0], "((1.23 * x^2) / y) - 123.123", &native_ex3);
	test(N, 2, &data[0], "(y + x / y) * (x - y / x)", &native_ex4);
	test(N, 2, &data[0], "x / ((x + y) + (x - y)) / y", &native_ex5);
	test(N, 2, &data[0], "1 - ((x * y) + (y / x)) - 3", &native_ex6);
	test(N, 2, &data[0], "1.1*x^1 + 2.2*y^2 - 3.3*x^3 + 4.4*y^15 - 5.5*x^23 + 6.6*y^55", &native_ex7);
	test(N, 2, &data[0], "sin(2 * x) + cos(pi / y)", &native_ex8);
	test(N, 2, &data[0], "1 - sin(2 * x) + cos(pi / y)", &native_ex9);
	test(N, 2, &data[0], "sqrt(111.111 - sin(2 * x) + cos(pi / y) / 333.333)", &native_ex10);
	test(N, 2, &data[0], "(x^2 / sin(2 * pi / y)) -x / 2", &native_ex11);
	test(N, 2, &data[0], "x + (cos(y - sin(2 / x * pi)) - sin(x - cos(2 * y / pi))) - y", &native_ex12);
	test(N, 1, &data[0], "x", &native_unity);
	test(N, 1, &data[0], "x^2", &native_square);
	test(N, 3, &data[0], "exp(-0.5*((x-z)/y)^2)/(sqrt(2*pi)*y)", &native_gaus);
	test(N, 3, &data[0], "tan(x) + trunc(y) + cos(abs(z)) - floor(x) + arctan(5) - arcosh(pi)", &native_ex_functions);
	test(N, 1, &data[0], "+ + + + x + + + x", &native_ex_plusses);


	for (int i = 0; i < MAXNARGS; ++i) {
		delete[] data[i];
	}

	// halt program execution
	printf("done.");
	getchar();
	return 0;
}
