CTA allows differentiation of basic functions such as 
`(x^2-1)^.5 * atan((x^2-1)^.5)` at compile time.

# Prerequisites

You'll need a compiler with support for C++14, cmake, and boost for the
boost preprocessor library.

# Installing

* `mkdir build` in the CTA folder
* `cmake ..`
* `make all test`
* `make install`

# Usage

CTA is a header-only library:

    #include <cta/cta.hpp>
    using namespace cta;

Let's declare some variables:

    auto x = make_var<double, 0>();
    auto y = make_var<double, 1>();

The first template argument defines the type of the variable, the second
defines the index. Here's a couple of functions:

    auto f = x*y + cos(x);
    auto g = f + x;

All CTA expressions can be printed to ostreams:

    std::cout << f << std::endl; // prints ((x0*x1)+cos(x0))

Use `operator()` to evaluate functions:

    f(10, 20); // ~199.16

Differentiation:

    auto fdx = differentiate(f, x);

Exponentiation currently is only supported with constant exponents. When the 
exponent is an integer, there is a form that allows slightly better 
simplification:
  
    auto h = pow(x, 2);
    auto i = pow<2>(x);

    std::cout << differentiate(h, x) << std::endl; // prints (2*pow(x0, 1))
    std::cout << differentiate(i, x) << std::endl; // prints (2*x0)

CTA can also handle the case where you know a value and its derivative(s) 
(think taylor series):

	auto t = make_var<double, 0>();
    auto g = make_dvar<double, 1>();
    auto f = t + pow<2>(g);
    auto fdt = differentiate(g, t); // 1 + 2*g'*g
	
	std::cout << fdt(1, { 5, 10 }) << std::endl; // prints 101

Not supplying a derivative required by an expression (for instance, calling
`fdt(1, { 5 })`) results in a compile error.

# Supported operations

* unary -
* binary +, -, *, /, 
* pow, sin, cos, atan

# Limitations

CTA can't differentiate everything. For instance:

    d/dx((x^2 - 1)^.5 - atan((x^2 - 1)^.5)) = (x^2 - 1)^.5 / x

As you can see, this is defined at `x = 1`. CTA's simplification logic isn't 
powerful to find this derivative, instead getting:

    x/(x^2-1)^.5 - x/((x^2-1)^.5 * x^2)

Both of these terms have a pole at `x = 1`, so the result at `x = 1` will be
undefined and numerical stability in the vicinity will be poor.

# Files

* `cta.hpp`: includes all of the following files
* `terms.hpp`: defines templates for the operations (e.g. cta::detail::sum for binary +).
* `operators.hpp`: defines `operator+-*/`
* `functions.hpp`: defines `pow`, `sin`, ...
* `evaluator.hpp`: defines `operator()`
* `differentiator.hpp`: defines `differentiate()`
* `is_constant.hpp`: used for checking if an expression is a constant.
* `simplifier.hpp`: defines `simplify()`
* `output.hpp`: defines `operator<<`
* `types.hpp`: used by most of the other files to generate code.
