// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#ifndef DOUBLE_N_HPP_
#define DOUBLE_N_HPP_

#include "impl.hpp"

template <size_t N>
struct double_n {
	double d[N];
	
	static double_n constant(double x);
	static double_n vals(const double *vals);
	void unpack(double *vals);
};

template <size_t N>
double_n<N> double_n<N>::constant(double x) {
	double_n<N> result;
	for (int i = 0; i < N; ++i) {
		result.d[i] = x;
	}
	return result;
}

template <size_t N>
double_n<N> double_n<N>::vals(const double *x) {
	double_n<N> result;
	for (int i = 0; i < N; ++i) {
		result.d[i] = x[i];
	}
	return result;
}

template <size_t N>
void double_n<N>::unpack(double *vals) {
	for (int i = 0; i < N; ++i) {
		vals[i] = d[i];
	}
}

template <size_t N>
static inline double_n<N> operator - (double_n<N> rhs) {
	double_n<N> result;
	for (int i = 0; i < N; ++i) {
		result.d[i] = -rhs.d[i];
	}
	return result;
}

template <size_t N>
static inline double_n<N> operator + (double_n<N> lhs, double_n<N> rhs) {
	double_n<N> result;
	for (int i = 0; i < N; ++i) {
		result.d[i] = lhs.d[i] + rhs.d[i];
	}
	return result;
}

template <size_t N>
static inline double_n<N> operator - (double_n<N> lhs, double_n<N> rhs) {
	double_n<N> result;
	for (int i = 0; i < N; ++i) {
		result.d[i] = lhs.d[i] - rhs.d[i];
	}
	return result;
}

template <size_t N>
static inline double_n<N> operator * (double_n<N> lhs, double_n<N> rhs) {
	double_n<N> result;
	for (int i = 0; i < N; ++i) {
		result.d[i] = lhs.d[i] * rhs.d[i];
	}
	return result;
}

template <size_t N>
static inline double_n<N> operator / (double_n<N> lhs, double_n<N> rhs) {
	double_n<N> result;
	for (int i = 0; i < N; ++i) {
		result.d[i] = lhs.d[i] / rhs.d[i];
	}
	return result;
}

template <size_t N>
static inline double_n<N> pow_i (double_n<N> base, int j) {
	double_n<N> result;
	for (int i = 0; i < N; ++i) {
		result.d[i] = std::pow(base.d[i], j);
	}
	return result;
}

template <> static inline double_n<16> inv_impl(double_n<16> x) { return double_n<16>::constant(1) / x; }

#define FUNCTION_COMPONENTWISE_DECLARATION(name) \
	template <> static inline double_n<16> name ## _impl  (double_n<16> x) { \
		double_n<16> result; for (int i = 0; i < 16; ++i) { result.d[i] = std::name(x.d[i]); }; return result; \
	}

FUNCTION_COMPONENTWISE_DECLARATION(sqrt)
FUNCTION_COMPONENTWISE_DECLARATION(sin)
FUNCTION_COMPONENTWISE_DECLARATION(cos)
FUNCTION_COMPONENTWISE_DECLARATION(tan)
FUNCTION_COMPONENTWISE_DECLARATION(asin)
FUNCTION_COMPONENTWISE_DECLARATION(acos)
FUNCTION_COMPONENTWISE_DECLARATION(atan)
FUNCTION_COMPONENTWISE_DECLARATION(sinh)
FUNCTION_COMPONENTWISE_DECLARATION(cosh)
FUNCTION_COMPONENTWISE_DECLARATION(tanh)
FUNCTION_COMPONENTWISE_DECLARATION(asinh)
FUNCTION_COMPONENTWISE_DECLARATION(acosh)
FUNCTION_COMPONENTWISE_DECLARATION(atanh)
FUNCTION_COMPONENTWISE_DECLARATION(log)
FUNCTION_COMPONENTWISE_DECLARATION(exp)
FUNCTION_COMPONENTWISE_DECLARATION(erf)
FUNCTION_COMPONENTWISE_DECLARATION(erfc)
FUNCTION_COMPONENTWISE_DECLARATION(abs)
FUNCTION_COMPONENTWISE_DECLARATION(floor)
FUNCTION_COMPONENTWISE_DECLARATION(ceil)
FUNCTION_COMPONENTWISE_DECLARATION(round)
FUNCTION_COMPONENTWISE_DECLARATION(trunc)

#undef FUNCTION_COMPONENTWISE_DECLARATION

template <> static inline double_n<16> pow_impl  (double_n<16> x, double_n<16> y) { \
	double_n<16> result; for (int i = 0; i < 16; ++i) { result.d[i] = std::pow(x.d[i], y.d[i]); }; return result; \
}

template <>
static inline double_n<16> pi_impl() {
	return double_n<16>::constant(3.14159265358979323846264338327950288419716939937510L);
}

template <>
static inline double_n<16> e_impl() {
	return double_n<16>::constant(2.71828182845904523536028747135266249775724709369995L);
}

#endif // DOUBLE_N_HPP_
