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
#define BOOST_TEST_MODULE cta_tests
#include <boost/test/unit_test.hpp>

#include <sstream>
#include <string>

#include "cta.hpp"

using namespace cta;

static auto x0 = make_var<double, 0>();
static auto x1 = make_var<double, 1>();

template <typename T>
std::string stringify(const T& t) {
	std::stringstream r;
	r << t;
	return r.str();
}

BOOST_AUTO_TEST_CASE(construct) {
	BOOST_CHECK_EQUAL("x0", stringify(x0));
	BOOST_CHECK_EQUAL("(-x0)", stringify(-x0));
	BOOST_CHECK_EQUAL("(x0+1)", stringify(x0 + 1.0));
	BOOST_CHECK_EQUAL("(x0-1)", stringify(x0 - 1.0));
	BOOST_CHECK_EQUAL("(x0*2)", stringify(x0 * 2.0));
	BOOST_CHECK_EQUAL("(x0*x1)", stringify(x0 * x1));
	BOOST_CHECK_EQUAL("(x0/3)", stringify(x0 / 3.0));
	BOOST_CHECK_EQUAL("sin(x0)", stringify(sin(x0)));
	BOOST_CHECK_EQUAL("cos(x0)", stringify(cos(x0)));
	BOOST_CHECK_EQUAL("atan(x0)", stringify(atan(x0)));
	BOOST_CHECK_EQUAL("pow(x0, 2)", stringify(pow(x0, 2)));
}

BOOST_AUTO_TEST_CASE(eval) {
	BOOST_CHECK_EQUAL(1, x0(1));
	BOOST_CHECK_EQUAL(-2, (-x0)(2));
	BOOST_CHECK_EQUAL(3, (x0 + x1)(1, 2));
	BOOST_CHECK_EQUAL(-1, (x0 - x1)(1, 2));
	BOOST_CHECK_EQUAL(2, (x0 / x1)(4, 2));
	BOOST_CHECK_EQUAL(8, (x0 * x1)(4, 2));
	BOOST_CHECK_EQUAL(0, sin(x0)(0));
	BOOST_CHECK_EQUAL(1, cos(x0)(0));
	BOOST_CHECK_EQUAL(9, pow(x0, 2)(3));
	BOOST_CHECK_CLOSE(M_PI / 8, atan(x0)(sqrt(2)-1), 1e-10);
}

BOOST_AUTO_TEST_CASE(diff) {
	auto test = [](const std::string& s, auto exp) {
		BOOST_CHECK_EQUAL(s, stringify(differentiate(exp, x0)));
	};

	test("1", x0);
	test("0", x1);
	test("-1", -x0);
	test("0", -x1);
	test("1", x0+x1);
	test("2", x0*2.0);
	test("0.5", x0/2.0);
	test("x1", x0*x1);
	test("(2*x0)", pow<2>(x0));
	test("(2*pow(x0, 1))", pow(x0, 2));
	test("cos(x0)", sin(x0));
	test("(-sin(x0))", cos(x0));
	test("(1/(pow(x0, 2)+1))", atan(x0));
}
