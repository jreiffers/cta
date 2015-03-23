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

} // namespace cta

#endif
