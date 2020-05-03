//
// arch/x86_64/rsp/vsub.h
//
// This file is subject to the terms and conditions defined in
// 'LICENSE', which is part of this source code package.
//

static inline __m128i rsp_vsub(__m128i vs, __m128i vt, __m128i carry, __m128i *acc_lo)
{
	__m128i unsat_diff, sat_diff, overflow, vd;

	// acc_lo uses saturated arithmetic.
	unsat_diff = _mm_sub_epi16(vt, carry);
	sat_diff = _mm_subs_epi16(vt, carry);

	*acc_lo = _mm_sub_epi16(vs, unsat_diff);
	vd = _mm_subs_epi16(vs, sat_diff);

	// VD is the signed diff of the two sources and the carry. Since we
	// have to saturate the diff of all three, we have to be clever.
	overflow = _mm_cmpgt_epi16(sat_diff, unsat_diff);
	return _mm_adds_epi16(vd, overflow);
}
