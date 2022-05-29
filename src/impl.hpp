// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Dammann
// See LICENSE file for details

#include "ops.hpp"

#include <cmath>
#include <stdexcept>

template <typename T>
static inline T pi_impl() {
	return T(3.14159265358979323846264338327950288419716939937510L);
}

template <typename T>
static inline T e_impl() {
	return T(2.71828182845904523536028747135266249775724709369995L);
}

template <typename T> static inline T neg_impl   (T x) { return -x; }
template <typename T> static inline T inv_impl   (T x) { return T(1) / x; }
template <typename T> static inline T sq_impl    (T x) { return x * x; }
template <typename T> static inline T cu_impl    (T x) { return x * x * x; }
template <typename T> static inline T sqrt_impl  (T x) { return std::sqrt(x); }
template <typename T> static inline T sin_impl   (T x) { return std::sin(x); }
template <typename T> static inline T cos_impl   (T x) { return std::cos(x); }
template <typename T> static inline T tan_impl   (T x) { return std::tan(x); }
template <typename T> static inline T asin_impl  (T x) { return std::asin(x); }
template <typename T> static inline T acos_impl  (T x) { return std::acos(x); }
template <typename T> static inline T atan_impl  (T x) { return std::atan(x); }
template <typename T> static inline T sinh_impl  (T x) { return std::sinh(x); }
template <typename T> static inline T cosh_impl  (T x) { return std::cosh(x); }
template <typename T> static inline T tanh_impl  (T x) { return std::tanh(x); }
template <typename T> static inline T asinh_impl (T x) { return std::asinh(x); }
template <typename T> static inline T acosh_impl (T x) { return std::acosh(x); }
template <typename T> static inline T atanh_impl (T x) { return std::atanh(x); }
template <typename T> static inline T log_impl   (T x) { return std::log(x); }
template <typename T> static inline T exp_impl   (T x) { return std::exp(x); }
template <typename T> static inline T erf_impl   (T x) { return std::erf(x); }
template <typename T> static inline T erfc_impl  (T x) { return std::erfc(x); }
template <typename T> static inline T abs_impl   (T x) { return std::abs(x); }
template <typename T> static inline T floor_impl (T x) { return std::floor(x); }
template <typename T> static inline T ceil_impl  (T x) { return std::ceil(x); }
template <typename T> static inline T round_impl (T x) { return std::round(x); }
template <typename T> static inline T trunc_impl (T x) { return std::trunc(x); }

template <typename T> static inline T add_impl (T x, T y) { return x + y; }
template <typename T> static inline T sub_impl (T x, T y) { return x - y; }
template <typename T> static inline T mul_impl (T x, T y) { return x * y; }
template <typename T> static inline T div_impl (T x, T y) { return x / y; }
template <typename T> static inline T pow_impl (T x, T y) { return std::pow(x, y); }

template <typename T>
static inline T op0_impl(Op op) {
	switch (op) {
	case OP_PI: return pi_impl<T>();
	case OP_E:  return e_impl<T>();
	default:    throw std::invalid_argument("Wrong number of arguments for operator");
	}
}

template <typename T>
static inline T op1_impl(Op op, T x) {
	switch (op) {
	case OP_NEG:   return neg_impl(x);
	case OP_INV:   return inv_impl(x);
	case OP_SQ:    return sq_impl(x);
	case OP_CU:    return cu_impl(x);
	case OP_SQRT:  return sqrt_impl(x);
	case OP_SIN:   return sin_impl(x);
	case OP_COS:   return cos_impl(x);
	case OP_TAN:   return tan_impl(x);
	case OP_ASIN:  return asin_impl(x);
	case OP_ACOS:  return acos_impl(x);
	case OP_ATAN:  return atan_impl(x);
	case OP_SINH:  return sinh_impl(x);
	case OP_COSH:  return cosh_impl(x);
	case OP_TANH:  return tanh_impl(x);
	case OP_ASINH: return asinh_impl(x);
	case OP_ACOSH: return acosh_impl(x);
	case OP_ATANH: return atanh_impl(x);
	case OP_EXP:   return exp_impl(x);
	case OP_LOG:   return log_impl(x);
	case OP_ERF:   return erf_impl(x);
	case OP_ERFC:  return erfc_impl(x);
	case OP_ABS:   return abs_impl(x);
	case OP_FLOOR: return floor_impl(x);
	case OP_CEIL:  return ceil_impl(x);
	case OP_ROUND: return round_impl(x);
	case OP_TRUNC: return trunc_impl(x);
	default:       throw std::invalid_argument("Wrong number of arguments for operator");
	}
}

template <typename T>
static inline T op2_impl(Op op, T x, T y) {
	switch (op) {
	case OP_ADD: return add_impl(x, y);
	case OP_SUB: return sub_impl(x, y);
	case OP_MUL: return mul_impl(x, y);
	case OP_DIV: return div_impl(x, y);
	case OP_POW: return pow_impl(x, y);
	default:     throw std::invalid_argument("Wrong number of arguments for operator");
	}
}
