//
// arch/x86_64/rsp/vmacf.h
//
// This file is subject to the terms and conditions defined in
// 'LICENSE', which is part of this source code package.
//

template <bool VMACU>
static inline __m128i rsp_vmacf_vmacu(__m128i vs, __m128i vt, __m128i zero, __m128i *acc_lo, __m128i *acc_md,
                                      __m128i *acc_hi)
{
	__m128i overflow_hi_mask, overflow_md_mask;
	__m128i lo, md, hi, carry, overflow_mask;

	// Get the product and shift it over
	// being sure to save the carries.
	lo = _mm_mullo_epi16(vs, vt);
	hi = _mm_mulhi_epi16(vs, vt);

	md = _mm_slli_epi16(hi, 1);
	carry = _mm_srli_epi16(lo, 15);
	hi = _mm_srai_epi16(hi, 15);
	md = _mm_or_si128(md, carry);
	lo = _mm_slli_epi16(lo, 1);

	// Tricky part: start accumulating everything.
	// Get/keep the carry as we'll add it in later.
	overflow_mask = _mm_adds_epu16(*acc_lo, lo);
	*acc_lo = _mm_add_epi16(*acc_lo, lo);

	overflow_mask = _mm_cmpeq_epi16(*acc_lo, overflow_mask);
	overflow_mask = _mm_cmpeq_epi16(overflow_mask, zero);

	// Add in the carry. If the middle portion is
	// already 0xFFFF and we have a carry, we have
	// to carry the all the way up to hi.
	md = _mm_sub_epi16(md, overflow_mask);
	carry = _mm_cmpeq_epi16(md, zero);
	carry = _mm_and_si128(carry, overflow_mask);
	hi = _mm_sub_epi16(hi, carry);

	// Accumulate the middle portion.
	overflow_mask = _mm_adds_epu16(*acc_md, md);
	*acc_md = _mm_add_epi16(*acc_md, md);

	overflow_mask = _mm_cmpeq_epi16(*acc_md, overflow_mask);
	overflow_mask = _mm_cmpeq_epi16(overflow_mask, zero);

	// Finish up the accumulation of the... accumulator.
	*acc_hi = _mm_add_epi16(*acc_hi, hi);
	*acc_hi = _mm_sub_epi16(*acc_hi, overflow_mask);

	// VMACU
	if (VMACU)
	{
		overflow_hi_mask = _mm_srai_epi16(*acc_hi, 15);
		overflow_md_mask = _mm_srai_epi16(*acc_md, 15);
		md = _mm_or_si128(overflow_md_mask, *acc_md);
		overflow_mask = _mm_cmpgt_epi16(*acc_hi, zero);
		md = _mm_andnot_si128(overflow_hi_mask, md);
		return _mm_or_si128(overflow_mask, md);
	}

	// VMACF
	else
		return rsp_sclamp_acc_tomd(*acc_md, *acc_hi);
}
