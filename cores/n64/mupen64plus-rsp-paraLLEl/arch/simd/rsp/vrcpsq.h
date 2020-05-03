//
// arch/x86_64/rsp/vrcpsq.c
//
// This file is subject to the terms and conditions defined in
// 'LICENSE', which is part of this source code package.
//

#include "../../../rsp/reciprocal.h"

template <bool VRSQ>
inline __m128i rsp_vrcp_vrsq(RSP::CPUState *rsp, int dp, unsigned src, unsigned e, unsigned dest, unsigned de)
{
	uint32_t dp_input, sp_input;
	int32_t input, result;

	int16_t vt;

	int32_t input_mask, data;
	unsigned shift, idx;

	// Get the element from VT.
	vt = rsp->cp2.regs[src].e[e & 0x7];

	dp_input = ((uint32_t)rsp->cp2.div_in << 16) | (uint16_t)vt;
	sp_input = vt;

	input = (dp) ? dp_input : sp_input;
	input_mask = input >> 31;
	data = input ^ input_mask;

	if (input > -32768)
		data -= input_mask;

	// Handle edge cases.
	if (data == 0)
		result = 0x7fffFFFFU;

	else if (input == -32768)
		result = 0xffff0000U;

	// Main case: compute the reciprocal.
	else
	{

		// TODO: Clean this up.
#ifdef _MSC_VER
		unsigned long bsf_index;
		_BitScanReverse(&bsf_index, data);
		shift = 31 - bsf_index;
#else
		shift = __builtin_clz(data);
#endif

		// VRSQ
		if (VRSQ)
		{
			idx = (((unsigned long long)data << shift) & 0x7FC00000U) >> 22;
			idx = ((idx | 0x200) & 0x3FE) | (shift % 2);
			result = rsp_reciprocal_rom[idx];

			result = ((0x10000 | result) << 14) >> ((31 - shift) >> 1);
		}

		// VRCP
		else
		{
			idx = (((unsigned long long)data << shift) & 0x7FC00000U) >> 22;
			result = rsp_reciprocal_rom[idx];

			result = ((0x10000 | result) << 14) >> (31 - shift);
		}

		result = result ^ input_mask;
	}

	// Write out the results.
	rsp->cp2.div_out = result >> 16;
	rsp->cp2.regs[dest].e[de & 0x7] = result;

	return rsp_vect_load_unshuffled_operand(rsp->cp2.regs[dest].e);
}
