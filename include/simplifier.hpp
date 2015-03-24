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
#ifndef CTA_SIMPLIFIER_HPP__
#define CTA_SIMPLIFIER_HPP__

#include <cmath>
#include <type_traits>

namespace cta {

namespace detail {

template <template <typename...> class A, template <typename...> class... B>
struct is_one_of;

template <template <typename...> class A>
struct is_one_of<A> : std::false_type {};

template <template <typename...> class A, template <typename...> class... B>
struct is_one_of<A, A, B...> : std::true_type {};

template <template <typename...> class A, template <typename...> class B,
          template <typename...> class... C>
struct is_one_of<A, B, C...> : is_one_of<A, C...> {};

struct dummy {};

template <int pass, typename A>
struct simplifier {
	static A simplify(const A& a) { return a; }
};

template <int pass, typename A>
struct simplified {
	using type = decltype(simplifier<pass, A>::simplify(*(const A*)0));
};

// number + number  -> number
template <typename number>
struct simplifier<0, sum<number, number, number>> {
	static number simplify(const sum<number, number, number>& a) { return a.a0 + a.a1; }
};

// number - number  -> number
template <typename number>
struct simplifier<0, difference<number, number, number>> {
	static number simplify(const difference<number, number, number>& a) { return a.a0 - a.a1; }
};

// number / number  -> number
template <typename number>
struct simplifier<0, fraction<number, number, number>> {
	static number simplify(const fraction<number, number, number>& a) { return a.a0 / a.a1; }
};

// number * number  -> number
template <typename number>
struct simplifier<0, product<number, number, number>> {
	static number simplify(const product<number, number, number>& a) { return a.a0 * a.a1; }
};

// -number
template <typename number>
struct simplifier<0, negative<number, number>> {
	static number simplify(const negative<number, number>& a) { return -a.a0; }
};

// 0 + 0
template <typename number, typename _, typename __>
struct simplifier<0, sum<number, zero_type<_>, zero_type<__>>> {
	static zero_type<number> simplify(const sum<number, zero_type<_>, zero_type<__>>&) {
		return {};
	}
};

// x + 0
template <typename number, typename _, typename A>
struct simplifier<0, sum<number, A, zero_type<_>>> {
	static decltype(auto) simplify(const sum<number, A, zero_type<_>>& a) {
		return simplifier<0, A>::simplify(a.a0);
	}
};

// 0 + x
template <typename number, typename _, typename A>
struct simplifier<0, sum<number, zero_type<_>, A>> {
	static decltype(auto) simplify(const sum<number, zero_type<_>, A>& a) {
		return simplifier<0, A>::simplify(a.a1);
	}
};

// 0 - 0
template <typename number, typename _, typename __>
struct simplifier<0, difference<number, zero_type<_>, zero_type<__>>> {
	static zero_type<number> simplify(const difference<number, zero_type<_>, zero_type<__>>&) {
		return {};
	}
};

// x - 0
template <typename number, typename _, typename A>
struct simplifier<0, difference<number, A, zero_type<_>>> {
	static decltype(auto) simplify(const difference<number, A, zero_type<_>>& a) {
		return simplifier<0, A>::simplify(a.a0);
	}
};

// 0 - x
template <typename number, typename _, typename A>
struct simplifier<0, difference<number, zero_type<_>, A>> {
	static negative<number, typename simplified<0, A>::type>
	simplify(const difference<number, zero_type<_>, A>& a) {
		return { simplifier<0, A>::simplify(a.a1) };
	}
};

// x - (-y)
template <typename number, typename A, typename B>
struct simplifier<1, difference<number, A, negative<number, B>>> {
	static sum<number, typename simplified<1, A>::type, typename simplified<1, B>::type>
	simplify(const difference<number, A, negative<number, B>>& a) {
		return { simplifier<1, A>::simplify(a.a0), simplifier<1, B>::simplify(a.a1.a0) };
	}
};

// -0
template <typename number, typename _>
struct simplifier<0, negative<number, zero_type<_>>> {
	static zero_type<number> simplify(const negative<number, zero_type<_>>&) { return {}; }
};

// -1
template <typename number, typename _>
struct simplifier<0, negative<number, one_type<_>>> {
	static number simplify(const negative<number, one_type<_>>&) { return -1; }
};

// 0 * 0
template <typename number, typename _, typename __>
struct simplifier<0, product<number, zero_type<_>, zero_type<__>>> {
	static zero_type<number> simplify(const product<number, zero_type<_>, zero_type<__>>&) {
		return {};
	}
};

// x * 0
template <typename number, typename _, typename A>
struct simplifier<0, product<number, A, zero_type<_>>> {
	static zero_type<number> simplify(const product<number, A, zero_type<_>>&) { return {}; }
};

// 0 * x
template <typename number, typename _, typename A>
struct simplifier<0, product<number, zero_type<_>, A>> {
	static zero_type<number> simplify(const product<number, zero_type<_>, A>&) { return {}; }
};

// 1 * 1
template <typename number, typename _, typename __>
struct simplifier<0, product<number, one_type<_>, one_type<__>>> {
	static one_type<number> simplify(const product<number, one_type<_>, one_type<__>>&) {
		return {};
	}
};

// x * 1
template <typename number, typename _, typename A>
struct simplifier<0, product<number, A, one_type<_>>> {
	static A simplify(const product<number, A, one_type<_>>& a) { return a.a0; }
};

// 1 * x
template <typename number, typename _, typename A>
struct simplifier<0, product<number, one_type<_>, A>> {
	static A simplify(const product<number, one_type<_>, A>& a) { return a.a1; }
};

// 0 / x
template <typename number, typename _, typename A>
struct simplifier<0, fraction<number, zero_type<_>, A>> {
	static zero_type<number> simplify(const fraction<number, zero_type<_>, A>&) { return {}; }
};

// x^0 -> 1
template <typename number, typename _>
struct simplifier<0, static_power<number, _, 0>> {
	static one_type<number> simplify(const static_power<number, _, 0>&) { return {}; }
};

// x^1 -> x
template <typename number, typename A>
struct simplifier<0, static_power<number, A, 1>> {
	static decltype(auto) simplify(const static_power<number, A, 1>& a) {
		return simplifier<0, A>::simplify(a.a0);
	}
};

// -x * -y -> x * y
template <typename number, typename A, typename B>
struct simplifier<1, product<number, negative<number, A>, negative<number, B>>> {
	static product<number, A, B>
	simplify(const product<number, negative<number, A>, negative<number, B>>& a) {
		return { simplifier<1, A>::simplify(a.a0), simplifier<1, B>::simplify(a.a1) };
	}
};

// -x * y -> -(x * y)
template <typename number, typename A, typename B>
struct simplifier<1, product<number, negative<number, A>, B>> {
	static negative<
	    number, product<number, typename simplified<1, A>::type, typename simplified<1, B>::type>>
	simplify(const product<number, negative<number, A>, B>& a) {
		return { { simplifier<1, A>::simplify(a.a0.a0), simplifier<1, B>::simplify(a.a1) } };
	}
};

// x * -y -> -(x * y)
template <typename number, typename A, typename B>
struct simplifier<1, product<number, A, negative<number, B>>> {
	static negative<
	    number, product<number, typename simplified<1, A>::type, typename simplified<1, B>::type>>
	simplify(const product<number, A, negative<number, B>>& a) {
		return { { simplifier<1, A>::simplify(a.a0), simplifier<1, B>::simplify(a.a1.a0) } };
	}
};

// (-x) + y -> y - x
template <typename number, typename A, typename B>
struct simplifier<1, sum<number, negative<number, A>, B>> {
	static difference<number, typename simplified<1, B>::type, typename simplified<1, A>::type>
	simplify(const sum<number, negative<number, A>, B>& a) {
		return { simplifier<1, B>::simplify(a.a1), simplifier<1, A>::simplify(a.a0.a0) };
	}
};


// c^n -> number
template <typename number, int exp>
struct simplifier<0, static_power<number, number, exp>> {
	static number simplify(const static_power<number, number, exp>& a) {
		return pow(a.a0, exp);
	}
};

template <int pass, typename number, template <typename n, typename A> class X, class A>
struct simplifier<pass, X<number, A>>
    : std::enable_if<is_one_of<X, negative, cos_type, atan_type, sin_type>::value, dummy>::type {

	static auto simplify(const X<number, A>& t)
	    -> X<number, decltype(simplifier<pass, A>::simplify(t.a0))> {
		return { simplifier<pass, A>::simplify(t.a0) };
	}
};

template <int pass, typename number, template <typename n, typename A, typename B> class X, class A,
          class B>
struct simplifier<pass, X<number, A, B>>
    : std::enable_if<is_one_of<X, sum, product, fraction, difference, power>::value, dummy>::type {

	static auto simplify(const X<number, A, B>& t)
	    -> X<number, decltype(simplifier<pass, A>::simplify(t.a0)),
	         decltype(simplifier<pass, B>::simplify(t.a1))> {
		return { simplifier<pass, A>::simplify(t.a0), simplifier<pass, B>::simplify(t.a1) };
	}
};

template <typename A>
struct simplify_all_passes {

	static decltype(auto) simplify(const A& a) {
		auto p0 = simplifier<0, A>::simplify(a);
		return simplifier<1, typename std::decay<decltype(p0)>::type>::simplify(p0);
	}
};

template <typename A, bool try_again, int max_depth>
struct simplify_r;

template <typename A, bool _>
struct simplify_r<A, _, 0> {

	static A simplify(const A& a) { return a; }
};

template <typename A>
struct simplify_r<A, true, 0> {

	static A simplify(const A& a) { return a; }
};

template <typename A>
struct simplify_r<A, false, 0> {

	static A simplify(const A& a) { return a; }
};

template <typename A, int _>
struct simplify_r<A, false, _> {

	static A simplify(const A& a) { return a; }
};

template <typename A, int max_depth>
struct simplify_r<A, true, max_depth> {

	static decltype(auto) simplify(const A& a) {
		auto sim_a = simplify_all_passes<A>::simplify(a);
		return simplify_r<decltype(sim_a), !std::is_same<A, decltype(sim_a)>::value,
		                  max_depth - 1>::simplify(sim_a);
	}
};

} // namespace detail

template <typename A, int max_depth = 10>
auto simplify(const A& a) -> decltype(detail::simplify_r<A, true, max_depth>::simplify(a)) {
	return detail::simplify_r<A, true, max_depth>::simplify(a);
}
}

#endif
