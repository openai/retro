//
// arch/x86_64/rsp/vcmp.h
//
// This file is subject to the terms and conditions defined in
// 'LICENSE', which is part of this source code package.
//

static inline __m128i rsp_veq(__m128i vs, __m128i vt, __m128i zero, __m128i *le, __m128i eq, __m128i sign)
{
	__m128i equal = _mm_cmpeq_epi16(vs, vt);

	*le = _mm_andnot_si128(eq, equal);

#ifdef __SSE4_1__
	return _mm_blendv_epi8(vt, vs, *le);
#else
	vs = _mm_and_si128(*le, vs);
	vt = _mm_andnot_si128(*le, vt);
	return _mm_or_si128(vs, vt);
#endif
}

static inline __m128i rsp_vge(__m128i vs, __m128i vt, __m128i zero, __m128i *le, __m128i eq, __m128i sign)
{
	__m128i equal = _mm_cmpeq_epi16(vs, vt);

	__m128i gt = _mm_cmpgt_epi16(vs, vt);
	__m128i equalsign = _mm_and_si128(eq, sign);

	equal = _mm_andnot_si128(equalsign, equal);
	*le = _mm_or_si128(gt, equal);

#ifdef __SSE4_1__
	return _mm_blendv_epi8(vt, vs, *le);
#else
	vs = _mm_and_si128(*le, vs);
	vt = _mm_andnot_si128(*le, vt);
	return _mm_or_si128(vs, vt);
#endif
}

static inline __m128i rsp_vlt(__m128i vs, __m128i vt, __m128i zero, __m128i *le, __m128i eq, __m128i sign)
{
	__m128i equal = _mm_cmpeq_epi16(vs, vt);
	__m128i lt = _mm_cmplt_epi16(vs, vt);

	equal = _mm_and_si128(eq, equal);
	equal = _mm_and_si128(sign, equal);
	*le = _mm_or_si128(lt, equal);

#ifdef __SSE4_1__
	return _mm_blendv_epi8(vt, vs, *le);
#else
	vs = _mm_and_si128(*le, vs);
	vt = _mm_andnot_si128(*le, vt);
	return _mm_or_si128(vs, vt);
#endif
}

static inline __m128i rsp_vne(__m128i vs, __m128i vt, __m128i zero, __m128i *le, __m128i eq, __m128i sign)
{
	__m128i equal = _mm_cmpeq_epi16(vs, vt);
	__m128i nequal = _mm_cmpeq_epi16(equal, zero);

	*le = _mm_and_si128(eq, equal);
	*le = _mm_or_si128(*le, nequal);

#ifdef INTENSE_DEBUG
	for (unsigned i = 0; i < 8; i++)
		fprintf(stderr, "VS[%d] = %d\n", i, reinterpret_cast<int16_t *>(&vs)[i]);
	for (unsigned i = 0; i < 8; i++)
		fprintf(stderr, "VT[%d] = %d\n", i, reinterpret_cast<int16_t *>(&vt)[i]);
#endif

#ifdef __SSE4_1__
	return _mm_blendv_epi8(vt, vs, *le);
#else
	vs = _mm_and_si128(*le, vs);
	vt = _mm_andnot_si128(*le, vt);
	return _mm_or_si128(vs, vt);
#endif
}
