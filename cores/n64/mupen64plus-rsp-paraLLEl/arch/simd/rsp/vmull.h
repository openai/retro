//
// arch/x86_64/rsp/vmadl.h
//
// This file is subject to the terms and conditions defined in
// 'LICENSE', which is part of this source code package.
//

template <bool VMADL>
static inline __m128i rsp_vmadl_vmudl(__m128i vs, __m128i vt, __m128i zero, __m128i *acc_lo, __m128i *acc_md,
                                      __m128i *acc_hi)
{
	__m128i hi, overflow_mask;

	hi = _mm_mulhi_epu16(vs, vt);

	// VMADL
	if (VMADL)
	{

		// Tricky part: start accumulate everything.
		// Get/keep the carry as we'll add it in later.
		overflow_mask = _mm_adds_epu16(*acc_lo, hi);
		*acc_lo = _mm_add_epi16(*acc_lo, hi);

		overflow_mask = _mm_cmpeq_epi16(*acc_lo, overflow_mask);
		overflow_mask = _mm_cmpeq_epi16(overflow_mask, zero);
		hi = _mm_sub_epi16(zero, overflow_mask);

		// Check for overflow of the upper sum.
		//
		// TODO: Since hi can only be {0,1}, we should
		// be able to generalize this for performance.
		overflow_mask = _mm_adds_epu16(*acc_md, hi);
		*acc_md = _mm_add_epi16(*acc_md, hi);

		overflow_mask = _mm_cmpeq_epi16(*acc_md, overflow_mask);
		overflow_mask = _mm_cmpeq_epi16(overflow_mask, zero);

		// Finish up the accumulation of the... accumulator.
		// Since the product was unsigned, only worry about
		// positive overflow (i.e.: borrowing not possible).
		*acc_hi = _mm_sub_epi16(*acc_hi, overflow_mask);

		return rsp_uclamp_acc(*acc_lo, *acc_md, *acc_hi, zero);
	}

	// VMUDL
	else
	{
		*acc_lo = hi;
		*acc_md = zero;
		*acc_hi = zero;

		return hi;
	}
}
