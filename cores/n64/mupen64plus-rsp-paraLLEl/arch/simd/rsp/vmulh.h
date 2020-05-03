//
// arch/x86_64/rsp/vmulh.h
//
// This file is subject to the terms and conditions defined in
// 'LICENSE', which is part of this source code package.
//

template <bool VMADH>
static inline __m128i rsp_vmadh_vmudh(__m128i vs, __m128i vt, __m128i zero, __m128i *acc_lo, __m128i *acc_md,
                                      __m128i *acc_hi)
{
	__m128i lo, hi, overflow_mask;

	lo = _mm_mullo_epi16(vs, vt);
	hi = _mm_mulhi_epi16(vs, vt);

	// VMADH
	if (VMADH)
	{
		// Tricky part: start accumulate everything.
		// Get/keep the carry as we'll add it in later.
		overflow_mask = _mm_adds_epu16(*acc_md, lo);
		*acc_md = _mm_add_epi16(*acc_md, lo);

		overflow_mask = _mm_cmpeq_epi16(*acc_md, overflow_mask);
		overflow_mask = _mm_cmpeq_epi16(overflow_mask, zero);

		hi = _mm_sub_epi16(hi, overflow_mask);
		*acc_hi = _mm_add_epi16(*acc_hi, hi);
	}

	// VMUDH
	else
	{
		*acc_lo = zero;
		*acc_md = lo;
		*acc_hi = hi;
	}

	return rsp_sclamp_acc_tomd(*acc_md, *acc_hi);
}
