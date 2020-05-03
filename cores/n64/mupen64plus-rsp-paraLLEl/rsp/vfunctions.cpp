//
// rsp/vfunctions.c: RSP vector execution functions.
//
// CEN64: Cycle-Accurate Nintendo 64 Emulator.
// Copyright (C) 2015, Tyler J. Stachecki.
//
// This file is subject to the terms and conditions defined in
// 'LICENSE', which is part of this source code package.
//

#include "../state.hpp"
#include "../rsp_op.hpp"
#include "rsp_impl.h"
#include <stdio.h>

#define LOAD_VS() rsp_vect_load_unshuffled_operand(rsp->cp2.regs[vs].e)
#define LOAD_VT() rsp_vect_load_and_shuffle_operand(rsp->cp2.regs[vt].e, e)
#define STORE_RESULT() rsp_vect_write_operand(rsp->cp2.regs[vd].e, result)

#ifdef TRACE_COP2
#define TRACE_VU(op) printf(#op " v%u, v%u, v%u[%u]\n", vd, vs, vt, e)
#else
#define TRACE_VU(op) ((void)0)
#endif

extern "C"
{
	//
	// VABS
	//
	void RSP_VABS(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VABS);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t acc_lo;
		rsp_vect_t result = rsp_vabs(LOAD_VS(), LOAD_VT(), &acc_lo);
		write_acc_lo(acc, acc_lo);
		rsp_vect_write_operand(rsp->cp2.regs[vd].e, result);
	}

	//
	// VADD
	//
	void RSP_VADD(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VADD);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t carry, acc_lo;

		carry = read_vco_lo(rsp->cp2.flags[RSP::RSP_VCO].e);
		rsp_vect_t result = rsp_vadd(LOAD_VS(), LOAD_VT(), carry, &acc_lo);

		write_vco_hi(rsp->cp2.flags[RSP::RSP_VCO].e, rsp_vzero());
		write_vco_lo(rsp->cp2.flags[RSP::RSP_VCO].e, rsp_vzero());
		write_acc_lo(acc, acc_lo);
		STORE_RESULT();
	}

	//
	// VADDC
	//
	void RSP_VADDC(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VADDC);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t sn;

		rsp_vect_t result = rsp_vaddc(LOAD_VS(), LOAD_VT(), rsp_vzero(), &sn);
		write_vco_hi(rsp->cp2.flags[RSP::RSP_VCO].e, rsp_vzero()); // TODO: Confirm.
		write_vco_lo(rsp->cp2.flags[RSP::RSP_VCO].e, sn);
		write_acc_lo(acc, result);
		STORE_RESULT();
	}

	//
	// VAND
	// VNAND
	//
	void RSP_VAND(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VAND);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t result = rsp_vand(LOAD_VS(), LOAD_VT());
		write_acc_lo(acc, result);
		STORE_RESULT();
	}

	void RSP_VNAND(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VNAND);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t result = rsp_vnand(LOAD_VS(), LOAD_VT());
		write_acc_lo(acc, result);
		STORE_RESULT();
	}

	//
	// VCH
	//
	void RSP_VCH(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VCH);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t ge, le, sign, eq, vce;

		rsp_vect_t result = rsp_vch(LOAD_VS(), LOAD_VT(), rsp_vzero(), &ge, &le, &eq, &sign, &vce);

		write_vcc_hi(rsp->cp2.flags[RSP::RSP_VCC].e, ge);
		write_vcc_lo(rsp->cp2.flags[RSP::RSP_VCC].e, le);
		write_vco_hi(rsp->cp2.flags[RSP::RSP_VCO].e, eq);
		write_vco_lo(rsp->cp2.flags[RSP::RSP_VCO].e, sign);
		write_vce(rsp->cp2.flags[RSP::RSP_VCE].e, vce);
		write_acc_lo(acc, result);
		STORE_RESULT();
	}

	//
	// VCL
	//
	void RSP_VCL(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VCL);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t ge, le, eq, sign, vce;

		ge = read_vcc_hi(rsp->cp2.flags[RSP::RSP_VCC].e);
		le = read_vcc_lo(rsp->cp2.flags[RSP::RSP_VCC].e);
		eq = read_vco_hi(rsp->cp2.flags[RSP::RSP_VCO].e);
		sign = read_vco_lo(rsp->cp2.flags[RSP::RSP_VCO].e);
		vce = read_vce(rsp->cp2.flags[RSP::RSP_VCE].e);

		rsp_vect_t result = rsp_vcl(LOAD_VS(), LOAD_VT(), rsp_vzero(), &ge, &le, eq, sign, vce);

		write_vcc_hi(rsp->cp2.flags[RSP::RSP_VCC].e, ge);
		write_vcc_lo(rsp->cp2.flags[RSP::RSP_VCC].e, le);
		write_vco_hi(rsp->cp2.flags[RSP::RSP_VCO].e, rsp_vzero());
		write_vco_lo(rsp->cp2.flags[RSP::RSP_VCO].e, rsp_vzero());
		write_vce(rsp->cp2.flags[RSP::RSP_VCE].e, rsp_vzero());
		write_acc_lo(acc, result);
		STORE_RESULT();
	}

	//
	// VCR
	//
	void RSP_VCR(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VCR);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t ge, le;

		rsp_vect_t result = rsp_vcr(LOAD_VS(), LOAD_VT(), rsp_vzero(), &ge, &le);

#ifdef INTENSE_DEBUG
		for (unsigned i = 0; i < 8; i++)
			fprintf(stderr, "VD[%d] = %d\n", i, reinterpret_cast<int16_t *>(&result)[i]);
#endif

		write_vcc_hi(rsp->cp2.flags[RSP::RSP_VCC].e, ge);
		write_vcc_lo(rsp->cp2.flags[RSP::RSP_VCC].e, le);
		write_vco_hi(rsp->cp2.flags[RSP::RSP_VCO].e, rsp_vzero());
		write_vco_lo(rsp->cp2.flags[RSP::RSP_VCO].e, rsp_vzero());
		write_vce(rsp->cp2.flags[RSP::RSP_VCE].e, rsp_vzero());
		write_acc_lo(acc, result);
		STORE_RESULT();
	}

	//
	// VEQ
	// VGE
	// VLT
	// VNE
	//
	void RSP_VEQ(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VEQ);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t le, eq, sign;

		eq = read_vco_hi(rsp->cp2.flags[RSP::RSP_VCO].e);
		sign = read_vco_lo(rsp->cp2.flags[RSP::RSP_VCO].e);

		rsp_vect_t result = rsp_veq(LOAD_VS(), LOAD_VT(), rsp_vzero(), &le, eq, sign);

		write_vcc_hi(rsp->cp2.flags[RSP::RSP_VCC].e, rsp_vzero());
		write_vcc_lo(rsp->cp2.flags[RSP::RSP_VCC].e, le);
		write_vco_hi(rsp->cp2.flags[RSP::RSP_VCO].e, rsp_vzero());
		write_vco_lo(rsp->cp2.flags[RSP::RSP_VCO].e, rsp_vzero());
		write_acc_lo(acc, result);
		STORE_RESULT();
	}

	void RSP_VGE(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VGE);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t le, eq, sign;

		eq = read_vco_hi(rsp->cp2.flags[RSP::RSP_VCO].e);
		sign = read_vco_lo(rsp->cp2.flags[RSP::RSP_VCO].e);

		rsp_vect_t result = rsp_vge(LOAD_VS(), LOAD_VT(), rsp_vzero(), &le, eq, sign);

		write_vcc_hi(rsp->cp2.flags[RSP::RSP_VCC].e, rsp_vzero());
		write_vcc_lo(rsp->cp2.flags[RSP::RSP_VCC].e, le);
		write_vco_hi(rsp->cp2.flags[RSP::RSP_VCO].e, rsp_vzero());
		write_vco_lo(rsp->cp2.flags[RSP::RSP_VCO].e, rsp_vzero());
		write_acc_lo(acc, result);
		STORE_RESULT();
	}

	void RSP_VLT(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VLT);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t le, eq, sign;

		eq = read_vco_hi(rsp->cp2.flags[RSP::RSP_VCO].e);
		sign = read_vco_lo(rsp->cp2.flags[RSP::RSP_VCO].e);

		rsp_vect_t result = rsp_vlt(LOAD_VS(), LOAD_VT(), rsp_vzero(), &le, eq, sign);

		write_vcc_hi(rsp->cp2.flags[RSP::RSP_VCC].e, rsp_vzero());
		write_vcc_lo(rsp->cp2.flags[RSP::RSP_VCC].e, le);
		write_vco_hi(rsp->cp2.flags[RSP::RSP_VCO].e, rsp_vzero());
		write_vco_lo(rsp->cp2.flags[RSP::RSP_VCO].e, rsp_vzero());
		write_acc_lo(acc, result);
		STORE_RESULT();
	}

	void RSP_VNE(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VNE);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t le, eq, sign;

		eq = read_vco_hi(rsp->cp2.flags[RSP::RSP_VCO].e);
		sign = read_vco_lo(rsp->cp2.flags[RSP::RSP_VCO].e);

		rsp_vect_t result = rsp_vne(LOAD_VS(), LOAD_VT(), rsp_vzero(), &le, eq, sign);
#ifdef INTENSE_DEBUG
		for (unsigned i = 0; i < 8; i++)
			fprintf(stderr, "VD[%d] = %d\n", i, reinterpret_cast<int16_t *>(&result)[i]);
#endif

		write_vcc_hi(rsp->cp2.flags[RSP::RSP_VCC].e, rsp_vzero());
		write_vcc_lo(rsp->cp2.flags[RSP::RSP_VCC].e, le);
		write_vco_hi(rsp->cp2.flags[RSP::RSP_VCO].e, rsp_vzero());
		write_vco_lo(rsp->cp2.flags[RSP::RSP_VCO].e, rsp_vzero());
		write_acc_lo(acc, result);
		STORE_RESULT();
	}

	//
	// VINVALID
	//
	void RSP_VINVALID(RSP::CPUState *, unsigned, unsigned, unsigned, unsigned)
	{
		fprintf(stderr, "Unimplemented ...\n");
	}

	//
	// VMACF
	// VMACU
	//
	void RSP_VMACF(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VMACF);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t acc_lo, acc_md, acc_hi, result;
		acc_lo = read_acc_lo(acc);
		acc_md = read_acc_md(acc);
		acc_hi = read_acc_hi(acc);

		result = rsp_vmacf_vmacu<false>(LOAD_VS(), LOAD_VT(), rsp_vzero(), &acc_lo, &acc_md, &acc_hi);

		write_acc_lo(acc, acc_lo);
		write_acc_md(acc, acc_md);
		write_acc_hi(acc, acc_hi);
		STORE_RESULT();
	}

	void RSP_VMACU(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VMACU);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t acc_lo, acc_md, acc_hi, result;
		acc_lo = read_acc_lo(acc);
		acc_md = read_acc_md(acc);
		acc_hi = read_acc_hi(acc);

		result = rsp_vmacf_vmacu<true>(LOAD_VS(), LOAD_VT(), rsp_vzero(), &acc_lo, &acc_md, &acc_hi);

		write_acc_lo(acc, acc_lo);
		write_acc_md(acc, acc_md);
		write_acc_hi(acc, acc_hi);
		STORE_RESULT();
	}

	//
	// VMADH
	// VMUDH
	//
	void RSP_VMADH(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VMADH);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t acc_lo, acc_md, acc_hi, result;

		acc_lo = read_acc_lo(acc);
		acc_md = read_acc_md(acc);
		acc_hi = read_acc_hi(acc);

		result = rsp_vmadh_vmudh<true>(LOAD_VS(), LOAD_VT(), rsp_vzero(), &acc_lo, &acc_md, &acc_hi);

		write_acc_lo(acc, acc_lo);
		write_acc_md(acc, acc_md);
		write_acc_hi(acc, acc_hi);
		STORE_RESULT();
	}

	void RSP_VMUDH(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VMUDH);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t acc_lo, acc_md, acc_hi, result;

		acc_lo = read_acc_lo(acc);
		acc_md = read_acc_md(acc);
		acc_hi = read_acc_hi(acc);

		result = rsp_vmadh_vmudh<false>(LOAD_VS(), LOAD_VT(), rsp_vzero(), &acc_lo, &acc_md, &acc_hi);

		write_acc_lo(acc, acc_lo);
		write_acc_md(acc, acc_md);
		write_acc_hi(acc, acc_hi);
		STORE_RESULT();
	}

	//
	// VMADL
	// VMUDL
	//
	void RSP_VMADL(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VMADL);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t acc_lo, acc_md, acc_hi, result;

		acc_lo = read_acc_lo(acc);
		acc_md = read_acc_md(acc);
		acc_hi = read_acc_hi(acc);

		result = rsp_vmadl_vmudl<true>(LOAD_VS(), LOAD_VT(), rsp_vzero(), &acc_lo, &acc_md, &acc_hi);

		write_acc_lo(acc, acc_lo);
		write_acc_md(acc, acc_md);
		write_acc_hi(acc, acc_hi);
		STORE_RESULT();
	}

	void RSP_VMUDL(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VMUDL);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t acc_lo, acc_md, acc_hi, result;

		acc_lo = read_acc_lo(acc);
		acc_md = read_acc_md(acc);
		acc_hi = read_acc_hi(acc);

		result = rsp_vmadl_vmudl<false>(LOAD_VS(), LOAD_VT(), rsp_vzero(), &acc_lo, &acc_md, &acc_hi);

		write_acc_lo(acc, acc_lo);
		write_acc_md(acc, acc_md);
		write_acc_hi(acc, acc_hi);
		STORE_RESULT();
	}

	//
	// VMADM
	// VMUDM
	//
	void RSP_VMADM(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VMADM);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t acc_lo, acc_md, acc_hi, result;

		acc_lo = read_acc_lo(acc);
		acc_md = read_acc_md(acc);
		acc_hi = read_acc_hi(acc);

		result = rsp_vmadm_vmudm<true>(LOAD_VS(), LOAD_VT(), rsp_vzero(), &acc_lo, &acc_md, &acc_hi);

		write_acc_lo(acc, acc_lo);
		write_acc_md(acc, acc_md);
		write_acc_hi(acc, acc_hi);
		STORE_RESULT();
	}

	void RSP_VMUDM(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VMUDM);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t acc_lo, acc_md, acc_hi, result;

		acc_lo = read_acc_lo(acc);
		acc_md = read_acc_md(acc);
		acc_hi = read_acc_hi(acc);

		result = rsp_vmadm_vmudm<false>(LOAD_VS(), LOAD_VT(), rsp_vzero(), &acc_lo, &acc_md, &acc_hi);

		write_acc_lo(acc, acc_lo);
		write_acc_md(acc, acc_md);
		write_acc_hi(acc, acc_hi);
		STORE_RESULT();
	}

	//
	// VMADN
	// VMUDN
	//
	void RSP_VMADN(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VMADN);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t acc_lo, acc_md, acc_hi, result;

		acc_lo = read_acc_lo(acc);
		acc_md = read_acc_md(acc);
		acc_hi = read_acc_hi(acc);

		result = rsp_vmadn_vmudn<true>(LOAD_VS(), LOAD_VT(), rsp_vzero(), &acc_lo, &acc_md, &acc_hi);

		write_acc_lo(acc, acc_lo);
		write_acc_md(acc, acc_md);
		write_acc_hi(acc, acc_hi);
		STORE_RESULT();
	}

	void RSP_VMUDN(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VMUDN);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t acc_lo, acc_md, acc_hi, result;

		acc_lo = read_acc_lo(acc);
		acc_md = read_acc_md(acc);
		acc_hi = read_acc_hi(acc);

		result = rsp_vmadn_vmudn<false>(LOAD_VS(), LOAD_VT(), rsp_vzero(), &acc_lo, &acc_md, &acc_hi);

		write_acc_lo(acc, acc_lo);
		write_acc_md(acc, acc_md);
		write_acc_hi(acc, acc_hi);
		STORE_RESULT();
	}

	//
	// VMOV
	//
	void RSP_VMOV(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VMOV);
		uint16_t *acc = rsp->cp2.acc.e;
		unsigned de = vs & 0x7;
		write_acc_lo(acc, LOAD_VT());
		__m128i result = rsp_vmov(rsp, vt, e, vd, de);
		STORE_RESULT();
	}

	//
	// VMRG
	//
	void RSP_VMRG(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VMRG);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t le;

		le = read_vcc_lo(rsp->cp2.flags[RSP::RSP_VCC].e);
		rsp_vect_t result = rsp_vmrg(LOAD_VS(), LOAD_VT(), le);
		write_vco_hi(rsp->cp2.flags[RSP::RSP_VCO].e, rsp_vzero());
		write_vco_lo(rsp->cp2.flags[RSP::RSP_VCO].e, rsp_vzero());
		write_acc_lo(acc, result);
		STORE_RESULT();
	}

	//
	// VMULF
	// VMULU
	//
	void RSP_VMULF(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VMULF);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t acc_lo, acc_md, acc_hi, result;

		result = rsp_vmulf_vmulu<false>(LOAD_VS(), LOAD_VT(), rsp_vzero(), &acc_lo, &acc_md, &acc_hi);

		write_acc_lo(acc, acc_lo);
		write_acc_md(acc, acc_md);
		write_acc_hi(acc, acc_hi);
		STORE_RESULT();
	}

	void RSP_VMULU(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VMULU);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t acc_lo, acc_md, acc_hi, result;

		result = rsp_vmulf_vmulu<true>(LOAD_VS(), LOAD_VT(), rsp_vzero(), &acc_lo, &acc_md, &acc_hi);

		write_acc_lo(acc, acc_lo);
		write_acc_md(acc, acc_md);
		write_acc_hi(acc, acc_hi);
		STORE_RESULT();
	}

	//
	// VNOP
	//
	void RSP_VNOP(RSP::CPUState *, unsigned, unsigned, unsigned, unsigned)
	{
	}

	//
	// VOR
	// VNOR
	//
	void RSP_VOR(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VOR);
		uint16_t *acc = rsp->cp2.acc.e;

		rsp_vect_t result = rsp_vor(LOAD_VS(), LOAD_VT());

		write_acc_lo(acc, result);
		STORE_RESULT();
	}

	void RSP_VNOR(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VNOR);
		uint16_t *acc = rsp->cp2.acc.e;

		rsp_vect_t result = rsp_vnor(LOAD_VS(), LOAD_VT());

		write_acc_lo(acc, result);
		STORE_RESULT();
	}

	//
	// VRCP
	// VRCPL
	// VRSQ
	// VRSQL
	//
	void RSP_VRCP(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VRCP);
		uint16_t *acc = rsp->cp2.acc.e;
		unsigned de = vs & 0x7;
		e &= 0x7;

		write_acc_lo(acc, LOAD_VT());

		rsp->cp2.dp_flag = 0;
		rsp_vect_t result = rsp_vrcp_vrsq<false>(rsp, 0, vt, e, vd, de);
		STORE_RESULT();
	}

	void RSP_VRCPL(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VRCPL);
		uint16_t *acc = rsp->cp2.acc.e;
		unsigned de = vs & 0x7;
		e &= 0x7;

		write_acc_lo(acc, LOAD_VT());

		int dp = rsp->cp2.dp_flag & 1;
		rsp->cp2.dp_flag = 0;

		rsp_vect_t result = rsp_vrcp_vrsq<false>(rsp, dp, vt, e, vd, de);
		STORE_RESULT();
	}

	void RSP_VRSQ(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VRSQ);
		uint16_t *acc = rsp->cp2.acc.e;
		unsigned de = vs & 0x7;
		e &= 0x7;

		write_acc_lo(acc, LOAD_VT());

		rsp->cp2.dp_flag = 0;
		rsp_vect_t result = rsp_vrcp_vrsq<true>(rsp, 0, vt, e, vd, de);
		STORE_RESULT();
	}

	void RSP_VRSQL(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VRSQL);
		uint16_t *acc = rsp->cp2.acc.e;
		unsigned de = vs & 0x7;
		e &= 0x7;

		write_acc_lo(acc, LOAD_VT());

		int dp = rsp->cp2.dp_flag & 1;
		rsp->cp2.dp_flag = 0;

		rsp_vect_t result = rsp_vrcp_vrsq<true>(rsp, dp, vt, e, vd, de);
		STORE_RESULT();
	}

	//
	// VRCPH
	// VRSQH
	//
	void RSP_VRCPH(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VRCPH);
		uint16_t *acc = rsp->cp2.acc.e;
		unsigned de = vs & 0x7;
		e &= 0x7;

		write_acc_lo(acc, LOAD_VT());

		// Specify double-precision for VRCPL on the next pass.
		rsp->cp2.dp_flag = 1;

		rsp_vect_t result = rsp_vdivh(rsp, vt, e, vd, de);
		STORE_RESULT();
	}

	void RSP_VRSQH(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VRSQH);
		uint16_t *acc = rsp->cp2.acc.e;
		unsigned de = vs & 0x7;
		e &= 0x7;

		write_acc_lo(acc, LOAD_VT());

		// Specify double-precision for VRCPL on the next pass.
		rsp->cp2.dp_flag = 1;

		rsp_vect_t result = rsp_vdivh(rsp, vt, e, vd, de);
		STORE_RESULT();
	}

	//
	// VSAR
	//
	void RSP_VSAR(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VSAR);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t result;

		switch (e)
		{
		case 8:
			result = read_acc_hi(acc);
			break;
		case 9:
			result = read_acc_md(acc);
			break;
		case 10:
			result = read_acc_lo(acc);
			break;
		default:
			result = rsp_vzero();
			break;
		}

		STORE_RESULT();
	}

	//
	// VSUB
	//
	void RSP_VSUB(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VSUB);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t carry, acc_lo;

		carry = read_vco_lo(rsp->cp2.flags[RSP::RSP_VCO].e);

		rsp_vect_t result = rsp_vsub(LOAD_VS(), LOAD_VT(), carry, &acc_lo);

		write_vco_hi(rsp->cp2.flags[RSP::RSP_VCO].e, rsp_vzero());
		write_vco_lo(rsp->cp2.flags[RSP::RSP_VCO].e, rsp_vzero());
		write_acc_lo(acc, acc_lo);
		STORE_RESULT();
	}

	//
	// VSUBC
	//
	void RSP_VSUBC(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VSUBC);
		uint16_t *acc = rsp->cp2.acc.e;
		rsp_vect_t eq, sn;

		rsp_vect_t result = rsp_vsubc(LOAD_VS(), LOAD_VT(), rsp_vzero(), &eq, &sn);

		write_vco_hi(rsp->cp2.flags[RSP::RSP_VCO].e, eq);
		write_vco_lo(rsp->cp2.flags[RSP::RSP_VCO].e, sn);
		write_acc_lo(acc, result);
		STORE_RESULT();
	}

	//
	// VXOR
	// VNXOR
	//
	void RSP_VXOR(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VXOR);
		uint16_t *acc = rsp->cp2.acc.e;

		rsp_vect_t result = rsp_vxor(LOAD_VS(), LOAD_VT());

		write_acc_lo(acc, result);
		STORE_RESULT();
	}

	void RSP_VNXOR(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	{
		TRACE_VU(VXNOR);
		uint16_t *acc = rsp->cp2.acc.e;

		rsp_vect_t result = rsp_vnxor(LOAD_VS(), LOAD_VT());

		write_acc_lo(acc, result);
		STORE_RESULT();
	}

	// RESERVED
	void RSP_RESERVED(RSP::CPUState *rsp, unsigned vd, unsigned, unsigned, unsigned)
	{
		rsp_vect_t result = rsp_vzero();
		STORE_RESULT();
	}
}
