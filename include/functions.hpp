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
#ifndef CTA_FUNCTIONS_HPP__
#define CTA_FUNCTIONS_HPP__

#include "terms.hpp"
#include <boost/preprocessor/repetition/enum_params.hpp>

namespace cta {

#if 0
// this is disabled because pow currently only supports constant exponents.
#define CTA_VISIT_TYPE(type, n, kind, op, ...) kind(type, op, n)

#define CTA_RET(...)
#define CTA_FUN(type, op, n) template <BOOST_PP_ENUM_PARAMS(n, typename T)>
    detail::type<typename T::number_t, BOOST_PP_ENUM_PARAMS(n, T)> op(
#define CTA_PREFIX(...)                                                                       
#define CTA_INFIX(...)                                                        

#include "types.hpp"
#endif

template <typename T>
detail::sin_type<typename T::number_t, T> sin(const T& t) {
	return { t };
}

template <typename T>
detail::cos_type<typename T::number_t, T> cos(const T& t) {
	return { t };
}

template <typename T>
detail::atan_type<typename T::number_t, T> atan(const T& t) {
	return { t };
}

template <typename T>
detail::power<typename T::number_t, T, double> pow(const T& t, typename T::number_t exp) {
	return { t, exp };
}

template <int Exp, typename T>
detail::static_power<typename T::number_t, T, Exp> pow(const T& t) {
	return { t };
}

} // namespace cta

#endif
