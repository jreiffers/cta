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
