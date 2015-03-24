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
