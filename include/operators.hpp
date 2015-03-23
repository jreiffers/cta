#ifndef CTA_OPERATORS_HPP__
#define CTA_OPERATORS_HPP__

namespace cta {
namespace operators {

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

} // namespace operators
} // namespace cta

#endif
