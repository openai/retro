//
// arch/x86_64/rsp/vmudh.h
//
// This file is subject to the terms and conditions defined in
// 'LICENSE', which is part of this source code package.
//

static inline __m128i rsp_vmudh(__m128i vs, __m128i vt, __m128i *acc_md, __m128i *acc_hi)
{
	*acc_md = _mm_mullo_epi16(vs, vt);
	*acc_hi = _mm_mulhi_epi16(vs, vt);

	return rsp_sclamp_acc_tomd(*acc_md, *acc_hi);
}
