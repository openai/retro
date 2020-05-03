//
// arch/x86_64/rsp/vch.h
//
// This file is subject to the terms and conditions defined in
// 'LICENSE', which is part of this source code package.
//

static inline __m128i rsp_vch(__m128i vs, __m128i vt, __m128i zero, __m128i *ge, __m128i *le, __m128i *eq,
                              __m128i *sign, __m128i *vce)
{

	__m128i sign_negvt, vt_neg;
	__m128i diff, diff_zero, diff_sel_mask;
	__m128i diff_gez, diff_lez;

	// sign = (vs ^ vt) < 0
	*sign = _mm_xor_si128(vs, vt);
	*sign = _mm_cmplt_epi16(*sign, zero);

	// sign_negvt = sign ? -vt : vt
	sign_negvt = _mm_xor_si128(vt, *sign);
	sign_negvt = _mm_sub_epi16(sign_negvt, *sign);

	// Compute diff, diff_zero:
	diff = _mm_sub_epi16(vs, sign_negvt);
	diff_zero = _mm_cmpeq_epi16(diff, zero);

	// Compute le/ge:
	vt_neg = _mm_cmplt_epi16(vt, zero);
	diff_lez = _mm_cmpgt_epi16(diff, zero);
	diff_gez = _mm_or_si128(diff_lez, diff_zero);
	diff_lez = _mm_cmpeq_epi16(zero, diff_lez);

#ifdef __SSE4_1__
	*ge = _mm_blendv_epi8(diff_gez, vt_neg, *sign);
	*le = _mm_blendv_epi8(vt_neg, diff_lez, *sign);
#else
	*ge = _mm_and_si128(*sign, vt_neg);
	diff_gez = _mm_andnot_si128(*sign, diff_gez);
	*ge = _mm_or_si128(*ge, diff_gez);

	*le = _mm_and_si128(*sign, diff_lez);
	diff_lez = _mm_andnot_si128(*sign, vt_neg);
	*le = _mm_or_si128(*le, diff_lez);
#endif

	// Compute vce:
	*vce = _mm_cmpeq_epi16(diff, *sign);
	*vce = _mm_and_si128(*vce, *sign);

	// Compute !eq:
	*eq = _mm_or_si128(diff_zero, *vce);
	*eq = _mm_cmpeq_epi16(*eq, zero);

	// Compute result:
#ifdef __SSE4_1__
	diff_sel_mask = _mm_blendv_epi8(*ge, *le, *sign);
	return _mm_blendv_epi8(vs, sign_negvt, diff_sel_mask);
#else
	diff_lez = _mm_and_si128(*sign, *le);
	diff_gez = _mm_andnot_si128(*sign, *ge);
	diff_sel_mask = _mm_or_si128(diff_lez, diff_gez);

	diff_lez = _mm_and_si128(diff_sel_mask, sign_negvt);
	diff_gez = _mm_andnot_si128(diff_sel_mask, vs);
	return _mm_or_si128(diff_lez, diff_gez);
#endif
}
