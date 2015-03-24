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
#ifndef CTA_IS_CONSTANT_HPP__
#define CTA_IS_CONSTANT_HPP__

#include <type_traits>
#include "terms.hpp"

namespace cta {
namespace detail {

template <int var, typename T>
struct is_constant : std::true_type {};

template <int var, typename number>
struct is_constant<var, var_type<number, var>> : std::false_type {};

template <int var, typename number, typename A, typename B>
struct is_constant<var, sum<number, A, B>>
    : std::integral_constant<bool, is_constant<var, A>::value && is_constant<var, B>::value> {};

template <int var, typename number, typename A, typename B>
struct is_constant<var, product<number, A, B>>
    : std::integral_constant<bool, is_constant<var, A>::value && is_constant<var, B>::value> {};

template <int var, typename number, typename A, typename B>
struct is_constant<var, fraction<number, A, B>>
    : std::integral_constant<bool, is_constant<var, A>::value && is_constant<var, B>::value> {};

template <int var, typename number, typename A, typename B>
struct is_constant<var, difference<number, A, B>>
    : std::integral_constant<bool, is_constant<var, A>::value && is_constant<var, B>::value> {};

template <int var, typename number, typename A, typename B>
struct is_constant<var, power<number, A, B>>
    : std::integral_constant<bool, is_constant<var, A>::value && is_constant<var, B>::value> {};

template <int var, typename number, typename A>
struct is_constant<var, sin_type<number, A>>
    : std::integral_constant<bool, is_constant<var, A>::value> {};

template <int var, typename number, typename A>
struct is_constant<var, cos_type<number, A>>
    : std::integral_constant<bool, is_constant<var, A>::value> {};

template <int var, typename number, typename A>
struct is_constant<var, atan_type<number, A>>
    : std::integral_constant<bool, is_constant<var, A>::value> {};

template <int var, typename number, typename A>
struct is_constant<var, negative<number, A>>
    : std::integral_constant<bool, is_constant<var, A>::value> {};

} // namespace detail
} // namespace cta

#endif
