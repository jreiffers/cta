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

# Supported operations

* unary -
* binary +, -, *, /, 
* pow, sin, cos, atan

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
