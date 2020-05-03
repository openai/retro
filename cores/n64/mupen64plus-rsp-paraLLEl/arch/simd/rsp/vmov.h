//
// arch/x86_64/rsp/vmov.c
//
// This file is subject to the terms and conditions defined in
// 'LICENSE', which is part of this source code package.
//

inline __m128i rsp_vmov(RSP::CPUState *rsp, unsigned src, unsigned e, unsigned dest, unsigned de)
{
	uint16_t data;

	// Get the element from VT.
	data = rsp->cp2.regs[src].e[e & 0x7];

	// Write out the upper part of the result.
	rsp->cp2.regs[dest].e[de & 0x7] = data;
	return rsp_vect_load_unshuffled_operand(rsp->cp2.regs[dest].e);
}
