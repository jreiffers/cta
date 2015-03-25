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
#ifndef CTA_EVALUATOR_HPP__
#define CTA_EVALUATOR_HPP__

#include <tuple>
#include <cmath>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include "macros.hpp"

namespace cta {
namespace detail {

template <typename A>
struct evaluator {
	template <typename... Args>
	static const A& evaluate(const A& a, const Args&...) {
		return a;
	}
};

template <typename number, int var>
struct evaluator<var_type<number, var>> {
	template <typename... Args>
	static number evaluate(const var_type<number, var>&, const Args&... args) {
		static_assert(var < sizeof...(args), "required variable not passed to eval");
		return std::get<var>(std::make_tuple(args...));
	}
};

template <typename number, int var, int index>
struct evaluator<dvar_type<number, var, index>> {
	template <typename... Args>
	static number evaluate(const dvar_type<number, var, index>&, const Args&... args) {
		static_assert(var < sizeof...(args), "required variable not passed to eval");
		return std::get<index>(std::get<var>(std::make_tuple(args...)));
	}
};

template <typename number, typename T, int p>
struct evaluator<static_power<number, T, p>> {
	template <typename... Args>
	static number evaluate(const static_power<number, T, p>& v, const Args&... args) {
		return pow(evaluator<T>::evaluate(v.a0, args...), p);
	}
};

template <typename... Args>
void silence_unused(const Args&...) {}

// evaluator for `type' with n arguments, operation type (infix, prefix, ...), argument for
// operation
#define CTA_VISIT_TYPE(type, n, op, arg, ...)                                                      \
	template <CTA_TEMPLATE_DECL_ARGS(n)>                                                           \
	struct evaluator<type<CTA_TEMPLATE_USE_ARGS(n)>> {                                             \
		template <typename... Args>                                                                \
		static number evaluate(const type<CTA_TEMPLATE_USE_ARGS(n)>& t, const Args&... args) {     \
			(void) t;                                                                              \
			silence_unused(args...);                                                               \
			return op(arg, n);                                                                     \
		}                                                                                          \
	};

// evaluation of argument n
#define CTA_EVAL_ARG(n) evaluator<BOOST_PP_CAT(A, n)>::evaluate(t.BOOST_PP_CAT(a, n), args...)

// infix operator
#define CTA_INFIX(operator, ...) CTA_EVAL_ARG(0) operator CTA_EVAL_ARG(1)
// prefix operator
#define CTA_PREFIX(operator, ...) operator CTA_EVAL_ARG(0)
// function application of `funname' with n evaluated arguments
#define CTA_EVAL_ARG_LIST(z, n, _) BOOST_PP_COMMA_IF(n) CTA_EVAL_ARG(n)
#define CTA_FUN(funname, n) funname(BOOST_PP_REPEAT(n, CTA_EVAL_ARG_LIST, _))
// return the given value
#define CTA_RET(value, ...) value

#include "types.hpp"

#undef CTA_EVAL_ARG
#undef CTA_EVAL_ARG_LIST

} // namespace detail

template <typename A, typename... Args>
auto evaluate(A& a, const Args&... args) -> decltype(detail::evaluator<A>().evaluate(a, args...)) {
	return detail::evaluator<A>::evaluate(a, args...);
}

template <typename number, typename T>
template <typename... Args>
number detail::cta_type<number, T>::operator()(const Args&... args) const {
	return detail::evaluator<T>::evaluate(*static_cast<const T*>(this), args...);
}

} // namespace cta

#endif

