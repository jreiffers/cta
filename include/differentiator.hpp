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
#ifndef CTA_DIFFERENTIATOR_HPP__
#define CTA_DIFFERENTIATOR_HPP__

#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/variadic/to_seq.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/rest_n.hpp>
#include <boost/preprocessor/seq/enum.hpp>

#include "is_constant.hpp"
#include "simplifier.hpp"

#ifndef BOOST_PP_SEQ_ENUM_0
#define BOOST_PP_SEQ_ENUM_0
#define CTA_BOOST_PP_SEQ_ENUM_0_DEFINED
#endif

namespace cta {

namespace detail {
template <int var, typename T>
struct differentiator {
	static zero_type<int> differentiate(const T&) { return {}; }
};

template <int var, typename number>
struct differentiator<var, var_type<number, var>> {
	static one_type<int> differentiate(const var_type<number, var>&) { return {}; }
};

template <int var, int ivar, typename number, int index>
struct differentiator<var, dvar_type<number, ivar, index>> {
	static dvar_type<number, ivar, index+1> differentiate(const dvar_type<number, ivar, index>&) { return {}; }
};

#define CTA_DERIV_TEMPLATE_ARG(_, __, a) , decltype(a)
#define CTA_DERIV_ARGS(seq) BOOST_PP_SEQ_FOR_EACH(CTA_DERIV_TEMPLATE_ARG, _, seq)
#define CTA(type, seq) (type<number CTA_DERIV_ARGS(seq)>{ BOOST_PP_SEQ_ENUM(seq) })
#define CTA_DERIV(arg) (differentiator<var, decltype(arg)>::differentiate(arg))
#define CTA_IF(condition, t) std::enable_if<condition::value, t>::type
#define CTA_STATIC_POW(t, pot) (static_power<number, decltype(t), pot>{ t })

#define CTA_VISIT_TYPE(type, n, _, __, def, ...)                                                   \
	template <int var, CTA_TEMPLATE_DECL_ARGS(n)>                                                  \
	struct differentiator<var, type<CTA_TEMPLATE_USE_ARGS(n)>> {                                   \
		static decltype(auto) differentiate(const type<CTA_TEMPLATE_USE_ARGS(n)>& t) {             \
			(void) t;                                                                              \
			return def;                                                                            \
		}                                                                                          \
	};

template <int var, typename number, typename A0>
struct differentiator<var, static_power<number, A0, 0>> {
	static zero_type<number> differentiate(const static_power<number, A0, 0>&) { return {}; }
};

template <int var, typename number, typename A0, int pow>
struct differentiator<var, static_power<number, A0, pow>> {
	static decltype(auto) differentiate(const static_power<number, A0, pow>& t) {
		auto deriv = differentiator<var, A0>::differentiate(t.a0);
		return product<number, decltype(deriv),
		               product<number, number, static_power<number, A0, pow - 1>>>{
			deriv, { pow, { t.a0 } }
		};
	}
};

#include "types.hpp"

} // namespace detail

template <int var, typename T>
decltype(auto) differentiate(const T& t) {
	return simplify(detail::differentiator<var, T>::differentiate(t));
}

template <int var, typename T>
decltype(auto) differentiate(const T& t, const detail::var_type<typename T::number_t, var>&) {
	return differentiate<var>(t);
}

} // namespace cta

#ifdef CTA_BOOST_PP_SEQ_ENUM_0_DEFINED
#undef CTA_BOOST_PP_SEQ_ENUM_0_DEFINED
#undef BOOST_PP_SEQ_ENUM_0
#endif

#endif
