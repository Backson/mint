// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
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

#define DECLARE_OP_3_CONTRACTION(name,expr) \
	template <typename T> static inline T name ## _impl (T x, T y, T z) { return expr; }

DECLARE_OP_3_CONTRACTION(fmm_l,(x*y)*z)
DECLARE_OP_3_CONTRACTION(fma_l,(x*y)+z)
DECLARE_OP_3_CONTRACTION(fms_l,(x*y)-z)
DECLARE_OP_3_CONTRACTION(fmd_l,(x*y)/z)
DECLARE_OP_3_CONTRACTION(fam_l,(x+y)*z)
DECLARE_OP_3_CONTRACTION(faa_l,(x+y)+z)
DECLARE_OP_3_CONTRACTION(fas_l,(x+y)-z)
DECLARE_OP_3_CONTRACTION(fad_l,(x+y)/z)
DECLARE_OP_3_CONTRACTION(fsm_l,(x-y)*z)
DECLARE_OP_3_CONTRACTION(fsa_l,(x-y)+z)
DECLARE_OP_3_CONTRACTION(fss_l,(x-y)-z)
DECLARE_OP_3_CONTRACTION(fsd_l,(x-y)/z)
DECLARE_OP_3_CONTRACTION(fdm_l,(x/y)*z)
DECLARE_OP_3_CONTRACTION(fda_l,(x/y)+z)
DECLARE_OP_3_CONTRACTION(fds_l,(x/y)-z)
DECLARE_OP_3_CONTRACTION(fdd_l,(x/y)/z)
DECLARE_OP_3_CONTRACTION(fmm_r,x*(y*z))
DECLARE_OP_3_CONTRACTION(fma_r,x*(y+z))
DECLARE_OP_3_CONTRACTION(fms_r,x*(y-z))
DECLARE_OP_3_CONTRACTION(fmd_r,x*(y/z))
DECLARE_OP_3_CONTRACTION(fam_r,x+(y*z))
DECLARE_OP_3_CONTRACTION(faa_r,x+(y+z))
DECLARE_OP_3_CONTRACTION(fas_r,x+(y-z))
DECLARE_OP_3_CONTRACTION(fad_r,x+(y/z))
DECLARE_OP_3_CONTRACTION(fsm_r,x-(y*z))
DECLARE_OP_3_CONTRACTION(fsa_r,x-(y+z))
DECLARE_OP_3_CONTRACTION(fss_r,x-(y-z))
DECLARE_OP_3_CONTRACTION(fsd_r,x-(y/z))
DECLARE_OP_3_CONTRACTION(fdm_r,x/(y*z))
DECLARE_OP_3_CONTRACTION(fda_r,x/(y+z))
DECLARE_OP_3_CONTRACTION(fds_r,x/(y-z))
DECLARE_OP_3_CONTRACTION(fdd_r,x/(y/z))

#undef DECLARE_OP_3_CONTRACTION

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

template <typename T>
static inline T op3_impl(Op op, T x, T y, T z) {
	switch (op) {
	case OP_FUSED_MM_L: return fmm_l_impl(x, y, z);
	case OP_FUSED_MA_L: return fma_l_impl(x, y, z);
	case OP_FUSED_MS_L: return fms_l_impl(x, y, z);
	case OP_FUSED_MD_L: return fmd_l_impl(x, y, z);
	case OP_FUSED_AM_L: return fam_l_impl(x, y, z);
	case OP_FUSED_AA_L: return faa_l_impl(x, y, z);
	case OP_FUSED_AS_L: return fas_l_impl(x, y, z);
	case OP_FUSED_AD_L: return fad_l_impl(x, y, z);
	case OP_FUSED_SM_L: return fsm_l_impl(x, y, z);
	case OP_FUSED_SA_L: return fsa_l_impl(x, y, z);
	case OP_FUSED_SS_L: return fss_l_impl(x, y, z);
	case OP_FUSED_SD_L: return fsd_l_impl(x, y, z);
	case OP_FUSED_DM_L: return fdm_l_impl(x, y, z);
	case OP_FUSED_DA_L: return fda_l_impl(x, y, z);
	case OP_FUSED_DS_L: return fds_l_impl(x, y, z);
	case OP_FUSED_DD_L: return fdd_l_impl(x, y, z);
	case OP_FUSED_MM_R: return fmm_r_impl(x, y, z);
	case OP_FUSED_MA_R: return fma_r_impl(x, y, z);
	case OP_FUSED_MS_R: return fms_r_impl(x, y, z);
	case OP_FUSED_MD_R: return fmd_r_impl(x, y, z);
	case OP_FUSED_AM_R: return fam_r_impl(x, y, z);
	case OP_FUSED_AA_R: return faa_r_impl(x, y, z);
	case OP_FUSED_AS_R: return fas_r_impl(x, y, z);
	case OP_FUSED_AD_R: return fad_r_impl(x, y, z);
	case OP_FUSED_SM_R: return fsm_r_impl(x, y, z);
	case OP_FUSED_SA_R: return fsa_r_impl(x, y, z);
	case OP_FUSED_SS_R: return fss_r_impl(x, y, z);
	case OP_FUSED_SD_R: return fsd_r_impl(x, y, z);
	case OP_FUSED_DM_R: return fdm_r_impl(x, y, z);
	case OP_FUSED_DA_R: return fda_r_impl(x, y, z);
	case OP_FUSED_DS_R: return fds_r_impl(x, y, z);
	case OP_FUSED_DD_R: return fdd_r_impl(x, y, z);
	default:     throw std::invalid_argument("Wrong number of arguments for operator");
	}
}
