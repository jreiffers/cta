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
