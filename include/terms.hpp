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
#ifndef TERMS_HPP__
#define TERMS_HPP__

#include <boost/preprocessor/repetition/enum_params.hpp>
#include "macros.hpp"

namespace cta {
namespace detail {
template <typename number, typename T>
struct cta_type {
	using number_t = number;

	template <typename... Args>
	number operator()(const Args&... args) const;
};

template <typename number, int var>
struct var_type : public cta_type<number, var_type<number, var>> {};

template <typename number, int var, int index>
struct dvar_type : public cta_type<number, dvar_type<number, var, index>> {};

template <typename number, typename A0, int power>
struct static_power : public cta_type<number, static_power<number, A0, power>> {
	A0 a0;
	static_power(A0 a0) : a0(a0) {}
};

#define CTA_FIELD(z, n, _) BOOST_PP_CAT(A, n) BOOST_PP_CAT(a, n);
#define CTA_ARG(z, n, _) BOOST_PP_COMMA_IF(n) BOOST_PP_CAT(A, n) BOOST_PP_CAT(a, n)
#define CTA_INIT(z, n, _) BOOST_PP_COMMA_IF(n) BOOST_PP_CAT(a, n)(BOOST_PP_CAT(a, n))

#define CTA_VISIT_TYPE(type, n, ...)                                                               \
	template <CTA_TEMPLATE_DECL_ARGS(n)>                                                           \
	struct type : public cta_type<number, type<CTA_TEMPLATE_USE_ARGS(n)>> {                        \
		BOOST_PP_REPEAT(n, CTA_FIELD, _)                                                           \
		type(BOOST_PP_REPEAT(n, CTA_ARG, _)) BOOST_PP_IF(n,                                        \
		                                                 :, ) BOOST_PP_REPEAT(n, CTA_INIT, _) {}   \
	};

#include "types.hpp"
#undef CTA_FIELD
#undef CTA_ARG
#undef CTA_INIT

} // namespace detail

template <typename number, int var>
detail::var_type<number, var> make_var() {
	return {};
}

template <typename number, int var>
detail::dvar_type<number, var, 0> make_dvar() {
	return {};
}

} // namespace cta

#endif
