/**
 * Copyright (c) 2015 Johannes Reifferscheid
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
 * NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef CTA_OUTPUT_HPP__
#define CTA_OUTPUT_HPP__

#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <iostream>

#include "terms.hpp"
#include "macros.hpp"

namespace cta {
namespace detail {

template <typename number, int var>
std::ostream& operator<<(std::ostream& o, const var_type<number, var>&) {
	return o << "x" << var;
}

template <typename number, typename T, int pow>
std::ostream& operator<<(std::ostream&, const static_power<number, T, pow>&);
#define CTA_FORWARD(type, n)                                                                       \
	template <CTA_TEMPLATE_DECL_ARGS(n)>                                                           \
	std::ostream& operator<<(std::ostream&, const type<CTA_TEMPLATE_USE_ARGS(n)>&);
#define CTA_VISIT_TYPE(type, n, ...) CTA_FORWARD(type, n)
#include "types.hpp"
#undef CTA_FORWARD

template <typename number, typename T, int pow>
std::ostream& operator<<(std::ostream& o, const static_power<number, T, pow>& v) {
	return o << "pow(" << v.a0 << ", " << pow << ")";
}

#define CTA_VISIT_TYPE(type, n, kind, op, ...) kind(type, op, n)
#define CTA_RET(type, op, n)                                                                       \
	template <CTA_TEMPLATE_DECL_ARGS(n)>                                                           \
	std::ostream& operator<<(std::ostream& o, const type<CTA_TEMPLATE_USE_ARGS(n)>& t) {           \
		(void) t;                                                                                  \
		return o << #op;                                                                           \
	}
#define CTA_FUN_ARG(_, n, __) BOOST_PP_IF(n, << ", " <<, << ) t.a##n
#define CTA_FUN(type, fun, n)                                                                      \
	template <CTA_TEMPLATE_DECL_ARGS(n)>                                                           \
	std::ostream& operator<<(std::ostream& o, const type<CTA_TEMPLATE_USE_ARGS(n)>& t) {           \
		(void) t;                                                                                  \
		return o << #fun << "(" BOOST_PP_REPEAT(n, CTA_FUN_ARG, _) << ")";                         \
	}
#define CTA_PREFIX(type, op, n)                                                                    \
	template <CTA_TEMPLATE_DECL_ARGS(n)>                                                           \
	std::ostream& operator<<(std::ostream& o, const type<CTA_TEMPLATE_USE_ARGS(n)>& t) {           \
		return o << "(" << #op << t.a0 << ")";                                                     \
	}
#define CTA_INFIX(type, op, n)                                                                     \
	template <CTA_TEMPLATE_DECL_ARGS(n)>                                                           \
	std::ostream& operator<<(std::ostream& o, const type<CTA_TEMPLATE_USE_ARGS(n)>& t) {           \
		return o << "(" << t.a0 << #op << t.a1 << ")";                                             \
	}

#include "types.hpp"
#undef CTA_FUN_ARG
} // namespace detail
} // namespace cta

#endif
