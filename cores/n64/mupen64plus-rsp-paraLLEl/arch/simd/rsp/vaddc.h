//
// arch/x86_64/rsp/vaddc.h
//
// This file is subject to the terms and conditions defined in
// 'LICENSE', which is part of this source code package.
//

static inline __m128i rsp_vaddc(__m128i vs, __m128i vt, __m128i zero, __m128i *sn)
{
	__m128i sat_sum, unsat_sum;

	sat_sum = _mm_adds_epu16(vs, vt);
	unsat_sum = _mm_add_epi16(vs, vt);

	*sn = _mm_cmpeq_epi16(sat_sum, unsat_sum);
	*sn = _mm_cmpeq_epi16(*sn, zero);

	return unsat_sum;
}
