// This file is part of MINT.  MINT is a Math INTerpreter.
// Copyright (C) 2015 Lars Droegemueller
// See LICENSE file for details

#include "expressions_test.hpp"

std::vector<NativeEntry> arithmetic_expressions_3_entries;
std::vector<NativeEntry> selection_entries;

#define EXPAND(expr, token, i)                                           \
double func_ ## i (const double *args) {                                 \
	using namespace std;                                                 \
	const double &a = args[0], &b = args[1], &c = args[2], &d = args[3]; \
	const double &x = args[0], &y = args[1], &z = args[2], &w = args[3]; \
	static const double pi =                                             \
	3.141592653589793238462643383279502884197169399375105820974944;      \
	static const double e =                                              \
	2.718281828459045235360287471352662497757247093699959574966967;      \
	return expr;                                                         \
};                                                                       \
struct StaticNativeEntriesInitializer_ ## i {                            \
	StaticNativeEntriesInitializer_ ## i () {                            \
		token.push_back(NativeEntry{( #expr ), &func_ ## i, i});         \
	}                                                                    \
} static_ ## token ## _initializer ## i;

#define EXPR2(expr,token,i) EXPAND(expr,token,i)
#define EXPR(expr,token) EXPR2((expr),token,__COUNTER__)
#define EXPR_T(expr) EXPR(expr, TOKEN)

#define TOKEN arithmetic_expressions_3_entries

EXPR_T(((x*y)*z))
EXPR_T(((x*y)+z))
EXPR_T(((x*y)-z))
EXPR_T(((x*y)/z))
EXPR_T(((x+y)*z))
EXPR_T(((x+y)+z))
EXPR_T(((x+y)-z))
EXPR_T(((x+y)/z))
EXPR_T(((x-y)*z))
EXPR_T(((x-y)+z))
EXPR_T(((x-y)-z))
EXPR_T(((x-y)/z))
EXPR_T(((x/y)*z))
EXPR_T(((x/y)+z))
EXPR_T(((x/y)-z))
EXPR_T(((x/y)/z))
EXPR_T((x*(y*z)))
EXPR_T((x*(y+z)))
EXPR_T((x*(y-z)))
EXPR_T((x*(y/z)))
EXPR_T((x+(y*z)))
EXPR_T((x+(y+z)))
EXPR_T((x+(y-z)))
EXPR_T((x+(y/z)))
EXPR_T((x-(y*z)))
EXPR_T((x-(y+z)))
EXPR_T((x-(y-z)))
EXPR_T((x-(y/z)))
EXPR_T((x/(y*z)))
EXPR_T((x/(y+z)))
EXPR_T((x/(y-z)))
EXPR_T((x/(y/z)))

#undef TOKEN
#define TOKEN selection_entries

EXPR_T(((y + x)))
EXPR_T((2 * (y + x)))
EXPR_T((((1.23 * pow(x,2)) / y) - 123.123))
EXPR_T(((y + x / y) * (x - y / x)))
EXPR_T((x / ((x + y) + (x - y)) / y))
EXPR_T((1 - ((x * y) + (y / x)) - 3))
EXPR_T((1.1*pow(x,1) + 2.2*pow(y,2) - 3.3*pow(x,3) + 4.4*pow(y,15) - 5.5*pow(x,23) + 6.6*pow(y,55)))
EXPR_T((sin(2 * x) + cos(pi / y)))
EXPR_T((1 - sin(2 * x) + cos(pi / y)))
EXPR_T((sqrt(111.111 - sin(2 * x) + cos(pi / y) / 333.333)))
EXPR_T(((pow(x,2) / sin(2 * pi / y)) -x / 2))
EXPR_T((x + (cos(y - sin(2 / x * pi)) - sin(x - cos(2 * y / pi))) - y))
EXPR_T((x))
EXPR_T((pow(x,2)))
EXPR_T((exp(-0.5*pow((x-z)/y,2))/(sqrt(2*pi)*y)))
EXPR_T((tan(x) + trunc(y) + cos(abs(z)) - floor(x) + atan(5) - acosh(pi)))
EXPR_T((+ + + + x + + + x))
EXPR_T((((((((((x+y)*7.123)-w)-((x+y)-(7.123*w)))-((x*(y-(7.123*w)))/((x*y)+(7.123+w))))-((((x+y)-(7.123*w))+((x/y)+(7.123+w)))+(((x*y)+(7.123+w))*((x/y)/(7.123-w)))))-(((((x/y)-(7.321/w))*((x-y)+(7.321+w)))*(((x-y)-(7.321+w))+((x-y)*(7.321/w))))*((((x-y)+(7.321+w))-((x*y)*(7.321+w)))-(((x-y)*(7.321/w))/(x+((y/7.321)+w)))))))))
EXPR_T((tan((((cos(tan(((tan((((1.92+(pi/(pi-sin((cos(((((tan((((sin((((sin((0.40+cos((tan((tan((((sin(((((((e+2.58)+3.88)+b)-b)+a)+e))*0.10)+a)+b))*e))*a))))+a)+pi)/0.78))+a)/pi)*3.29))*a)/b)/2.71)*3.53))+1.87)))))/2.72)/e))/a)+0.49)))+pi)+a)-b))))
EXPR_T((- (- - + - a) + (((((((a)+b))-b)))) + b - ((a))))
EXPR_T((x + (y * (z + w))))
