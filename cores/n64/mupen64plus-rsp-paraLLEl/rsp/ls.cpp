#include "../state.hpp"

#ifdef TRACE_COP2
#include <stdio.h>
#define TRACE_LS(op) printf(#op " v%u, %u, %d(r%u)\n", rt, e, offset, base)
#else
#define TRACE_LS(op) ((void)0)
#endif

extern "C"
{
	// Using mostly CXD4 implementation as a base here since it's easier to follow.
	// CEN64's implementation seems much better, but takes more effort to port for now.
	// Reading wide words together with SSE4 blend, SSSE3 pshufb, etc should make this much faster.

	// Load 8-bit
	void RSP_LBV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LBV);
		unsigned addr = (rsp->sr[base] + offset * 1) & 0xfff;
		reinterpret_cast<uint8_t *>(rsp->cp2.regs[rt].e)[MES(e)] = READ_MEM_U8(rsp->dmem, addr);
	}

	// Store 8-bit
	void RSP_SBV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(SBV);
		unsigned addr = (rsp->sr[base] + offset * 1) & 0xfff;
		uint8_t v = reinterpret_cast<uint8_t *>(rsp->cp2.regs[rt].e)[MES(e)];

#ifdef INTENSE_DEBUG
		fprintf(stderr, "SBV: 0x%x (0x%x)\n", addr, v);
#endif

		WRITE_MEM_U8(rsp->dmem, addr, v);
	}

	// Load 16-bit
	void RSP_LSV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LSV);
		if (e & 1)
			return;

		unsigned addr = (rsp->sr[base] + offset * 2) & 0xfff;
		unsigned correction = addr & 3;
		if (correction == 3)
			return;

		uint16_t result;
		if (correction == 1)
			result = (READ_MEM_U8(rsp->dmem, addr + 0) << 8) | (READ_MEM_U8(rsp->dmem, addr + 1) << 0);
		else
			result = READ_MEM_U16(rsp->dmem, addr);

		rsp->cp2.regs[rt].e[e >> 1] = result;
	}

	// Store 16-bit
	void RSP_SSV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(SSV);
		unsigned addr = (rsp->sr[base] + offset * 2) & 0xfff;
		uint8_t v0 = reinterpret_cast<uint8_t *>(rsp->cp2.regs[rt].e)[MES(e)];
		uint8_t v1 = reinterpret_cast<uint8_t *>(rsp->cp2.regs[rt].e)[MES((e + 1) & 0xf)];

#ifdef INTENSE_DEBUG
		fprintf(stderr, "SSV: 0x%x (0x%x, 0x%x)\n", addr, v0, v1);
#endif

		WRITE_MEM_U8(rsp->dmem, addr, v0);
		WRITE_MEM_U8(rsp->dmem, (addr + 1) & 0xfff, v1);
	}

	// Load 32-bit
	void RSP_LLV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LLV);
		unsigned addr = (rsp->sr[base] + offset * 4) & 0xfff;
		if (e & 1)
			return;
		if (addr & 1)
			return;
		e >>= 1;

		rsp->cp2.regs[rt].e[e] = READ_MEM_U16(rsp->dmem, addr);
		rsp->cp2.regs[rt].e[(e + 1) & 7] = READ_MEM_U16(rsp->dmem, (addr + 2) & 0xfff);
	}

	// Store 32-bit
	void RSP_SLV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(SLV);
		if ((e & 1) || (e > 0xc))
			return;
		unsigned addr = (rsp->sr[base] + offset * 4) & 0xfff;

#ifdef INTENSE_DEBUG
		fprintf(stderr, "SLV 0x%x, e = %u\n", addr, e);
#endif

		if (addr & 1)
			return;
		e >>= 1;

		uint16_t v0 = rsp->cp2.regs[rt].e[e];
		uint16_t v1 = rsp->cp2.regs[rt].e[e + 1];
		WRITE_MEM_U16(rsp->dmem, addr, v0);
		WRITE_MEM_U16(rsp->dmem, (addr + 2) & 0xfff, v1);
	}

	// Load 64-bit
	void RSP_LDV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LDV);
		if (e & 1)
			return;
		unsigned addr = (rsp->sr[base] + offset * 8) & 0xfff;
		auto *reg = rsp->cp2.regs[rt].e;
		e >>= 1;

		if (addr & 1)
		{
			reg[e + 0] = (READ_MEM_U8(rsp->dmem, addr + 0) << 8) | READ_MEM_U8(rsp->dmem, addr + 1);
			reg[e + 1] = (READ_MEM_U8(rsp->dmem, addr + 2) << 8) | READ_MEM_U8(rsp->dmem, addr + 3);
			reg[e + 2] = (READ_MEM_U8(rsp->dmem, addr + 4) << 8) | READ_MEM_U8(rsp->dmem, addr + 5);
			reg[e + 3] = (READ_MEM_U8(rsp->dmem, addr + 6) << 8) | READ_MEM_U8(rsp->dmem, addr + 7);
		}
		else
		{
			reg[e + 0] = READ_MEM_U16(rsp->dmem, addr);
			reg[e + 1] = READ_MEM_U16(rsp->dmem, (addr + 2) & 0xfff);
			reg[e + 2] = READ_MEM_U16(rsp->dmem, (addr + 4) & 0xfff);
			reg[e + 3] = READ_MEM_U16(rsp->dmem, (addr + 6) & 0xfff);
		}
	}

	// Store 64-bit
	void RSP_SDV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(SDV);
		unsigned addr = (rsp->sr[base] + offset * 8) & 0xfff;

#ifdef INTENSE_DEBUG
		fprintf(stderr, "SDV 0x%x, e = %u\n", addr, e);
#endif

		// Handle illegal scenario.
		if ((e > 8) || (e & 1) || (addr & 1))
		{
			for (unsigned i = 0; i < 8; i++)
			{
				WRITE_MEM_U8(rsp->dmem, (addr + i) & 0xfff,
				             reinterpret_cast<const uint8_t *>(rsp->cp2.regs[rt].e)[MES((e + i) & 0xf)]);
			}
		}
		else
		{
			e >>= 1;
			for (unsigned i = 0; i < 4; i++)
			{
				WRITE_MEM_U16(rsp->dmem, (addr + 2 * i) & 0xfff, rsp->cp2.regs[rt].e[e + i]);
			}
		}
	}

	// Load 8x8-bit into high bits.
	void RSP_LPV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LPV);
		if (e != 0)
			return;

		unsigned addr = (rsp->sr[base] + offset * 8) & 0xfff;
		auto *reg = rsp->cp2.regs[rt].e;
		for (unsigned i = 0; i < 8; i++)
			reg[i] = READ_MEM_U8(rsp->dmem, (addr + i) & 0xfff) << 8;
	}

	void RSP_SPV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(SPV);
		if (e != 0)
			return;
		unsigned addr = (rsp->sr[base] + offset * 8) & 0xfff;
		auto *reg = rsp->cp2.regs[rt].e;
		for (unsigned i = 0; i < 8; i++)
			WRITE_MEM_U8(rsp->dmem, (addr + i) & 0xfff, int16_t(reg[i]) >> 8);
	}

	// Load 8x8-bit into high bits, but shift by 7 instead of 8.
	// Was probably used for certain fixed point algorithms to get more headroom without
	// saturation, but weird nonetheless.
	void RSP_LUV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LUV);
		unsigned addr = (rsp->sr[base] + offset * 8) & 0xfff;
		auto *reg = rsp->cp2.regs[rt].e;

		if (e != 0)
		{
			// Special path for Mia Hamm soccer.
			addr += -e & 0xf;
			for (unsigned b = 0; b < 8; b++)
			{
				reg[b] = READ_MEM_U8(rsp->dmem, addr) << 7;
				--e;
				addr -= e ? 0 : 16;
				++addr;
			}
		}
		else
		{
			for (unsigned i = 0; i < 8; i++)
				reg[i] = READ_MEM_U8(rsp->dmem, (addr + i) & 0xfff) << 7;
		}
	}

	void RSP_SUV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(SUV);
		if (e != 0)
			return;
		unsigned addr = (rsp->sr[base] + offset * 8) & 0xfff;
		auto *reg = rsp->cp2.regs[rt].e;
		for (unsigned i = 0; i < 8; i++)
			WRITE_MEM_U8(rsp->dmem, (addr + i) & 0xfff, int16_t(reg[i]) >> 7);
	}

	// Load 8x8-bits into high bits, but shift by 7 instead of 8.
	// Seems to differ from LUV in that it loads every other byte instead of packed bytes.
	void RSP_LHV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LHV);
		if (e != 0)
			return;
		unsigned addr = (rsp->sr[base] + offset * 16) & 0xfff;
		if (addr & 0xe)
			return;

		auto *reg = rsp->cp2.regs[rt].e;
		for (unsigned i = 0; i < 8; i++)
			reg[i] = READ_MEM_U8(rsp->dmem, addr + 2 * i) << 7;
	}

	void RSP_SHV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(SHV);
		if (e != 0)
			return;
		unsigned addr = (rsp->sr[base] + offset * 16) & 0xfff;
		auto *reg = rsp->cp2.regs[rt].e;
		for (unsigned i = 0; i < 8; i++)
			WRITE_MEM_U8(rsp->dmem, (addr + 2 * i) & 0xfff, int16_t(reg[i]) >> 7);
	}

	// No idea what the purpose of this is.
	void RSP_SFV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(SFV);
		unsigned addr = (rsp->sr[base] + offset * 16) & 0xff3;
		auto *reg = rsp->cp2.regs[rt].e;
		switch (e)
		{
		case 0:
			WRITE_MEM_U8(rsp->dmem, (addr + 0) & 0xfff, int16_t(reg[0]) >> 7);
			WRITE_MEM_U8(rsp->dmem, (addr + 4) & 0xfff, int16_t(reg[1]) >> 7);
			WRITE_MEM_U8(rsp->dmem, (addr + 8) & 0xfff, int16_t(reg[2]) >> 7);
			WRITE_MEM_U8(rsp->dmem, (addr + 12) & 0xfff, int16_t(reg[3]) >> 7);
			break;

		case 8:
			WRITE_MEM_U8(rsp->dmem, (addr + 0) & 0xfff, int16_t(reg[4]) >> 7);
			WRITE_MEM_U8(rsp->dmem, (addr + 4) & 0xfff, int16_t(reg[5]) >> 7);
			WRITE_MEM_U8(rsp->dmem, (addr + 8) & 0xfff, int16_t(reg[6]) >> 7);
			WRITE_MEM_U8(rsp->dmem, (addr + 12) & 0xfff, int16_t(reg[7]) >> 7);
			break;

		default:
			break;
		}
	}

	// Loads full 128-bit register, however, it seems to handle unaligned addresses in a very
	// strange way.
	void RSP_LQV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LQV);
		if (e & 1)
			return;
		unsigned addr = (rsp->sr[base] + offset * 16) & 0xfff;

#ifdef INTENSE_DEBUG
		fprintf(stderr, "LQV: 0x%x, e = %u, vt = %u, base = %u\n", addr, e, rt, base);
#endif

		if (addr & 1)
			return;

		unsigned b = (addr & 0xf) >> 1;
		e >>= 1;

		auto *reg = rsp->cp2.regs[rt].e;
		for (unsigned i = b; i < 8; i++, e++, addr += 2)
			reg[e] = READ_MEM_U16(rsp->dmem, addr & 0xfff);
	}

	void RSP_SQV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(SQV);
		unsigned addr = (rsp->sr[base] + offset * 16) & 0xfff;
		if (addr & 1)
			return;

		unsigned b = addr & 0xf;

		auto *reg = rsp->cp2.regs[rt].e;

		if (e != 0)
		{
			// Mia Hamm Soccer
			for (unsigned i = 0; i < 16 - b; i++, addr++)
			{
				WRITE_MEM_U8(rsp->dmem, addr & 0xfff, reinterpret_cast<const uint8_t *>(reg)[MES((e + i) & 0xf)]);
			}
		}
		else
		{
			b >>= 1;
			for (unsigned i = b; i < 8; i++, e++, addr += 2)
				WRITE_MEM_U16(rsp->dmem, addr & 0xfff, reg[e]);
		}
	}

	// Complements LQV?
	void RSP_LRV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LRV);
		if (e != 0)
			return;
		unsigned addr = (rsp->sr[base] + offset * 16) & 0xfff;
		if (addr & 1)
			return;

		unsigned b = (addr & 0xf) >> 1;
		addr &= ~0xf;

		auto *reg = rsp->cp2.regs[rt].e;
		for (e = 8 - b; e < 8; e++, addr += 2)
			reg[e] = READ_MEM_U16(rsp->dmem, addr & 0xfff);
	}

	void RSP_SRV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(SRV);
		if (e != 0)
			return;
		unsigned addr = (rsp->sr[base] + offset * 16) & 0xfff;
		if (addr & 1)
			return;

		unsigned b = (addr & 0xf) >> 1;
		addr &= ~0xf;

		auto *reg = rsp->cp2.regs[rt].e;
		for (e = 8 - b; e < 8; e++, addr += 2)
			WRITE_MEM_U16(rsp->dmem, addr & 0xfff, reg[e]);
	}

	// Transposed stuff?
	void RSP_LTV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(LTV);
		if (e & 1)
			return;
		if (rt & 7)
			return;
		unsigned addr = (rsp->sr[base] + offset * 16) & 0xfff;
		if (addr & 0xf)
			return;

		for (unsigned i = 0; i < 8; i++)
			rsp->cp2.regs[rt + i].e[(-e / 2 + i) & 7] = READ_MEM_U16(rsp->dmem, addr + 2 * i);
	}

	void RSP_STV(RSP::CPUState *rsp, unsigned rt, unsigned e, int offset, unsigned base)
	{
		TRACE_LS(STV);
		if (e & 1)
			return;
		if (rt & 7)
			return;
		unsigned addr = (rsp->sr[base] + offset * 16) & 0xfff;
		if (addr & 0xf)
			return;

		for (unsigned i = 0; i < 8; i++)
		{
			WRITE_MEM_U16(rsp->dmem, addr + 2 * i, rsp->cp2.regs[rt + ((e / 2 + i) & 7)].e[i]);
		}
	}
}
