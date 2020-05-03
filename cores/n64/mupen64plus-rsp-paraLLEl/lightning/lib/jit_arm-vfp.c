/*
 * Copyright (C) 2012-2019  Free Software Foundation, Inc.
 *
 * This file is part of GNU lightning.
 *
 * GNU lightning is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU lightning is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * Authors:
 *	Paulo Cesar Pereira de Andrade
 */

#if PROTO
/* as per vfp_regno macro, required due to "support" to soft float registers
 * or using integer registers as arguments to float operations */
#  define _D8_REGNO			32
#  define ARM_V_Q			0x00000040
#  define FPSCR_N			0x80000000 /* Negative flag */
#  define FPSCR_Z			0x40000000 /* Zero flag */
#  define FPSCR_C			0x20000000 /* Carry flag */
#  define FPSCR_V			0x10000000 /* Overflow flag */
#  define FPSCR_QC			0x08000000 /* Cumulative saturation */
#  define FPSCR_AHP			0x04000000 /* Alt. half-precision */
#  define FPSCR_DN			0x02000000 /* Default NaN mode */
#  define FPSCR_FZ			0x01000000 /* Flush to zero */
#  define FPSCR_RMASK			0x00c00000
#    define FPSCR_RN			0x00000000 /* Round to Nearest */
#    define FPSCR_RP			0x00400000 /* Round to Plus Infinity */
#    define FPSCR_RM			0x00800000 /* Round to Minus Infinity */
#    define FPSCR_RZ			0x00c00000 /* Round towards Zero */
#  define FPSCR_STRIDE			0x00300000
#  define FPSCR_RES1			0x00080000 /* Reserved, UNK/SBZP */
#  define FPSCR_LEN			0x00070000
#  define FPSCR_IDE			0x00008000 /* Input Denormal trap */
#  define FPSCR_IXE			0x00001000 /* Inexact trap */
#  define FPSCR_UFE			0x00000800 /* Underflow trap */
#  define FPSCR_OFE			0x00000400 /* Overflow trap */
#  define FPSCR_DZE			0x00000200 /* Division by zero trap */
#  define FPSCR_IOE			0x00000100 /* Invalid Operation trap */
#  define FPSCR_IDC			0x00000080 /* Input Denormal flag */
#  define FPSCR_RES0			0x00000060 /* Reserved, UNK/SBZP */
#  define FPSCR_IXC			0x00000010 /* Inexact flag */
#  define FPSCR_UFC			0x00000008 /* Underflow flag */
#  define FPSCR_OFC			0x00000004 /* Overflow flag */
#  define FPSCR_DZC			0x00000002 /* Division by zero flag */
#  define FPSCR_IOC			0x00000001 /* Invalid Operation flag */
#  define ARM_V_E			0x00000080 /* ARM_VCMP except if NaN */
#  define ARM_V_Z			0x00010000 /* ARM_VCMP with zero */
#  define ARM_V_F64			0x00000100
#  define ARM_VADD_F			0x0e300a00
#  define ARM_VSUB_F			0x0e300a40
#  define ARM_VMUL_F			0x0e200a00
#  define ARM_VDIV_F			0x0e800a00
#  define ARM_VABS_F			0x0eb00ac0
#  define ARM_VNEG_F			0x0eb10a40
#  define ARM_VSQRT_F			0x0eb10ac0
#  define ARM_VMOV_F			0x0eb00a40
#  define ARM_VMOV_A_S			0x0e100a10 /* vmov rn, sn */
#  define ARM_VMOV_S_A			0x0e000a10 /* vmov sn, rn */
#  define ARM_VMOV_AA_D			0x0c500b10 /* vmov rn,rn, dn */
#  define ARM_VMOV_D_AA			0x0c400b10 /* vmov dn, rn,rn */
#  define ARM_VCMP			0x0eb40a40
#  define ARM_VMRS			0x0ef10a10
#  define ARM_VMSR			0x0ee10a10
#  define ARM_VCVT_2I			0x00040000 /* to integer */
#  define ARM_VCVT_2S			0x00010000 /* to signed */
#  define ARM_VCVT_RS			0x00000080 /* round to zero or signed */
#  define ARM_VCVT			0x0eb80a40
#  define ARM_VCVT_S32_F32		ARM_VCVT|ARM_VCVT_2I|ARM_VCVT_2S|ARM_VCVT_RS
#  define ARM_VCVT_U32_F32		ARM_VCVT|ARM_VCVT_2I|ARM_VCVT_RS
#  define ARM_VCVT_S32_F64		ARM_VCVT|ARM_VCVT_2I|ARM_VCVT_2S|ARM_VCVT_RS|ARM_V_F64
#  define ARM_VCVT_U32_F64		ARM_VCVT|ARM_VCVT_2I|ARM_VCVT_RS|ARM_V_F64
#  define ARM_VCVT_F32_S32		ARM_VCVT|ARM_VCVT_RS
#  define ARM_VCVT_F32_U32		ARM_VCVT
#  define ARM_VCVT_F64_S32		ARM_VCVT|ARM_VCVT_RS|ARM_V_F64
#  define ARM_VCVT_F64_U32		ARM_VCVT|ARM_V_F64
#  define ARM_VCVT_F			0x0eb70ac0
#  define ARM_VCVT_F32_F64		ARM_VCVT_F
#  define ARM_VCVT_F64_F32		ARM_VCVT_F|ARM_V_F64
#  define ARM_VCVTR_S32_F32		ARM_VCVT|ARM_VCVT_2I|ARM_VCVT_2S
#  define ARM_VCVTR_U32_F32		ARM_VCVT|ARM_VCVT_2I
#  define ARM_VCVTR_S32_F64		ARM_VCVT|ARM_VCVT_2I|ARM_VCVT_2S|ARM_V_F64
#  define ARM_VCVTR_U32_F64		ARM_VCVT|ARM_VCVT_2I|ARM_V_F64
#  define ARM_V_D			0x00400000
#  define ARM_V_N			0x00000080
#  define ARM_V_Q			0x00000040
#  define ARM_V_M			0x00000020
#  define ARM_V_U			0x01000000
#  define ARM_V_I16			0x00100000
#  define ARM_V_I32			0x00200000
#  define ARM_V_I64			0x00300000
#  define ARM_V_S16			0x00040000
#  define ARM_V_S32			0x00080000
#  define ARM_VADD_I			0x02000800
#  define ARM_VQADD_I			0x02000010 /* set flag on over/carry */
#  define ARM_VADDL_I			0x02800000 /* q=d+d */
#  define ARM_VADDW_I			0x02800100 /* q=q+d */
#  define ARM_VSUB_I			0x03000800
#  define ARM_VQSUB_I			0x02000210 /* set flag on over/carry */
#  define ARM_VSUBL_I			0x02800200
#  define ARM_VSUBW_I			0x02800300
#  define ARM_VMUL_I			0x02000910
#  define ARM_VMULL_I			0x02800c00
#  define ARM_VABS_I			0x03b10300
#  define ARM_VQABS_I			0x03b00700 /* sets flag on overflow */
#  define ARM_VNEG_I			0x03b10380
#  define ARM_VQNEG_I			0x03b00780 /* sets flag on overflow */
#  define ARM_VAND			0x02000110
#  define ARM_VBIC			0x02100110
#  define ARM_VORR			0x02200110
#  define ARM_VORN			0x02300110
#  define ARM_VEOR			0x03000110
#  define ARM_VMOVL_S8			0x00080000
#  define ARM_VMOVL_S16			0x00100000
#  define ARM_VMOVL_S32			0x00200000
#  define ARM_VMOVL_I			0x02800a10
#  define ARM_VMOVI			0x02800010
#  define ARM_VMVNI			0x02800030
#  define ARM_VLDR			0x0d100a00
#  define ARM_VSTR			0x0d000a00
#  define ARM_VM			0x0c000a00
#  define ARM_VMOV_ADV_U		0x00800000 /* zero extend */
#  define ARM_VMOV_ADV_8		0x00400000
#  define ARM_VMOV_ADV_16		0x00000020
#  define ARM_VMOV_A_D			0x0e100b10
#  define ARM_VMOV_D_A			0x0e000b10

#  define vodi(oi,r0)			_vodi(_jit,oi,r0)
static void _vodi(jit_state_t*,int,int) maybe_unused;
#  define voqi(oi,r0)			_voqi(_jit,oi,r0)
static void _voqi(jit_state_t*,int,int) maybe_unused;
#  define vo_ss(o,r0,r1)		_cc_vo_ss(_jit,ARM_CC_NV,o,r0,r1)
#  define cc_vo_ss(cc,o,r0,r1)		_cc_vo_ss(_jit,cc,o,r0,r1)
static void _cc_vo_ss(jit_state_t*,int,int,int,int);
#  define vo_dd(o,r0,r1)		_cc_vo_dd(_jit,ARM_CC_NV,o,r0,r1)
#  define cc_vo_dd(cc,o,r0,r1)		_cc_vo_dd(_jit,cc,o,r0,r1)
static void _cc_vo_dd(jit_state_t*,int,int,int,int);
#  define vo_qd(o,r0,r1)		_cc_vo_qd(_jit,ARM_CC_NV,o,r0,r1)
#  define cc_vo_qd(cc,o,r0,r1)		_cc_vo_qd(_jit,cc,o,r0,r1)
static void _cc_vo_qd(jit_state_t*,int,int,int,int) maybe_unused;
#  define vo_qq(o,r0,r1)		_cc_vo_qq(_jit,ARM_CC_NV,o,r0,r1)
#  define cc_vo_qq(cc,o,r0,r1)		_cc_vo_qq(_jit,cc,o,r0,r1)
static void _cc_vo_qq(jit_state_t*,int,int,int,int) maybe_unused;
#  define vorr_(o,r0,r1)		_cc_vorr_(_jit,ARM_CC_NV,o,r0,r1)
#  define cc_vorr_(cc,o,r0,r1)		_cc_vorr_(_jit,cc,o,r0,r1)
static void _cc_vorr_(jit_state_t*,int,int,int,int);
#  define vors_(o,r0,r1)		_cc_vors_(_jit,ARM_CC_NV,o,r0,r1)
#  define cc_vors_(cc,o,r0,r1)		_cc_vors_(_jit,cc,o,r0,r1)
static void _cc_vors_(jit_state_t*,int,int,int,int);
#  define vorv_(o,r0,r1)		_cc_vorv_(_jit,ARM_CC_NV,o,r0,r1)
#  define cc_vorv_(cc,o,r0,r1)		_cc_vorv_(_jit,cc,o,r0,r1)
static void _cc_vorv_(jit_state_t*,int,int,int,int) maybe_unused;
#  define vori_(o,r0,r1)		_cc_vori_(_jit,ARM_CC_NV,o,r0,r1)
#  define cc_vori_(cc,o,r0,r1)		_cc_vori_(_jit,cc,o,r0,r1)
static void _cc_vori_(jit_state_t*,int,int,int,int);
#  define vorrd(o,r0,r1,r2)		_cc_vorrd(_jit,ARM_CC_NV,o,r0,r1,r2)
#  define cc_vorrd(cc,o,r0,r1,r2)	_cc_vorrd(_jit,cc,o,r0,r1,r2)
static void _cc_vorrd(jit_state_t*,int,int,int,int,int);
#  define vosss(o,r0,r1,r2)		_cc_vosss(_jit,ARM_CC_NV,o,r0,r1,r2)
#  define cc_vosss(cc,o,r0,r1,r2)	_cc_vosss(_jit,cc,o,r0,r1,r2)
static void _cc_vosss(jit_state_t*,int,int,int,int,int);
#  define voddd(o,r0,r1,r2)		_cc_voddd(_jit,ARM_CC_NV,o,r0,r1,r2)
#  define cc_voddd(cc,o,r0,r1,r2)	_cc_voddd(_jit,cc,o,r0,r1,r2)
static void _cc_voddd(jit_state_t*,int,int,int,int,int);
#  define voqdd(o,r0,r1,r2)		_cc_voqdd(_jit,ARM_CC_NV,o,r0,r1,r2)
#  define cc_voqdd(cc,o,r0,r1,r2)	_cc_voqdd(_jit,cc,o,r0,r1,r2)
static void _cc_voqdd(jit_state_t*,int,int,int,int,int) maybe_unused;
#  define voqqd(o,r0,r1,r2)		_cc_voqqd(_jit,ARM_CC_NV,o,r0,r1,r2)
#  define cc_voqqd(cc,o,r0,r1,r2)	_cc_voqqd(_jit,cc,o,r0,r1,r2)
static void _cc_voqqd(jit_state_t*,int,int,int,int,int) maybe_unused;
#  define voqqq(o,r0,r1,r2)		_cc_voqqq(_jit,ARM_CC_NV,o,r0,r1,r2)
#  define cc_voqqq(cc,o,r0,r1,r2)	_cc_voqqq(_jit,cc,o,r0,r1,r2)
static void _cc_voqqq(jit_state_t*,int,int,int,int,int) maybe_unused;
#  define cc_vldst(cc,o,r0,r1,i0)	_cc_vldst(_jit,cc,o,r0,r1,i0)
static void _cc_vldst(jit_state_t*,int,int,int,int,int);
#  define cc_vorsl(cc,o,r0,r1,i0)	_cc_vorsl(_jit,cc,o,r0,r1,i0)
static void _cc_vorsl(jit_state_t*,int,int,int,int,int);
#  define CC_VADD_F32(cc,r0,r1,r2)	cc_vosss(cc,ARM_VADD_F,r0,r1,r2)
#  define VADD_F32(r0,r1,r2)		CC_VADD_F32(ARM_CC_AL,r0,r1,r2)
#  define CC_VADD_F64(cc,r0,r1,r2)	cc_voddd(cc,ARM_VADD_F|ARM_V_F64,r0,r1,r2)
#  define VADD_F64(r0,r1,r2)		CC_VADD_F64(ARM_CC_AL,r0,r1,r2)
#  define CC_VSUB_F32(cc,r0,r1,r2)	cc_vosss(cc,ARM_VSUB_F,r0,r1,r2)
#  define VSUB_F32(r0,r1,r2)		CC_VSUB_F32(ARM_CC_AL,r0,r1,r2)
#  define CC_VSUB_F64(cc,r0,r1,r2)	cc_voddd(cc,ARM_VSUB_F|ARM_V_F64,r0,r1,r2)
#  define VSUB_F64(r0,r1,r2)		CC_VSUB_F64(ARM_CC_AL,r0,r1,r2)
#  define CC_VMUL_F32(cc,r0,r1,r2)	cc_vosss(cc,ARM_VMUL_F,r0,r1,r2)
#  define VMUL_F32(r0,r1,r2)		CC_VMUL_F32(ARM_CC_AL,r0,r1,r2)
#  define CC_VMUL_F64(cc,r0,r1,r2)	cc_voddd(cc,ARM_VMUL_F|ARM_V_F64,r0,r1,r2)
#  define VMUL_F64(r0,r1,r2)		CC_VMUL_F64(ARM_CC_AL,r0,r1,r2)
#  define CC_VDIV_F32(cc,r0,r1,r2)	cc_vosss(cc,ARM_VDIV_F,r0,r1,r2)
#  define VDIV_F32(r0,r1,r2)		CC_VDIV_F32(ARM_CC_AL,r0,r1,r2)
#  define CC_VDIV_F64(cc,r0,r1,r2)	cc_voddd(cc,ARM_VDIV_F|ARM_V_F64,r0,r1,r2)
#  define VDIV_F64(r0,r1,r2)		CC_VDIV_F64(ARM_CC_AL,r0,r1,r2)
#  define CC_VABS_F32(cc,r0,r1)		cc_vo_ss(cc,ARM_VABS_F,r0,r1)
#  define VABS_F32(r0,r1)		CC_VABS_F32(ARM_CC_AL,r0,r1)
#  define CC_VABS_F64(cc,r0,r1)		cc_vo_dd(cc,ARM_VABS_F|ARM_V_F64,r0,r1)
#  define VABS_F64(r0,r1)		CC_VABS_F64(ARM_CC_AL,r0,r1)
#  define CC_VNEG_F32(cc,r0,r1)		cc_vo_ss(cc,ARM_VNEG_F,r0,r1)
#  define VNEG_F32(r0,r1)		CC_VNEG_F32(ARM_CC_AL,r0,r1)
#  define CC_VNEG_F64(cc,r0,r1)		cc_vo_dd(cc,ARM_VNEG_F|ARM_V_F64,r0,r1)
#  define VNEG_F64(r0,r1)		CC_VNEG_F64(ARM_CC_AL,r0,r1)
#  define CC_VSQRT_F32(cc,r0,r1)	cc_vo_ss(cc,ARM_VSQRT_F,r0,r1)
#  define VSQRT_F32(r0,r1)		CC_VSQRT_F32(ARM_CC_AL,r0,r1)
#  define CC_VSQRT_F64(cc,r0,r1)	cc_vo_dd(cc,ARM_VSQRT_F|ARM_V_F64,r0,r1)
#  define VSQRT_F64(r0,r1)		CC_VSQRT_F64(ARM_CC_AL,r0,r1)
#  define CC_VMOV_F32(cc,r0,r1)		cc_vo_ss(cc,ARM_VMOV_F,r0,r1)
#  define VMOV_F32(r0,r1)		CC_VMOV_F32(ARM_CC_AL,r0,r1)
#  define CC_VMOV_F64(cc,r0,r1)		cc_vo_dd(cc,ARM_VMOV_F|ARM_V_F64,r0,r1)
#  define VMOV_F64(r0,r1)		CC_VMOV_F64(ARM_CC_AL,r0,r1)
#  define CC_VMOV_AA_D(cc,r0,r1,r2)	cc_vorrd(cc,ARM_VMOV_AA_D,r0,r1,r2)
#  define VMOV_AA_D(r0,r1,r2)		CC_VMOV_AA_D(ARM_CC_AL,r0,r1,r2)
#  define CC_VMOV_D_AA(cc,r0,r1,r2)	cc_vorrd(cc,ARM_VMOV_D_AA,r1,r2,r0)
#  define VMOV_D_AA(r0,r1,r2)		CC_VMOV_D_AA(ARM_CC_AL,r0,r1,r2)
#  define CC_VMOV_A_S(cc,r0,r1)		cc_vors_(cc,ARM_VMOV_A_S,r0,r1)
#  define VMOV_A_S(r0,r1)		CC_VMOV_A_S(ARM_CC_AL,r0,r1)
#  define CC_VMOV_S_A(cc,r0,r1)		cc_vors_(cc,ARM_VMOV_S_A,r1,r0)
#  define VMOV_S_A(r0,r1)		CC_VMOV_S_A(ARM_CC_AL,r0,r1)
#  define CC_VCMP_F32(cc,r0,r1)		cc_vo_ss(cc,ARM_VCMP,r0,r1)
#  define VCMP_F32(r0,r1)		CC_VCMP_F32(ARM_CC_AL,r0,r1)
#  define CC_VCMP_F64(cc,r0,r1)		cc_vo_dd(cc,ARM_VCMP|ARM_V_F64,r0,r1)
#  define VCMP_F64(r0,r1)		CC_VCMP_F64(ARM_CC_AL,r0,r1)
#  define CC_VCMPE_F32(cc,r0,r1)	cc_vo_ss(cc,ARM_VCMP|ARM_V_E,r0,r1)
#  define VCMPE_F32(r0,r1)		CC_VCMPE_F32(ARM_CC_AL,r0,r1)
#  define CC_VCMPE_F64(cc,r0,r1)	cc_vo_dd(cc,ARM_VCMP|ARM_V_E|ARM_V_F64,r0,r1)
#  define VCMPE_F64(r0,r1)		CC_VCMPE_F64(ARM_CC_AL,r0,r1)
#  define CC_VCMPZ_F32(cc,r0)		cc_vo_ss(cc,ARM_VCMP|ARM_V_Z,r0,0)
#  define VCMPZ_F32(r0)			CC_VCMPZ_F32(ARM_CC_AL,r0)
#  define CC_VCMPZ_F64(cc,r0)		cc_vo_dd(cc,ARM_VCMP|ARM_V_Z|ARM_V_F64,r0,0)
#  define VCMPZ_F64(r0)			CC_VCMPZ_F64(ARM_CC_AL,r0)
#  define CC_VCMPEZ_F32(cc,r0)		cc_vo_ss(cc,ARM_VCMP|ARM_V_Z|ARM_V_E,r0,0)
#  define VCMPEZ_F32(r0)		CC_VCMPEZ_F32(ARM_CC_AL,r0)
#  define CC_VCMPEZ_F64(cc,r0)		cc_vo_dd(cc,ARM_VCMP|ARM_V_Z|ARM_V_E|ARM_V_F64,r0,0)
#  define VCMPEZ_F64(r0)		CC_VCMPEZ_F64(ARM_CC_AL,r0)
#  define CC_VMRS(cc,r0)		cc_vorr_(cc,ARM_VMRS,r0,0)
#  define VMRS(r0)			CC_VMRS(ARM_CC_AL,r0)
#  define CC_VMSR(cc,r0)		cc_vorr_(cc,ARM_VMSR,r0,0)
#  define VMSR(r0)			CC_VMSR(ARM_CC_AL,r0)
#  define CC_VCVT_S32_F32(cc,r0,r1)	cc_vo_ss(cc,ARM_VCVT_S32_F32,r0,r1)
#  define VCVT_S32_F32(r0,r1)		CC_VCVT_S32_F32(ARM_CC_AL,r0,r1)
#  define CC_VCVT_U32_F32(cc,r0,r1)	cc_vo_ss(cc,ARM_VCVT_U32_F32,r0,r1)
#  define VCVT_U32_F32(r0,r1)		CC_VCVT_U32_F32(ARM_CC_AL,r0,r1)
#  define CC_VCVT_S32_F64(cc,r0,r1)	cc_vo_ss(cc,ARM_VCVT_S32_F64,r0,r1)
#  define VCVT_S32_F64(r0,r1)		CC_VCVT_S32_F64(ARM_CC_AL,r0,r1)
#  define CC_VCVT_U32_F64(cc,r0,r1)	cc_vo_ss(cc,ARM_VCVT_U32_F64,r0,r1)
#  define VCVT_U32_F64(r0,r1)		CC_VCVT_U32_F64(ARM_CC_AL,r0,r1)
#  define CC_VCVT_F32_S32(cc,r0,r1)	cc_vo_ss(cc,ARM_VCVT_F32_S32,r0,r1)
#  define VCVT_F32_S32(r0,r1)		CC_VCVT_F32_S32(ARM_CC_AL,r0,r1)
#  define CC_VCVT_F32_U32(cc,r0,r1)	cc_vo_ss(cc,ARM_VCVT_F32_U32,r0,r1)
#  define VCVT_F32_U32(r0,r1)		CC_VCVT_F32_U32(ARM_CC_AL,r0,r1)
#  define CC_VCVT_F64_S32(cc,r0,r1)	cc_vo_ss(cc,ARM_VCVT_F64_S32,r0,r1)
#  define VCVT_F64_S32(r0,r1)		CC_VCVT_F64_S32(ARM_CC_AL,r0,r1)
#  define CC_VCVT_F64_U32(cc,r0,r1)	cc_vo_ss(cc,ARM_VCVT_F64_U32,r0,r1)
#  define VCVT_F64_U32(r0,r1)		CC_VCVT_F64_U32(ARM_CC_AL,r0,r1)
#  define CC_VCVT_F32_F64(cc,r0,r1)	cc_vo_ss(cc,ARM_VCVT_F32_F64,r0,r1)
#  define VCVT_F32_F64(r0,r1)		CC_VCVT_F32_F64(ARM_CC_AL,r0,r1)
#  define CC_VCVT_F64_F32(cc,r0,r1)	cc_vo_ss(cc,ARM_VCVT_F64_F32,r0,r1)
#  define VCVT_F64_F32(r0,r1)		CC_VCVT_F64_F32(ARM_CC_AL,r0,r1)
#  define CC_VCVTR_S32_F32(cc,r0,r1)	cc_vo_ss(cc,ARM_VCVTR_S32_F32,r0,r1)
#  define VCVTR_S32_F32(r0,r1)		CC_VCVTR_S32_F32(ARM_CC_AL,r0,r1)
#  define CC_VCVTR_U32_F32(cc,r0,r1)	cc_vo_ss(cc,ARM_VCVTR_U32_F32,r0,r1)
#  define VCVTR_U32_F32(r0,r1)		CC_VCVTR_U32_F32(ARM_CC_AL,r0,r1)
#  define CC_VCVTR_S32_F64(cc,r0,r1)	cc_vo_ss(cc,ARM_VCVTR_S32_F64,r0,r1)
#  define VCVTR_S32_F64(r0,r1)		CC_VCVTR_S32_F64(ARM_CC_AL,r0,r1)
#  define CC_VCVTR_U32_F64(cc,r0,r1)	cc_vo_ss(cc,ARM_VCVTR_U32_F64,r0,r1)
#  define VCVTR_U32_F64(r0,r1)		CC_VCVTR_U32_F64(ARM_CC_AL,r0,r1)
#  define CC_VLDMIA_F32(cc,r0,r1,i0)	cc_vorsl(cc,ARM_VM|ARM_M_L|ARM_M_I,r0,r1,i0)
#  define VLDMIA_F32(r0,r1,i0)		CC_VLDMIA_F32(ARM_CC_AL,r0,r1,i0)
#  define CC_VLDMIA_F64(cc,r0,r1,i0)	cc_vorsl(cc,ARM_VM|ARM_M_L|ARM_M_I|ARM_V_F64,r0,r1,i0)
#  define VLDMIA_F64(r0,r1,i0)		CC_VLDMIA_F64(ARM_CC_AL,r0,r1,i0)
#  define CC_VSTMIA_F32(cc,r0,r1,i0)	cc_vorsl(cc,ARM_VM|ARM_M_I,r0,r1,i0)
#  define VSTMIA_F32(r0,r1,i0)		CC_VSTMIA_F32(ARM_CC_AL,r0,r1,i0)
#  define CC_VSTMIA_F64(cc,r0,r1,i0)	cc_vorsl(cc,ARM_VM|ARM_M_I|ARM_V_F64,r0,r1,i0)
#  define VSTMIA_F64(r0,r1,i0)		CC_VSTMIA_F64(ARM_CC_AL,r0,r1,i0)
#  define CC_VLDMIA_U_F32(cc,r0,r1,i0)	cc_vorsl(cc,ARM_VM|ARM_M_L|ARM_M_I|ARM_M_U,r0,r1,i0)
#  define VLDMIA_U_F32(r0,r1,i0)	CC_VLDMIA_U_F32(ARM_CC_AL,r0,r1,i0)
#  define CC_VLDMIA_U_F64(cc,r0,r1,i0)	cc_vorsl(cc,ARM_VM|ARM_M_L|ARM_M_I|ARM_M_U|ARM_V_F64,r0,r1,i0)
#  define VLDMIA_U_F64(r0,r1,i0)	CC_VLDMIA_U_F64(ARM_CC_AL,r0,r1,i0)
#  define CC_VSTMIA_U_F32(cc,r0,r1,i0)	cc_vorsl(cc,ARM_VM|ARM_M_I|ARM_M_U,r0,r1,i0)
#  define VSTMIA_U_F32(r0,r1,i0)	CC_VSTMIA_U_F32(ARM_CC_AL,r0,r1,i0)
#  define CC_VSTMIA_U_F64(cc,r0,r1,i0)	cc_vorsl(cc,ARM_VM|ARM_M_I|ARM_M_U|ARM_V_F64,r0,r1,i0)
#  define VSTMIA_U_F64(r0,r1,i0)	CC_VSTMIA_U_F64(ARM_CC_AL,r0,r1,i0)
#  define CC_VLDMDB_U_F32(cc,r0,r1,i0)	cc_vorsl(cc,ARM_VM|ARM_M_L|ARM_M_B|ARM_M_U,r0,r1,i0)
#  define VLDMDB_U_F32(r0,r1,i0)	CC_VLDMDB_U_F32(ARM_CC_AL,r0,r1,i0)
#  define CC_VLDMDB_U_F64(cc,r0,r1,i0)	cc_vorsl(cc,ARM_VM|ARM_M_L|ARM_M_B|ARM_M_U|ARM_V_F64,r0,r1,i0)
#  define VLDMDB_U_F64(r0,r1,i0)	CC_VLDMDB_U_F64(ARM_CC_AL,r0,r1,i0)
#  define CC_VSTMDB_U_F32(cc,r0,r1,i0)	cc_vorsl(cc,ARM_VM|ARM_M_B|ARM_M_U,r0,r1,i0)
#  define VSTMDB_U_F32(r0,r1,i0)	CC_VSTMDB_U_F32(ARM_CC_AL,r0,r1,i0)
#  define CC_VSTMDB_U_F64(cc,r0,r1,i0)	cc_vorsl(cc,ARM_VM|ARM_M_B|ARM_M_U|ARM_V_F64,r0,r1,i0)
#  define VSTMDB_U_F64(r0,r1,i0)	CC_VSTMDB_U_F64(ARM_CC_AL,r0,r1,i0)
#  define CC_VPUSH_F32(cc,r0,i0)	CC_VSTMDB_U_F32(cc,_SP_REGNO,r0,i0)
#  define VPUSH_F32(r0,i0)		CC_VPUSH_F32(ARM_CC_AL,r0,i0)
#  define CC_VPUSH_F64(cc,r0,i0)	CC_VSTMDB_U_F64(cc,_SP_REGNO,r0,i0)
#  define VPUSH_F64(r0,i0)		CC_VPUSH_F64(ARM_CC_AL,r0,i0)
#  define CC_VPOP_F32(cc,r0,i0)		CC_VLDMIA_U_F32(cc,_SP_REGNO,r0,i0)
#  define VPOP_F32(r0,i0)		CC_VPOP_F32(ARM_CC_AL,r0,i0)
#  define CC_VPOP_F64(cc,r0,i0)		CC_VLDMIA_U_F64(cc,_SP_REGNO,r0,i0)
#  define VPOP_F64(r0,i0)		CC_VPOP_F64(ARM_CC_AL,r0,i0)
#  define CC_VMOV_A_S8(cc,r0,r1)	cc_vorv_(cc,ARM_VMOV_A_D|ARM_VMOV_ADV_8,r0,r1)
#  define VMOV_A_S8(r0,r1)		CC_VMOV_A_S8(ARM_CC_AL,r0,r1)
#  define CC_VMOV_A_U8(cc,r0,r1)	cc_vorv_(cc,ARM_VMOV_A_D|ARM_VMOV_ADV_8|ARM_VMOV_ADV_U,r0,r1)
#  define VMOV_A_U8(r0,r1)		CC_VMOV_A_U8(ARM_CC_AL,r0,r1)
#  define CC_VMOV_A_S16(cc,r0,r1)	cc_vorv_(cc,ARM_VMOV_A_D|ARM_VMOV_ADV_16,r0,r1)
#  define VMOV_A_S16(r0,r1)		CC_VMOV_A_S16(ARM_CC_AL,r0,r1)
#  define CC_VMOV_A_U16(cc,r0,r1)	cc_vorv_(cc,ARM_VMOV_A_D|ARM_VMOV_ADV_16|ARM_VMOV_ADV_U,r0,r1)
#  define VMOV_A_U16(r0,r1)		CC_VMOV_A_U16(ARM_CC_AL,r0,r1)
#  define CC_VMOV_A_S32(cc,r0,r1)	cc_vori_(cc,ARM_VMOV_A_D,r0,r1)
#  define VMOV_A_S32(r0,r1)		CC_VMOV_A_S32(ARM_CC_AL,r0,r1)
#  define CC_VMOV_A_U32(cc,r0,r1)	cc_vori_(cc,ARM_VMOV_A_D|ARM_VMOV_ADV_U,r0,r1)
#  define VMOV_A_U32(r0,r1)		CC_VMOV_A_U32(ARM_CC_AL,r0,r1)
#  define CC_VMOV_V_I8(cc,r0,r1)	cc_vorv_(cc,ARM_VMOV_D_A|ARM_VMOV_ADV_8,r1,r0)
#  define VMOV_V_I8(r0,r1)		CC_VMOV_V_I8(ARM_CC_AL,r0,r1)
#  define CC_VMOV_V_I16(cc,r0,r1)	cc_vorv_(cc,ARM_VMOV_D_A|ARM_VMOV_ADV_16,r1,r0)
#  define VMOV_V_I16(r0,r1)		CC_VMOV_V_I16(ARM_CC_AL,r0,r1)
#  define CC_VMOV_V_I32(cc,r0,r1)	cc_vori_(cc,ARM_VMOV_D_A,r1,r0)
#  define VMOV_V_I32(r0,r1)		CC_VMOV_V_I32(ARM_CC_AL,r0,r1)
#  define VADD_I8(r0,r1,r2)		voddd(ARM_VADD_I,r0,r1,r2)
#  define VADDQ_I8(r0,r1,r2)		voqqq(ARM_VADD_I|ARM_V_Q,r0,r1,r2)
#  define VADD_I16(r0,r1,r2)		voddd(ARM_VADD_I|ARM_V_I16,r0,r1,r2)
#  define VADDQ_I16(r0,r1,r2)		voqqq(ARM_VADD_I|ARM_V_I16|ARM_V_Q,r0,r1,r2)
#  define VADD_I32(r0,r1,r2)		voddd(ARM_VADD_I|ARM_V_I32,r0,r1,r2)
#  define VADDQ_I32(r0,r1,r2)		voqqq(ARM_VADD_I|ARM_V_I32|ARM_V_Q,r0,r1,r2)
#  define VADD_I64(r0,r1,r2)		voddd(ARM_VADD_I|ARM_V_I64,r0,r1,r2)
#  define VADDQ_I64(r0,r1,r2)		voqqq(ARM_VADD_I|ARM_V_I64|ARM_V_Q,r0,r1,r2)
#  define VQADD_S8(r0,r1,r2)		voddd(ARM_VQADD_I,r0,r1,r2)
#  define VQADDQ_S8(r0,r1,r2)		voqqq(ARM_VQADD_I|ARM_V_Q,r0,r1,r2)
#  define VQADD_U8(r0,r1,r2)		voddd(ARM_VQADD_I|ARM_V_U,r0,r1,r2)
#  define VQADDQ_U8(r0,r1,r2)		voqqq(ARM_VQADD_I|ARM_V_U|ARM_V_Q,r0,r1,r2)
#  define VQADD_S16(r0,r1,r2)		voddd(ARM_VQADD_I|ARM_V_I16,r0,r1,r2)
#  define VQADDQ_S16(r0,r1,r2)		voqqq(ARM_VQADD_I|ARM_V_I16|ARM_V_Q,r0,r1,r2)
#  define VQADD_U16(r0,r1,r2)		voddd(ARM_VQADD_I|ARM_V_I16|ARM_V_U,r0,r1,r2)
#  define VQADDQ_U16(r0,r1,r2)		voqqq(ARM_VQADD_I|ARM_V_I16|ARM_V_U|ARM_V_Q,r0,r1,r2)
#  define VQADD_S32(r0,r1,r2)		voddd(ARM_VQADD_I|ARM_V_I32,r0,r1,r2)
#  define VQADDQ_S32(r0,r1,r2)		voqqq(ARM_VQADD_I|ARM_V_I32|ARM_V_Q,r0,r1,r2)
#  define VQADD_U32(r0,r1,r2)		voddd(ARM_VQADD_I|ARM_V_I32|ARM_V_U,r0,r1,r2)
#  define VQADDQ_U32(r0,r1,r2)		voqqq(ARM_VQADD_I|ARM_V_I32|ARM_V_U|ARM_V_Q,r0,r1,r2)
#  define VQADD_S64(r0,r1,r2)		voddd(ARM_VQADD_I|ARM_V_I64,r0,r1,r2)
#  define VQADDQ_S64(r0,r1,r2)		voqqq(ARM_VQADD_I|ARM_V_I64|ARM_V_Q,r0,r1,r2)
#  define VQADD_U64(r0,r1,r2)		voddd(ARM_VQADD_I|ARM_V_I64|ARM_V_U,r0,r1,r2)
#  define VQADDQ_U64(r0,r1,r2)		voqqq(ARM_VQADD_I|ARM_V_I64|ARM_V_U|ARM_V_Q,r0,r1,r2)
#  define VADDL_S8(r0,r1,r2)		voqdd(ARM_VADDL_I,r0,r1,r2)
#  define VADDL_U8(r0,r1,r2)		voqdd(ARM_VADDL_I|ARM_V_U,r0,r1,r2)
#  define VADDL_S16(r0,r1,r2)		voqdd(ARM_VADDL_I|ARM_V_I16,r0,r1,r2)
#  define VADDL_U16(r0,r1,r2)		voqdd(ARM_VADDL_I|ARM_V_I16|ARM_V_U,r0,r1,r2)
#  define VADDL_S32(r0,r1,r2)		voqdd(ARM_VADDL_I|ARM_V_I32,r0,r1,r2)
#  define VADDL_U32(r0,r1,r2)		voqdd(ARM_VADDL_I|ARM_V_I32|ARM_V_U,r0,r1,r2)
#  define VADDW_S8(r0,r1,r2)		voqqd(ARM_VADDW_I,r0,r1,r2)
#  define VADDW_U8(r0,r1,r2)		voqqd(ARM_VADDW_I|ARM_V_U,r0,r1,r2)
#  define VADDW_S16(r0,r1,r2)		voqqd(ARM_VADDW_I|ARM_V_I16,r0,r1,r2)
#  define VADDW_U16(r0,r1,r2)		voqqd(ARM_VADDW_I|ARM_V_I16|ARM_V_U,r0,r1,r2)
#  define VADDW_S32(r0,r1,r2)		voqqd(ARM_VADDW_I|ARM_V_I32,r0,r1,r2)
#  define VADDW_U32(r0,r1,r2)		voqqd(ARM_VADDW_I|ARM_V_I32|ARM_V_U,r0,r1,r2)
#  define VSUB_I8(r0,r1,r2)		voddd(ARM_VSUB_I,r0,r1,r2)
#  define VSUBQ_I8(r0,r1,r2)		voqqq(ARM_VSUB_I|ARM_V_Q,r0,r1,r2)
#  define VSUB_I16(r0,r1,r2)		voddd(ARM_VSUB_I|ARM_V_I16,r0,r1,r2)
#  define VSUBQ_I16(r0,r1,r2)		voqqq(ARM_VSUB_I|ARM_V_I16|ARM_V_Q,r0,r1,r2)
#  define VSUB_I32(r0,r1,r2)		voddd(ARM_VSUB_I|ARM_V_I32,r0,r1,r2)
#  define VSUBQ_I32(r0,r1,r2)		voqqq(ARM_VSUB_I|ARM_V_I32|ARM_V_Q,r0,r1,r2)
#  define VSUB_I64(r0,r1,r2)		voddd(ARM_VSUB_I|ARM_V_I64,r0,r1,r2)
#  define VSUBQ_I64(r0,r1,r2)		voqqq(ARM_VSUB_I|ARM_V_I64|ARM_V_Q,r0,r1,r2)
#  define VQSUB_S8(r0,r1,r2)		voddd(ARM_VQSUB_I,r0,r1,r2)
#  define VQSUBQ_S8(r0,r1,r2)		voqqq(ARM_VQSUB_I|ARM_V_Q,r0,r1,r2)
#  define VQSUB_U8(r0,r1,r2)		voddd(ARM_VQSUB_I|ARM_V_U,r0,r1,r2)
#  define VQSUBQ_U8(r0,r1,r2)		voqqq(ARM_VQSUB_I|ARM_V_U|ARM_V_Q,r0,r1,r2)
#  define VQSUB_S16(r0,r1,r2)		voddd(ARM_VQSUB_I|ARM_V_I16,r0,r1,r2)
#  define VQSUBQ_S16(r0,r1,r2)		voqqq(ARM_VQSUB_I|ARM_V_I16|ARM_V_Q,r0,r1,r2)
#  define VQSUB_U16(r0,r1,r2)		voddd(ARM_VQSUB_I|ARM_V_I16|ARM_V_U,r0,r1,r2)
#  define VQSUBQ_U16(r0,r1,r2)		voqqq(ARM_VQSUB_I|ARM_V_I16|ARM_V_U|ARM_V_Q,r0,r1,r2)
#  define VQSUB_S32(r0,r1,r2)		voddd(ARM_VQSUB_I|ARM_V_I32,r0,r1,r2)
#  define VQSUBQ_S32(r0,r1,r2)		voqqq(ARM_VQSUB_I|ARM_V_I32|ARM_V_Q,r0,r1,r2)
#  define VQSUB_U32(r0,r1,r2)		voddd(ARM_VQSUB_I|ARM_V_I32|ARM_V_U,r0,r1,r2)
#  define VQSUBQ_U32(r0,r1,r2)		voqqq(ARM_VQSUB_I|ARM_V_I32|ARM_V_U|ARM_V_Q,r0,r1,r2)
#  define VQSUB_S64(r0,r1,r2)		voddd(ARM_VQSUB_I|ARM_V_I64,r0,r1,r2)
#  define VQSUBQ_S64(r0,r1,r2)		voqqq(ARM_VQSUB_I|ARM_V_I64|ARM_V_Q,r0,r1,r2)
#  define VQSUB_U64(r0,r1,r2)		voddd(ARM_VQSUB_I|ARM_V_I64|ARM_V_U,r0,r1,r2)
#  define VQSUBQ_U64(r0,r1,r2)		voqqq(ARM_VQSUB_I|ARM_V_I64|ARM_V_U|ARM_V_Q,r0,r1,r2)
#  define VSUBL_S8(r0,r1,r2)		voqdd(ARM_VSUBL_I,r0,r1,r2)
#  define VSUBL_U8(r0,r1,r2)		voqdd(ARM_VSUBL_I|ARM_V_U,r0,r1,r2)
#  define VSUBL_S16(r0,r1,r2)		voqdd(ARM_VSUBL_I|ARM_V_I16,r0,r1,r2)
#  define VSUBL_U16(r0,r1,r2)		voqdd(ARM_VSUBL_I|ARM_V_I16|ARM_V_U,r0,r1,r2)
#  define VSUBL_S32(r0,r1,r2)		voqdd(ARM_VSUBL_I|ARM_V_I32,r0,r1,r2)
#  define VSUBL_U32(r0,r1,r2)		voqdd(ARM_VSUBL_I|ARM_V_I32|ARM_V_U,r0,r1,r2)
#  define VSUBW_S8(r0,r1,r2)		voqqd(ARM_VSUBW_I,r0,r1,r2)
#  define VSUBW_U8(r0,r1,r2)		voqqd(ARM_VSUBW_I|ARM_V_U,r0,r1,r2)
#  define VSUBW_S16(r0,r1,r2)		voqqd(ARM_VSUBW_I|ARM_V_I16,r0,r1,r2)
#  define VSUBW_U16(r0,r1,r2)		voqqd(ARM_VSUBW_I|ARM_V_I16|ARM_V_U,r0,r1,r2)
#  define VSUBW_S32(r0,r1,r2)		voqqd(ARM_VSUBW_I|ARM_V_I32,r0,r1,r2)
#  define VSUBW_U32(r0,r1,r2)		voqqd(ARM_VSUBW_I|ARM_V_I32|ARM_V_U,r0,r1,r2)
#  define VMUL_I8(r0,r1,r2)		voddd(ARM_VMUL_I,r0,r1,r2)
#  define VMULQ_I8(r0,r1,r2)		voqqq(ARM_VMUL_I|ARM_V_Q,r0,r1,r2)
#  define VMUL_I16(r0,r1,r2)		voddd(ARM_VMUL_I|ARM_V_I16,r0,r1,r2)
#  define VMULQ_I16(r0,r1,r2)		voqqq(ARM_VMUL_I|ARM_V_Q|ARM_V_I16,r0,r1,r2)
#  define VMUL_I32(r0,r1,r2)		voddd(ARM_VMUL_I|ARM_V_I32,r0,r1,r2)
#  define VMULQ_I32(r0,r1,r2)		voqqq(ARM_VMUL_I|ARM_V_Q|ARM_V_I32,r0,r1,r2)
#  define VMULL_S8(r0,r1,r2)		voddd(ARM_VMULL_I,r0,r1,r2)
#  define VMULL_U8(r0,r1,r2)		voqqq(ARM_VMULL_I|ARM_V_U,r0,r1,r2)
#  define VMULL_S16(r0,r1,r2)		voddd(ARM_VMULL_I|ARM_V_I16,r0,r1,r2)
#  define VMULL_U16(r0,r1,r2)		voqqq(ARM_VMULL_I|ARM_V_U|ARM_V_I16,r0,r1,r2)
#  define VMULL_S32(r0,r1,r2)		voddd(ARM_VMULL_I|ARM_V_I32,r0,r1,r2)
#  define VMULL_U32(r0,r1,r2)		voqqq(ARM_VMULL_I|ARM_V_U|ARM_V_I32,r0,r1,r2)
#  define VABS_S8(r0,r1)		vo_dd(ARM_VABS_I,r0,r1)
#  define VABSQ_S8(r0,r1)		vo_qq(ARM_VABS_I|ARM_V_Q,r0,r1)
#  define VABS_S16(r0,r1)		vo_dd(ARM_VABS_I|ARM_V_S16,r0,r1)
#  define VABSQ_S16(r0,r1)		vo_qq(ARM_VABS_I|ARM_V_S16|ARM_V_Q,r0,r1)
#  define VABS_S32(r0,r1)		vo_dd(ARM_VABS_I|ARM_V_S32,r0,r1)
#  define VABSQ_S32(r0,r1)		vo_qq(ARM_VABS_I|ARM_V_S32|ARM_V_Q,r0,r1)
#  define VQABS_S8(r0,r1)		vo_dd(ARM_VQABS_I,r0,r1)
#  define VQABSQ_S8(r0,r1)		vo_qq(ARM_VQABS_I|ARM_V_Q,r0,r1)
#  define VQABS_S16(r0,r1)		vo_dd(ARM_VQABS_I|ARM_V_S16,r0,r1)
#  define VQABSQ_S16(r0,r1)		vo_qq(ARM_VQABS_I|ARM_V_S16|ARM_V_Q,r0,r1)
#  define VQABS_S32(r0,r1)		vo_dd(ARM_VQABS_I|ARM_V_S32,r0,r1)
#  define VQABSQ_S32(r0,r1)		vo_qq(ARM_VQABS_I|ARM_V_S32|ARM_V_Q,r0,r1)
#  define VNEG_S8(r0,r1)		vo_dd(ARM_VNEG_I,r0,r1)
#  define VNEGQ_S8(r0,r1)		vo_qq(ARM_VNEG_I|ARM_V_Q,r0,r1)
#  define VNEG_S16(r0,r1)		vo_dd(ARM_VNEG_I|ARM_V_S16,r0,r1)
#  define VNEGQ_S16(r0,r1)		vo_qq(ARM_VNEG_I|ARM_V_S16|ARM_V_Q,r0,r1)
#  define VNEG_S32(r0,r1)		vo_dd(ARM_VNEG_I|ARM_V_S32,r0,r1)
#  define VNEGQ_S32(r0,r1)		vo_qq(ARM_VNEG_I|ARM_V_S32|ARM_V_Q,r0,r1)
#  define VQNEG_S8(r0,r1)		vo_dd(ARM_VQNEG_I,r0,r1)
#  define VQNEGQ_S8(r0,r1)		vo_qq(ARM_VQNEG_I|ARM_V_Q,r0,r1)
#  define VQNEG_S16(r0,r1)		vo_dd(ARM_VQNEG_I|ARM_V_S16,r0,r1)
#  define VQNEGQ_S16(r0,r1)		vo_qq(ARM_VQNEG_I|ARM_V_S16|ARM_V_Q,r0,r1)
#  define VQNEG_S32(r0,r1)		vo_dd(ARM_VQNEG_I|ARM_V_S32,r0,r1)
#  define VQNEGQ_S32(r0,r1)		vo_qq(ARM_VQNEG_I|ARM_V_S32|ARM_V_Q,r0,r1)
#  define VAND(r0,r1,r2)		voddd(ARM_VAND,r0,r1,r2)
#  define VANDQ(r0,r1,r2)		voqqq(ARM_VAND|ARM_V_Q,r0,r1,r2)
#  define VBIC(r0,r1,r2)		voddd(ARM_VBIC,r0,r1,r2)
#  define VBICQ(r0,r1,r2)		voqqq(ARM_VBIC|ARM_V_Q,r0,r1,r2)
#  define VORR(r0,r1,r2)		voddd(ARM_VORR,r0,r1,r2)
#  define VORRQ(r0,r1,r2)		voqqq(ARM_VORR|ARM_V_Q,r0,r1,r2)
#  define VORN(r0,r1,r2)		voddd(ARM_VORN,r0,r1,r2)
#  define VORNQ(r0,r1,r2)		voqqq(ARM_VORN|ARM_V_Q,r0,r1,r2)
#  define VEOR(r0,r1,r2)		voddd(ARM_VEOR,r0,r1,r2)
#  define VEORQ(r0,r1,r2)		voqqq(ARM_VEOR|ARM_V_Q,r0,r1,r2)
#  define VMOV(r0,r1)			VORR(r0,r1,r1)
#  define VMOVQ(r0,r1)			VORRQ(r0,r1,r1)
#  define VMOVL_S8(r0,r1)		vo_qd(ARM_VMOVL_I|ARM_VMOVL_S8,r0,r1)
#  define VMOVL_U8(r0,r1)		vo_qd(ARM_VMOVL_I|ARM_V_U|ARM_VMOVL_S8,r0,r1)
#  define VMOVL_S16(r0,r1)		vo_qd(ARM_VMOVL_I|ARM_VMOVL_S16,r0,r1)
#  define VMOVL_U16(r0,r1)		vo_qd(ARM_VMOVL_I|ARM_V_U|ARM_VMOVL_S16,r0,r1)
#  define VMOVL_S32(r0,r1)		vo_qd(ARM_VMOVL_I|ARM_VMOVL_S32,r0,r1)
#  define VMOVL_U32(r0,r1)		vo_qd(ARM_VMOVL_I|ARM_V_U|ARM_VMOVL_S32,r0,r1)
/* "oi" should be the result of encode_vfp_double */
#  define VIMM(oi,r0)			vodi(oi,r0)
#  define VIMMQ(oi,r0)			voqi(oi|ARM_V_Q,r0)
/* index is multipled by four */
#  define CC_VLDRN_F32(cc,r0,r1,i0)	cc_vldst(cc,ARM_VLDR,r0,r1,i0)
#  define VLDRN_F32(r0,r1,i0)		CC_VLDRN_F32(ARM_CC_AL,r0,r1,i0)
#  define CC_VLDR_F32(cc,r0,r1,i0)	cc_vldst(cc,ARM_VLDR|ARM_P,r0,r1,i0)
#  define VLDR_F32(r0,r1,i0)		CC_VLDR_F32(ARM_CC_AL,r0,r1,i0)
#  define CC_VLDRN_F64(cc,r0,r1,i0)	cc_vldst(cc,ARM_VLDR|ARM_V_F64,r0,r1,i0)
#  define VLDRN_F64(r0,r1,i0)		CC_VLDRN_F64(ARM_CC_AL,r0,r1,i0)
#  define CC_VLDR_F64(cc,r0,r1,i0)	cc_vldst(cc,ARM_VLDR|ARM_V_F64|ARM_P,r0,r1,i0)
#  define VLDR_F64(r0,r1,i0)		CC_VLDR_F64(ARM_CC_AL,r0,r1,i0)
#  define CC_VSTRN_F32(cc,r0,r1,i0)	cc_vldst(cc,ARM_VSTR,r0,r1,i0)
#  define VSTRN_F32(r0,r1,i0)		CC_VSTRN_F32(ARM_CC_AL,r0,r1,i0)
#  define CC_VSTR_F32(cc,r0,r1,i0)	cc_vldst(cc,ARM_VSTR|ARM_P,r0,r1,i0)
#  define VSTR_F32(r0,r1,i0)		CC_VSTR_F32(ARM_CC_AL,r0,r1,i0)
#  define CC_VSTRN_F64(cc,r0,r1,i0)	cc_vldst(cc,ARM_VSTR|ARM_V_F64,r0,r1,i0)
#  define VSTRN_F64(r0,r1,i0)		CC_VSTRN_F64(ARM_CC_AL,r0,r1,i0)
#  define CC_VSTR_F64(cc,r0,r1,i0)	cc_vldst(cc,ARM_VSTR|ARM_V_F64|ARM_P,r0,r1,i0)
#  define VSTR_F64(r0,r1,i0)		CC_VSTR_F64(ARM_CC_AL,r0,r1,i0)
#  define vfp_movr_f(r0,r1)		_vfp_movr_f(_jit,r0,r1)
static void _vfp_movr_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define vfp_movr_d(r0,r1)		_vfp_movr_d(_jit,r0,r1)
static void _vfp_movr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define vfp_movi_f(r0,i0)		_vfp_movi_f(_jit,r0,i0)
static void _vfp_movi_f(jit_state_t*,jit_int32_t,jit_float32_t);
#  define vfp_movi_d(r0,i0)		_vfp_movi_d(_jit,r0,i0)
static void _vfp_movi_d(jit_state_t*,jit_int32_t,jit_float64_t);
#  define vfp_extr_f(r0,r1)		_vfp_extr_f(_jit,r0,r1)
static void _vfp_extr_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define vfp_extr_d(r0,r1)		_vfp_extr_d(_jit,r0,r1)
static void _vfp_extr_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define vfp_extr_d_f(r0,r1)		_vfp_extr_d_f(_jit,r0,r1)
static void _vfp_extr_d_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define vfp_extr_f_d(r0,r1)		_vfp_extr_f_d(_jit,r0,r1)
static void _vfp_extr_f_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define vfp_truncr_f_i(r0,r1)		_vfp_truncr_f_i(_jit,r0,r1)
static void _vfp_truncr_f_i(jit_state_t*,jit_int32_t,jit_int32_t);
#  define vfp_truncr_d_i(r0,r1)		_vfp_truncr_d_i(_jit,r0,r1)
static void _vfp_truncr_d_i(jit_state_t*,jit_int32_t,jit_int32_t);
#  define vfp_absr_f(r0,r1)		VABS_F32(r0,r1)
#  define vfp_absr_d(r0,r1)		VABS_F64(r0,r1)
#  define vfp_negr_f(r0,r1)		VNEG_F32(r0,r1)
#  define vfp_negr_d(r0,r1)		VNEG_F64(r0,r1)
#  define vfp_sqrtr_f(r0,r1)		VSQRT_F32(r0,r1)
#  define vfp_sqrtr_d(r0,r1)		VSQRT_F64(r0,r1)
#  define vfp_addr_f(r0,r1,r2)		VADD_F32(r0,r1,r2)
#  define vfp_addi_f(r0,r1,i0)		_vfp_addi_f(_jit,r0,r1,i0)
static void _vfp_addi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define vfp_addr_d(r0,r1,r2)		VADD_F64(r0,r1,r2)
#  define vfp_addi_d(r0,r1,i0)		_vfp_addi_d(_jit,r0,r1,i0)
static void _vfp_addi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define vfp_subr_f(r0,r1,r2)		VSUB_F32(r0,r1,r2)
#  define vfp_subi_f(r0,r1,i0)		_vfp_subi_f(_jit,r0,r1,i0)
static void _vfp_subi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define vfp_subr_d(r0,r1,r2)		VSUB_F64(r0,r1,r2)
#  define vfp_subi_d(r0,r1,i0)		_vfp_subi_d(_jit,r0,r1,i0)
static void _vfp_subi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define vfp_rsbr_f(r0,r1,r2)		vfp_subr_f(r0,r2,r1)
#  define vfp_rsbi_f(r0,r1,i0)		_vfp_rsbi_f(_jit,r0,r1,i0)
static void _vfp_rsbi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define vfp_rsbr_d(r0,r1,r2)		vfp_subr_d(r0,r2,r1)
#  define vfp_rsbi_d(r0,r1,i0)		_vfp_rsbi_d(_jit,r0,r1,i0)
static void _vfp_rsbi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define vfp_mulr_f(r0,r1,r2)		VMUL_F32(r0,r1,r2)
#  define vfp_muli_f(r0,r1,i0)		_vfp_muli_f(_jit,r0,r1,i0)
static void _vfp_muli_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define vfp_mulr_d(r0,r1,r2)		VMUL_F64(r0,r1,r2)
#  define vfp_muli_d(r0,r1,i0)		_vfp_muli_d(_jit,r0,r1,i0)
static void _vfp_muli_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define vfp_divr_f(r0,r1,r2)		VDIV_F32(r0,r1,r2)
#  define vfp_divi_f(r0,r1,i0)		_vfp_divi_f(_jit,r0,r1,i0)
static void _vfp_divi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define vfp_divr_d(r0,r1,r2)		VDIV_F64(r0,r1,r2)
#  define vfp_divi_d(r0,r1,i0)		_vfp_divi_d(_jit,r0,r1,i0)
static void _vfp_divi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define vfp_cmp_f(r0,r1)		_vfp_cmp_f(_jit,r0,r1)
static void _vfp_cmp_f(jit_state_t*,jit_int32_t,jit_int32_t);
#  define vfp_cmp_d(r0,r1)		_vfp_cmp_d(_jit,r0,r1)
static void _vfp_cmp_d(jit_state_t*,jit_int32_t,jit_int32_t);
#  define vcmp01_x(c0,c1,r0)		_vcmp01_x(_jit,c0,c1,r0)
static void _vcmp01_x(jit_state_t*,int,int,jit_int32_t);
#  define vcmp01_f(c0,c1,r0,r1,r2)	_vcmp01_f(_jit,c0,c1,r0,r1,r2)
static void _vcmp01_f(jit_state_t*,int,int,jit_int32_t,jit_int32_t,jit_int32_t);
#  define vcmp01_d(c0,c1,r0,r1,r2)	_vcmp01_d(_jit,c0,c1,r0,r1,r2)
static void _vcmp01_d(jit_state_t*,int,int,jit_int32_t,jit_int32_t,jit_int32_t);
#  define vfp_ltr_f(r0,r1,r2)		vcmp01_f(ARM_CC_PL,ARM_CC_MI,r0,r1,r2)
#  define vfp_lti_f(r0,r1,i0)		_vfp_lti_f(_jit,r0,r1,i0)
static void _vfp_lti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define vfp_ltr_d(r0,r1,r2)		vcmp01_d(ARM_CC_PL,ARM_CC_MI,r0,r1,r2)
#  define vfp_lti_d(r0,r1,i0)		_vfp_lti_d(_jit,r0,r1,i0)
static void _vfp_lti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define vfp_ler_f(r0,r1,r2)		vcmp01_f(ARM_CC_HS,ARM_CC_LS,r0,r1,r2)
#  define vfp_lei_f(r0,r1,i0)		_vfp_lei_f(_jit,r0,r1,i0)
static void _vfp_lei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define vfp_ler_d(r0,r1,r2)		vcmp01_d(ARM_CC_HS,ARM_CC_LS,r0,r1,r2)
#  define vfp_lei_d(r0,r1,i0)		_vfp_lei_d(_jit,r0,r1,i0)
static void _vfp_lei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define vfp_eqr_f(r0,r1,r2)		vcmp01_f(ARM_CC_NE,ARM_CC_EQ,r0,r1,r2)
#  define vfp_eqi_f(r0,r1,i0)		_vfp_eqi_f(_jit,r0,r1,i0)
static void _vfp_eqi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define vfp_eqr_d(r0,r1,r2)		vcmp01_d(ARM_CC_NE,ARM_CC_EQ,r0,r1,r2)
#  define vfp_eqi_d(r0,r1,i0)		_vfp_eqi_d(_jit,r0,r1,i0)
static void _vfp_eqi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define vfp_ger_f(r0,r1,r2)		vcmp01_f(ARM_CC_LT,ARM_CC_GE,r0,r1,r2)
#  define vfp_gei_f(r0,r1,i0)		_vfp_gei_f(_jit,r0,r1,i0)
static void _vfp_gei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define vfp_ger_d(r0,r1,r2)		vcmp01_d(ARM_CC_LT,ARM_CC_GE,r0,r1,r2)
#  define vfp_gei_d(r0,r1,i0)		_vfp_gei_d(_jit,r0,r1,i0)
static void _vfp_gei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define vfp_gtr_f(r0,r1,r2)		vcmp01_f(ARM_CC_LE,ARM_CC_GT,r0,r1,r2)
#  define vfp_gti_f(r0,r1,i0)		_vfp_gti_f(_jit,r0,r1,i0)
static void _vfp_gti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define vfp_gtr_d(r0,r1,r2)		vcmp01_d(ARM_CC_LE,ARM_CC_GT,r0,r1,r2)
#  define vfp_gti_d(r0,r1,i0)		_vfp_gti_d(_jit,r0,r1,i0)
static void _vfp_gti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define vfp_ner_f(r0,r1,r2)		vcmp01_f(ARM_CC_EQ,ARM_CC_NE,r0,r1,r2)
#  define vfp_nei_f(r0,r1,i0)		_vfp_nei_f(_jit,r0,r1,i0)
static void _vfp_nei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define vfp_ner_d(r0,r1,r2)		vcmp01_d(ARM_CC_EQ,ARM_CC_NE,r0,r1,r2)
#  define vfp_nei_d(r0,r1,i0)		_vfp_nei_d(_jit,r0,r1,i0)
static void _vfp_nei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define vcmp10_x(c0,r0)		_vcmp10_x(_jit,c0,r0)
static void _vcmp10_x(jit_state_t*,int,jit_int32_t);
#  define vcmp_10_f(c0,r0,r1,r2)	_vcmp_10_f(_jit,c0,r0,r1,r2)
static void _vcmp_10_f(jit_state_t*,int,jit_int32_t,jit_int32_t,jit_int32_t);
#  define vcmp_10_d(c0,r0,r1,r2)	_vcmp_10_d(_jit,c0,r0,r1,r2)
static void _vcmp_10_d(jit_state_t*,int,jit_int32_t,jit_int32_t,jit_int32_t);
#  define vfp_unltr_f(r0,r1,r2)		vcmp_10_f(ARM_CC_GE,r0,r1,r2)
#  define vfp_unlti_f(r0,r1,i0)		_vfp_unlti_f(_jit,r0,r1,i0)
static void _vfp_unlti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define vfp_unltr_d(r0,r1,r2)		vcmp_10_d(ARM_CC_GE,r0,r1,r2)
#  define vfp_unlti_d(r0,r1,i0)		_vfp_unlti_d(_jit,r0,r1,i0)
static void _vfp_unlti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define vfp_unler_f(r0,r1,r2)		vcmp_10_f(ARM_CC_GT,r0,r1,r2)
#  define vfp_unlei_f(r0,r1,i0)		_vfp_unlei_f(_jit,r0,r1,i0)
static void _vfp_unlei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define vfp_unler_d(r0,r1,r2)		vcmp_10_d(ARM_CC_GT,r0,r1,r2)
#  define vfp_unlei_d(r0,r1,i0)		_vfp_unlei_d(_jit,r0,r1,i0)
static void _vfp_unlei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define vfp_uneqr_x(r0)		_vfp_uneqr_x(_jit,r0)
static void _vfp_uneqr_x(jit_state_t*,jit_int32_t);
#  define vfp_uneqr_f(r0,r1,r2)		_vfp_uneqr_f(_jit,r0,r1,r2)
static void _vfp_uneqr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define vfp_uneqi_f(r0,r1,i0)		_vfp_uneqi_f(_jit,r0,r1,i0)
static void _vfp_uneqi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define vfp_uneqr_d(r0,r1,r2)		_vfp_uneqr_d(_jit,r0,r1,r2)
static void _vfp_uneqr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define vfp_uneqi_d(r0,r1,i0)		_vfp_uneqi_d(_jit,r0,r1,i0)
static void _vfp_uneqi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define vcmp_01_x(c0,r0)		_vcmp_01_x(_jit,c0,r0)
static void _vcmp_01_x(jit_state_t*,int,jit_int32_t);
#  define vcmp_01_f(c0,r0,r1,r2)	_vcmp_01_f(_jit,c0,r0,r1,r2)
static void _vcmp_01_f(jit_state_t*,int,jit_int32_t,jit_int32_t,jit_int32_t);
#  define vcmp_01_d(c0,r0,r1,r2)	_vcmp_01_d(_jit,c0,r0,r1,r2)
static void _vcmp_01_d(jit_state_t*,int,jit_int32_t,jit_int32_t,jit_int32_t);
#  define vfp_unger_f(r0,r1,r2)		vcmp_01_f(ARM_CC_CS,r0,r1,r2)
#  define vfp_ungei_f(r0,r1,i0)		_vfp_ungei_f(_jit,r0,r1,i0)
static void _vfp_ungei_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define vfp_unger_d(r0,r1,r2)		vcmp_01_d(ARM_CC_CS,r0,r1,r2)
#  define vfp_ungei_d(r0,r1,i0)		_vfp_ungei_d(_jit,r0,r1,i0)
static void _vfp_ungei_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define vfp_ungtr_f(r0,r1,r2)		vcmp_01_f(ARM_CC_HI,r0,r1,r2)
#  define vfp_ungti_f(r0,r1,i0)		_vfp_ungti_f(_jit,r0,r1,i0)
static void _vfp_ungti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define vfp_ungtr_d(r0,r1,r2)		vcmp_01_d(ARM_CC_HI,r0,r1,r2)
#  define vfp_ungti_d(r0,r1,i0)		_vfp_ungti_d(_jit,r0,r1,i0)
static void _vfp_ungti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define vfp_ltgtr_x(r0)		_vfp_ltgtr_x(_jit,r0)
static void _vfp_ltgtr_x(jit_state_t*,jit_int32_t);
#  define vfp_ltgtr_f(r0,r1,r2)		_vfp_ltgtr_f(_jit,r0,r1,r2)
static void _vfp_ltgtr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define vfp_ltgti_f(r0,r1,i0)		_vfp_ltgti_f(_jit,r0,r1,i0)
static void _vfp_ltgti_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define vfp_ltgtr_d(r0,r1,r2)		_vfp_ltgtr_d(_jit,r0,r1,r2)
static void _vfp_ltgtr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define vfp_ltgti_d(r0,r1,i0)		_vfp_ltgti_d(_jit,r0,r1,i0)
static void _vfp_ltgti_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define vfp_ordr_f(r0,r1,r2)		_vfp_ordr_f(_jit,r0,r1,r2)
static void _vfp_ordr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define vfp_ordi_f(r0,r1,i0)		_vfp_ordi_f(_jit,r0,r1,i0)
static void _vfp_ordi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define vfp_ordr_d(r0,r1,r2)		_vfp_ordr_d(_jit,r0,r1,r2)
static void _vfp_ordr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define vfp_ordi_d(r0,r1,i0)		_vfp_ordi_d(_jit,r0,r1,i0)
static void _vfp_ordi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define vfp_unordr_f(r0,r1,r2)	_vfp_unordr_f(_jit,r0,r1,r2)
static void _vfp_unordr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define vfp_unordi_f(r0,r1,i0)	_vfp_unordi_f(_jit,r0,r1,i0)
static void _vfp_unordi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_float32_t);
#  define vfp_unordr_d(r0,r1,r2)	_vfp_unordr_d(_jit,r0,r1,r2)
static void _vfp_unordr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define vfp_unordi_d(r0,r1,i0)	_vfp_unordi_d(_jit,r0,r1,i0)
static void _vfp_unordi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_float64_t);
#  define vbcmp_x(cc,i0)		_vbcmp_x(_jit,cc,i0)
static jit_word_t _vbcmp_x(jit_state_t*,int,jit_word_t);
#  define vbcmp_f(cc,i0,r0,r1)		_vbcmp_f(_jit,cc,i0,r0,r1)
static jit_word_t
_vbcmp_f(jit_state_t*,int,jit_word_t,jit_int32_t,jit_int32_t);
#  define vbcmp_x(cc,i0)		_vbcmp_x(_jit,cc,i0)
static jit_word_t _vbcmp_x(jit_state_t*,int,jit_word_t);
#  define vbcmp_d(cc,i0,r0,r1)		_vbcmp_d(_jit,cc,i0,r0,r1)
static jit_word_t
_vbcmp_d(jit_state_t*,int,jit_word_t,jit_int32_t,jit_int32_t);
#  define vfp_bltr_f(i0,r0,r1)		vbcmp_f(ARM_CC_MI,i0,r0,r1)
#  define vfp_blti_f(i0,r0,i1)		_vfp_blti_f(_jit,i0,r0,i1)
static jit_word_t _vfp_blti_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define vfp_bltr_d(i0,r0,r1)		vbcmp_d(ARM_CC_MI,i0,r0,r1)
static jit_word_t _vfp_blti_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define vfp_blti_d(i0,r0,i1)		_vfp_blti_d(_jit,i0,r0,i1)
#  define vfp_bler_f(i0,r0,r1)		vbcmp_f(ARM_CC_LS,i0,r0,r1)
#  define vfp_blei_f(i0,r0,i1)		_vfp_blei_f(_jit,i0,r0,i1)
static jit_word_t _vfp_blei_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define vfp_bler_d(i0,r0,r1)		vbcmp_d(ARM_CC_LS,i0,r0,r1)
#  define vfp_blei_d(i0,r0,i1)		_vfp_blei_d(_jit,i0,r0,i1)
static jit_word_t _vfp_blei_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define vfp_beqr_f(i0,r0,r1)		vbcmp_f(ARM_CC_EQ,i0,r0,r1)
#  define vfp_beqi_f(i0,r0,i1)		_vfp_beqi_f(_jit,i0,r0,i1)
static jit_word_t _vfp_beqi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define vfp_beqr_d(i0,r0,r1)		vbcmp_d(ARM_CC_EQ,i0,r0,r1)
#  define vfp_beqi_d(i0,r0,i1)		_vfp_beqi_d(_jit,i0,r0,i1)
static jit_word_t _vfp_beqi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define vfp_bger_f(i0,r0,r1)		vbcmp_f(ARM_CC_GE,i0,r0,r1)
#  define vfp_bgei_f(i0,r0,i1)		_vfp_bgei_f(_jit,i0,r0,i1)
static jit_word_t _vfp_bgei_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define vfp_bger_d(i0,r0,r1)		vbcmp_d(ARM_CC_GE,i0,r0,r1)
#  define vfp_bgei_d(i0,r0,i1)		_vfp_bgei_d(_jit,i0,r0,i1)
static jit_word_t _vfp_bgei_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define vfp_bgtr_f(i0,r0,r1)		vbcmp_f(ARM_CC_GT,i0,r0,r1)
#  define vfp_bgti_f(i0,r0,i1)		_vfp_bgti_f(_jit,i0,r0,i1)
static jit_word_t _vfp_bgti_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define vfp_bgtr_d(i0,r0,r1)		vbcmp_d(ARM_CC_GT,i0,r0,r1)
#  define vfp_bgti_d(i0,r0,i1)		_vfp_bgti_d(_jit,i0,r0,i1)
static jit_word_t _vfp_bgti_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define vfp_bner_f(i0,r0,r1)		vbcmp_f(ARM_CC_NE,i0,r0,r1)
#  define vfp_bnei_f(i0,r0,i1)		_vfp_bnei_f(_jit,i0,r0,i1)
static jit_word_t _vfp_bnei_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define vfp_bner_d(i0,r0,r1)		vbcmp_d(ARM_CC_NE,i0,r0,r1)
#  define vfp_bnei_d(i0,r0,i1)		_vfp_bnei_d(_jit,i0,r0,i1)
static jit_word_t _vfp_bnei_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define vbncmp_x(cc,i0)		_vbncmp_x(_jit,cc,i0)
static jit_word_t _vbncmp_x(jit_state_t*,int,jit_word_t);
#  define vbncmp_f(cc,i0,r0,r1)		_vbncmp_f(_jit,cc,i0,r0,r1)
static jit_word_t
_vbncmp_f(jit_state_t*,int,jit_word_t,jit_int32_t,jit_int32_t);
#  define vbncmp_d(cc,i0,r0,r1)		_vbncmp_d(_jit,cc,i0,r0,r1)
static jit_word_t
_vbncmp_d(jit_state_t*,int,jit_word_t,jit_int32_t,jit_int32_t);
#  define vfp_bunltr_f(i0,r0,r1)	vbncmp_f(ARM_CC_GE,i0,r0,r1)
#  define vfp_bunlti_f(i0,r0,i1)	_vfp_bunlti_f(_jit,i0,r0,i1)
static jit_word_t
_vfp_bunlti_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define vfp_bunltr_d(i0,r0,r1)	vbncmp_d(ARM_CC_GE,i0,r0,r1)
#  define vfp_bunlti_d(i0,r0,i1)	_vfp_bunlti_d(_jit,i0,r0,i1)
static jit_word_t
_vfp_bunlti_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define vfp_bunler_f(i0,r0,r1)	vbncmp_f(ARM_CC_GT,i0,r0,r1)
#  define vfp_bunlei_f(i0,r0,i1)	_vfp_bunlei_f(_jit,i0,r0,i1)
static jit_word_t
_vfp_bunlei_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define vfp_bunler_d(i0,r0,r1)	vbncmp_d(ARM_CC_GT,i0,r0,r1)
#  define vfp_bunlei_d(i0,r0,i1)	_vfp_bunlei_d(_jit,i0,r0,i1)
static jit_word_t
_vfp_bunlei_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define vfp_buneqr_x(i0)		_vfp_buneqr_x(_jit,i0)
static jit_word_t _vfp_buneqr_x(jit_state_t*,jit_word_t);
#  define vfp_buneqr_f(i0,r0,r1)	_vfp_buneqr_f(_jit,i0,r0,r1)
static jit_word_t
_vfp_buneqr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define vfp_buneqi_f(i0,r0,i1)	_vfp_buneqi_f(_jit,i0,r0,i1)
static jit_word_t
_vfp_buneqi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define vfp_buneqr_d(i0,r0,r1)	_vfp_buneqr_d(_jit,i0,r0,r1)
static jit_word_t
_vfp_buneqr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define vfp_buneqi_d(i0,r0,i1)	_vfp_buneqi_d(_jit,i0,r0,i1)
static jit_word_t
_vfp_buneqi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define vfp_bunger_x(i0)		_vfp_bunger_x(_jit,i0)
static jit_word_t _vfp_bunger_x(jit_state_t*,jit_word_t);
#  define vfp_bunger_f(i0,r0,r1)	_vfp_bunger_f(_jit,i0,r0,r1)
static jit_word_t
_vfp_bunger_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define vfp_bungei_f(i0,r0,i1)	_vfp_bungei_f(_jit,i0,r0,i1)
static jit_word_t
_vfp_bungei_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define vfp_bunger_d(i0,r0,r1)	_vfp_bunger_d(_jit,i0,r0,r1)
static jit_word_t
_vfp_bunger_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define vfp_bungei_d(i0,r0,i1)	_vfp_bungei_d(_jit,i0,r0,i1)
static jit_word_t
_vfp_bungei_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define vfp_bungtr_f(i0,r0,r1)	vbcmp_f(ARM_CC_HI,i0,r0,r1)
#  define vfp_bungti_f(i0,r0,i1)	_vfp_bungti_f(_jit,i0,r0,i1)
static jit_word_t
_vfp_bungti_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define vfp_bungtr_d(i0,r0,r1)	vbcmp_d(ARM_CC_HI,i0,r0,r1)
#  define vfp_bungti_d(i0,r0,i1)	_vfp_bungti_d(_jit,i0,r0,i1)
static jit_word_t
_vfp_bungti_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define vfp_bltgtr_x(i0)		_vfp_bltgtr_x(_jit,i0)
static jit_word_t _vfp_bltgtr_x(jit_state_t*,jit_word_t);
#  define vfp_bltgtr_f(i0,r0,r1)	_vfp_bltgtr_f(_jit,i0,r0,r1)
static jit_word_t
_vfp_bltgtr_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define vfp_bltgti_f(i0,r0,i1)	_vfp_bltgti_f(_jit,i0,r0,i1)
static jit_word_t
_vfp_bltgti_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define vfp_bltgtr_d(i0,r0,r1)	_vfp_bltgtr_d(_jit,i0,r0,r1)
static jit_word_t
_vfp_bltgtr_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define vfp_bltgti_d(i0,r0,i1)	_vfp_bltgti_d(_jit,i0,r0,i1)
static jit_word_t
_vfp_bltgti_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define vfp_bordr_f(i0,r0,r1)		vbcmp_f(ARM_CC_VC,i0,r0,r1)
#  define vfp_bordi_f(i0,r0,i1)		_vfp_bordi_f(_jit,i0,r0,i1)
static jit_word_t
_vfp_bordi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define vfp_bordr_d(i0,r0,r1)		vbcmp_d(ARM_CC_VC,i0,r0,r1)
#  define vfp_bordi_d(i0,r0,i1)		_vfp_bordi_d(_jit,i0,r0,i1)
static jit_word_t
_vfp_bordi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define vfp_bunordr_f(i0,r0,r1)	vbcmp_f(ARM_CC_VS,i0,r0,r1)
#  define vfp_bunordi_f(i0,r0,i1)	_vfp_bunordi_f(_jit,i0,r0,i1)
static jit_word_t
_vfp_bunordi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_float32_t);
#  define vfp_bunordr_d(i0,r0,r1)	vbcmp_d(ARM_CC_VS,i0,r0,r1)
#  define vfp_bunordi_d(i0,r0,i1)	_vfp_bunordi_d(_jit,i0,r0,i1)
static jit_word_t
_vfp_bunordi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_float64_t);
#  define vfp_ldr_f(r0,r1)		VLDR_F32(r0,r1,0)
#  define vfp_ldr_d(r0,r1)		VLDR_F64(r0,r1,0)
#  define vfp_ldi_f(r0,i0)		_vfp_ldi_f(_jit,r0,i0)
static void _vfp_ldi_f(jit_state_t*,jit_int32_t,jit_word_t);
#  define vfp_ldi_d(r0,i0)		_vfp_ldi_d(_jit,r0,i0)
static void _vfp_ldi_d(jit_state_t*,jit_int32_t,jit_word_t);
#  define vfp_ldxr_f(r0,r1,r2)		_vfp_ldxr_f(_jit,r0,r1,r2)
static void _vfp_ldxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define vfp_ldxr_d(r0,r1,r2)		_vfp_ldxr_d(_jit,r0,r1,r2)
static void _vfp_ldxr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define vfp_ldxi_f(r0,r1,i0)		_vfp_ldxi_f(_jit,r0,r1,i0)
static void _vfp_ldxi_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define vfp_ldxi_d(r0,r1,i0)		_vfp_ldxi_d(_jit,r0,r1,i0)
static void _vfp_ldxi_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define vfp_str_f(r0,r1)		VSTR_F32(r1,r0,0)
#  define vfp_str_d(r0,r1)		VSTR_F64(r1,r0,0)
#  define vfp_sti_f(i0,r0)		_vfp_sti_f(_jit,i0,r0)
static void _vfp_sti_f(jit_state_t*,jit_word_t,jit_int32_t);
#  define vfp_sti_d(i0,r0)		_vfp_sti_d(_jit,i0,r0)
static void _vfp_sti_d(jit_state_t*,jit_word_t,jit_int32_t);
#  define vfp_stxr_f(r0,r1,r2)		_vfp_stxr_f(_jit,r0,r1,r2)
static void _vfp_stxr_f(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define vfp_stxr_d(r0,r1,r2)		_vfp_stxr_d(_jit,r0,r1,r2)
static void _vfp_stxr_d(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define vfp_stxi_f(i0,r0,r1)		_vfp_stxi_f(_jit,i0,r0,r1)
static void _vfp_stxi_f(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define vfp_stxi_d(i0,r0,r1)		_vfp_stxi_d(_jit,i0,r0,r1)
static void _vfp_stxi_d(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define vfp_vaarg_d(r0, r1)		_vfp_vaarg_d(_jit, r0, r1)
static void _vfp_vaarg_d(jit_state_t*, jit_int32_t, jit_int32_t);
#endif

#if CODE
#  define vfp_regno(rn)		(((rn) - 16) >> 1)

static int
encode_vfp_double(int mov, int inv, unsigned lo, unsigned hi)
{
    int		code, mode, imm, mask;

    if (hi != lo) {
	if (mov && !inv) {
	    /* (I64)
	     *	aaaaaaaabbbbbbbbccccccccddddddddeeeeeeeeffffffffgggggggghhhhhhhh
	     */
	    for (mode = 0, mask = 0xff; mode < 4; mask <<= 8, mode++) {
		imm = lo & mask;
		if (imm != mask && imm != 0)
		    goto fail;
		imm = hi & mask;
		if (imm != mask && imm != 0)
		    goto fail;
	    }
	    mode = 0xe20;
	    imm = (((hi & 0x80000000) >> 24) | ((hi & 0x00800000) >> 17) |
		   ((hi & 0x00008000) >> 10) | ((hi & 0x00000080) >>  3) |
		   ((lo & 0x80000000) >> 28) | ((lo & 0x00800000) >> 21) |
		   ((lo & 0x00008000) >> 14) | ((lo & 0x00000080) >>  7));
	    goto success;
	}
	goto fail;
    }
    /*  (I32)
     *  00000000 00000000 00000000 abcdefgh
     *  00000000 00000000 abcdefgh 00000000
     *  00000000 abcdefgh 00000000 00000000
     *  abcdefgh 00000000 00000000 00000000 */
    for (mode = 0, mask = 0xff; mode < 4; mask <<= 8, mode++) {
	if ((lo & mask) == lo) {
	    imm = lo >> (mode << 3);
	    mode <<= 9;
	    goto success;
	}
    }
    /*  (I16)
     *  00000000 abcdefgh 00000000 abcdefgh
     *  abcdefgh 00000000 abcdefgh 00000000 */
    for (mode = 0, mask = 0xff; mode < 2; mask <<= 8, mode++) {
	if ((lo & mask) && ((lo & (mask << 16)) >> 16) == (lo & mask)) {
	    imm = lo >> (mode << 3);
	    mode = 0x800 | (mode << 9);
	    goto success;
	}
    }
    if (mov) {
	/*  (I32)
	 *  00000000 00000000 abcdefgh 11111111
	 *  00000000 abcdefgh 11111111 11111111 */
	for (mode = 0, mask = 0xff; mode < 2;
	     mask = (mask << 8) | 0xff, mode++) {
	    if ((lo & mask) == mask &&
		!((lo & ~mask) >> 8) &&
		(imm = lo >> (8 + (mode << 8)))) {
		mode = 0xc00 | (mode << 8);
		goto success;
	    }
	}
	if (!inv) {
	    /* (F32)
	     *  aBbbbbbc defgh000 00000000 00000000
	     *  from the ARM Architecture Reference Manual:
	     *  In this entry, B = NOT(b). The bit pattern represents the
	     *  floating-point number (-1)^s* 2^exp * mantissa, where
	     *  S = UInt(a),
	     *  exp = UInt(NOT(b):c:d)-3 and
	     *  mantissa = (16+UInt(e:f:g:h))/16. */
	    if ((lo & 0x7ffff) == 0 &&
		(((lo & 0x7e000000) == 0x3e000000) ||
		 ((lo & 0x7e000000) == 0x40000000))) {
		mode = 0xf00;
		imm = ((lo >> 24) & 0x80) | ((lo >> 19) & 0x7f);
		goto success;
	    }
	}
    }

fail:
    /* need another approach (load from memory, move from arm register, etc) */
    return (-1);

success:
    code = inv ? ARM_VMVNI : ARM_VMOVI;
    switch ((mode & 0xf00) >> 8) {
	case 0x0:	case 0x2:	case 0x4:	case 0x6:
	case 0x8:	case 0xa:
	    if (inv)	mode |= 0x20;
	    if (!mov)	mode |= 0x100;
	    break;
	case 0x1:	case 0x3:	case 0x5:	case 0x7:
	    /* should actually not reach here */
	    assert(!inv);
	case 0x9:	case 0xb:
	    assert(!mov);
	    break;
	case 0xc:	case 0xd:
	    /* should actually not reach here */
	    assert(inv);
	case 0xe:
	    assert(mode & 0x20);
	    assert(mov && !inv);
	    break;
	default:
	    assert(!(mode & 0x20));
	    break;
    }
    imm = ((imm & 0x80) << 17) | ((imm & 0x70) << 12) | (imm & 0x0f);
    code |= mode | imm;
    if (jit_thumb_p()) {
	if (code & 0x1000000)
	    code |= 0xff000000;
	else
	    code |= 0xef000000;
    }
    else
	code |= ARM_CC_NV;
    return (code);
}

static void
_vodi(jit_state_t *_jit, int oi, int r0)
{
    jit_thumb_t	thumb;
    assert(!(oi  & 0x0000f000));
    assert(!(r0 & 1));	r0 = vfp_regno(r0);
    thumb.i = oi|(_u4(r0)<<12);
    if (jit_thumb_p())
	iss(thumb.s[0], thumb.s[1]);
    else
	ii(thumb.i);
}

static void
_voqi(jit_state_t *_jit, int oi, int r0)
{
    jit_thumb_t	thumb;
    assert(!(oi  & 0x0000f000));
    assert(!(r0 & 3));	r0 = vfp_regno(r0);
    thumb.i = oi|(_u4(r0)<<12);
    if (jit_thumb_p())
	iss(thumb.s[0], thumb.s[1]);
    else
	ii(thumb.i);
}

static void
_cc_vo_ss(jit_state_t *_jit, int cc, int o, int r0, int r1)
{
    jit_thumb_t	thumb;
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf000f00f));
    if (r0 & 1)	o |= ARM_V_D;	r0 = vfp_regno(r0);
    if (r1 & 1)	o |= ARM_V_M;	r1 = vfp_regno(r1);
    thumb.i = cc|o|(_u4(r0)<<12)|_u4(r1);
    if (jit_thumb_p())
	iss(thumb.s[0], thumb.s[1]);
    else
	ii(thumb.i);
}

static void
_cc_vo_dd(jit_state_t *_jit, int cc, int o, int r0, int r1)
{
    jit_thumb_t	thumb;
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf000f00f));
    assert(!(r0 & 1) && !(r1 & 1));
    r0 = vfp_regno(r0);	r1 = vfp_regno(r1);
    thumb.i = cc|o|(_u4(r0)<<12)|_u4(r1);
    if (jit_thumb_p())
	iss(thumb.s[0], thumb.s[1]);
    else
	ii(thumb.i);
}

static void
_cc_vo_qd(jit_state_t *_jit, int cc, int o, int r0, int r1)
{
    jit_thumb_t	thumb;
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf000f00f));
    assert(!(r0 & 3) && !(r1 & 1));
    r0 = vfp_regno(r0);	r1 = vfp_regno(r1);
    thumb.i = cc|o|(_u4(r0)<<12)|_u4(r1);
    if (jit_thumb_p())
	iss(thumb.s[0], thumb.s[1]);
    else
	ii(thumb.i);
}

static void
_cc_vo_qq(jit_state_t *_jit, int cc, int o, int r0, int r1)
{
    jit_thumb_t	thumb;
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf000f00f));
    assert(!(r0 & 3) && !(r1 & 3));
    r0 = vfp_regno(r0);	r1 = vfp_regno(r1);
    thumb.i = cc|o|(_u4(r0)<<12)|_u4(r1);
    if (jit_thumb_p())
	iss(thumb.s[0], thumb.s[1]);
    else
	ii(thumb.i);
}

static void
_cc_vorr_(jit_state_t *_jit, int cc, int o, int r0, int r1)
{
    jit_thumb_t	thumb;
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf000f00f));
    thumb.i = cc|o|(_u4(r1)<<16)|(_u4(r0)<<12);
    if (jit_thumb_p())
	iss(thumb.s[0], thumb.s[1]);
    else
	ii(thumb.i);
}

static void
_cc_vors_(jit_state_t *_jit, int cc, int o, int r0, int r1)
{
    jit_thumb_t	thumb;
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf000f00f));
    if (r1 & 1)	o |= ARM_V_N;	r1 = vfp_regno(r1);
    thumb.i = cc|o|(_u4(r1)<<16)|(_u4(r0)<<12);
    if (jit_thumb_p())
	iss(thumb.s[0], thumb.s[1]);
    else
	ii(thumb.i);
}

static void
_cc_vorv_(jit_state_t *_jit, int cc, int o, int r0, int r1)
{
    jit_thumb_t	thumb;
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf000f00f));
    if (r1 & 1)	o |= ARM_V_M;	r1 = vfp_regno(r1);
    thumb.i = cc|o|(_u4(r1)<<16)|(_u4(r0)<<12);
    if (jit_thumb_p())
	iss(thumb.s[0], thumb.s[1]);
    else
	ii(thumb.i);
}

static void
_cc_vori_(jit_state_t *_jit, int cc, int o, int r0, int r1)
{
    jit_thumb_t	thumb;
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf000f00f));
    /* use same bit pattern, to set opc1... */
    if (r1 & 1)	o |= ARM_V_I32;	r1 = vfp_regno(r1);
    thumb.i = cc|o|(_u4(r1)<<16)|(_u4(r0)<<12);
    if (jit_thumb_p())
	iss(thumb.s[0], thumb.s[1]);
    else
	ii(thumb.i);
}

static void
_cc_vorrd(jit_state_t *_jit, int cc, int o, int r0, int r1, int r2)
{
    jit_thumb_t	thumb;
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf00ff00f));
    assert(!(r2 & 1));
    r2 = vfp_regno(r2);
    thumb.i = cc|o|(_u4(r1)<<16)|(_u4(r0)<<12)|_u4(r2);
    if (jit_thumb_p())
	iss(thumb.s[0], thumb.s[1]);
    else
	ii(thumb.i);
}

static void
_cc_vosss(jit_state_t *_jit, int cc, int o, int r0, int r1, int r2)
{
    jit_thumb_t	thumb;
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf00ff00f));
    if (r0 & 1)	o |= ARM_V_D;	r0 = vfp_regno(r0);
    if (r1 & 1)	o |= ARM_V_N;	r1 = vfp_regno(r1);
    if (r2 & 1)	o |= ARM_V_M;	r2 = vfp_regno(r2);
    thumb.i = cc|o|(_u4(r1)<<16)|(_u4(r0)<<12)|_u4(r2);
    if (jit_thumb_p())
	iss(thumb.s[0], thumb.s[1]);
    else
	ii(thumb.i);
}

static void
_cc_voddd(jit_state_t *_jit, int cc, int o, int r0, int r1, int r2)
{
    jit_thumb_t	thumb;
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf00ff00f));
    assert(!(r0 & 1) && !(r1 & 1) && !(r2 & 1));
    r0 = vfp_regno(r0);	r1 = vfp_regno(r1);	r2 = vfp_regno(r2);
    thumb.i = cc|o|(_u4(r1)<<16)|(_u4(r0)<<12)|_u4(r2);
    if (jit_thumb_p())
	iss(thumb.s[0], thumb.s[1]);
    else
	ii(thumb.i);
}

static void
_cc_voqdd(jit_state_t *_jit, int cc, int o, int r0, int r1, int r2)
{
    jit_thumb_t	thumb;
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf00ff00f));
    assert(!(r0 & 3) && !(r1 & 1) && !(r2 & 1));
    r0 = vfp_regno(r0);	r1 = vfp_regno(r1);	r2 = vfp_regno(r2);
    thumb.i = cc|o|(_u4(r1)<<16)|(_u4(r0)<<12)|_u4(r2);
    if (jit_thumb_p())
	iss(thumb.s[0], thumb.s[1]);
    else
	ii(thumb.i);
}

static void
_cc_voqqd(jit_state_t *_jit, int cc, int o, int r0, int r1, int r2)
{
    jit_thumb_t	thumb;
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf00ff00f));
    assert(!(r0 & 3) && !(r1 & 3) && !(r2 & 1));
    r0 = vfp_regno(r0);	r1 = vfp_regno(r1);	r2 = vfp_regno(r2);
    thumb.i = cc|o|(_u4(r1)<<16)|(_u4(r0)<<12)|_u4(r2);
    if (jit_thumb_p())
	iss(thumb.s[0], thumb.s[1]);
    else
	ii(thumb.i);
}

static void
_cc_voqqq(jit_state_t *_jit, int cc, int o, int r0, int r1, int r2)
{
    jit_thumb_t	thumb;
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf00ff00f));
    assert(!(r0 & 3) && !(r1 & 3) && !(r2 & 3));
    r0 = vfp_regno(r0);	r1 = vfp_regno(r1);	r2 = vfp_regno(r2);
    thumb.i = cc|o|(_u4(r1)<<16)|(_u4(r0)<<12)|_u4(r2);
    if (jit_thumb_p())
	iss(thumb.s[0], thumb.s[1]);
    else
	ii(thumb.i);
}

static void
_cc_vldst(jit_state_t *_jit, int cc, int o, int r0, int r1, int i0)
{
    jit_thumb_t	thumb;
    /* i0 << 2 is byte offset */
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf00ff0ff));
    if (r0 & 1) {
	assert(!(o & ARM_V_F64));
	o |= ARM_V_D;
    }
    r0 = vfp_regno(r0);
    thumb.i = cc|o|(_u4(r1)<<16)|(_u4(r0)<<12)|_u8(i0);
    if (jit_thumb_p())
	iss(thumb.s[0], thumb.s[1]);
    else
	ii(thumb.i);
}

static void
_cc_vorsl(jit_state_t *_jit, int cc, int o, int r0, int r1, int i0)
{
    jit_thumb_t	thumb;
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf00ff0ff));
    /* save i0 double precision registers */
    if (o & ARM_V_F64)		i0 <<= 1;
    /* if (r1 & 1) cc & ARM_V_F64 must be false */
    if (r1 & 1)	o |= ARM_V_D;	r1 = vfp_regno(r1);
    assert(i0 && !(i0 & 1) && r1 + i0 <= 32);
    thumb.i = cc|o|(_u4(r0)<<16)|(_u4(r1)<<12)|_u8(i0);
    if (jit_thumb_p())
	iss(thumb.s[0], thumb.s[1]);
    else
	ii(thumb.i);
}

static void
_vfp_movr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 != r1) {
	if (jit_fpr_p(r1)) {
	    if (jit_fpr_p(r0))
		VMOV_F32(r0, r1);
	    else
		VMOV_A_S(r0, r1);
	}
	else if (jit_fpr_p(r0))
	    VMOV_S_A(r0, r1);
	else
	    movr(r0, r1);
    }
}

static void
_vfp_movr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 != r1) {
	if (jit_fpr_p(r1)) {
	    if (jit_fpr_p(r0))
		VMOV_F64(r0, r1);
	    else
		VMOV_AA_D(r0, r0 + 1, r1);
	}
	else if (jit_fpr_p(r0))
	    VMOV_D_AA(r0, r1, r1 + 1);
	else {
	    /* minor consistency check */
	    assert(r0 + 1 != r1 && r0 -1 != r1);
	    movr(r0, r1);
	    movr(r0 + 1, r1 + 1);
	}
    }
}

static void
_vfp_movi_f(jit_state_t *_jit, jit_int32_t r0, jit_float32_t i0)
{
    union {
	jit_int32_t	i;
	jit_float32_t	f;
    } u;
    jit_int32_t		reg;
    jit_int32_t		code;
    u.f = i0;
    if (jit_fpr_p(r0)) {
	/* float arguments are packed, for others,
	 * lightning only address even registers */
	if (!(r0 & 1) && (r0 - 16) >= 0 &&
	    ((code = encode_vfp_double(1, 0, u.i, u.i)) != -1 ||
	     (code = encode_vfp_double(1, 1, ~u.i, ~u.i)) != -1))
	    VIMM(code, r0);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), u.i);
	    VMOV_S_A(r0, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else
	movi(r0, u.i);
}

static void
_vfp_movi_d(jit_state_t *_jit, jit_int32_t r0, jit_float64_t i0)
{
    union {
	jit_int32_t	i[2];
	jit_float64_t	d;
    } u;
    jit_int32_t		code;
    jit_int32_t		rg0, rg1;
    u.d = i0;
    if (jit_fpr_p(r0)) {
	if ((code = encode_vfp_double(1, 0, u.i[0], u.i[1])) != -1 ||
	    (code = encode_vfp_double(1, 1, ~u.i[0], ~u.i[1])) != -1)
	    VIMM(code, r0);
	else {
	    rg0 = jit_get_reg(jit_class_gpr);
	    rg1 = jit_get_reg(jit_class_gpr);
	    movi(rn(rg0), u.i[0]);
	    movi(rn(rg1), u.i[1]);
	    VMOV_D_AA(r0, rn(rg0), rn(rg1));
	    jit_unget_reg(rg1);
	    jit_unget_reg(rg0);
	}
    }
    else {
	movi(r0, u.i[0]);
	movi(r0 + 1, u.i[1]);
    }
}

static void
_vfp_extr_d_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r1)) {
	if (jit_fpr_p(r0))
	    VCVT_F64_F32(r0, r1);
	else {
	    reg = jit_get_reg(jit_class_fpr);
	    VCVT_F64_F32(rn(reg), r1);
	    VMOV_A_S(r0, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	reg = jit_get_reg(jit_class_fpr);
	VMOV_S_A(rn(reg), r1);
	VCVT_F64_F32(rn(reg), rn(reg));
	if (jit_fpr_p(r0))
	    VMOV_F32(r0, rn(reg));
	else
	    VMOV_A_S(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_vfp_extr_f_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r1)) {
	if (jit_fpr_p(r0))
	    VCVT_F32_F64(r0, r1);
	else {
	    reg = jit_get_reg(jit_class_fpr);
	    VCVT_F32_F64(rn(reg), r1);
	    VMOV_AA_D(r0, r0 + 1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	reg = jit_get_reg(jit_class_fpr);
	VMOV_D_AA(rn(reg), r1, r1 + 1);
	VCVT_F32_F64(rn(reg), rn(reg));
	if (jit_fpr_p(r0))
	    VMOV_F64(r0, rn(reg));
	else
	    VMOV_AA_D(r0, r0 + 1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_vfp_extr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r0)) {
	VMOV_V_I32(r0, r1);
	VCVT_F32_S32(r0, r0);
    }
    else {
	reg = jit_get_reg(jit_class_fpr);
	VMOV_V_I32(rn(reg), r1);
	VCVT_F32_S32(rn(reg), rn(reg));
	VMOV_F32(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_vfp_extr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r0)) {
	VMOV_V_I32(r0, r1);
	VCVT_F64_S32(r0, r0);
    }
    else {
	reg = jit_get_reg(jit_class_fpr);
	VMOV_V_I32(rn(reg), r1);
	VCVT_F64_S32(rn(reg), rn(reg));
	VMOV_F64(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_vfp_truncr_f_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr);
    if (jit_fpr_p(r1))
	VCVT_S32_F32(rn(reg), r1);
    else {
	VMOV_V_I32(rn(reg), r1);
	VCVT_S32_F32(rn(reg), rn(reg));
    }
    VMOV_A_S32(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_vfp_truncr_d_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_fpr);
    if (jit_fpr_p(r1))
	VCVT_S32_F64(rn(reg), r1);
    else {
	VMOV_V_I32(rn(reg), r1);
	VCVT_S32_F64(rn(reg), rn(reg));
    }
    VMOV_A_S32(r0, rn(reg));
    jit_unget_reg(reg);
}

#  define fopi(name)							\
static void								\
_vfp_##name##i_f(jit_state_t *_jit,					\
		 jit_int32_t r0, jit_int32_t r1, jit_float32_t i0)	\
{									\
    jit_int32_t		reg = jit_get_reg(jit_class_fpr);		\
    vfp_movi_f(rn(reg), i0);						\
    vfp_##name##r_f(r0, r1, rn(reg));					\
    jit_unget_reg(reg);							\
}
#  define dopi(name)							\
static void								\
_vfp_##name##i_d(jit_state_t *_jit,					\
		 jit_int32_t r0, jit_int32_t r1, jit_float64_t i0)	\
{									\
    jit_int32_t		reg = jit_get_reg(jit_class_fpr);		\
    vfp_movi_d(rn(reg), i0);						\
    vfp_##name##r_d(r0, r1, rn(reg));					\
    jit_unget_reg(reg);							\
}
#  define fbopi(name)							\
static jit_word_t							\
_vfp_b##name##i_f(jit_state_t *_jit,					\
		  jit_int32_t r0, jit_int32_t r1, jit_float32_t i0)	\
{									\
    jit_word_t		word;						\
    jit_int32_t		reg = jit_get_reg(jit_class_fpr|		\
					  jit_class_nospill);		\
    vfp_movi_f(rn(reg), i0);						\
    word = vfp_b##name##r_f(r0, r1, rn(reg));				\
    jit_unget_reg(reg);							\
    return (word);							\
}
#  define dbopi(name)							\
static jit_word_t							\
_vfp_b##name##i_d(jit_state_t *_jit,					\
		  jit_int32_t r0, jit_int32_t r1, jit_float64_t i0)	\
{									\
    jit_word_t		word;						\
    jit_int32_t		reg = jit_get_reg(jit_class_fpr|		\
					  jit_class_nospill);		\
    vfp_movi_d(rn(reg), i0);						\
    word = vfp_b##name##r_d(r0, r1, rn(reg));				\
    jit_unget_reg(reg);							\
    return (word);							\
}

fopi(add)
dopi(add)
fopi(sub)
fopi(rsb)
dopi(rsb)
dopi(sub)
fopi(mul)
dopi(mul)
fopi(div)
dopi(div)

static void
_vfp_cmp_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		rg0, rg1;
    if (jit_fpr_p(r0)) {
	if (jit_fpr_p(r1))
	    VCMP_F32(r0, r1);
	else {
	    rg1 = jit_get_reg(jit_class_fpr);
	    VMOV_S_A(rn(rg1), r1);
	    VCMP_F32(r0, rn(rg1));
	    jit_unget_reg(rg1);
	}
    }
    else {
	rg0 = jit_get_reg(jit_class_fpr);
	VMOV_S_A(rn(rg0), r0);
	if (jit_fpr_p(r1))
	    VCMP_F32(rn(rg0), r1);
	else {
	    rg1 = jit_get_reg(jit_class_fpr);
	    VMOV_S_A(rn(rg1), r1);
	    VCMP_F32(rn(rg0), rn(rg1));
	    jit_unget_reg(rg1);
	}
	jit_unget_reg(rg0);
    }
}

static void
_vfp_cmp_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		rg0, rg1;
    if (jit_fpr_p(r0)) {
	if (jit_fpr_p(r1))
	    VCMP_F64(r0, r1);
	else {
	    rg1 = jit_get_reg(jit_class_fpr);
	    VMOV_D_AA(rn(rg1), r1, r1 + 1);
	    VCMP_F64(r0, rn(rg1));
	    jit_unget_reg(rg1);
	}
    }
    else {
	rg0 = jit_get_reg(jit_class_fpr);
	VMOV_D_AA(rn(rg0), r0, r0 + 1);
	if (jit_fpr_p(r1))
	    VCMP_F64(rn(rg0), r1);
	else {
	    rg1 = jit_get_reg(jit_class_fpr);
	    VMOV_D_AA(rn(rg1), r1, r1 + 1);
	    VCMP_F64(rn(rg0), rn(rg1));
	    jit_unget_reg(rg1);
	}
	jit_unget_reg(rg0);
    }
}

static void
_vcmp01_x(jit_state_t *_jit, int c0, int c1, jit_int32_t r0)
{
    VMRS(_R15_REGNO);
    if (jit_thumb_p()) {
	if ((c0 ^ c1) >> 28 == 1) {
	    ITE(c0);
	    if (r0 < 8) {
		T1_MOVI(r0, 0);
		T1_MOVI(r0, 1);
	    }
	    else {
		T2_MOVI(r0, 0);
		T2_MOVI(r0, 1);
	    }
	}
	else {
	    if (r0 < 8) {
		IT(c0);
		T1_MOVI(r0, 0);
		IT(c1);
		T1_MOVI(r0, 1);
	    }
	    else {
		IT(c0);
		T2_MOVI(r0, 0);
		IT(c1);
		T2_MOVI(r0, 1);
	    }
	}
    }
    else {
	CC_MOVI(c0, r0, 0);
	CC_MOVI(c1, r0, 1);
    }
}

static void
_vcmp01_f(jit_state_t *_jit, int c0, int c1,
	  jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    vfp_cmp_f(r1, r2);
    vcmp01_x(c0, c1, r0);
}

static void
_vcmp01_d(jit_state_t *_jit, int c0, int c1,
	  jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    vfp_cmp_d(r1, r2);
    vcmp01_x(c0, c1, r0);
}

static void
_vcmp10_x(jit_state_t *_jit, int cc, jit_int32_t r0)
{
    if (jit_thumb_p()) {
	if (r0 < 8) {
	    T1_MOVI(r0, 1);
	    VMRS(_R15_REGNO);
	    IT(cc);
	    T1_MOVI(r0, 0);
	}
	else {
	    T2_MOVI(r0, 1);
	    VMRS(_R15_REGNO);
	    IT(cc);
	    T2_MOVI(r0, 0);
	}
    }
    else {
	VMRS(_R15_REGNO);
	MOVI(r0, 1);
	CC_MOVI(cc, r0, 0);
    }
}
static void
_vcmp_10_f(jit_state_t *_jit, int cc,
	   jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    vfp_cmp_f(r1, r2);
    vcmp10_x(cc, r0);
}

static void
_vcmp_10_d(jit_state_t *_jit, int cc,
	   jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    vfp_cmp_d(r1, r2);
    vcmp10_x(cc, r0);
}

fopi(lt)
dopi(lt)
fopi(le)
dopi(le)
fopi(eq)
dopi(eq)
fopi(ge)
dopi(ge)
fopi(gt)
dopi(gt)
fopi(ne)
dopi(ne)
fopi(unlt)
dopi(unlt)
fopi(unle)
dopi(unle)

static void
_vfp_uneqr_x(jit_state_t *_jit, jit_int32_t r0)
{
    VMRS(_R15_REGNO);
    if (jit_thumb_p()) {
	ITE(ARM_CC_NE);
	if (r0 < 8) {
	    T1_MOVI(r0, 0);
	    T1_MOVI(r0, 1);
	    IT(ARM_CC_VS);
	    T1_MOVI(r0, 1);
	}
	else {
	    T2_MOVI(r0, 0);
	    T2_MOVI(r0, 1);
	    IT(ARM_CC_VS);
	    T2_MOVI(r0, 1);
	}
    }
    else {
	CC_MOVI(ARM_CC_NE, r0, 0);
	CC_MOVI(ARM_CC_EQ, r0, 1);
	CC_MOVI(ARM_CC_VS, r0, 1);
    }
}

static void
_vfp_uneqr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    vfp_cmp_f(r1, r2);
    vfp_uneqr_x(r0);
}

fopi(uneq)

static void
_vfp_uneqr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    vfp_cmp_d(r1, r2);
    vfp_uneqr_x(r0);
}

dopi(uneq)

static void
_vcmp_01_x(jit_state_t *_jit, int cc, jit_int32_t r0)
{
    if (jit_thumb_p()) {
	if (r0 < 8) {
	    T1_MOVI(r0, 0);
	    VMRS(_R15_REGNO);
	    IT(cc);
	    T1_MOVI(r0, 1);
	}
	else {
	    T2_MOVI(r0, 0);
	    VMRS(_R15_REGNO);
	    IT(cc);
	    T2_MOVI(r0, 1);
	}
    }
    else {
	MOVI(r0, 0);
	VMRS(_R15_REGNO);
	CC_MOVI(cc, r0, 1);
    }
}

static void
_vcmp_01_f(jit_state_t *_jit, int cc,
	   jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    vfp_cmp_f(r1, r2);
    vcmp_01_x(cc, r0);
}

static void
_vcmp_01_d(jit_state_t *_jit, int cc,
	   jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    vfp_cmp_d(r1, r2);
    vcmp_01_x(cc, r0);
}

fopi(unge)
dopi(unge)
fopi(ungt)
dopi(ungt)

static void
_vfp_ltgtr_x(jit_state_t *_jit, jit_int32_t r0)
{
    VMRS(_R15_REGNO);
    if (jit_thumb_p()) {
	ITE(ARM_CC_NE);
	if (r0 < 8) {
	    T1_MOVI(r0, 1);
	    T1_MOVI(r0, 0);
	    IT(ARM_CC_VS);
	    T1_MOVI(r0, 0);
	}
	else {
	    T2_MOVI(r0, 1);
	    T2_MOVI(r0, 0);
	    IT(ARM_CC_VS);
	    T2_MOVI(r0, 0);
	}
    }
    else {
	CC_MOVI(ARM_CC_NE, r0, 1);
	CC_MOVI(ARM_CC_EQ, r0, 0);
	CC_MOVI(ARM_CC_VS, r0, 0);
    }
}

static void
_vfp_ltgtr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    vfp_cmp_f(r1, r2);
    vfp_ltgtr_x(r0);
}

fopi(ltgt)

static void
_vfp_ltgtr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    vfp_cmp_d(r1, r2);
    vfp_ltgtr_x(r0);
}

dopi(ltgt)

static void
_vfp_ordr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    vfp_cmp_f(r1, r2);
    vcmp10_x(ARM_CC_VS, r0);
}

fopi(ord)

static void
_vfp_ordr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    vfp_cmp_d(r1, r2);
    vcmp10_x(ARM_CC_VS, r0);
}

dopi(ord)

static void
_vfp_unordr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    vfp_cmp_f(r1, r2);
    vcmp_01_x(ARM_CC_VS, r0);
}

fopi(unord)

static void
_vfp_unordr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    vfp_cmp_d(r1, r2);
    vcmp_01_x(ARM_CC_VS, r0);
}

dopi(unord)

static jit_word_t
_vbcmp_x(jit_state_t *_jit, int cc, jit_word_t i0)
{
    jit_word_t		d, w;
    VMRS(_R15_REGNO);
    w = _jit->pc.w;
    if (jit_thumb_p()) {
	d = ((i0 - w) >> 1) - 2;
	assert(_s20P(d));
	T2_CC_B(cc, encode_thumb_cc_jump(d));
    }
    else {
	d = ((i0 - w) >> 2) - 2;
	assert(_s24P(d));
	CC_B(cc, d & 0x00ffffff);
    }
    return (w);
}


static jit_word_t
_vbcmp_f(jit_state_t *_jit, int cc,
	 jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    vfp_cmp_f(r0, r1);
    return (vbcmp_x(cc, i0));
}

static jit_word_t
_vbcmp_d(jit_state_t *_jit, int cc,
	 jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    vfp_cmp_d(r0, r1);
    return (vbcmp_x(cc, i0));
}

static jit_word_t
_vbncmp_x(jit_state_t *_jit, int cc, jit_word_t i0)
{
    jit_word_t		d, p, w;
    VMRS(_R15_REGNO);
    p = _jit->pc.w;
    if (jit_thumb_p()) {
	T2_CC_B(cc, 0);
	w = _jit->pc.w;
	d = ((i0 - w) >> 1) - 2;
	assert(_s20P(d));
	T2_B(encode_thumb_jump(d));
    }
    else {
	CC_B(cc, 0);
	w = _jit->pc.w;
	d = ((i0 - w) >> 2) - 2;
	assert(_s24P(d));
	B(d & 0x00ffffff);
    }
    patch_at(arm_patch_jump, p, _jit->pc.w);
    return (w);
}

static jit_word_t
_vbncmp_f(jit_state_t *_jit, int cc,
	  jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    vfp_cmp_f(r0, r1);
    return (vbncmp_x(cc, i0));
}

static jit_word_t
_vbncmp_d(jit_state_t *_jit, int cc,
	  jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    vfp_cmp_d(r0, r1);
    return (vbncmp_x(cc, i0));
}

fbopi(lt)
dbopi(lt)
fbopi(le)
dbopi(le)
fbopi(eq)
dbopi(eq)
fbopi(ge)
dbopi(ge)
fbopi(gt)
dbopi(gt)
fbopi(ne)
dbopi(ne)
fbopi(unlt)
dbopi(unlt)
fbopi(unle)
dbopi(unle)

static jit_word_t
_vfp_buneqr_x(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		d, p, q, w;
    VMRS(_R15_REGNO);
    p = _jit->pc.w;
    if (jit_thumb_p()) {
	T2_CC_B(ARM_CC_VS, 0);
	q = _jit->pc.w;
	T2_CC_B(ARM_CC_NE, 0);
	patch_at(arm_patch_jump, p, _jit->pc.w);
	w = _jit->pc.w;
	d = ((i0 - w) >> 1) - 2;
	assert(_s20P(d));
	T2_B(encode_thumb_jump(d));
    }
    else {
	CC_B(ARM_CC_VS, 0);
	q = _jit->pc.w;
	CC_B(ARM_CC_NE, 0);
	patch_at(arm_patch_jump, p, _jit->pc.w);
	w = _jit->pc.w;
	d = ((i0 - w) >> 2) - 2;
	assert(_s24P(d));
	B(d & 0x00ffffff);
    }
    patch_at(arm_patch_jump, q, _jit->pc.w);
    return (w);
}

static jit_word_t
_vfp_buneqr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    vfp_cmp_f(r0, r1);
    return (vfp_buneqr_x(i0));
}

fbopi(uneq)

static jit_word_t
_vfp_buneqr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    vfp_cmp_d(r0, r1);
    return (vfp_buneqr_x(i0));
}

dbopi(uneq)

static jit_word_t
_vfp_bunger_x(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		d, p, w;
    VMRS(_R15_REGNO);
    p = _jit->pc.w;
    if (jit_thumb_p()) {
	T2_CC_B(ARM_CC_MI, 0);
	w = _jit->pc.w;
	d = ((i0 - w) >> 1) - 2;
	assert(_s20P(d));
	T2_CC_B(ARM_CC_HS, encode_thumb_cc_jump(d));
    }
    else {
	CC_B(ARM_CC_MI, 0);
	w = _jit->pc.w;
	d = ((i0 - w) >> 2) - 2;
	assert(_s24P(d));
	CC_B(ARM_CC_HS, d & 0x00ffffff);
    }
    patch_at(arm_patch_jump, p, _jit->pc.w);
    return (w);
}

static jit_word_t
_vfp_bunger_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    vfp_cmp_f(r0, r1);
    return (vfp_bunger_x(i0));
}

fbopi(unge)

static jit_word_t
_vfp_bunger_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    vfp_cmp_d(r0, r1);
    return (vfp_bunger_x(i0));
}

dbopi(unge)

static jit_word_t
_vfp_bltgtr_x(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		d, p, q, w;
    VMRS(_R15_REGNO);
    p = _jit->pc.w;
    if (jit_thumb_p()) {
	T2_CC_B(ARM_CC_VS, 0);
	q = _jit->pc.w;
	T2_CC_B(ARM_CC_EQ, 0);
	w = _jit->pc.w;
	d = ((i0 - w) >> 1) - 2;
	assert(_s20P(d));
	T2_B(encode_thumb_jump(d));
    }
    else {
	CC_B(ARM_CC_VS, 0);
	q = _jit->pc.w;
	CC_B(ARM_CC_EQ, 0);
	w = _jit->pc.w;
	d = ((i0 - w) >> 2) - 2;
	assert(_s24P(d));
	B(d & 0x00ffffff);
    }
    patch_at(arm_patch_jump, p, _jit->pc.w);
    patch_at(arm_patch_jump, q, _jit->pc.w);
    return (w);
}

static jit_word_t
_vfp_bltgtr_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    vfp_cmp_f(r0, r1);
    return (vfp_bltgtr_x(i0));
}

fbopi(ungt)
dbopi(ungt)
fbopi(ltgt)

static jit_word_t
_vfp_bltgtr_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    vfp_cmp_d(r0, r1);
    return (vfp_bltgtr_x(i0));
}

dbopi(ltgt)
fbopi(ord)
dbopi(ord)
fbopi(unord)
dbopi(unord)

static void
_vfp_ldi_f(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		gpr;
    if (jit_fpr_p(r0)) {
	gpr = jit_get_reg(jit_class_gpr);
	movi(rn(gpr), i0);
	VLDR_F32(r0, rn(gpr), 0);
	jit_unget_reg(gpr);
    }
    else
	ldi_i(r0, i0);
}

static void
_vfp_ldi_d(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    if (jit_fpr_p(r0))
	VLDR_F64(r0, rn(reg), 0);
    else {
	ldr_i(r0, rn(reg));
	ldxi_i(r0 + 1, rn(reg), 4);
    }
    jit_unget_reg(reg);
}

static void
_vfp_ldxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r0)) {
	reg = jit_get_reg(jit_class_gpr);
	addr(rn(reg), r1, r2);
	VLDR_F32(r0, rn(reg), 0);
	jit_unget_reg(reg);
    }
    else
	ldxr_i(r0, r1, r2);
}

static void
_vfp_ldxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r1, r2);
    if (jit_fpr_p(r0))
	VLDR_F64(r0, rn(reg), 0);
    else {
	ldr_i(r0, rn(reg));
	ldxi_i(r0 + 1, rn(reg), 4);
    }
    jit_unget_reg(reg);
}

static void
_vfp_ldxi_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r0)) {
	if (i0 >= 0) {
	    assert(!(i0 & 3));
	    if (i0 < 1024)
		VLDR_F32(r0, r1, i0 >> 2);
	    else {
		reg = jit_get_reg(jit_class_gpr);
		addi(rn(reg), r1, i0);
		VLDR_F32(r0, rn(reg), 0);
		jit_unget_reg(reg);
	    }
	}
	else {
	    i0 = -i0;
	    assert(!(i0 & 3));
	    if (i0 < 1024)
		VLDRN_F32(r0, r1, i0 >> 2);
	    else {
		reg = jit_get_reg(jit_class_gpr);
		subi(rn(reg), r1, i0);
		VLDR_F32(r0, rn(reg), 0);
		jit_unget_reg(reg);
	    }
	}
    }
    else
	ldxi_i(r0, r1, i0);
}

static void
_vfp_ldxi_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r0)) {
	if (i0 >= 0) {
	    assert(!(i0 & 3));
	    if (i0 < 1024)
		VLDR_F64(r0, r1, i0 >> 2);
	    else {
		reg = jit_get_reg(jit_class_gpr);
		addi(rn(reg), r1, i0);
		VLDR_F64(r0, rn(reg), 0);
		jit_unget_reg(reg);
	    }
	}
	else {
	    i0 = -i0;
	    assert(!(i0 & 3));
	    if (i0 < 1024)
		VLDRN_F64(r0, r1, i0 >> 2);
	    else {
		reg = jit_get_reg(jit_class_gpr);
		subi(rn(reg), r1, i0);
		VLDR_F64(r0, rn(reg), 0);
		jit_unget_reg(reg);
	    }
	}
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r1, i0);
	ldr_i(r0, rn(reg));
	ldxi_i(r0 + 1, rn(reg), 4);
	jit_unget_reg(reg);
    }
}

static void
_vfp_sti_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r0)) {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	VSTR_F32(r0, rn(reg), 0);
	jit_unget_reg(reg);
    }
    else
	sti_i(i0, r0);
}

static void
_vfp_sti_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    if (jit_fpr_p(r0))
	VSTR_F64(r0, rn(reg), 0);
    else {
	str_i(rn(reg), r0);
	stxi_i(4, rn(reg), r0 + 1);
    }
    jit_unget_reg(reg);
}

static void
_vfp_stxr_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r2)) {
	reg = jit_get_reg(jit_class_gpr);
	addr(rn(reg), r0, r1);
	VSTR_F32(r2, rn(reg), 0);
	jit_unget_reg(reg);
    }
    else
	stxr_i(r0, r1, r2);
}

static void
_vfp_stxr_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    addr(rn(reg), r0, r1);
    if (jit_fpr_p(r2))
	VSTR_F64(r2, rn(reg), 0);
    else {
	str_i(rn(reg), r2);
	stxi_i(4, rn(reg), r2 + 1);
    }
    jit_unget_reg(reg);
}

static void
_vfp_stxi_f(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r1)) {
	if (i0 >= 0) {
	    assert(!(i0 & 3));
	    if (i0 < 1024)
		VSTR_F32(r1, r0, i0 >> 2);
	    else {
		reg = jit_get_reg(jit_class_gpr);
		addi(rn(reg), r0, i0);
		VSTR_F32(r1, rn(reg), 0);
		jit_unget_reg(reg);
	    }
	}
	else {
	    i0 = -i0;
	    assert(!(i0 & 3));
	    if (i0 < 1024)
		VSTRN_F32(r1, r0, i0 >> 2);
	    else {
		reg = jit_get_reg(jit_class_gpr);
		subi(rn(reg), r0, i0);
		VSTR_F32(r1, rn(reg), 0);
		jit_unget_reg(reg);
	    }
	}
    }
    else
	stxi_i(i0, r0, r1);
}

static void
_vfp_stxi_d(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (jit_fpr_p(r1)) {
	if (i0 >= 0) {
	    assert(!(i0 & 3));
	    if (i0 < 0124)
		VSTR_F64(r1, r0, i0 >> 2);
	    else {
		reg = jit_get_reg(jit_class_gpr);
		addi(rn(reg), r0, i0);
		VSTR_F64(r1, rn(reg), 0);
		jit_unget_reg(reg);
	    }
	}
	else {
	    i0 = -i0;
	    assert(!(i0 & 3));
	    if (i0 < 1024)
		VSTRN_F64(r1, r0, i0 >> 2);
	    else {
		reg = jit_get_reg(jit_class_gpr);
		subi(rn(reg), r0, i0);
		VSTR_F64(r1, rn(reg), 0);
		jit_unget_reg(reg);
	    }
	}
    }
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_i(rn(reg), r1);
	stxi_i(4, rn(reg), r1 + 1);
	jit_unget_reg(reg);
    }
}

static void
_vfp_vaarg_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;

    assert(_jitc->function->self.call & jit_call_varargs);

    /* Adjust pointer. */
    reg = jit_get_reg(jit_class_gpr);
    andi(rn(reg), r1, 7);
    addr(r1, r1, rn(reg));
    jit_unget_reg(reg);

    /* Load argument. */
    vfp_ldr_d(r0, r1);

    /* Update stack pointer. */
    addi(r1, r1, sizeof(jit_float64_t));
}
#  undef dbopi
#  undef fbopi
#  undef dopi
#  undef fopi
#endif
