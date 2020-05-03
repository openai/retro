#ifndef RSP_OP_HPP__
#define RSP_OP_HPP__

#include "state.hpp"

extern "C"
{
	int RSP_MFC0(RSP::CPUState *rsp, unsigned rt, unsigned rd);
	int RSP_MTC0(RSP::CPUState *rsp, unsigned rd, unsigned rt);

	void RSP_MTC2(RSP::CPUState *rsp, unsigned rt, unsigned vd, unsigned e);
	void RSP_MFC2(RSP::CPUState *rsp, unsigned rt, unsigned vs, unsigned e);
	void RSP_CFC2(RSP::CPUState *rsp, unsigned rt, unsigned rd);
	void RSP_CTC2(RSP::CPUState *rsp, unsigned rt, unsigned rd);

	void RSP_CALL(void *opaque, unsigned target, unsigned ret);
	void RSP_RETURN(void *opaque, unsigned pc);
	void RSP_EXIT(void *opaque, int mode);
	void RSP_REPORT_PC(void *rsp, unsigned pc, unsigned instr);

#define DECL_LS(op) void RSP_##op(RSP::CPUState *rsp, unsigned rt, unsigned element, int offset, unsigned base)

	DECL_LS(LBV);
	DECL_LS(LSV);
	DECL_LS(LLV);
	DECL_LS(LDV);
	DECL_LS(LQV);
	DECL_LS(LRV);
	DECL_LS(LPV);
	DECL_LS(LUV);
	DECL_LS(LHV);
	DECL_LS(LFV);
	DECL_LS(LTV);

	DECL_LS(SBV);
	DECL_LS(SSV);
	DECL_LS(SLV);
	DECL_LS(SDV);
	DECL_LS(SQV);
	DECL_LS(SRV);
	DECL_LS(SPV);
	DECL_LS(SUV);
	DECL_LS(SHV);
	DECL_LS(SFV);
	DECL_LS(STV);

#define DECL_COP2(op) void RSP_##op(RSP::CPUState *rsp, unsigned vd, unsigned vs, unsigned vt, unsigned e)
	DECL_COP2(VMULF);
	DECL_COP2(VMULU);
	DECL_COP2(VMUDL);
	DECL_COP2(VMUDM);
	DECL_COP2(VMUDN);
	DECL_COP2(VMUDH);
	DECL_COP2(VMACF);
	DECL_COP2(VMACU);
	DECL_COP2(VMACQ);
	DECL_COP2(VMADL);
	DECL_COP2(VMADM);
	DECL_COP2(VMADN);
	DECL_COP2(VMADH);
	DECL_COP2(VADD);
	DECL_COP2(VSUB);
	DECL_COP2(VABS);
	DECL_COP2(VADDC);
	DECL_COP2(VSUBC);
	DECL_COP2(VSAR);
	DECL_COP2(VLT);
	DECL_COP2(VEQ);
	DECL_COP2(VNE);
	DECL_COP2(VGE);
	DECL_COP2(VCL);
	DECL_COP2(VCH);
	DECL_COP2(VCR);
	DECL_COP2(VMRG);
	DECL_COP2(VAND);
	DECL_COP2(VNAND);
	DECL_COP2(VOR);
	DECL_COP2(VNOR);
	DECL_COP2(VXOR);
	DECL_COP2(VNXOR);
	DECL_COP2(VRCP);
	DECL_COP2(VRCPL);
	DECL_COP2(VRCPH);
	DECL_COP2(VMOV);
	DECL_COP2(VRSQ);
	DECL_COP2(VRSQL);
	DECL_COP2(VRSQH);
	DECL_COP2(VNOP);
	DECL_COP2(RESERVED);
}

#endif
