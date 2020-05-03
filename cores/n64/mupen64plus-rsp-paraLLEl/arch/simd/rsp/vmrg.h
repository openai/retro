//
// arch/x86_64/rsp/vmrg.h
//
// This file is subject to the terms and conditions defined in
// 'LICENSE', which is part of this source code package.
//

static inline __m128i rsp_vmrg(__m128i vs, __m128i vt, __m128i le)
{
#ifdef __SSE4_1__
	return _mm_blendv_epi8(vt, vs, le);
#else
	vs = _mm_and_si128(le, vs);
	vt = _mm_andnot_si128(le, vt);
	return _mm_or_si128(vs, vt);
#endif
}
