//
// arch/x86_64/rsp/clamp.h
//
// This file is subject to the terms and conditions defined in
// 'LICENSE', which is part of this source code package.
//

static inline __m128i rsp_sclamp_acc_tomd(__m128i acc_md, __m128i acc_hi)
{
	__m128i l = _mm_unpacklo_epi16(acc_md, acc_hi);
	__m128i h = _mm_unpackhi_epi16(acc_md, acc_hi);
	return _mm_packs_epi32(l, h);
}

static inline __m128i rsp_uclamp_acc(__m128i val, __m128i acc_md, __m128i acc_hi, __m128i zero)
{
	__m128i clamp_mask, clamped_val;
	__m128i hi_sign_check, md_sign_check;
	__m128i md_negative, hi_negative;
	__m128i tmp;

	hi_negative = _mm_srai_epi16(acc_hi, 15);
	md_negative = _mm_srai_epi16(acc_md, 15);

	// We don't have to clamp if the HI part of the
	// accumulator is sign-extended down to the MD part.
	hi_sign_check = _mm_cmpeq_epi16(hi_negative, acc_hi);
	md_sign_check = _mm_cmpeq_epi16(hi_negative, md_negative);
	clamp_mask = _mm_and_si128(md_sign_check, hi_sign_check);

	// Generate the value in the event we need to clamp.
	//   * hi_negative, mid_sign => xxxx
	//   * hi_negative, !mid_sign => 0000
	//   * !hi_negative, mid_sign => FFFF
	//   * !hi_negative, !mid_sign => xxxx
	clamped_val = _mm_cmpeq_epi16(hi_negative, zero);

#ifndef __SSE4_1__
	tmp = _mm_and_si128(clamp_mask, val);
	val = _mm_andnot_si128(clamp_mask, clamped_val);
	return _mm_or_si128(val, tmp);
#else
	return _mm_blendv_epi8(clamped_val, val, clamp_mask);
#endif
}
