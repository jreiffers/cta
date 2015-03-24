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
// no include guard; this file is usually included more than once.

// a0'
#define CTA_A0p (CTA_DERIV(t.a0))
// a1'
#define CTA_A1p (CTA_DERIV(t.a1))

// a0' a1
#define CTA_A0pA1 (CTA(product, CTA_A0p (t.a1)))
// a0 a1'
#define CTA_A0A1p (CTA(product, (t.a0) CTA_A1p ))

// name of type, number of args, type of eval, argument for eval, derivative
CTA_VISIT_TYPE(zero_type,  0, CTA_RET,    0,
		CTA(zero_type, ))
CTA_VISIT_TYPE(one_type,   0, CTA_RET,    1,
		CTA(zero_type, ))

CTA_VISIT_TYPE(negative,    1, CTA_PREFIX, -,
		CTA(negative, CTA_A0p))

CTA_VISIT_TYPE(sin_type,   1, CTA_FUN,    sin,
		CTA(product,  CTA_A0p (CTA(cos_type, (t.a0)))))
CTA_VISIT_TYPE(cos_type,   1, CTA_FUN,    cos,
		CTA(negative,  (CTA(product, CTA_A0p (CTA(sin_type, (t.a0)))))))
CTA_VISIT_TYPE(atan_type,  1, CTA_FUN,    atan,
		CTA(fraction, CTA_A0p (CTA(sum, (CTA_STATIC_POW(t.a0, 2))(1)))))

CTA_VISIT_TYPE(product,    2, CTA_INFIX,  *,
		CTA(sum, CTA_A0pA1 CTA_A0A1p))
CTA_VISIT_TYPE(sum,        2, CTA_INFIX,  +,
		CTA(sum, CTA_A0p CTA_A1p))
CTA_VISIT_TYPE(difference, 2, CTA_INFIX,  -,
		CTA(difference, CTA_A0p CTA_A1p))
CTA_VISIT_TYPE(fraction,   2, CTA_INFIX,  /,
		CTA(fraction, (CTA(difference, CTA_A0pA1 CTA_A0A1p))(CTA_STATIC_POW(t.a1, 2))))

// currently, powers can only be created with constant exponents (no logarithms).
// the derivative is only correct for this case
CTA_VISIT_TYPE(power,      2, CTA_FUN,    pow,
		CTA(product, (CTA(product, (t.a1)(CTA(power, (t.a0)(CTA(difference, (t.a1)(number{1}))))))) CTA_A0p))

#undef CTA_A0p
#undef CTA_A1p
#undef CTA_A0pA1
#undef CTA_A0A1p

#ifndef CTA_NO_UNDEFINE
#undef CTA_VISIT_TYPE
#undef CTA_INFIX
#undef CTA_PREFIX
#undef CTA_FUN
#undef CTA_RET
#undef CTA
#undef CTA_DERIV
#undef CTA_IF
#undef CTA_ALWAYS
#endif
