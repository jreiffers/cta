#ifndef CTA_OUTPUT_HPP__
#define CTA_OUTPUT_HPP__

#include "terms.hpp"
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include "macros.hpp"

namespace cta {

template <typename number, int var>
std::ostream& operator<<(std::ostream& o, const detail::var_type<number, var>&) {
	return o << "x" << var;
}

template <typename number, typename T, int pow>
std::ostream& operator<<(std::ostream&, const detail::static_power<number, T, pow>&);
#define CTA_FORWARD(type, n)                                                                       \
	template <CTA_TEMPLATE_DECL_ARGS(n)>                                                           \
	std::ostream& operator<<(std::ostream&, const detail::type<CTA_TEMPLATE_USE_ARGS(n)>&);
#define CTA_VISIT_TYPE(type, n, ...) CTA_FORWARD(type, n)
#include "types.hpp"
#undef CTA_FORWARD

template <typename number, typename T, int pow>
std::ostream& operator<<(std::ostream& o, const detail::static_power<number, T, pow>& v) {
	return o << "pow(" << v.a0 << ", " << pow << ")";
}

#define CTA_VISIT_TYPE(type, n, kind, op, ...) kind(type, op, n)
#define CTA_RET(type, op, n)                                                                       \
	template <CTA_TEMPLATE_DECL_ARGS(n)>                                                           \
	std::ostream& operator<<(std::ostream& o, const detail::type<CTA_TEMPLATE_USE_ARGS(n)>& t) {   \
		(void) t;                                                                                  \
		return o << #op;                                                                           \
	}
#define CTA_FUN_ARG(_, n, __) BOOST_PP_IF(n, << ", " <<, << ) t.a##n
#define CTA_FUN(type, fun, n)                                                                      \
	template <CTA_TEMPLATE_DECL_ARGS(n)>                                                           \
	std::ostream& operator<<(std::ostream& o, const detail::type<CTA_TEMPLATE_USE_ARGS(n)>& t) {   \
		(void) t;                                                                                  \
		return o << #fun << "(" BOOST_PP_REPEAT(n, CTA_FUN_ARG, _) << ")";                         \
	}
#define CTA_PREFIX(type, op, n)                                                                    \
	template <CTA_TEMPLATE_DECL_ARGS(n)>                                                           \
	std::ostream& operator<<(std::ostream& o, const detail::type<CTA_TEMPLATE_USE_ARGS(n)>& t) {   \
		return o << "(" << #op << t.a0 << ")";                                                     \
	}
#define CTA_INFIX(type, op, n)                                                                     \
	template <CTA_TEMPLATE_DECL_ARGS(n)>                                                           \
	std::ostream& operator<<(std::ostream& o, const detail::type<CTA_TEMPLATE_USE_ARGS(n)>& t) {   \
		return o << "(" << t.a0 << #op << t.a1 << ")";                                             \
	}

#include "types.hpp"
#undef CTA_FUN_ARG
}

#endif
