#ifndef CTA_MACROS_HPP__
#define CTA_MACROS_HPP__

#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

#define CTA_TEMPLATE_DECL_ARGS(n)                                                                  \
	typename number BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename A)
#define CTA_TEMPLATE_USE_ARGS(n) number BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, A)

#endif
