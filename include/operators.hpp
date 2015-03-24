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
#ifndef CTA_OPERATORS_HPP__
#define CTA_OPERATORS_HPP__

namespace cta {
namespace detail {

#define CTA_VISIT_TYPE(type, _, kind, op, ...) kind(type, op)

#define CTA_RET(...)
#define CTA_FUN(...)
#define CTA_PREFIX(type, op)                                                                       \
	template <typename number, typename T>                                                         \
	detail::type<number, T> operator op(const detail::cta_type<number, T>& t) {                    \
		return { *static_cast<const T*>(&t) };                                                     \
	}
#define CTA_INFIX(type, op)                                                                        \
	template <typename number, typename A, typename B>                                             \
	detail::type<number, A, B> operator op(const detail::cta_type<number, A>& a,                   \
	                                       const detail::cta_type<number, B>& b) {                 \
		return { *static_cast<const A*>(&a), *static_cast<const B*>(&b) };                         \
	}                                                                                              \
	template <typename number, typename A>                                                         \
	detail::type<number, A, number> operator op(const detail::cta_type<number, A>& a,              \
	                                            const number& b) {                                 \
		return { *static_cast<const A*>(&a), b };                                                  \
	}                                                                                              \
	template <typename number, typename A>                                                         \
	detail::type<number, number, A> operator op(const number& b,                                   \
	                                            const detail::cta_type<number, A>& a) {            \
		return { b, *static_cast<const A*>(&a) };                                                  \
	}

#include "types.hpp"

} // namespace detail
} // namespace cta

#endif
