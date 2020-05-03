#include "../state.hpp"

extern "C"
{

	void RSP_CFC2(RSP::CPUState *rsp, unsigned rt, unsigned rd)
	{
		unsigned src = rd & 3;
		if (src == 3)
			src = 2;

		int16_t res = rsp_get_flags(rsp->cp2.flags[src].e);
		if (rt)
			rsp->sr[rt] = res;
	}

	void RSP_CTC2(RSP::CPUState *rsp, unsigned rt, unsigned rd)
	{
		rt = rsp->sr[rt] & 0xffff;

		unsigned dst = rd & 3;
		if (dst >= 2)
		{
			rt &= 0xff;
			dst = 2;
		}
		rsp_set_flags(rsp->cp2.flags[dst].e, rt);
	}

	void RSP_MTC2(RSP::CPUState *rsp, unsigned rt, unsigned rd, unsigned element)
	{
		uint16_t *e = rsp->cp2.regs[rd].e;

#ifdef INTENSE_DEBUG
		fprintf(stderr, "MTC2, rt = %u, [rt] = 0x%x, rd = %u, e = %u\n", rt, rsp->sr[rt], rd, element);
#endif

		unsigned lo = element >> 1;
		rt = rsp->sr[rt];

		if (element & 1)
		{
			unsigned hi = (element + 1) >> 1;
			e[lo] = (e[lo] & 0xff00) | ((rt >> 8) & 0xff);
			e[hi] = (e[lo] & 0x00ff) | ((rt & 0xff) << 8);
		}
		else
			e[lo] = rt;
	}

	void RSP_MFC2(RSP::CPUState *rsp, unsigned rt, unsigned rd, unsigned element)
	{
		if (rt == 0)
			return;

		const uint16_t *e = rsp->cp2.regs[rd].e;
		unsigned lo = element >> 1;

		if (element & 1)
		{
			unsigned hi = ((element + 1) >> 1) & 7;
			uint16_t high = e[lo] << 8;
			uint8_t low = e[hi] >> 8;
			rsp->sr[rt] = int16_t(high | low);
		}
		else
			rsp->sr[rt] = int16_t(e[lo]);
	}
}
