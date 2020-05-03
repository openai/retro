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
#  define stxi(i0,r0,r1)		stxi_i(i0,r0,r1)
#  define ldxi(r0,r1,i0)		ldxi_i(r0,r1,i0)
#  define ldr(r0,r1)			ldr_i(r0,r1)
#  define _s20P(d)			((d) >= -(int)0x80000 && d <= 0x7ffff)
#  define _s24P(d)			((d) >= -(int)0x800000 && d <= 0x7fffff)
#  define _u3(v)			((v) & 0x7)
#  define _u4(v)			((v) & 0xf)
#  define _u5(v)			((v) & 0x1f)
#  define _u8(v)			((v) & 0xff)
#  define _u12(v)			((v) & 0xfff)
#  define _u13(v)			((v) & 0x1fff)
#  define _u16(v)			((v) & 0xffff)
#  define _u24(v)			((v) & 0xffffff)
#  define jit_thumb_p()			jit_cpu.thumb
#  define jit_no_set_flags()		_jitc->no_set_flags
#  define jit_armv5_p()			(jit_cpu.version >= 5)
#  define jit_armv5e_p()		(jit_cpu.version > 5 || (jit_cpu.version == 5 && jit_cpu.extend))
#  define jit_armv6_p()			(jit_cpu.version >= 6)
#  define jit_armv7r_p()		0
#  define stack_framesize		48
extern int	__aeabi_idivmod(int, int);
extern unsigned	__aeabi_uidivmod(unsigned, unsigned);
#  define _R0_REGNO			0x00
#  define _R1_REGNO			0x01
#  define _R2_REGNO			0x02
#  define _R3_REGNO			0x03
#  define _R4_REGNO			0x04
#  define _R5_REGNO			0x05
#  define _R6_REGNO			0x06
#  define _R7_REGNO			0x07
#  define _R8_REGNO			0x08
#  define _R9_REGNO			0x09
#  define _R10_REGNO			0x0a
#  define _R11_REGNO			0x0b
#  define _R12_REGNO			0x0c
#  define _R13_REGNO			0x0d
#  define _R14_REGNO			0x0e
#  define _R15_REGNO			0x0f
#  define _FP_REGNO			_R11_REGNO
#  define _SP_REGNO			_R13_REGNO
#  define _LR_REGNO			_R14_REGNO
#  define _PC_REGNO			_R15_REGNO
#  define ARM_CC_EQ			0x00000000	/* Z=1 */
#  define ARM_CC_NE			0x10000000	/* Z=0 */
#  define ARM_CC_HS			0x20000000	/* C=1 */
#    define ARM_CC_CS			ARM_CC_HS
#  define ARM_CC_LO			0x30000000	/* C=0 */
#    define ARM_CC_CC			ARM_CC_LO
#  define ARM_CC_MI			0x40000000	/* N=1 */
#  define ARM_CC_PL			0x50000000	/* N=0 */
#  define ARM_CC_VS			0x60000000	/* V=1 */
#  define ARM_CC_VC			0x70000000	/* V=0 */
#  define ARM_CC_HI			0x80000000	/* C=1 && Z=0 */
#  define ARM_CC_LS			0x90000000	/* C=0 || Z=1 */
#  define ARM_CC_GE			0xa0000000	/* N=V */
#  define ARM_CC_LT			0xb0000000	/* N!=V */
#  define ARM_CC_GT			0xc0000000	/* Z=0 && N=V */
#  define ARM_CC_LE			0xd0000000	/* Z=1 || N!=V */
#  define ARM_CC_AL			0xe0000000	/* always */
#  define ARM_CC_NV			0xf0000000	/* reserved */
#  define THUMB2_IT			0
#  define THUMB2_ITT			1
#  define THUMB2_ITE			2
#  define THUMB2_ITTT			3
#  define THUMB2_ITET			4
#  define THUMB2_ITTE			5
#  define THUMB2_ITEE			6
#  define THUMB2_ITTTT			7
#  define THUMB2_ITETT			8
#  define THUMB2_ITTET			9
#  define THUMB2_ITEET			10
#  define THUMB2_ITTTE			11
#  define THUMB2_ITETE			12
#  define THUMB2_ITTEE			13
#  define THUMB2_ITEEE			14
#  define ARM_MOV			0x01a00000
#  define THUMB_MOV			    0x4600
#  define ARM_MOVWI			0x03000000	/* v6t2, v7 */
#  define THUMB_MOVI			    0x2000
#  define THUMB2_MOVI			0xf0400000
#  define THUMB2_MOVWI			0xf2400000
#  define ARM_MOVTI			0x03400000
#  define THUMB2_MOVTI			0xf2c00000
#  define ARM_MVN			0x01e00000
#  define THUMB_MVN			    0x43c0
#  define THUMB2_MVN			0xea600000
#  define THUMB2_MVNI			0xf0600000
#  define ARM_I				0x02000000 /* immediate */
#  define ARM_S				0x00100000 /* set flags */
#  define ARM_ADD			0x00800000
#  define THUMB_ADD			    0x1800
#  define THUMB_ADDX			    0x4400
#  define THUMB2_ADD			0xeb000000
#  define THUMB_ADDI3			    0x1c00
#  define THUMB_ADDI8			    0x3000
#  define THUMB2_ADDI			0xf1000000
#  define THUMB2_ADDWI			0xf2000000
#  define ARM_ADC			0x00a00000
#  define THUMB_ADC			    0x4140
#  define THUMB2_ADC			0xeb400000
#  define THUMB2_ADCI			0xf1400000
#  define ARM_SUB			0x00400000
#  define THUMB_SUB			    0x1a00
#  define THUMB2_SUB			0xeba00000
#  define THUMB_SUBI3			    0x1e00
#  define THUMB_SUBI8			    0x3800
#  define THUMB2_SUBI			0xf1a00000
#  define THUMB2_SUBWI			0xf2a00000
#  define ARM_SBC			0x00c00000
#  define THUMB_SBC			    0x4180
#  define THUMB2_SBC			0xeb600000
#  define THUMB2_SBCI			0xf1600000
#  define ARM_RSB			0x00600000
#  define THUMB_RSBI			    0x4240
#  define THUMB2_RSBI			0xf1c00000
#  define ARM_MUL			0x00000090
#  define THUMB_MUL			    0x4340
#  define THUMB2_MUL			0xfb00f000
#  define ARM_UMULL			0x00800090
#  define THUMB2_UMULL			0xfba00000
#  define ARM_SMULL			0x00c00090
#  define THUMB2_SMULL			0xfb800000
#  define THUMB2_SDIV			0xfb90f0f0
#  define THUMB2_UDIV			0xfbb0f0f0
#  define ARM_AND			0x00000000
#  define THUMB_AND			    0x4000
#  define THUMB2_AND			0xea000000
#  define THUMB2_ANDI			0xf0000000
#  define ARM_BIC			0x01c00000
#  define THUMB2_BIC			0xea200000
#  define THUMB2_BICI			0xf0200000
#  define ARM_ORR			0x01800000
#  define THUMB_ORR			    0x4300
#  define THUMB2_ORR			0xea400000
#  define THUMB2_ORRI			0xf0400000
#  define ARM_EOR			0x00200000
#  define THUMB_EOR			    0x4040
#  define THUMB2_EOR			0xea800000
#  define THUMB2_EORI			0xf0800000
/* >> ARMv6* */
#  define ARM_REV			0x06bf0f30
#  define THUMB_REV			    0xba00
#  define THUMB2_REV			0xfa90f080
#  define ARM_REV16			0x06bf0fb0
#  define THUMB_REV16			    0xba40
#  define THUMB2_REV16			0xfa90f090
#  define ARM_SXTB			0x06af0070
#  define THUMB_SXTB			    0xb240
#  define THUMB2_SXTB			0xfa40f080
#  define ARM_UXTB			0x06ef0070
#  define THUMB_UXTB			    0xb2c0
#  define THUMB2_UXTB			0xfa50f080
#  define ARM_SXTH			0x06bf0070
#  define THUMB_SXTH			    0xb200
#  define THUMB2_SXTH			0xfa00f080
#  define ARM_UXTH			0x06ff0070
#  define THUMB_UXTH			    0xb280
#  define THUMB2_UXTH			0xfa10f080
#  define ARM_XTR8			0x00000400 /* ?xt? rotate 8 bits */
#  define ARM_XTR16			0x00000800 /* ?xt? rotate 16 bits */
#  define ARM_XTR24			0x00000c00 /* ?xt? rotate 24 bits */
/* << ARMv6* */
#  define ARM_SHIFT			0x01a00000
#  define ARM_R				0x00000010 /* register shift */
#  define ARM_LSL			0x00000000
#  define THUMB_LSL			    0x4080
#  define THUMB2_LSL			0xfa00f000
#  define THUMB_LSLI			    0x0000
#  define THUMB2_LSLI			0xea4f0000
#  define ARM_LSR			0x00000020
#  define THUMB_LSR			    0x40c0
#  define THUMB2_LSR			0xfa20f000
#  define THUMB_LSRI			    0x0800
#  define THUMB2_LSRI			0xea4f0010
#  define ARM_ASR			0x00000040
#  define THUMB_ASR			    0x4100
#  define THUMB2_ASR			0xfa40f000
#  define THUMB_ASRI			    0x1000
#  define THUMB2_ASRI			0xea4f0020
#  define ARM_ROR			0x00000060
#  define ARM_CMP			0x01500000
#  define THUMB_CMP			    0x4280
#  define THUMB_CMPX			    0x4500
#  define THUMB2_CMP			0xebb00000
#  define THUMB_CMPI			    0x2800
#  define THUMB2_CMPI			0xf1b00000
#  define ARM_CMN			0x01700000
#  define THUMB_CMN			    0x42c0
#  define THUMB2_CMN			0xeb100000
#  define THUMB2_CMNI			0xf1100000
#  define ARM_TST			0x01100000
#  define THUMB_TST			    0x4200
#  define THUMB2_TST			0xea100000
#  define THUMB2_TSTI			0xf0100000
#  define ARM_TEQ			0x01300000
/* branch */
#  define ARM_BX			0x012fff10
#  define ARM_BLX			0x012fff30
#  define THUMB_BLX			    0x4780
#  define ARM_BLXI			0xfa000000
#  define THUMB2_BLXI			0xf000c000
#  define ARM_B				0x0a000000
#  define THUMB_CC_B			    0xd000
#  define THUMB_B			    0xe000
#  define THUMB2_CC_B			0xf0008000
#  define THUMB2_B			0xf0009000
#  define ARM_BLI			0x0b000000
#  define THUMB2_BLI			0xf000d000
/* ldr/str */
#  define ARM_P				0x00800000 /* positive offset */
#  define THUMB2_P			0x00000400
#  define THUMB2_U			0x00000200
#  define THUMB2_W			0x00000100
#  define ARM_LDRSB			0x011000d0
#  define THUMB_LDRSB			    0x5600
#  define THUMB2_LDRSB			0xf9100000
#  define ARM_LDRSBI			0x015000d0
#  define THUMB2_LDRSBI			0xf9100c00
#  define THUMB2_LDRSBWI		0xf9900000
#  define ARM_LDRB			0x07500000
#  define THUMB_LDRB			    0x5c00
#  define THUMB2_LDRB			0xf8100000
#  define ARM_LDRBI			0x05500000
#  define THUMB_LDRBI			    0x7800
#  define THUMB2_LDRBI			0xf8100c00
#  define THUMB2_LDRBWI			0xf8900000
#  define ARM_LDRSH			0x011000f0
#  define THUMB_LDRSH			    0x5e00
#  define THUMB2_LDRSH			0xf9300000
#  define ARM_LDRSHI			0x015000f0
#  define THUMB2_LDRSHI			0xf9300c00
#  define THUMB2_LDRSHWI		0xf9b00000
#  define ARM_LDRH			0x011000b0
#  define THUMB_LDRH			    0x5a00
#  define THUMB2_LDRH			0xf8300000
#  define ARM_LDRHI			0x015000b0
#  define THUMB_LDRHI			    0x8800
#  define THUMB2_LDRHI			0xf8300c00
#  define THUMB2_LDRHWI			0xf8b00000
#  define ARM_LDR			0x07100000
#  define THUMB_LDR			    0x5800
#  define THUMB2_LDR			0xf8500000
#  define ARM_LDRI			0x05100000
#  define THUMB_LDRI			    0x6800
#  define THUMB_LDRISP			    0x9800
#  define THUMB2_LDRI			0xf8500c00
#  define THUMB2_LDRWI			0xf8d00000
#  define ARM_LDRD			0x010000d0
#  define ARM_LDRDI			0x014000d0
#  define THUMB2_LDRDI			0xe8500000
#  define ARM_STRB			0x07400000
#  define THUMB_STRB			    0x5400
#  define THUMB2_STRB			0xf8000000
#  define ARM_STRBI			0x05400000
#  define THUMB_STRBI			    0x7000
#  define THUMB2_STRBI			0xf8000c00
#  define THUMB2_STRBWI			0xf8800000
#  define ARM_STRH			0x010000b0
#  define THUMB_STRH			    0x5200
#  define THUMB2_STRH			0xf8200000
#  define ARM_STRHI			0x014000b0
#  define THUMB_STRHI			    0x8000
#  define THUMB2_STRHI			0xf8200c00
#  define THUMB2_STRHWI			0xf8a00000
#  define ARM_STR			0x07000000
#  define THUMB_STR			    0x5000
#  define THUMB2_STR			0xf8400000
#  define ARM_STRI			0x05000000
#  define THUMB_STRI			    0x6000
# define THUMB2_STRWI			0xf8c00000
#  define THUMB_STRISP			    0x9000
#  define THUMB2_STRI			0xf8400c00
#  define ARM_STRD			0x010000f0
# define ARM_STRDI			0x014000f0
#  define THUMB2_STRDI			0xe8400000
/* ldm/stm */
#  define ARM_M				0x08000000
#  define ARM_M_L			0x00100000 /* load; store if not set */
#  define ARM_M_I			0x00800000 /* inc; dec if not set */
#  define ARM_M_B			0x01000000 /* before; after if not set */
#  define ARM_M_U			0x00200000 /* update Rn */
#  define THUMB2_LDM_W			0x00200000
#  define THUMB2_LDM_P			0x00008000
#  define THUMB2_LDM_M			0x00004000
#  define THUMB_LDMIA			    0xc800
#  define THUMB2_LDMIA			0xe8900000
#  define THUMB2_LDMB			0xe9100000
#  define THUMB_PUSH			    0xb400
#  define THUMB2_PUSH			0xe92d0000
#  define THUMB_POP			    0xbc00
#  define THUMB2_POP			0xe8bd0000
#  define ii(i)				*_jit->pc.ui++ = i
#  define is(i)				*_jit->pc.us++ = i
#  if __BYTE_ORDER == __LITTLE_ENDIAN
#    define iss(i, j)			do { is(j); is(i); } while (0)
#    define code2thumb(t0, t1, c0, c1)	do { t1 = c0; t0 = c1; } while (0)
#    define thumb2code(t0, t1, c0, c1)	do { c0 = t1; c1 = t0; } while (0)
#  else
#    define iss(i, j)			do { is(i); is(j); } while (0)
#    define code2thumb(t0, t1, c0, c1)	do { t0 = c0; t1 = c1; } while (0)
#    define thumb2code(t0, t1, c0, c1)	do { c0 = t0; c1 = t1; } while (0)
#  endif
static int encode_arm_immediate(unsigned int v);
static int encode_thumb_immediate(unsigned int v);
static int encode_thumb_word_immediate(unsigned int v);
static int encode_thumb_jump(int v);
static int encode_thumb_cc_jump(int v);
static int encode_thumb_shift(int v, int type) maybe_unused;
#  define corrr(cc,o,rn,rd,rm)		_corrr(_jit,cc,o,rn,rd,rm)
static void _corrr(jit_state_t*,int,int,int,int,int);
#  define corri(cc,o,rn,rd,im)		_corri(_jit,cc,o,rn,rd,im)
static void _corri(jit_state_t*,int,int,int,int,int);
#define corri8(cc,o,rn,rt,im)	_corri8(_jit,cc,o,rn,rt,im)
static void _corri8(jit_state_t*,int,int,int,int,int);
#  define torrr(o,rn,rd,rm)		_torrr(_jit,o,rn,rd,rm)
static void _torrr(jit_state_t*,int,int,int,int);
#  define torrrs(o,rn,rd,rm,im)		_torrrs(_jit,o,rn,rd,rm,im)
static void _torrrs(jit_state_t*,int,int,int,int,int) maybe_unused;
#  define torxr(o,rn,rt,rm)		_torxr(_jit,o,rn,rt,rm)
static void _torxr(jit_state_t*,int,int,int,int);
#  define torrrr(o,rn,rl,rh,rm)		_torrrr(_jit,o,rn,rl,rh,rm)
static void _torrrr(jit_state_t*,int,int,int,int,int) maybe_unused;
#  define torrri8(o,rn,rt,rt2,im)	_torrri8(_jit,o,rn,rt,rt2,im)
static void _torrri8(jit_state_t*,int,int,int,int,int) maybe_unused;
#  define coriw(cc,o,rd,im)		_coriw(_jit,cc,o,rd,im)
static void _coriw(jit_state_t*,int,int,int,int);
#  define torri(o,rd,rn,im)		_torri(_jit,o,rd,rn,im)
static void _torri(jit_state_t*,int,int,int,int);
#  define torri8(o,rn,rt,im)		_torri8(_jit,o,rn,rt,im)
static void _torri8(jit_state_t*,int,int,int,int);
#  define torri12(o,rn,rt,im)		_torri12(_jit,o,rn,rt,im)
static void _torri12(jit_state_t*,int,int,int,int);
#  define tshift(o,rd,rm,im)		_tshift(_jit,o,rd,rm,im)
static void _tshift(jit_state_t*,int,int,int,int);
#  define toriw(o,rd,im)		_toriw(_jit,o,rd,im)
static void _toriw(jit_state_t*,int,int,int);
#  define tc8(cc,im)			_tc8(_jit,cc,im)
static void _tc8(jit_state_t*,int,int) maybe_unused;
#  define t11(im)			_t11(_jit,im)
static void _t11(jit_state_t*,int);
#  define tcb(cc,im)			_tcb(_jit,cc,im)
static void _tcb(jit_state_t*,int,int);
#  define blxi(im)			_blxi(_jit,im)
static void _blxi(jit_state_t*,int) maybe_unused;
#  define tb(o,im)			_tb(_jit,o,im)
static void _tb(jit_state_t*,int,int);
#  define corrrr(cc,o,rh,rl,rm,rn)	_corrrr(_jit,cc,o,rh,rl,rm,rn)
static void _corrrr(jit_state_t*,int,int,int,int,int,int);
#  define corrrs(cc,o,rn,rd,rm,im)	_corrrs(_jit,cc,o,rn,rd,rm,im)
static void _corrrs(jit_state_t*,int,int,int,int,int,int);
#  define cshift(cc,o,rd,rm,rn,im)	_cshift(_jit,cc,o,rd,rm,rn,im)
static void _cshift(jit_state_t*,int,int,int,int,int,int);
#  define cb(cc,o,im)			_cb(_jit,cc,o,im)
static void _cb(jit_state_t*,int,int,int);
#  define cbx(cc,o,rm)			_cbx(_jit,cc,o,rm)
static void _cbx(jit_state_t*,int,int,int);
#  define corl(cc,o,r0,i0)		_corl(_jit,cc,o,r0,i0)
static void _corl(jit_state_t*,int,int,int,int);
#  define c6orr(cc,o,r0,r1)		_c6orr(_jit,cc,o,r0,r1)
static void _c6orr(jit_state_t*,int,int,int,int);
#  define tcit(cc,it)			_tcit(_jit,cc,it)
static void _tcit(jit_state_t*,unsigned int,int);
#  define IT(cc)			tcit(cc,THUMB2_IT)
#  define ITT(cc)			tcit(cc,THUMB2_ITT)
#  define ITE(cc)			tcit(cc,THUMB2_ITE)
#  define ITTT(cc)			tcit(cc,THUMB2_ITTT)
#  define ITTE(cc)			tcit(cc,THUMB2_ITTE)
#  define ITET(cc)			tcit(cc,THUMB2_ITET)
#  define ITEE(cc)			tcit(cc,THUMB2_ITEE)
#  define ITTTT(cc)			tcit(cc,THUMB2_ITTTT)
#  define ITETT(cc)			tcit(cc,THUMB2_ITETT)
#  define ITTET(cc)			tcit(cc,THUMB2_ITTET)
#  define ITEET(cc)			tcit(cc,THUMB2_ITEET)
#  define ITTTE(cc)			tcit(cc,THUMB2_ITTTE)
#  define ITETE(cc)			tcit(cc,THUMB2_ITETE)
#  define ITTEE(cc)			tcit(cc,THUMB2_ITTEE)
#  define ITEEE(cc)			tcit(cc,THUMB2_ITEEE)
#  define tpp(o,im)			_tpp(_jit,o,im)
static void _tpp(jit_state_t*,int,int);
#  define torl(o,rn,im)			_torl(_jit,o,rn,im)
static void _torl(jit_state_t*,int,int,int) maybe_unused;
#  define CC_MOV(cc,rd,rm)		corrr(cc,ARM_MOV,0,rd,rm)
#  define MOV(rd,rm)			CC_MOV(ARM_CC_AL,rd,rm)
#  define T1_MOV(rd,rm)			is(THUMB_MOV|((_u4(rd)&8)<<4)|(_u4(rm)<<3)|(rd&7))
#  define T2_MOV(rd,rm)			T2_ORR(rd,_R15_REGNO,rm)
#  define CC_MOVI(cc,rd,im)		corri(cc,ARM_MOV|ARM_I,0,rd,im)
#  define MOVI(rd,im)			CC_MOVI(ARM_CC_AL,rd,im)
#  define CC_MOVWI(cc,rd,im)		coriw(cc,ARM_MOVWI,rd,im)
#  define MOVWI(rd,im)			CC_MOVWI(ARM_CC_AL,rd,im)
#  define T1_MOVI(rd,im)		is(THUMB_MOVI|(_u3(rd)<<8)|_u8(im))
#  define T2_MOVI(rd,im)		torri(THUMB2_MOVI,_R15_REGNO,rd,im)
#  define T2_MOVWI(rd,im)		toriw(THUMB2_MOVWI,rd,im)
#  define CC_MOVTI(cc,rd,im)		coriw(cc,ARM_MOVTI,rd,im)
#  define MOVTI(rd,im)			CC_MOVTI(ARM_CC_AL,rd,im)
#  define T2_MOVTI(rd,im)		toriw(THUMB2_MOVTI,rd,im)
#  define CC_MVN(cc,rd,rm)		corrr(cc,ARM_MVN,0,rd,rm)
#  define MVN(rd,rm)			CC_MVN(ARM_CC_AL,rd,rm)
#  define T1_MVN(rd,rm)			is(THUMB_MVN|(_u3(rm)<<3)|_u3(rd))
#  define T2_MVN(rd,rm)			torrr(THUMB2_MVN,_R15_REGNO,rd,rm)
#  define CC_MVNI(cc,rd,im)		corri(cc,ARM_MVN|ARM_I,0,rd,im)
#  define MVNI(rd,im)			CC_MVNI(ARM_CC_AL,rd,im)
#  define T2_MVNI(rd,im)		torri(THUMB2_MVNI,_R15_REGNO,rd,im)
#  define CC_NOT(cc,rd,rm)		CC_MVN(cc,rd,rm)
#  define NOT(rd,rm)			CC_NOT(ARM_CC_AL,rd,rm)
#  define T1_NOT(rd,rm)			T1_MVN(rd,rm)
#  define T2_NOT(rd,rm)			T2_MVN(rd,rm)
#  define NOP()				MOV(_R0_REGNO, _R0_REGNO)
#  define T1_NOP()			is(0xbf00)
#  define CC_ADD(cc,rd,rn,rm)		corrr(cc,ARM_ADD,rn,rd,rm)
#  define ADD(rd,rn,rm)			CC_ADD(ARM_CC_AL,rd,rn,rm)
#  define T1_ADD(rd,rn,rm)		is(THUMB_ADD|(_u3(rm)<<6)|(_u3(rn)<<3)|_u3(rd))
#  define T1_ADDX(rdn,rm)		is(THUMB_ADDX|((_u4(rdn)&8)<<4)|(_u4(rm)<<3)|(rdn&7))
#  define T2_ADD(rd,rn,rm)		torrr(THUMB2_ADD,rn,rd,rm)
#  define CC_ADDI(cc,rd,rn,im)		corri(cc,ARM_ADD|ARM_I,rn,rd,im)
#  define ADDI(rd,rn,im)		CC_ADDI(ARM_CC_AL,rd,rn,im)
#  define T1_ADDI3(rd,rn,im)		is(THUMB_ADDI3|(_u3(im)<<6)|(_u3(rn)<<3)|_u3(rd))
#  define T1_ADDI8(rdn,im)		is(THUMB_ADDI8|(_u3(rdn)<<8)|_u8(im))
#  define T2_ADDI(rd,rn,im)		torri(THUMB2_ADDI,rn,rd,im)
#  define T2_ADDWI(rd,rn,im)		torri(THUMB2_ADDWI,rn,rd,im)
#  define CC_ADDS(cc,rd,rn,rm)		corrr(cc,ARM_ADD|ARM_S,rn,rd,rm)
#  define ADDS(rd,rn,rm)		CC_ADDS(ARM_CC_AL,rd,rn,rm)
#  define T2_ADDS(rd,rn,rm)		torrr(THUMB2_ADD|ARM_S,rn,rd,rm)
#  define ADDSI(rd,rn,im)		corri(ARM_CC_AL,ARM_ADD|ARM_S|ARM_I,rn,rd,im)
#  define T2_ADDSI(rd,rn,im)		torri(THUMB2_ADDI|ARM_S,rn,rd,im)
#  define CC_ADC(cc,rd,rn,rm)		corrr(cc,ARM_ADC,rn,rd,rm)
#  define ADC(rd,rn,rm)			CC_ADC(ARM_CC_AL,rd,rn,rm)
#  define T1_ADC(rdn,rm)		is(THUMB_ADC|(_u3(rm)<<3)|_u3(rdn))
#  define T2_ADC(rd,rn,rm)		torrr(THUMB2_ADC,rn,rd,rm)
#  define CC_ADCI(cc,rd,rn,im)		corri(cc,ARM_ADC|ARM_I,rn,rd,im)
#  define ADCI(rd,rn,im)		CC_ADCI(ARM_CC_AL,rd,rn,im)
#  define T2_ADCI(rd,rn,im)		torri(THUMB2_ADCI,rn,rd,im)
#  define CC_ADCS(cc,rd,rn,rm)		corrr(cc,ARM_ADC|ARM_S,rn,rd,rm)
#  define ADCS(rd,rn,rm)		CC_ADCS(ARM_CC_AL,rd,rn,rm)
#  define T2_ADCS(rd,rn,rm)		torrr(THUMB2_ADC|ARM_S,rn,rd,rm)
#  define CC_ADCSI(cc,rd,rn,im)		corri(cc,ARM_ADC|ARM_S|ARM_I,rn,rd,im)
#  define ADCSI(rd,rn,im)		CC_ADCSI(ARM_CC_AL,rd,rn,im)
#  define T2_ADCSI(rd,rn,im)		torri(THUMB2_ADCI|ARM_S,rn,rd,im)
#  define CC_SUB(cc,rd,rn,rm)		corrr(cc,ARM_SUB,rn,rd,rm)
#  define SUB(rd,rn,rm)			CC_SUB(ARM_CC_AL,rd,rn,rm)
#  define T1_SUB(rd,rn,rm)		is(THUMB_SUB|(_u3(rm)<<6)|(_u3(rn)<<3)|_u3(rd))
#  define T2_SUB(rd,rn,rm)		torrr(THUMB2_SUB,rn,rd,rm)
#  define CC_SUBI(cc,rd,rn,im)		corri(cc,ARM_SUB|ARM_I,rn,rd,im)
#  define SUBI(rd,rn,im)		CC_SUBI(ARM_CC_AL,rd,rn,im)
#  define T1_SUBI3(rd,rn,im)		is(THUMB_SUBI3|(_u3(im)<<6)|(_u3(rn)<<3)|_u3(rd))
#  define T1_SUBI8(rdn,im)		is(THUMB_SUBI8|(_u3(rdn)<<8)|_u8(im))
#  define T2_SUBI(rd,rn,im)		torri(THUMB2_SUBI,rn,rd,im)
#  define T2_SUBWI(rd,rn,im)		torri(THUMB2_SUBWI,rn,rd,im)
#  define CC_SUBS(cc,rd,rn,rm)		corrr(cc,ARM_SUB|ARM_S,rn,rd,rm)
#  define SUBS(rd,rn,rm)		CC_SUBS(ARM_CC_AL,rd,rn,rm)
#  define T2_SUBS(rd,rn,rm)		torrr(THUMB2_SUB|ARM_S,rn,rd,rm)
#  define CC_SUBSI(cc,rd,rn,im)		corri(cc,ARM_SUB|ARM_S|ARM_I,rn,rd,im)
#  define SUBSI(rd,rn,im)		CC_SUBSI(ARM_CC_AL,rd,rn,im)
#  define T2_SUBSI(rd,rn,im)		torri(THUMB2_SUBI|ARM_S,rn,rd,im)
#  define CC_SBC(cc,rd,rn,rm)		corrr(cc,ARM_SBC,rn,rd,rm)
#  define SBC(rd,rn,rm)			CC_SBC(ARM_CC_AL,rd,rn,rm)
#  define T1_SBC(rdn,rm)		is(THUMB_SBC|(_u3(rm)<<3)|_u3(rdn))
#  define T2_SBC(rd,rn,rm)		torrr(THUMB2_SBC,rn,rd,rm)
#  define CC_SBCI(cc,rd,rn,im)		corri(cc,ARM_SBC|ARM_I,rn,rd,im)
#  define SBCI(rd,rn,im)		CC_SBCI(ARM_CC_AL,rd,rn,im)
#  define T2_SBCI(rd,rn,im)		torri(THUMB2_SBCI,rn,rd,im)
#  define CC_SBCS(cc,rd,rn,rm)		corrr(cc,ARM_SBC|ARM_S,rn,rd,rm)
#  define SBCS(rd,rn,rm)		CC_SBCS(ARM_CC_AL,rd,rn,rm)
#  define T2_SBCS(rd,rn,rm)		torrr(THUMB2_SBC|ARM_S,rn,rd,rm)
#  define CC_SBCSI(cc,rd,rn,im)		corri(cc,ARM_SBC|ARM_S|ARM_I,rn,rd,im)
#  define SBCSI(rd,rn,im)		CC_SBCSI(ARM_CC_AL,rd,rn,im)
#  define T2_SBCSI(rd,rn,im)		torri(THUMB2_SBCI|ARM_S,rn,rd,im)
#  define CC_RSB(cc,rd,rn,rm)		corrr(cc,ARM_RSB,rn,rd,rm)
#  define RSB(rd,rn,rm)			CC_RSB(ARM_CC_AL,rd,rn,rm)
#  define T2_RSB(rd,rn,rm)		torrr(THUMB2_RSB,rn,rd,rm)
#  define CC_RSBI(cc,rd,rn,im)		corri(cc,ARM_RSB|ARM_I,rn,rd,im)
#  define RSBI(rd,rn,im)		CC_RSBI(ARM_CC_AL,rd,rn,im)
#  define T1_RSBI(rd,rn)		is(THUMB_RSBI|(_u3(rn)<<3)|_u3(rd))
#  define T2_RSBI(rd,rn,im)		torri(THUMB2_RSBI,rn,rd,im)
#  define CC_MUL(cc,rl,rn,rm)		corrrr(cc,ARM_MUL,rl,0,rm,rn)
#  define MUL(rl,rn,rm)			CC_MUL(ARM_CC_AL,rl,rn,rm)
#  define T1_MUL(rdm,rn)		is(THUMB_MUL|(_u3(rn)<<3)|_u3(rdm))
#  define T2_MUL(rd,rn,rm)		torrr(THUMB2_MUL,rn,rd,rm)
#  define CC_SMULL(cc,rl,rh,rn,rm)	corrrr(cc,ARM_SMULL,rh,rl,rm,rn)
#  define SMULL(rl,rh,rn,rm)		CC_SMULL(ARM_CC_AL,rl,rh,rn,rm)
#  define T2_SMULL(rl,rh,rn,rm)		torrrr(THUMB2_SMULL,rn,rl,rh,rm)
#  define CC_UMULL(cc,rl,rh,rn,rm)	corrrr(cc,ARM_UMULL,rh,rl,rm,rn)
#  define UMULL(rl,rh,rn,rm)		CC_UMULL(ARM_CC_AL,rl,rh,rn,rm)
#  define T2_UMULL(rl,rh,rn,rm)		torrrr(THUMB2_UMULL,rn,rl,rh,rm)
#  define T2_SDIV(rd,rn,rm)		torrr(THUMB2_SDIV,rn,rd,rm)
#  define T2_UDIV(rd,rn,rm)		torrr(THUMB2_UDIV,rn,rd,rm)
#  define CC_AND(cc,rd,rn,rm)		corrr(cc,ARM_AND,rn,rd,rm)
#  define AND(rd,rn,rm)			CC_AND(ARM_CC_AL,rd,rn,rm)
#  define T1_AND(rdn,rm)		is(THUMB_AND|(_u3(rm)<<3)|_u3(rdn))
#  define T2_AND(rd,rn,rm)		torrr(THUMB2_AND,rn,rd,rm)
#  define CC_ANDI(cc,rd,rn,im)		corri(cc,ARM_AND|ARM_I,rn,rd,im)
#  define ANDI(rd,rn,im)		CC_ANDI(ARM_CC_AL,rd,rn,im)
#  define T2_ANDI(rd,rn,im)		torri(THUMB2_ANDI,rn,rd,im)
#  define CC_ANDS(cc,rd,rn,rm)		corrr(cc,ARM_AND|ARM_S,rn,rd,rm)
#  define ANDS(rd,rn,rm)		CC_ANDS(ARM_CC_AL,rd,rn,rm)
#  define T2_ANDS(rd,rn,rm)		torrr(THUMB2_AND|ARM_S,rn,rd,rm)
#  define CC_ANDSI(cc,rd,rn,im)		corri(cc,ARM_AND|ARM_S|ARM_I,rn,rd,im)
#  define ANDSI(rd,rn,im)		CC_ANDSI(ARM_CC_AL,rd,rn,im)
#  define T2_ANDSI(rd,rn,im)		torri(ARM_CC_AL,THUMB2_ANDI|ARM_S,rn,rd,im)
#  define CC_BIC(cc,rd,rn,rm)		corrr(cc,ARM_BIC,rn,rd,rm)
#  define BIC(rd,rn,rm)			CC_BIC(ARM_CC_AL,rd,rn,rm)
#  define T2_BIC(rd,rn,rm)		torrr(THUMB2_BIC,rn,rd,rm)
#  define CC_BICI(cc,rd,rn,im)		corri(cc,ARM_BIC|ARM_I,rn,rd,im)
#  define BICI(rd,rn,im)		CC_BICI(ARM_CC_AL,rd,rn,im)
#  define T2_BICI(rd,rn,im)		torri(THUMB2_BICI,rn,rd,im)
#  define CC_BICS(cc,rd,rn,rm)		corrr(cc,ARM_BIC|ARM_S,rn,rd,rm)
#  define BICS(rd,rn,rm)		CC_BICS(ARM_CC_AL,rd,rn,rm)
#  define T2_BICS(rd,rn,rm)		torrr(THUMB2_BIC|ARM_S,rn,rd,rm)
#  define CC_BICSI(cc,rd,rn,im)		corri(cc,ARM_BIC|ARM_S|ARM_I,rn,rd,im)
#  define BICSI(rd,rn,im)		CC_BICSI(ARM_CC_AL,rd,rn,im)
#  define T2_BICSI(rd,rn,im)		torri(ARM_CC_AL,THUMB2_BICI|ARM_S,rn,rd,im)
#  define CC_ORR(cc,rd,rn,rm)		corrr(cc,ARM_ORR,rn,rd,rm)
#  define ORR(rd,rn,rm)			CC_ORR(ARM_CC_AL,rd,rn,rm)
#  define T1_ORR(rdn,rm)		is(THUMB_ORR|(_u3(rm)<<3)|_u3(rdn))
#  define T2_ORR(rd,rn,rm)		torrr(THUMB2_ORR,rn,rd,rm)
#  define CC_ORR_SI(cc,rd,rn,rt,sh,im)	corrrs(cc,ARM_ORR|sh,rn,rd,rm,im)
#  define ORR_SI(r0,r1,r2,sh,im)	CC_ORR_SI(ARM_CC_AL,r0,r1,r2,sh,im)
#  define CC_ORRI(cc,rd,rn,im)		corri(cc,ARM_ORR|ARM_I,rn,rd,im)
#  define ORRI(rd,rn,im)		CC_ORRI(ARM_CC_AL,rd,rn,im)
#  define T2_ORRI(rd,rn,im)		torri(THUMB2_ORRI,rn,rd,im)
#  define CC_EOR(cc,rd,rn,rm)		corrr(cc,ARM_EOR,rn,rd,rm)
#  define EOR(rd,rn,rm)			CC_EOR(ARM_CC_AL,rd,rn,rm)
#  define T1_EOR(rdn,rm)		is(THUMB_EOR|(_u3(rm)<<3)|_u3(rdn))
#  define T2_EOR(rd,rn,rm)		torrr(THUMB2_EOR,rn,rd,rm)
#  define CC_EOR_SI(cc,rd,rn,rm,sh,im)	corrrs(cc,ARM_EOR|sh,rn,rd,rm,im)
#  define EOR_SI(r0,r1,r2,sh,im)	CC_EOR_SI(ARM_CC_AL,r0,r1,r2,sh,im)
#  define CC_EORI(cc,rd,rn,im)		corri(cc,ARM_EOR|ARM_I,rn,rd,im)
#  define EORI(rd,rn,im)		CC_EORI(ARM_CC_AL,rd,rn,im)
#  define T2_EORI(rd,rn,im)		torri(THUMB2_EORI,rn,rd,im)
#  define CC_REV(cc,rd,rm)		c6orr(cc,ARM_REV,rd,rm)
#  define REV(rd,rm)			CC_REV(ARM_CC_AL,rd,rm)
#  define T1_REV(rd,rm)			is(THUMB_REV|(_u3(rm)<<3)|_u3(rd))
#  define T2_REV(rd,rm)			torrr(THUMB2_REV,rm,rd,rm)
#  define CC_REV16(cc,rd,rm)		c6orr(cc,ARM_REV16,rd,rm)
#  define REV16(rd,rm)			CC_REV16(ARM_CC_AL,rd,rm)
#  define T1_REV16(rd,rm)		is(THUMB_REV16|(_u3(rm)<<3)|_u3(rd))
#  define T2_REV16(rd,rm)		torrr(THUMB2_REV16,rm,rd,rm)
#  define CC_SXTB(cc,rd,rm)		c6orr(cc,ARM_SXTB,rd,rm)
#  define SXTB(rd,rm)			CC_SXTB(ARM_CC_AL,rd,rm)
#  define T1_SXTB(rd,rm)		is(THUMB_SXTB|(_u3(rm)<<3)|_u3(rd))
#  define T2_SXTB(rd,rm)		torrr(THUMB2_SXTB,_R15_REGNO,rd,rm)
#  define CC_UXTB(cc,rd,rm)		c6orr(cc,ARM_UXTB,rd,rm)
#  define UXTB(rd,rm)			CC_UXTB(ARM_CC_AL,rd,rm)
#  define T1_UXTB(rd,rm)		is(THUMB_UXTB|(_u3(rm)<<3)|_u3(rd))
#  define T2_UXTB(rd,rm)		torrr(THUMB2_UXTB,_R15_REGNO,rd,rm)
#  define CC_SXTH(cc,rd,rm)		c6orr(cc,ARM_SXTH,rd,rm)
#  define SXTH(rd,rm)			CC_SXTH(ARM_CC_AL,rd,rm)
#  define T1_SXTH(rd,rm)		is(THUMB_SXTH|(_u3(rm)<<3)|_u3(rd))
#  define T2_SXTH(rd,rm)		torrr(THUMB2_SXTH,_R15_REGNO,rd,rm)
#  define CC_UXTH(cc,rd,rm)		c6orr(cc,ARM_UXTH,rd,rm)
#  define UXTH(rd,rm)			CC_UXTH(ARM_CC_AL,rd,rm)
#  define T1_UXTH(rd,rm)		is(THUMB_UXTH|(_u3(rm)<<3)|_u3(rd))
#  define T2_UXTH(rd,rm)		torrr(THUMB2_UXTH,_R15_REGNO,rd,rm)
#  define CC_SHIFT(cc,o,rd,rm,rn,im)	cshift(cc,o,rd,rm,rn,im)
#  define CC_LSL(cc,rd,rn,rm)		CC_SHIFT(cc,ARM_LSL|ARM_R,rd,rm,rn,0)
#  define LSL(rd,rn,rm)			CC_LSL(ARM_CC_AL,rd,rn,rm)
#  define T1_LSL(rdn,rm)		is(THUMB_LSL|(_u3(rm)<<3)|_u3(rdn))
#  define T2_LSL(rd,rn,rm)		torrr(THUMB2_LSL,rn,rd,rm)
#  define CC_LSLI(cc,rd,rn,im)		CC_SHIFT(cc,ARM_LSL,rd,0,rn,im)
#  define LSLI(rd,rn,im)		CC_LSLI(ARM_CC_AL,rd,rn,im)
#  define T1_LSLI(rd,rm,im)		is(THUMB_LSLI|(_u5(im)<<6)|(_u3(rm)<<3)|_u3(rd))
#  define T2_LSLI(rd,rm,im)		tshift(THUMB2_LSLI,rd,rm,im)
#  define CC_LSR(cc,rd,rn,rm)		CC_SHIFT(cc,ARM_LSR|ARM_R,rd,rm,rn,0)
#  define LSR(rd,rn,rm)			CC_LSR(ARM_CC_AL,rd,rn,rm)
#  define T1_LSR(rdn,rm)		is(THUMB_LSR|(_u3(rm)<<3)|_u3(rdn))
#  define T2_LSR(rd,rn,rm)		torrr(THUMB2_LSR,rn,rd,rm)
#  define CC_LSRI(cc,rd,rn,im)		CC_SHIFT(cc,ARM_LSR,rd,0,rn,im)
#  define LSRI(rd,rn,im)		CC_LSRI(ARM_CC_AL,rd,rn,im)
#  define T1_LSRI(rd,rm,im)		is(THUMB_LSRI|(_u5(im)<<6)|(_u3(rm)<<3)|_u3(rd))
#  define T2_LSRI(rd,rm,im)		tshift(THUMB2_LSRI,rd,rm,im)
#  define CC_ASR(cc,rd,rn,rm)		CC_SHIFT(cc,ARM_ASR|ARM_R,rd,rm,rn,0)
#  define ASR(rd,rn,rm)			CC_ASR(ARM_CC_AL,rd,rn,rm)
#  define T1_ASR(rdn,rm)		is(THUMB_ASR|(_u3(rm)<<3)|_u3(rdn))
#  define T2_ASR(rd,rn,rm)		torrr(THUMB2_ASR,rn,rd,rm)
#  define CC_ASRI(cc,rd,rn,im)		CC_SHIFT(cc,ARM_ASR,rd,0,rn,im)
#  define ASRI(rd,rn,im)		CC_ASRI(ARM_CC_AL,rd,rn,im)
#  define T1_ASRI(rd,rm,im)		is(THUMB_ASRI|(_u5(im)<<6)|(_u3(rm)<<3)|_u3(rd))
#  define T2_ASRI(rd,rm,im)		tshift(THUMB2_ASRI,rd,rm,im)
#  define CC_CMP(cc,rn,rm)		corrr(cc,ARM_CMP,rn,0,rm)
#  define CMP(rn,rm)			CC_CMP(ARM_CC_AL,rn,rm)
#  define T1_CMP(rn,rm)			is(THUMB_CMP|(_u3(rm)<<3)|_u3(rn))
#  define T1_CMPX(rn,rm)		is(THUMB_CMPX|((_u4(rn)&8)<<4)|(_u4(rm)<<3)|(rn&7))
#  define T2_CMP(rn,rm)			torrr(THUMB2_CMP,rn,_R15_REGNO,rm)
#  define CC_CMPI(cc,rn,im)		corri(cc,ARM_CMP|ARM_I,rn,0,im)
#  define CMPI(rn,im)			CC_CMPI(ARM_CC_AL,rn,im)
#  define T1_CMPI(rn,im)		is(THUMB_CMPI|(_u3(rn)<<8)|_u8(im))
#  define T2_CMPI(rn,im)		torri(THUMB2_CMPI,rn,_R15_REGNO,im)
#  define CC_CMN(cc,rn,rm)		corrr(cc,ARM_CMN,rn,0,rm)
#  define CMN(rn,rm)			CC_CMN(ARM_CC_AL,rn,rm)
#  define T1_CMN(rn,rm)			is(THUMB_CMN|(_u3(rm)<<3)|_u3(rm))
#  define T2_CMN(rn,rm)			torrr(THUMB2_CMN,rn,_R15_REGNO,rm)
#  define CC_CMNI(cc,rn,im)		corri(cc,ARM_CMN|ARM_I,rn,0,im)
#  define CMNI(rn,im)			CC_CMNI(ARM_CC_AL,rn,im)
#  define T2_CMNI(rn,im)		torri(THUMB2_CMNI,rn,_R15_REGNO,im)
#  define CC_TST(cc,rn,rm)		corrr(cc,ARM_TST,rn,r0,rm)
#  define TST(rn,rm)			CC_TST(ARM_CC_AL,rn,rm)
#  define T1_TST(rn,rm)			is(THUMB_TST|(_u3(rm)<<3)|_u3(rn))
#  define T2_TST(rn,rm)			torrr(THUMB2_TST,rn,_R15_REGNO,rm)
#  define CC_TSTI(cc,rn,im)		corri(cc,ARM_TST|ARM_I,rn,0,im)
#  define TSTI(rn,im)			CC_TSTI(ARM_CC_AL,rn,im)
#  define T2_TSTI(rn,im)		torri(THUMB2_TSTI,rn,_R15_REGNO,im)
#  define CC_TEQ(cc,rn,rm)		corrr(cc,ARM_TEQ,rn,0,rm)
#  define TEQ(rn,rm)			CC_TEQ(ARM_CC_AL,rn,rm)
#  define CC_TEQI(cc,rm,im)		corri(cc,ARM_TEQ|ARM_I,rn,0,im)
#  define TEQI(rn,im)			CC_TEQI(ARM_CC_AL,rn,im)
#  define CC_BX(cc,rm)			cbx(cc,ARM_BX,rm)
#  define BX(rm)			CC_BX(ARM_CC_AL,rm)
#  define T1_BX(rm)			is(0x4700|(_u4(rm)<<3))
#  define CC_BLX(cc,rm)			cbx(cc,ARM_BLX,rm)
#  define BLX(rm)			CC_BLX(ARM_CC_AL,rm)
#  define T1_BLX(rm)			is(THUMB_BLX|(_u4(rm)<<3))
#  define BLXI(im)			blxi(im)
#  define T2_BLXI(im)			tb(THUMB2_BLXI,im)
#  define CC_B(cc,im)			cb(cc,ARM_B,im)
#  define B(im)				CC_B(ARM_CC_AL,im)
#  define T1_CC_B(cc,im)		tc8(cc,im)
#  define T1_B(im)			t11(im)
#  define T2_CC_B(cc,im)		tcb(cc,im)
#  define T2_B(im)			tb(THUMB2_B,im)
#  define CC_BLI(cc,im)			cb(cc,ARM_BLI,im)
#  define BLI(im)			CC_BLI(ARM_CC_AL,im)
#  define T2_BLI(im)			tb(THUMB2_BLI,im)
#  define CC_LDRSB(cc,rt,rn,rm)		corrr(cc,ARM_LDRSB|ARM_P,rn,rt,rm)
#  define LDRSB(rt,rn,rm)		CC_LDRSB(ARM_CC_AL,rt,rn,rm)
#  define T1_LDRSB(rt,rn,rm)		is(THUMB_LDRSB|(_u3(rm)<<6)|(_u3(rn)<<3)|_u3(rt))
#  define T2_LDRSB(rt,rn,rm)		torxr(THUMB2_LDRSB,rn,rt,rm)
#  define CC_LDRSBN(cc,rt,rn,rm)	corrr(cc,ARM_LDRSB,rn,rt,rm)
#  define LDRSBN(rt,rn,rm)		CC_LDRSBN(ARM_CC_AL,rt,rn,rm)
#  define CC_LDRSBI(cc,rt,rn,im)	corri8(cc,ARM_LDRSBI|ARM_P,rn,rt,im)
#  define LDRSBI(rt,rn,im)		CC_LDRSBI(ARM_CC_AL,rt,rn,im)
#  define T2_LDRSBI(rt,rn,im)		torri8(THUMB2_LDRSBI|THUMB2_U,rn,rt,im)
#  define T2_LDRSBWI(rt,rn,im)		torri12(THUMB2_LDRSBWI,rn,rt,im)
#  define CC_LDRSBIN(cc,rt,rn,im)	corri8(cc,ARM_LDRSBI,rn,rt,im)
#  define LDRSBIN(rt,rn,im)		CC_LDRSBIN(ARM_CC_AL,rt,rn,im)
#  define T2_LDRSBIN(rt,rn,im)		torri8(THUMB2_LDRSBI,rn,rt,im)
#  define CC_LDRB(cc,rt,rn,rm)		corrr(cc,ARM_LDRB|ARM_P,rn,rt,rm)
#  define LDRB(rt,rn,rm)		CC_LDRB(ARM_CC_AL,rt,rn,rm)
#  define T1_LDRB(rt,rn,rm)		is(THUMB_LDRB|(_u3(rm)<<6)|(_u3(rn)<<3)|_u3(rt))
#  define T2_LDRB(rt,rn,rm)		torxr(THUMB2_LDRB,rn,rt,rm)
#  define CC_LDRBN(cc,rt,rn,rm)		corrr(cc,ARM_LDRB,rn,rt,rm)
#  define LDRBN(rt,rn,rm)		CC_LDRBN(ARM_CC_AL,rt,rn,rm)
#  define CC_LDRBI(cc,rt,rn,im)		corri(cc,ARM_LDRBI|ARM_P,rn,rt,im)
#  define LDRBI(rt,rn,im)		CC_LDRBI(ARM_CC_AL,rt,rn,im)
#  define T1_LDRBI(rt,rn,im)		is(THUMB_LDRBI|(_u5(im)<<6)|(_u3(rn)<<3)|_u3(rt))
#  define T2_LDRBI(rt,rn,im)		torri8(THUMB2_LDRBI|THUMB2_U,rn,rt,im)
#  define T2_LDRBWI(rt,rn,im)		torri12(THUMB2_LDRBWI,rn,rt,im)
#  define CC_LDRBIN(cc,rt,rn,im)	corri(cc,ARM_LDRBI,rn,rt,im)
#  define LDRBIN(rt,rn,im)		CC_LDRBIN(ARM_CC_AL,rt,rn,im)
#  define T2_LDRBIN(rt,rn,im)		torri8(THUMB2_LDRBI,rn,rt,im)
#  define CC_LDRSH(cc,rt,rn,rm)		corrr(cc,ARM_LDRSH|ARM_P,rn,rt,rm)
#  define LDRSH(rt,rn,rm)		CC_LDRSH(ARM_CC_AL,rt,rn,rm)
#  define T1_LDRSH(rt,rn,rm)		is(THUMB_LDRSH|(_u3(rm)<<6)|(_u3(rn)<<3)|_u3(rt))
#  define T2_LDRSH(rt,rn,rm)		torxr(THUMB2_LDRSH,rn,rt,rm)
#  define CC_LDRSHN(cc,rt,rn,rm)	corrr(cc,ARM_LDRSH,rn,rt,rm)
#  define LDRSHN(rt,rn,rm)		CC_LDRSHN(ARM_CC_AL,rt,rn,rm)
#  define CC_LDRSHI(cc,rt,rn,im)	corri8(cc,ARM_LDRSHI|ARM_P,rn,rt,im)
#  define LDRSHI(rt,rn,im)		CC_LDRSHI(ARM_CC_AL,rt,rn,im)
#  define T2_LDRSHI(rt,rn,im)		torri8(THUMB2_LDRSHI|THUMB2_U,rn,rt,im)
#  define T2_LDRSHWI(rt,rn,im)		torri12(THUMB2_LDRSHWI,rn,rt,im)
#  define CC_LDRSHIN(cc,rt,rn,im)	corri8(cc,ARM_LDRSHI,rn,rt,im)
#  define LDRSHIN(rt,rn,im)		CC_LDRSHIN(ARM_CC_AL,rt,rn,im)
#  define T2_LDRSHIN(rt,rn,im)		torri8(THUMB2_LDRSHI,rn,rt,im)
#  define CC_LDRH(cc,rt,rn,rm)		corrr(cc,ARM_LDRH|ARM_P,rn,rt,rm)
#  define LDRH(rt,rn,rm)		CC_LDRH(ARM_CC_AL,rt,rn,rm)
#  define T1_LDRH(rt,rn,rm)		is(THUMB_LDRH|(_u3(rm)<<6)|(_u3(rn)<<3)|_u3(rt))
#  define T2_LDRH(rt,rn,rm)		torxr(THUMB2_LDRH,rn,rt,rm)
#  define CC_LDRHN(cc,rt,rn,rm)		corrr(cc,ARM_LDRH,rn,rt,rm)
#  define LDRHN(rt,rn,rm)		CC_LDRHN(ARM_CC_AL,rt,rn,rm)
#  define CC_LDRHI(cc,rt,rn,im)		corri8(cc,ARM_LDRHI|ARM_P,rn,rt,im)
#  define LDRHI(rt,rn,im)		CC_LDRHI(ARM_CC_AL,rt,rn,im)
#  define T1_LDRHI(rt,rn,im)		is(THUMB_LDRHI|(_u5(im)<<6)|(_u3(rn)<<3)|_u3(rt))
#  define T2_LDRHI(rt,rn,im)		torri8(THUMB2_LDRHI|THUMB2_U,rn,rt,im)
#  define T2_LDRHWI(rt,rn,im)		torri12(THUMB2_LDRHWI,rn,rt,im)
#  define CC_LDRHIN(cc,rt,rn,im)	corri8(cc,ARM_LDRHI,rn,rt,im)
#  define LDRHIN(rt,rn,im)		CC_LDRHIN(ARM_CC_AL,rt,rn,im)
#  define T2_LDRHIN(rt,rn,im)		torri8(THUMB2_LDRHI,rn,rt,im)
#  define CC_LDR(cc,rt,rn,rm)		corrr(cc,ARM_LDR|ARM_P,rn,rt,rm)
#  define LDR(rt,rn,rm)			CC_LDR(ARM_CC_AL,rt,rn,rm)
#  define T1_LDR(rt,rn,rm)		is(THUMB_LDR|(_u3(rm)<<6)|(_u3(rn)<<3)|_u3(rt))
#  define T2_LDR(rt,rn,rm)		torxr(THUMB2_LDR,rn,rt,rm)
#  define CC_LDRN(cc,rt,rn,rm)		corrr(cc,ARM_LDR,rn,rt,rm)
#  define LDRN(rt,rn,rm)		CC_LDRN(ARM_CC_AL,rt,rn,rm)
#  define CC_LDRI(cc,rt,rn,im)		corri(cc,ARM_LDRI|ARM_P,rn,rt,im)
#  define LDRI(rt,rn,im)		CC_LDRI(ARM_CC_AL,rt,rn,im)
#  define T1_LDRI(rt,rn,im)		is(THUMB_LDRI|(_u5(im)<<6)|(_u3(rn)<<3)|_u3(rt))
#  define T1_LDRISP(rt,im)		is(THUMB_LDRISP|(_u3(rt)<<8)|_u8(im))
#  define T2_LDRI(rt,rn,im)		torri8(THUMB2_LDRI|THUMB2_U,rn,rt,im)
#  define T2_LDRWI(rt,rn,im)		torri12(THUMB2_LDRWI,rn,rt,im)
#  define CC_LDRIN(cc,rt,rn,im)		corri(cc,ARM_LDRI,rn,rt,im)
#  define LDRIN(rt,rn,im)		CC_LDRIN(ARM_CC_AL,rt,rn,im)
#  define T2_LDRIN(rt,rn,im)		torri8(THUMB2_LDRI,rn,rt,im)
#  define CC_LDRD(cc,rt,rn,rm)		corrr(cc,ARM_LDRD|ARM_P,rn,rt,rm)
#  define LDRD(rt,rn,rm)		CC_LDRD(ARM_CC_AL,rt,rn,rm)
#  define T2_LDRDI(rt,rt2,rn,im)	torrri8(THUMB2_LDRDI|ARM_P,rn,rt,rt2,im)
#  define CC_LDRDN(cc,rt,rn,rm)		corrr(cc,ARM_LDRD,rn,rt,rm)
#  define LDRDN(rd,rn,rm)		CC_LDRDN(ARM_CC_AL,rt,rn,rm)
#  define CC_LDRDI(cc,rt,rn,im)		corri8(cc,ARM_LDRDI|ARM_P,rn,rt,im)
#  define LDRDI(rt,rn,im)		CC_LDRDI(ARM_CC_AL,rt,rn,im)
#  define CC_LDRDIN(cc,rt,rn,im)	corri8(cc,ARM_LDRDI,rn,rt,im)
#  define LDRDIN(rt,rn,im)		CC_LDRDIN(ARM_CC_AL,rt,rn,im)
#  define T2_LDRDIN(rt,rt2,rn,im)	torrri8(THUMB2_LDRDI,rn,rt,rt2,im)
#  define CC_STRB(cc,rt,rn,rm)		corrr(cc,ARM_STRB|ARM_P,rn,rt,rm)
#  define STRB(rt,rn,rm)		CC_STRB(ARM_CC_AL,rt,rn,rm)
#  define T1_STRB(rt,rn,rm)		is(THUMB_STRB|(_u3(rm)<<6)|(_u3(rn)<<3)|_u3(rt))
#  define T2_STRB(rt,rn,rm)		torxr(THUMB2_STRB,rn,rt,rm)
#  define CC_STRBN(cc,rt,rn,rm)		corrr(cc,ARM_STRB,rn,rt,rm)
#  define STRBN(rt,rn,rm)		CC_STRBN(ARM_CC_AL,rt,rn,rm)
#  define CC_STRBI(cc,rt,rn,im)		corri(cc,ARM_STRBI|ARM_P,rn,rt,im)
#  define STRBI(rt,rn,im)		CC_STRBI(ARM_CC_AL,rt,rn,im)
#  define T1_STRBI(rt,rn,im)		is(THUMB_STRBI|(_u5(im)<<6)|(_u3(rn)<<3)|_u3(rt))
#  define T2_STRBI(rt,rn,im)		torri8(THUMB2_STRBI|THUMB2_U,rn,rt,im)
#  define T2_STRBWI(rt,rn,im)		torri12(THUMB2_STRBWI,rn,rt,im)
#  define CC_STRBIN(cc,rt,rn,im)	corri(cc,ARM_STRBI,rn,rt,im)
#  define STRBIN(rt,rn,im)		CC_STRBIN(ARM_CC_AL,rt,rn,im)
#  define T2_STRBIN(rt,rn,im)		torri8(THUMB2_STRBI,rn,rt,im)
#  define CC_STRH(cc,rt,rn,rm)		corrr(cc,ARM_STRH|ARM_P,rn,rt,rm)
#  define STRH(rt,rn,rm)		CC_STRH(ARM_CC_AL,rt,rn,rm)
#  define T1_STRH(rt,rn,rm)		is(THUMB_STRH|(_u3(rm)<<6)|(_u3(rn)<<3)|_u3(rt))
#  define T2_STRH(rt,rn,rm)		torxr(THUMB2_STRH,rn,rt,rm)
#  define CC_STRHN(cc,rt,rn,rm)		corrr(cc,ARM_STRH,rn,rt,rm)
#  define STRHN(rt,rn,rm)		CC_STRHN(ARM_CC_AL,rt,rn,rm)
#  define CC_STRHI(cc,rt,rn,im)		corri8(cc,ARM_STRHI|ARM_P,rn,rt,im)
#  define STRHI(rt,rn,im)		CC_STRHI(ARM_CC_AL,rt,rn,im)
#  define T1_STRHI(rt,rn,im)		is(THUMB_STRHI|(_u5(im)<<6)|(_u3(rn)<<3)|_u3(rt))
#  define T2_STRHI(rt,rn,im)		torri8(THUMB2_STRHI|THUMB2_U,rn,rt,im)
#  define T2_STRHWI(rt,rn,im)		torri12(THUMB2_STRHWI,rn,rt,im)
#  define CC_STRHIN(cc,rt,rn,im)	corri8(cc,ARM_STRHI,rn,rt,im)
#  define STRHIN(rt,rn,im)		CC_STRHIN(ARM_CC_AL,rt,rn,im)
#  define T2_STRHIN(rt,rn,im)		torri8(THUMB2_STRHI,rn,rt,im)
#  define CC_STR(cc,rt,rn,rm)		corrr(cc,ARM_STR|ARM_P,rn,rt,rm)
#  define STR(rt,rn,rm)			CC_STR(ARM_CC_AL,rt,rn,rm)
#  define T1_STR(rt,rn,rm)		is(THUMB_STR|(_u3(rm)<<6)|(_u3(rn)<<3)|_u3(rt))
#  define T2_STR(rt,rn,rm)		torxr(THUMB2_STR,rn,rt,rm)
#  define CC_STRN(cc,rt,rn,rm)		corrr(cc,ARM_STR,rn,rt,rm)
#  define STRN(rt,rn,rm)		CC_STRN(ARM_CC_AL,rt,rn,rm)
#  define CC_STRI(cc,rt,rn,im)		corri(cc,ARM_STRI|ARM_P,rn,rt,im)
#  define STRI(rt,rn,im)		CC_STRI(ARM_CC_AL,rt,rn,im)
#  define T1_STRI(rt,rn,im)		is(THUMB_STRI|(_u5(im)<<6)|(_u3(rn)<<3)|_u3(rt))
#  define T1_STRISP(rt,im)		is(THUMB_STRISP|(_u3(rt)<<8)|(_u8(im)))
#  define T2_STRI(rt,rn,im)		torri8(THUMB2_STRI|THUMB2_U,rn,rt,im)
#  define T2_STRWI(rt,rn,im)		torri12(THUMB2_STRWI,rn,rt,im)
#  define CC_STRIN(cc,rt,rn,im)		corri(cc,ARM_STRI,rn,rt,im)
#  define STRIN(rt,rn,im)		CC_STRIN(ARM_CC_AL,rt,rn,im)
#  define T2_STRIN(rt,rn,im)		torri8(THUMB2_STRI,rn,rt,im)
#  define CC_STRD(cc,rt,rn,rm)		corrr(cc,ARM_STRD|ARM_P,rn,rt,rm)
#  define STRD(rt,rn,rm)		CC_STRD(ARM_CC_AL,rt,rn,rm)
#  define CC_STRDN(cc,rt,rn,rm)		corrr(cc,ARM_STRD,rn,rt,rm)
#  define STRDN(rt,rn,rm)		CC_STRDN(ARM_CC_AL,rt,rn,rm)
#  define CC_STRDI(cc,rt,rn,im)		corri8(cc,ARM_STRDI|ARM_P,rn,rt,im)
#  define STRDI(rt,rn,im)		CC_STRDI(ARM_CC_AL,rt,rn,im)
#  define T2_STRDI(rt,rt2,rn,im)	torrri8(THUMB2_STRDI|ARM_P,rn,rt,rt2,im)
#  define CC_STRDIN(cc,rt,rn,im)	corri8(cc,ARM_STRDI,rn,rt,im)
#  define STRDIN(rt,rn,im)		CC_STRDIN(ARM_CC_AL,rt,rn,im)
#  define T2_STRDIN(rt,rt2,rn,im)	torrri8(THUMB2_STRDI,rn,rt,rt2,im)
#  define CC_LDMIA(cc,rn,im)		corl(cc,ARM_M|ARM_M_L|ARM_M_I,rn,im)
#  define LDMIA(rn,im)			CC_LDMIA(ARM_CC_AL,rn,im)
#  define CC_LDM(cc,rn,im)		CC_LDMIA(cc,rn,im)
#  define LDM(rn,im)			LDMIA(rn,im)
#  define T1_LDMIA(rn,im)		is(THUMB_LDMIA|(_u3(rn)<<8)|im)
#  define T2_LDMIA(rn,im)		torl(THUMB2_LDMIA,rn,im)
#  define CC_LDMIA_U(cc,rn,im)		corl(cc,ARM_M|ARM_M_L|ARM_M_I|ARM_M_U,rn,im)
#  define LDMIA_U(rn,im)		CC_LDMIA_U(ARM_CC_AL,rn,im)
#  define LDM_U(r0,i0)			LDMIA_U(r0,i0)
#  define CC_LDMIB(cc,rn,im)		corl(cc,ARM_M|ARM_M_L|ARM_M_I|ARM_M_B,rn,im)
#  define LDMIB(rn,im)			CC_LDMIB(ARM_CC_AL,rn,im)
#  define CC_LDMIB_U(cc,rn,im)		corl(cc,ARM_M|ARM_M_L|ARM_M_I|ARM_M_B|ARM_M_U,rn,im)
#  define LDMIB_U(rn,im)		CC_LDMIB_U(ARM_CC_AL,rn,im)
#  define CC_LDMDA(cc,rn,im)		corl(cc,ARM_M|ARM_M_L,rn,im)
#  define LDMDA(rn,im)			CC_LDMDA(ARM_CC_AL,rn,im)
#  define CC_LDMDA_U(cc,rn,im)		corl(cc,ARM_M|ARM_M_L|ARM_M_U,rn,im)
#  define LDMDA_U(rn,im)		CC_LDMDA_U(ARM_CC_AL,rn,im)
#  define CC_LDMDB(cc,rn,im)		corl(cc,ARM_M|ARM_M_L|ARM_M_B,rn,im)
#  define LDMDB(rn,im)			CC_LDMDB(ARM_CC_AL,rn,im)
#  define T2_LDMDB(rn,im)		torl(THUMB2_LDMDB,rn,im)
#  define CC_LDMDB_U(cc,rn,im)		corl(cc,ARM_M|ARM_M_L|ARM_M_B|ARM_M_U,rn,im)
#  define LDMDB_U(rn,im)		CC_LDMDB_U(ARM_CC_AL,rn,im)
#  define CC_STMIA(cc,rn,im)		corl(cc,ARM_M|ARM_M_I,rn,im)
#  define STMIA(rn,im)			CC_STMIA(ARM_CC_AL,rn,im)
#  define CC_STM(cc,rn,im)		CC_STMIA(cc,rn,im)
#  define STM(rn,im)			STMIA(rn,im)
#  define CC_STMIA_U(cc,rn,im)		corl(cc,ARM_M|ARM_M_I|ARM_M_U,rn,im)
#  define STMIA_U(rn,im)		CC_STMIA_U(ARM_CC_AL,rn,im)
#  define CC_STM_U(cc,rn,im)		CC_STMIA_U(cc,rn,im)
#  define STM_U(rn,im)			STMIA_U(rn,im)
#  define CC_STMIB(cc,rn,im)		corl(cc,ARM_M|ARM_M_I|ARM_M_B,rn,im)
#  define STMIB(rn,im)			CC_STMIB(ARM_CC_AL,rn,im)
#  define CC_STMIB_U(cc,rn,im)		corl(cc,ARM_M|ARM_M_I|ARM_M_B|ARM_M_U,rn,im)
#  define STMIB_U(rn,im)		CC_STMIB_U(ARM_CC_AL,rn,im)
#  define CC_STMDA(cc,rn,im)		corl(cc,ARM_M,rn,im)
#  define STMDA(rn,im)			CC_STMDA(ARM_CC_AL,rn,im)
#  define CC_STMDA_U(cc,rn,im)		corl(cc,ARM_M|ARM_M_U,rn,im)
#  define STMDA_U(rn,im)		CC_STMDA_U(ARM_CC_AL,rn,im)
#  define CC_STMDB(cc,rn,im)		corl(cc,ARM_M|ARM_M_B,rn,im)
#  define STMDB(rn,im)			CC_STMDB(ARM_CC_AL,rn,im)
#  define CC_STMDB_U(cc,rn,im)		corl(cc,ARM_M|ARM_M_B|ARM_M_U,rn,im)
#  define STMDB_U(rn,im)		CC_STMDB_U(ARM_CC_AL,rn,im)
#  define CC_PUSH(cc,im)		CC_STMDB_U(cc,_SP_REGNO,im)
#  define PUSH(im)			STMDB_U(_SP_REGNO,im)
#  define T1_PUSH(im)			is(THUMB_PUSH|((im&0x4000)>>6)|(im&0xff))
#  define T2_PUSH(im)			tpp(THUMB2_PUSH,im)
#  define CC_POP(cc,im)			LDMIA_U(cc,_SP_REGNO,im)
#  define POP(im)			LDMIA_U(_SP_REGNO,im)
#  define T1_POP(im)			is(THUMB_POP|((im&0x8000)>>7)|(im&0xff))
#  define T2_POP(im)			tpp(THUMB2_POP,im)
#  define jit_get_reg_args()						\
    do {								\
	(void)jit_get_reg(_R0|jit_class_named|jit_class_gpr);		\
	(void)jit_get_reg(_R1|jit_class_named|jit_class_gpr);		\
	(void)jit_get_reg(_R2|jit_class_named|jit_class_gpr);		\
	(void)jit_get_reg(_R3|jit_class_named|jit_class_gpr);		\
    } while (0)
#  define jit_unget_reg_args()						\
    do {								\
	jit_unget_reg(_R3);						\
	jit_unget_reg(_R2);						\
	jit_unget_reg(_R1);						\
	jit_unget_reg(_R0);						\
    } while (0)
#  define nop(i0)			_nop(_jit,i0)
static void _nop(jit_state_t*,jit_int32_t);
#  define movr(r0,r1)			_movr(_jit,r0,r1)
static void _movr(jit_state_t*,jit_int32_t,jit_int32_t);
#  define movi(r0,i0)			_movi(_jit,r0,i0)
static void _movi(jit_state_t*,jit_int32_t,jit_word_t);
#  define movi_p(r0,i0)			_movi_p(_jit,r0,i0)
static jit_word_t _movi_p(jit_state_t*,jit_int32_t,jit_word_t);
#  define comr(r0,r1)			_comr(_jit,r0,r1)
static void _comr(jit_state_t*,jit_int32_t,jit_int32_t);
#  define negr(r0,r1)			_negr(_jit,r0,r1)
static void _negr(jit_state_t*,jit_int32_t,jit_int32_t);
#  define addr(r0,r1,r2)		_addr(_jit,r0,r1,r2)
static void _addr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define addi(r0,r1,i0)		_addi(_jit,r0,r1,i0)
static void _addi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define addcr(r0,r1,r2)		_addcr(_jit,r0,r1,r2)
static void _addcr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define addci(r0,r1,i0)		_addci(_jit,r0,r1,i0)
static void _addci(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define addxr(r0,r1,r2)		_addxr(_jit,r0,r1,r2)
static void _addxr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define addxi(r0,r1,i0)		_addxi(_jit,r0,r1,i0)
static void _addxi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define subr(r0,r1,r2)		_subr(_jit,r0,r1,r2)
static void _subr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define subi(r0,r1,i0)		_subi(_jit,r0,r1,i0)
static void _subi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define subcr(r0,r1,r2)		_subcr(_jit,r0,r1,r2)
static void _subcr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define subci(r0,r1,i0)		_subci(_jit,r0,r1,i0)
static void _subci(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define subxr(r0,r1,r2)		_subxr(_jit,r0,r1,r2)
static void _subxr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define subxi(r0,r1,i0)		_subxi(_jit,r0,r1,i0)
static void _subxi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define rsbi(r0, r1, i0)		_rsbi(_jit, r0, r1, i0)
static void _rsbi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define mulr(r0,r1,r2)		_mulr(_jit,r0,r1,r2)
static void _mulr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define muli(r0,r1,i0)		_muli(_jit,r0,r1,i0)
static void _muli(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define qmulr(r0,r1,r2,r3)		iqmulr(r0,r1,r2,r3,1)
#  define qmulr_u(r0,r1,r2,r3)		iqmulr(r0,r1,r2,r3,0)
#  define iqmulr(r0,r1,r2,r3,cc)	_iqmulr(_jit,r0,r1,r2,r3,cc)
static void _iqmulr(jit_state_t*,jit_int32_t,jit_int32_t,
		    jit_int32_t,jit_int32_t,jit_bool_t);
#  define qmuli(r0,r1,r2,i0)		iqmuli(r0,r1,r2,i0,1)
#  define qmuli_u(r0,r1,r2,i0)		iqmuli(r0,r1,r2,i0,0)
#  define iqmuli(r0,r1,r2,i0,cc)	_iqmuli(_jit,r0,r1,r2,i0,cc)
static void _iqmuli(jit_state_t*,jit_int32_t,jit_int32_t,
		    jit_int32_t,jit_word_t,jit_bool_t);
#  define divrem(d,s,r0,r1,r2)		_divrem(_jit,d,s,r0,r1,r2)
static void _divrem(jit_state_t*,int,int,jit_int32_t,jit_int32_t,jit_int32_t);
#  define divr(r0,r1,r2)		_divr(_jit,r0,r1,r2)
static void _divr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define divi(r0,r1,i0)		_divi(_jit,r0,r1,i0)
static void _divi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define divr_u(r0,r1,r2)		_divr_u(_jit,r0,r1,r2)
static void _divr_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define divi_u(r0,r1,i0)		_divi_u(_jit,r0,r1,i0)
static void _divi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define qdivr(r0,r1,r2,r3)		iqdivr(r0,r1,r2,r3,1)
#  define qdivr_u(r0,r1,r2,r3)		iqdivr(r0,r1,r2,r3,0)
#  define iqdivr(r0,r1,r2,r3,cc)	_iqdivr(_jit,r0,r1,r2,r3,cc)
static void _iqdivr(jit_state_t*,jit_int32_t,jit_int32_t,
		    jit_int32_t,jit_int32_t,jit_bool_t);
#  define qdivi(r0,r1,r2,i0)		iqdivi(r0,r1,r2,i0,1)
#  define qdivi_u(r0,r1,r2,i0)		iqdivi(r0,r1,r2,i0,0)
#  define iqdivi(r0,r1,r2,i0,cc)	_iqdivi(_jit,r0,r1,r2,i0,cc)
static void _iqdivi(jit_state_t*,jit_int32_t,jit_int32_t,
		    jit_int32_t,jit_word_t,jit_bool_t);
#  define remr(r0,r1,r2)		_remr(_jit,r0,r1,r2)
static void _remr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define remi(r0,r1,i0)		_remi(_jit,r0,r1,i0)
static void _remi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define remr_u(r0,r1,r2)		_remr_u(_jit,r0,r1,r2)
static void _remr_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define remi_u(r0,r1,i0)		_remi_u(_jit,r0,r1,i0)
static void _remi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define andr(r0,r1,r2)		_andr(_jit,r0,r1,r2)
static void _andr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define andi(r0,r1,i0)		_andi(_jit,r0,r1,i0)
static void _andi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define orr(r0,r1,r2)			_orr(_jit,r0,r1,r2)
static void _orr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ori(r0,r1,i0)			_ori(_jit,r0,r1,i0)
static void _ori(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define xorr(r0,r1,r2)		_xorr(_jit,r0,r1,r2)
static void _xorr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define xori(r0,r1,i0)		_xori(_jit,r0,r1,i0)
static void _xori(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define lshr(r0,r1,r2)		_lshr(_jit,r0,r1,r2)
static void _lshr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define lshi(r0,r1,i0)		_lshi(_jit,r0,r1,i0)
static void _lshi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define rshr(r0,r1,r2)		_rshr(_jit,r0,r1,r2)
static void _rshr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define rshi(r0,r1,i0)		_rshi(_jit,r0,r1,i0)
static void _rshi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define rshr_u(r0,r1,r2)		_rshr_u(_jit,r0,r1,r2)
static void _rshr_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define rshi_u(r0,r1,i0)		_rshi_u(_jit,r0,r1,i0)
static void _rshi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ccr(ct,cf,r0,r1,r2)		_ccr(_jit,ct,cf,r0,r1,r2)
static void _ccr(jit_state_t*,int,int,jit_int32_t,jit_int32_t,jit_int32_t);
#  define cci(ct,cf,r0,r1,i0)		_cci(_jit,ct,cf,r0,r1,i0)
static void _cci(jit_state_t*,int,int,jit_int32_t,jit_int32_t,jit_word_t);
#  define ltr(r0, r1, r2)		ccr(ARM_CC_LT,ARM_CC_GE,r0,r1,r2)
#  define lti(r0, r1, i0)		cci(ARM_CC_LT,ARM_CC_GE,r0,r1,i0)
#  define ltr_u(r0, r1, r2)		ccr(ARM_CC_LO,ARM_CC_HS,r0,r1,r2)
#  define lti_u(r0, r1, i0)		cci(ARM_CC_LO,ARM_CC_HS,r0,r1,i0)
#  define ler(r0, r1, r2)		ccr(ARM_CC_LE,ARM_CC_GT,r0,r1,r2)
#  define lei(r0, r1, i0)		cci(ARM_CC_LE,ARM_CC_GT,r0,r1,i0)
#  define ler_u(r0, r1, r2)		ccr(ARM_CC_LS,ARM_CC_HI,r0,r1,r2)
#  define lei_u(r0, r1, i0)		cci(ARM_CC_LS,ARM_CC_HI,r0,r1,i0)
#  define eqr(r0, r1, r2)		ccr(ARM_CC_EQ,ARM_CC_NE,r0,r1,r2)
#  define eqi(r0, r1, i0)		cci(ARM_CC_EQ,ARM_CC_NE,r0,r1,i0)
#  define ger(r0, r1, r2)		ccr(ARM_CC_GE,ARM_CC_LT,r0,r1,r2)
#  define gei(r0, r1, i0)		cci(ARM_CC_GE,ARM_CC_LT,r0,r1,i0)
#  define ger_u(r0, r1, r2)		ccr(ARM_CC_HS,ARM_CC_LO,r0,r1,r2)
#  define gei_u(r0, r1, i0)		cci(ARM_CC_HS,ARM_CC_LO,r0,r1,i0)
#  define gtr(r0, r1, r2)		ccr(ARM_CC_GT,ARM_CC_LE,r0,r1,r2)
#  define gti(r0, r1, i0)		cci(ARM_CC_GT,ARM_CC_LE,r0,r1,i0)
#  define gtr_u(r0, r1, r2)		ccr(ARM_CC_HI,ARM_CC_LS,r0,r1,r2)
#  define gti_u(r0, r1, i0)		cci(ARM_CC_HI,ARM_CC_LS,r0,r1,i0)
#  define ner(r0,r1,r2)			_ner(_jit,r0,r1,r2)
static void _ner(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define nei(r0,r1,i0)			_nei(_jit,r0,r1,i0)
static void _nei(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define jmpr(r0)			_jmpr(_jit,r0)
static void _jmpr(jit_state_t*,jit_int32_t);
#  define jmpi(i0)			_jmpi(_jit,i0)
static void _jmpi(jit_state_t*,jit_word_t);
#  define jmpi_p(i0, i1)		_jmpi_p(_jit,i0, i1)
static jit_word_t _jmpi_p(jit_state_t*,jit_word_t,jit_bool_t);
#  define bccr(cc,i0,r0,r1)		_bccr(_jit,cc,i0,r0,r1)
static jit_word_t _bccr(jit_state_t*,int,jit_word_t,jit_int32_t,jit_int32_t);
#  define bcci(cc,i0,r0,i1)		_bcci(_jit,cc,i0,r0,i1)
static jit_word_t _bcci(jit_state_t*,int,jit_word_t,jit_int32_t,jit_word_t);
#  define bltr(i0, r0, r1)		bccr(ARM_CC_LT,i0,r0,r1)
#  define blti(i0, r0, i1)		bcci(ARM_CC_LT,i0,r0,i1)
#  define bltr_u(i0, r0, r1)		bccr(ARM_CC_LO,i0,r0,r1)
#  define blti_u(i0, r0, i1)		bcci(ARM_CC_LO,i0,r0,i1)
#  define bler(i0, r0, r1)		bccr(ARM_CC_LE,i0,r0,r1)
#  define blei(i0, r0, i1)		bcci(ARM_CC_LE,i0,r0,i1)
#  define bler_u(i0, r0, r1)		bccr(ARM_CC_LS,i0,r0,r1)
#  define blei_u(i0, r0, i1)		bcci(ARM_CC_LS,i0,r0,i1)
#  define beqr(i0, r0, r1)		bccr(ARM_CC_EQ,i0,r0,r1)
#  define beqi(i0, r0, i1)		bcci(ARM_CC_EQ,i0,r0,i1)
#  define bger(i0, r0, r1)		bccr(ARM_CC_GE,i0,r0,r1)
#  define bgei(i0, r0, i1)		bcci(ARM_CC_GE,i0,r0,i1)
#  define bger_u(i0, r0, r1)		bccr(ARM_CC_HS,i0,r0,r1)
#  define bgei_u(i0, r0, i1)		bcci(ARM_CC_HS,i0,r0,i1)
#  define bgtr(i0, r0, r1)		bccr(ARM_CC_GT,i0,r0,r1)
#  define bgti(i0, r0, i1)		bcci(ARM_CC_GT,i0,r0,i1)
#  define bgtr_u(i0, r0, r1)		bccr(ARM_CC_HI,i0,r0,r1)
#  define bgti_u(i0, r0, i1)		bcci(ARM_CC_HI,i0,r0,i1)
#  define bner(i0, r0, r1)		bccr(ARM_CC_NE,i0,r0,r1)
#  define bnei(i0, r0, i1)		bcci(ARM_CC_NE,i0,r0,i1)
#  define baddr(cc,i0,r0,r1)		_baddr(_jit,cc,i0,r0,r1)
static jit_word_t _baddr(jit_state_t*,int,jit_word_t,jit_int32_t,jit_int32_t);
#  define baddi(cc,i0,r0,r1)		_baddi(_jit,cc,i0,r0,r1)
static jit_word_t _baddi(jit_state_t*,int,jit_word_t,jit_int32_t,jit_word_t);
#  define boaddr(i0,r0,r1)		baddr(ARM_CC_VS,i0,r0,r1)
#  define boaddi(i0,r0,i1)		baddi(ARM_CC_VS,i0,r0,i1)
#  define boaddr_u(i0,r0,r1)		baddr(ARM_CC_HS,i0,r0,r1)
#  define boaddi_u(i0,r0,i1)		baddi(ARM_CC_HS,i0,r0,i1)
#  define bxaddr(i0,r0,r1)		baddr(ARM_CC_VC,i0,r0,r1)
#  define bxaddi(i0,r0,i1)		baddi(ARM_CC_VC,i0,r0,i1)
#  define bxaddr_u(i0,r0,r1)		baddr(ARM_CC_LO,i0,r0,r1)
#  define bxaddi_u(i0,r0,i1)		baddi(ARM_CC_LO,i0,r0,i1)
#  define bsubr(cc,i0,r0,r1)		_bsubr(_jit,cc,i0,r0,r1)
static jit_word_t _bsubr(jit_state_t*,int,jit_word_t,jit_int32_t,jit_int32_t);
#  define bsubi(cc,i0,r0,r1)		_bsubi(_jit,cc,i0,r0,r1)
static jit_word_t _bsubi(jit_state_t*,int,jit_word_t,jit_int32_t,jit_word_t);
#  define bosubr(i0,r0,r1)		bsubr(ARM_CC_VS,i0,r0,r1)
#  define bosubi(i0,r0,i1)		bsubi(ARM_CC_VS,i0,r0,i1)
#  define bosubr_u(i0,r0,r1)		bsubr(ARM_CC_LO,i0,r0,r1)
#  define bosubi_u(i0,r0,i1)		bsubi(ARM_CC_LO,i0,r0,i1)
#  define bxsubr(i0,r0,r1)		bsubr(ARM_CC_VC,i0,r0,r1)
#  define bxsubi(i0,r0,i1)		bsubi(ARM_CC_VC,i0,r0,i1)
#  define bxsubr_u(i0,r0,r1)		bsubr(ARM_CC_HS,i0,r0,r1)
#  define bxsubi_u(i0,r0,i1)		bsubi(ARM_CC_HS,i0,r0,i1)
#  define bmxr(cc,i0,r0,r1)		_bmxr(_jit,cc,i0,r0,r1)
static jit_word_t _bmxr(jit_state_t*,int,jit_word_t,jit_int32_t,jit_int32_t);
#  define bmxi(cc,i0,r0,r1)		_bmxi(_jit,cc,i0,r0,r1)
static jit_word_t _bmxi(jit_state_t*,int,jit_word_t,jit_int32_t,jit_word_t);
#  define bmsr(i0,r0,r1)		bmxr(ARM_CC_NE,i0,r0,r1)
#  define bmsi(i0,r0,i1)		bmxi(ARM_CC_NE,i0,r0,i1)
#  define bmcr(i0,r0,r1)		bmxr(ARM_CC_EQ,i0,r0,r1)
#  define bmci(i0,r0,i1)		bmxi(ARM_CC_EQ,i0,r0,i1)
#  define ldr_c(r0,r1)			_ldr_c(_jit,r0,r1)
static void _ldr_c(jit_state_t*,jit_int32_t,jit_int32_t);
#  define ldi_c(r0,i0)			_ldi_c(_jit,r0,i0)
static void _ldi_c(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_c(r0,r1,r2)		_ldxr_c(_jit,r0,r1,r2)
static void _ldxr_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_c(r0,r1,i0)		_ldxi_c(_jit,r0,r1,i0)
static void _ldxi_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldr_uc(r0,r1)			_ldr_uc(_jit,r0,r1)
static void _ldr_uc(jit_state_t*,jit_int32_t,jit_int32_t);
#  define ldi_uc(r0,i0)			_ldi_uc(_jit,r0,i0)
static void _ldi_uc(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_uc(r0,r1,r2)		_ldxr_uc(_jit,r0,r1,r2)
static void _ldxr_uc(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_uc(r0,r1,i0)		_ldxi_uc(_jit,r0,r1,i0)
static void _ldxi_uc(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldr_s(r0,r1)			_ldr_s(_jit,r0,r1)
static void _ldr_s(jit_state_t*,jit_int32_t,jit_int32_t);
#  define ldi_s(r0,i0)			_ldi_s(_jit,r0,i0)
static void _ldi_s(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_s(r0,r1,r2)		_ldxr_s(_jit,r0,r1,r2)
static void _ldxr_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_s(r0,r1,i0)		_ldxi_s(_jit,r0,r1,i0)
static void _ldxi_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldr_us(r0,r1)			_ldr_us(_jit,r0,r1)
static void _ldr_us(jit_state_t*,jit_int32_t,jit_int32_t);
#  define ldi_us(r0,i0)			_ldi_us(_jit,r0,i0)
static void _ldi_us(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_us(r0,r1,r2)		_ldxr_us(_jit,r0,r1,r2)
static void _ldxr_us(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_us(r0,r1,i0)		_ldxi_us(_jit,r0,r1,i0)
static void _ldxi_us(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldr_i(r0,r1)			_ldr_i(_jit,r0,r1)
static void _ldr_i(jit_state_t*,jit_int32_t,jit_int32_t);
#  define ldi_i(r0,i0)			_ldi_i(_jit,r0,i0)
static void _ldi_i(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_i(r0,r1,r2)		_ldxr_i(_jit,r0,r1,r2)
static void _ldxr_i(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_i(r0,r1,i0)		_ldxi_i(_jit,r0,r1,i0)
static void _ldxi_i(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define str_c(r0,r1)			_str_c(_jit,r0,r1)
static void _str_c(jit_state_t*,jit_int32_t,jit_int32_t);
#  define sti_c(i0,r0)			_sti_c(_jit,i0,r0)
static void _sti_c(jit_state_t*,jit_word_t,jit_int32_t);
#  define stxr_c(r0,r1,r2)		_stxr_c(_jit,r0,r1,r2)
static void _stxr_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_c(r0,r1,i0)		_stxi_c(_jit,r0,r1,i0)
static void _stxi_c(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define str_s(r0,r1)			_str_s(_jit,r0,r1)
static void _str_s(jit_state_t*,jit_int32_t,jit_int32_t);
#  define sti_s(i0,r0)			_sti_s(_jit,i0,r0)
static void _sti_s(jit_state_t*,jit_word_t,jit_int32_t);
#  define stxr_s(r0,r1,r2)		_stxr_s(_jit,r0,r1,r2)
static void _stxr_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_s(r0,r1,i0)		_stxi_s(_jit,r0,r1,i0)
static void _stxi_s(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define str_i(r0,r1)			_str_i(_jit,r0,r1)
static void _str_i(jit_state_t*,jit_int32_t,jit_int32_t);
#  define sti_i(i0,r0)			_sti_i(_jit,i0,r0)
static void _sti_i(jit_state_t*,jit_word_t,jit_int32_t);
#  define stxr_i(r0,r1,r2)		_stxr_i(_jit,r0,r1,r2)
static void _stxr_i(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define stxi_i(r0,r1,i0)		_stxi_i(_jit,r0,r1,i0)
static void _stxi_i(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  if __BYTE_ORDER == __LITTLE_ENDIAN
#  define htonr_us(r0,r1)		_htonr_us(_jit,r0,r1)
static void _htonr_us(jit_state_t*,jit_int32_t,jit_int32_t);
#  define htonr_ui(r0,r1)		_htonr_ui(_jit,r0,r1)
static void _htonr_ui(jit_state_t*,jit_int32_t,jit_int32_t);
#  else
#    define htonr_us(r0,r1)		extr_us(r0,r1)
#    define htonr(r0,r1)		movr(r0,r1)
#  endif
#  define extr_c(r0,r1)			_extr_c(_jit,r0,r1)
static void _extr_c(jit_state_t*,jit_int32_t,jit_int32_t);
#  define extr_uc(r0,r1)		_extr_uc(_jit,r0,r1)
static void _extr_uc(jit_state_t*,jit_int32_t,jit_int32_t);
#  define extr_s(r0,r1)			_extr_s(_jit,r0,r1)
static void _extr_s(jit_state_t*,jit_int32_t,jit_int32_t);
#  define extr_us(r0,r1)		_extr_us(_jit,r0,r1)
static void _extr_us(jit_state_t*,jit_int32_t,jit_int32_t);
#  define prolog(i0)			_prolog(_jit,i0)
static void _prolog(jit_state_t*,jit_node_t*);
#  define epilog(i0)			_epilog(_jit,i0)
static void _epilog(jit_state_t*,jit_node_t*);
#  define callr(r0)			_callr(_jit,r0)
static void _callr(jit_state_t*,jit_int32_t);
#  define calli(i0)			_calli(_jit,i0)
static void _calli(jit_state_t*,jit_word_t);
#  define calli_p(i0)			_calli_p(_jit,i0)
static jit_word_t _calli_p(jit_state_t*,jit_word_t);
#  define vastart(r0)			_vastart(_jit, r0)
static void _vastart(jit_state_t*, jit_int32_t);
#  define vaarg(r0, r1)			_vaarg(_jit, r0, r1)
static void _vaarg(jit_state_t*, jit_int32_t, jit_int32_t);
#  define patch_at(kind,jump,label)	_patch_at(_jit,kind,jump,label)
static void _patch_at(jit_state_t*,jit_int32_t,jit_word_t,jit_word_t);
#endif

#if CODE
/* from binutils */
#  define rotate_left(v, n)	(v << n | v >> (32 - n))
static int
encode_arm_immediate(unsigned int v)
{
    unsigned int	a, i;

    for (i = 0; i < 32; i += 2)
	if ((a = rotate_left(v, i)) <= 0xff)
	    return (a | (i << 7));

    return (-1);
}

static int
encode_thumb_immediate(unsigned int v)
{
    int			i;
    unsigned int	m;
    unsigned int	n;
    /* 00000000 00000000 00000000 abcdefgh */
    if ((v & 0xff) == v)
	return (v);
    /* 00000000 abcdefgh 00000000 abcdefgh */
    if ((v & 0xff00ff) == v && ((v & 0xff0000) >> 16) == (v & 0xff))
	return ((v & 0xff) | (1 << 12));
    /* abcdefgh 00000000 abcdefgh 00000000 */
    if (((v & 0xffff0000) >> 16) == (v & 0xffff) && (v & 0xff) == 0)
	return ((v & 0x000000ff) | (2 << 12));
    /* abcdefgh abcdefgh abcdefgh abcdefgh */
    if ( (v &    0xff)        == ((v &     0xff00) >>  8) &&
	((v &   0xff00) >> 8) == ((v &   0xff0000) >> 16) &&
	((v & 0xff0000) << 8) ==  (v & 0xff000000))
	return ((v & 0xff) | (3 << 12));
    /* 1bcdefgh << 24 ... 1bcdefgh << 1 */
    for (i = 8, m = 0xff000000, n = 0x80000000;
	 i < 23; i++, m >>= 1,  n >>= 1) {
	if ((v & m) == v && (v & n)) {
	    v >>= 32 - i;
	    if (!(i & 1))
		v &= 0x7f;
	    i >>= 1;
	    return (((i & 7) << 12) | ((i & 8) << 23) | v);
	}
    }
    return (-1);
}

static int
encode_thumb_word_immediate(unsigned int v)
{
    if ((v & 0xfffff000) == 0)
	return (((v & 0x800) << 15) | ((v & 0x700) << 4) | (v & 0xff));
    return (-1);
}

static int
encode_thumb_jump(int v)
{
    int		s, i1, i2, j1, j2;
    if (v >= (int)-0x800000 && v <= 0x7fffff) {
	s  = !!(v & 0x800000);
	i1 = !!(v & 0x400000);
	i2 = !!(v & 0x200000);
	j1 = s ? i1 : !i1;
	j2 = s ? i2 : !i2;
	return ((s<<26)|((v&0x1ff800)<<5)|(j1<<13)|(j2<<11)|(v&0x7ff));
    }
    return (-1);
}

static int
encode_thumb_cc_jump(int v)
{
    int		s, j1, j2;
    if (v >= (int)-0x80000 && v <= 0x7ffff) {
	s  = !!(v & 0x80000);
	j1 = !!(v & 0x20000);
	j2 = !!(v & 0x40000);
	return ((s<<26)|((v&0x1f800)<<5)|(j1<<13)|(j2<<11)|(v&0x7ff));
    }
    return (-1);
}

static int
encode_thumb_shift(int v, int type)
{
    switch (type) {
	case ARM_ASR:
	case ARM_LSL:
	case ARM_LSR:		type >>= 1;	break;
	default:		assert(!"handled shift");
    }
    assert(v >= 0 && v <= 31);
    return (((v & 0x1c) << 10) | ((v & 3) << 6) | type);
}

static void
_tcit(jit_state_t *_jit, unsigned int tc, int it)
{
    int		c;
    int		m;
    c = (tc >> 28) & 1;
    assert(!(tc & 0xfffffff) && tc != ARM_CC_NV);
    switch (it) {
	case THUMB2_IT:		m =   1<<3; 			break;
	case THUMB2_ITT:	m =  (c<<3)| (1<<2);		break;
	case THUMB2_ITE:	m = (!c<<3)| (1<<2);		break;
	case THUMB2_ITTT:	m =  (c<<3)| (c<<2)| (1<<1);	break;
	case THUMB2_ITET:	m = (!c<<3)| (c<<2)| (1<<1);	break;
	case THUMB2_ITTE:	m =  (c<<3)|(!c<<2)| (1<<1);	break;
	case THUMB2_ITEE:	m = (!c<<3)|(!c<<2)| (1<<1);	break;
	case THUMB2_ITTTT:	m =  (c<<3)| (c<<2)| (c<<1)|1;	break;
	case THUMB2_ITETT:	m = (!c<<3)| (c<<2)| (c<<1)|1;	break;
	case THUMB2_ITTET:	m =  (c<<3)|(!c<<2)| (c<<1)|1;	break;
	case THUMB2_ITEET:	m = (!c<<3)|(!c<<2)| (c<<1)|1;	break;
	case THUMB2_ITTTE:	m =  (c<<3)| (c<<2)|(!c<<1)|1;	break;
	case THUMB2_ITETE:	m = (!c<<3)| (c<<2)|(!c<<1)|1;	break;
	case THUMB2_ITTEE:	m =  (c<<3)|(!c<<2)|(!c<<1)|1;	break;
	case THUMB2_ITEEE:	m = (!c<<3)|(!c<<2)|(!c<<1)|1;	break;
	default:		abort();
    }
    assert(m && (tc != ARM_CC_AL || !(m & (m - 1))));
    is(0xbf00 | (tc >> 24) | m);
}

static void
_corrr(jit_state_t *_jit, int cc, int o, int rn, int rd, int rm)
{
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf00fff0f));
    ii(cc|o|(_u4(rn)<<16)|(_u4(rd)<<12)|_u4(rm));
}

static void
_corri(jit_state_t *_jit, int cc, int o, int rn, int rd, int im)
{
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf00fffff));
    assert(!(im & 0xfffff000));
    ii(cc|o|(_u4(rn)<<16)|(_u4(rd)<<12)|_u12(im));
}

static void
_corri8(jit_state_t *_jit, int cc, int o, int rn, int rt, int im)
{
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf00fff0f));
    assert(!(im & 0xffffff00));
    ii(cc|o|(_u4(rn)<<16)|(_u4(rt)<<12)|((im&0xf0)<<4)|(im&0x0f));
}

static void
_coriw(jit_state_t *_jit, int cc, int o, int rd, int im)
{
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf00fffff));
    assert(!(im & 0xffff0000));
    ii(cc|o|((im&0xf000)<<4)|(_u4(rd)<<12)|(im&0xfff));
}

static void
_torrr(jit_state_t *_jit, int o, int rn, int rd, int rm)
{
    jit_thumb_t	thumb;
    assert(!(o & 0xf0f0f));
    thumb.i = o|(_u4(rn)<<16)|(_u4(rd)<<8)|_u4(rm);
    iss(thumb.s[0], thumb.s[1]);
}

static void
_torrrs(jit_state_t *_jit, int o, int rn, int rd, int rm, int im)
{
    jit_thumb_t	thumb;
    assert(!(o  & 0x000f0f0f));
    assert(!(im & 0xffff8f0f));
    thumb.i = o|(_u4(rn)<<16)|(_u4(rd)<<8)|im|_u4(rm);
    iss(thumb.s[0], thumb.s[1]);
}

static void
_torxr(jit_state_t *_jit, int o, int rn, int rt, int rm)
{
    jit_thumb_t	thumb;
    assert(!(o & 0xf0f0f));
    thumb.i = o|(_u4(rn)<<16)|(_u4(rt)<<12)|_u4(rm);
    iss(thumb.s[0], thumb.s[1]);
}

static void
_torrrr(jit_state_t *_jit, int o, int rn, int rl, int rh, int rm)
{
    jit_thumb_t	thumb;
    assert(!(o & 0x000fff0f));
    thumb.i = o|(_u4(rn)<<16)|(_u4(rl)<<12)|(_u4(rh)<<8)|_u4(rm);
    iss(thumb.s[0], thumb.s[1]);
}

static void
_torrri8(jit_state_t *_jit, int o, int rn, int rt, int rt2, int im)
{
    jit_thumb_t	thumb;
    assert(!(o  & 0x000fffff));
    assert(!(im & 0xffffff00));
    thumb.i = o|(_u4(rn)<<16)|(_u4(rt)<<12)|(_u4(rt2)<<8)|im;
    iss(thumb.s[0], thumb.s[1]);
}

static void
_torri(jit_state_t *_jit, int o, int rn, int rd, int im)
{
    jit_thumb_t	thumb;
    assert(!(o  & 0x0c0f7fff));
    assert(!(im & 0xfbff8f00));
    thumb.i = o|(_u4(rn)<<16)|(_u4(rd)<<8)|im;
    iss(thumb.s[0], thumb.s[1]);
}

static void
_torri8(jit_state_t *_jit, int o, int rn, int rt, int im)
{
    jit_thumb_t	thumb;
    assert(!(o  & 0x000ff0ff));
    assert(!(im & 0xffffff00));
    thumb.i = o|(_u4(rn)<<16)|(_u4(rt)<<12)|im;
    iss(thumb.s[0], thumb.s[1]);
}

static void
_torri12(jit_state_t *_jit, int o, int rn, int rt, int im)
{
    jit_thumb_t	thumb;
    assert(!(o  & 0x000fffff));
    assert(!(im & 0xfffff000));
    thumb.i = o|(_u4(rn)<<16)|(_u4(rt)<<12)|im;
    iss(thumb.s[0], thumb.s[1]);
}

static void
_tshift(jit_state_t *_jit, int o, int rd, int rm, int im)
{
    jit_thumb_t	thumb;
    assert(!(o & 0x7fcf));
    assert(im >= 0 && im < 32);
    thumb.i = o|((im&0x1c)<<10)|(_u4(rd)<<8)|((im&3)<<6)|_u4(rm);
    iss(thumb.s[0], thumb.s[1]);
}

static void
_toriw(jit_state_t *_jit, int o, int rd, int im)
{
    jit_thumb_t	thumb;
    assert(!(im & 0xffff0000));
    thumb.i = o|((im&0xf000)<<4)|((im&0x800)<<15)|((im&0x700)<<4)|(_u4(rd)<<8)|(im&0xff);
    iss(thumb.s[0], thumb.s[1]);
}

static void
_tc8(jit_state_t *_jit, int cc, int im)
{
    assert(!(cc & 0x0fffffff));
    assert(cc != ARM_CC_AL && cc != ARM_CC_NV);
    assert(im >= -128 && im <= 127);
    is(THUMB_CC_B|(cc>>20)|(im&0xff));
}

static void
_t11(jit_state_t *_jit, int im)
{
    assert(!(im & 0xfffff800));
    is(THUMB_B|im);
}

static void
_tcb(jit_state_t *_jit, int cc, int im)
{
    jit_thumb_t	thumb;
    assert(!(cc & 0xfffffff));
    assert(cc != ARM_CC_AL && cc != ARM_CC_NV);
    cc = ((jit_uint32_t)cc) >> 6;
    assert(!(im & (THUMB2_CC_B|cc)));
    thumb.i = THUMB2_CC_B|cc|im;
    iss(thumb.s[0], thumb.s[1]);
}

static void
_blxi(jit_state_t *_jit, int im)
{
    assert(!(im & 0xfe000000));
    ii(ARM_BLXI|im);
}

static void
_tb(jit_state_t *_jit, int o, int im)
{
    jit_thumb_t	thumb;
    assert(!(o & 0x07ff2fff));
    assert(!(o & im));
    thumb.i = o|im;
    iss(thumb.s[0], thumb.s[1]);
}

static void
_corrrr(jit_state_t *_jit, int cc, int o, int rh, int rl, int rm, int rn)
{
    assert(!(cc & 0x0fffffff));
    assert(!(o & 0xf00fff0f));
    ii(cc|o|(_u4(rh)<<16)|(_u4(rl)<<12)|(_u4(rm)<<8)|_u4(rn));
}

static void
_corrrs(jit_state_t *_jit, int cc, int o, int rn, int rd, int rm, int im)
{
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf000ff8f));
    ii(cc|o|(_u4(rd)<<12)|(_u4(rn)<<16)|(im<<7)|_u4(rm));
}

static void
_cshift(jit_state_t *_jit, int cc, int o, int rd, int rm, int rn, int im)
{
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xffe0ff8f));
    assert(((_u4(rm)<<8)&(im<<7)) == 0);
    ii(cc|ARM_SHIFT|o|(_u4(rd)<<12)|(_u4(rm)<<8)|(im<<7)|_u4(rn));
}

static void
_cb(jit_state_t *_jit, int cc, int o, int im)
{
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf0ffffff));
    ii(cc|o|_u24(im));
}

static void
_cbx(jit_state_t *_jit, int cc, int o, int rm)
{
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf000000f));
    ii(cc|o|_u4(rm));
}

static void
_corl(jit_state_t *_jit, int cc, int o, int r0, int i0)
{
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf00fffff));
    ii(cc|o|(_u4(r0)<<16)|_u16(i0));
}

static void
_c6orr(jit_state_t *_jit, int cc, int o, int rd, int rm)
{
    assert(!(cc & 0x0fffffff));
    assert(!(o  & 0xf000f00f));
    ii(cc|o|(_u4(rd)<<12)|_u4(rm));
}

static void
_tpp(jit_state_t *_jit, int o, int im)
{
    jit_thumb_t	thumb;
    assert(!(o & 0x0000ffff));
    if (o == THUMB2_PUSH)
	assert(!(im & 0x8000));
    assert(__builtin_popcount(im & 0x1fff) > 1);
    thumb.i = o|im;
    iss(thumb.s[0], thumb.s[1]);
}

static void
_torl(jit_state_t *_jit, int o, int rn, int im)
{
    jit_thumb_t	thumb;
    assert(!(o & 0xf1fff));
    assert(rn != _R15 || !im || ((o & 0xc000) == 0xc000));
    assert(!(o & THUMB2_LDM_W) || !(im & (1 << rn)));
    thumb.i = o | (_u4(rn)<<16)|_u13(im);
    iss(thumb.s[0], thumb.s[1]);
}

static void
_nop(jit_state_t *_jit, jit_int32_t i0)
{
    if (jit_thumb_p()) {
	for (; i0 > 0; i0 -= 2)
	    T1_NOP();
    }
    else {
	for (; i0 > 0; i0 -= 4)
	    NOP();
    }
    assert(i0 == 0);
}

static void
_movr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 != r1) {
	if (jit_thumb_p())
	    T1_MOV(r0, r1);
	else
	    MOV(r0, r1);
    }
}

static void
_movi(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    int			i;
    if (jit_thumb_p()) {
	if (!jit_no_set_flags() && r0 < 8 && !(i0 & 0xffffff80))
	    T1_MOVI(r0, i0);
	else if ((i = encode_thumb_immediate(i0)) != -1)
	    T2_MOVI(r0, i);
	else if ((i = encode_thumb_immediate(~i0)) != -1)
	    T2_MVNI(r0, i);
	else {
	    T2_MOVWI(r0, (jit_uint16_t)i0);
	    if (i0 & 0xffff0000)
		T2_MOVTI(r0, (jit_uint16_t)((unsigned)i0 >> 16));
	}
    }
    else {
	if (jit_armv6_p() && !(i0 & 0xffff0000))
	    MOVWI(r0, i0);
	else if ((i = encode_arm_immediate(i0)) != -1)
	    MOVI(r0, i);
	else if ((i = encode_arm_immediate(~i0)) != -1)
	    MVNI(r0, i);
	else if (jit_armv6_p()) {
	    MOVWI(r0, (jit_uint16_t)(i0));
	    if ((i0 & 0xffff0000))
		MOVTI(r0, (jit_uint16_t)((unsigned)i0 >> 16));
	}
	else
	    load_const(0, r0, i0);
    }
}

static jit_word_t
_movi_p(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    w = _jit->pc.w;
    if (jit_thumb_p()) {
	T2_MOVWI(r0, (jit_uint16_t)(i0));
	T2_MOVTI(r0, (jit_uint16_t)((unsigned)i0 >> 16));
    }
    else
	load_const(1, r0, 0);
    return (w);
}

static void
_comr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_thumb_p()) {
	if (!jit_no_set_flags() && (r0|r1) < 8)
	    T1_NOT(r0, r1);
	else
	    T2_NOT(r0, r1);
    }
    else
	NOT(r0, r1);
}

static void
_negr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_thumb_p()) {
	if (!jit_no_set_flags() && (r0|r1) < 8)
	    T1_RSBI(r0, r1);
	else
	    T2_RSBI(r0, r1, 0);
    }
    else
	RSBI(r0, r1, 0);
}

static void
_addr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_thumb_p()) {
	if (!jit_no_set_flags() && (r0|r1|r2) < 8)
	    T1_ADD(r0, r1, r2);
	else if (r0 == r1 || r0 == r2)
	    T1_ADDX(r0, r0 == r1 ? r2 : r1);
	else
	    T2_ADD(r0, r1, r2);
    }
    else
	ADD(r0, r1, r2);
}

static void
_addi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    int			i;
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if (!jit_no_set_flags() && (r0|r1) < 8 && !(i0 & ~7))
	    T1_ADDI3(r0, r1, i0);
	else if (!jit_no_set_flags() && (r0|r1) < 8 && !(-i0 & ~7))
	    T1_SUBI3(r0, r1, -i0);
	else if (!jit_no_set_flags() && r0 < 8 && r0 == r1 && !(i0 & ~0xff))
	    T1_ADDI8(r0, i0);
	else if (!jit_no_set_flags() && r0 < 8 && r0 == r1 && !(-i0 & ~0xff))
	    T1_SUBI8(r0, -i0);
	else if ((i = encode_thumb_immediate(i0)) != -1)
	    T2_ADDI(r0, r1, i);
	else if ((i = encode_thumb_immediate(-i0)) != -1)
	    T2_SUBI(r0, r1, i);
	else if ((i = encode_thumb_word_immediate(i0)) != -1)
	    T2_ADDWI(r0, r1, i);
	else if ((i = encode_thumb_word_immediate(-i0)) != -1)
	    T2_SUBWI(r0, r1, i);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    T2_ADD(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	if ((i = encode_arm_immediate(i0)) != -1)
	    ADDI(r0, r1, i);
	else if ((i = encode_arm_immediate(-i0)) != -1)
	    SUBI(r0, r1, i);
	else if (r0 != r1) {
	    movi(r0, i0);
	    ADD(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    ADD(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

static void
_addcr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_thumb_p()) {
	/* thumb auto set carry if not inside IT block */
	if ((r0|r1|r2) < 8)
	    T1_ADD(r0, r1, r2);
	else
	    T2_ADDS(r0, r1, r2);
    }
    else
	ADDS(r0, r1, r2);
}

static void
_addci(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    int			i;
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if ((r0|r1) < 8 && !(i0 & ~7))
	    T1_ADDI3(r0, r1, i0);
	else if ((r0|r1) < 8 && !(-i0 & ~7))
	    T1_SUBI3(r0, r1, -i0);
	else if (r0 < 8 && r0 == r1 && !(i0 & ~0xff))
	    T1_ADDI8(r0, i0);
	else if (r0 < 8 && r0 == r1 && !(-i0 & ~0xff))
	    T1_SUBI8(r0, -i0);
	else if ((i = encode_thumb_immediate(i0)) != -1)
	    T2_ADDSI(r0, r1, i);
	else if ((i = encode_thumb_immediate(-i0)) != -1)
	    T2_SUBSI(r0, r1, i);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    T2_ADDS(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	if ((i = encode_arm_immediate(i0)) != -1)
	    ADDSI(r0, r1, i);
	else if ((i = encode_arm_immediate(-i0)) != -1)
	    SUBSI(r0, r1, i);
	else if (r0 != r1) {
	    movi(r0, i0);
	    ADDS(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    ADDS(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

static void
_addxr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    /* keep setting carry because don't know last ADC */
    if (jit_thumb_p()) {
	/* thumb auto set carry if not inside IT block */
	if ((r0|r1|r2) < 8 && (r0 == r1 || r0 == r2))
	    T1_ADC(r0, r0 == r1 ? r2 : r1);
	else
	    T2_ADCS(r0, r1, r2);
    }
    else
	ADCS(r0, r1, r2);
}

static void
_addxi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    int			i;
    jit_int32_t		reg;
    int			no_set_flags;
    if (jit_thumb_p()) {
	no_set_flags = jit_no_set_flags();
	jit_no_set_flags() = 1;
	if ((i = encode_thumb_immediate(i0)) != -1)
	    T2_ADCSI(r0, r1, i);
	else if ((i = encode_thumb_immediate(-i0)) != -1)
	    T2_SBCSI(r0, r1, i);
	else if (r0 != r1) {
	    movi(r0, i0);
	    T2_ADCS(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    T2_ADCS(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
	jit_no_set_flags() = no_set_flags;
    }
    else {
	if ((i = encode_arm_immediate(i0)) != -1)
	    ADCSI(r0, r1, i);
	else if ((i = encode_arm_immediate(-i0)) != -1)
	    SBCSI(r0, r1, i);
	else if (r0 != r1) {
	    movi(r0, i0);
	    ADCS(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    ADCS(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

static void
_subr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_thumb_p()) {
	if (!jit_no_set_flags() && (r0|r1|r2) < 8)
	    T1_SUB(r0, r1, r2);
	else
	    T2_SUB(r0, r1, r2);
    }
    else
	SUB(r0, r1, r2);
}

static void
_subi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    int			i;
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if (!jit_no_set_flags() && (r0|r1) < 8 && !(i0 & ~7))
	    T1_SUBI3(r0, r1, i0);
	else if (!jit_no_set_flags() && (r0|r1) < 8 && !(-i0 & ~7))
	    T1_ADDI3(r0, r1, -i0);
	else if (!jit_no_set_flags() && r0 < 8 && r0 == r1 && !(i0 & ~0xff))
	    T1_SUBI8(r0, i0);
	else if (!jit_no_set_flags() && r0 < 8 && r0 == r1 && !(-i0 & ~0xff))
	    T1_ADDI8(r0, -i0);
	else if ((i = encode_thumb_immediate(i0)) != -1)
	    T2_SUBI(r0, r1, i);
	else if ((i = encode_thumb_immediate(-i0)) != -1)
	    T2_ADDI(r0, r1, i);
	else if ((i = encode_thumb_word_immediate(i0)) != -1)
	    T2_SUBWI(r0, r1, i);
	else if ((i = encode_thumb_word_immediate(-i0)) != -1)
	    T2_ADDWI(r0, r1, i);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    T2_SUB(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	if ((i = encode_arm_immediate(i0)) != -1)
	    SUBI(r0, r1, i);
	else if ((i = encode_arm_immediate(-i0)) != -1)
	    ADDI(r0, r1, i);
	else if (r0 != r1) {
	    movi(r0, i0);
	    SUB(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    SUB(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

static void
_subcr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_thumb_p()) {
	/* thumb auto set carry if not inside IT block */
	if ((r0|r1|r2) < 8)
	    T1_SUB(r0, r1, r2);
	else
	    T2_SUBS(r0, r1, r2);
    }
    else
	SUBS(r0, r1, r2);
}

static void
_subci(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    int			i;
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if ((r0|r1) < 8 && !(i0 & ~7))
	    T1_SUBI3(r0, r1, i0);
	else if ((r0|r1) < 8 && !(-i0 & ~7))
	    T1_ADDI3(r0, r1, -i0);
	else if (r0 < 8 && r0 == r1 && !(i0 & ~0xff))
	    T1_SUBI8(r0, i0);
	else if (r0 < 8 && r0 == r1 && !(-i0 & ~0xff))
	    T1_ADDI8(r0, -i0);
	else if ((i = encode_thumb_immediate(i0)) != -1)
	    T2_SUBSI(r0, r1, i);
	else if ((i = encode_thumb_immediate(-i0)) != -1)
	    T2_ADDSI(r0, r1, i);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    T2_SUBS(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	if ((i = encode_arm_immediate(i0)) != -1)
	    SUBSI(r0, r1, i);
	else if ((i = encode_arm_immediate(-i0)) != -1)
	    ADDSI(r0, r1, i);
	else if (r0 != r1) {
	    movi(r0, i0);
	    SUBS(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    SUBS(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

static void
_subxr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    /* keep setting carry because don't know last SBC */
    if (jit_thumb_p()) {
	/* thumb auto set carry if not inside IT block */
	if ((r0|r1|r2) < 8 && r0 == r1)
	    T1_SBC(r0, r2);
	else
	    T2_SBCS(r0, r1, r2);
    }
    else
	SBCS(r0, r1, r2);
}

static void
_subxi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    int			i;
    jit_int32_t		reg;
    int			no_set_flags;
    if (jit_thumb_p()) {
	no_set_flags = jit_no_set_flags();
	jit_no_set_flags() = 1;
	if ((i = encode_arm_immediate(i0)) != -1)
	    T2_SBCSI(r0, r1, i);
	else if ((i = encode_arm_immediate(-i0)) != -1)
	    T2_ADCSI(r0, r1, i);
	else if (r0 != r1) {
	    movi(r0, i0);
	    T2_SBCS(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    SBCS(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
	jit_no_set_flags() = no_set_flags;
    }
    else {
	if ((i = encode_arm_immediate(i0)) != -1)
	    SBCSI(r0, r1, i);
	else if ((i = encode_arm_immediate(-i0)) != -1)
	    ADCSI(r0, r1, i);
	else if (r0 != r1) {
	    movi(r0, i0);
	    SBCS(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    SBCS(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

static void
_rsbi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    subi(r0, r1, i0);
    negr(r0, r0);
}

static void
_mulr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if (!jit_no_set_flags() && r0 == r2 && (r0|r1) < 8)
	    T1_MUL(r0, r1);
	else if (!jit_no_set_flags() && r0 == r1 && (r0|r2) < 8)
	    T1_MUL(r0, r2);
	else
	    T2_MUL(r0, r1, r2);
    }
    else {
	if (r0 == r1 && !jit_armv6_p()) {
	    if (r0 != r2)
		MUL(r0, r2, r1);
	    else {
		reg = jit_get_reg(jit_class_gpr);
		MOV(rn(reg), r1);
		MUL(r0, rn(reg), r2);
		jit_unget_reg(reg);
	    }
	}
	else
	    MUL(r0, r1, r2);
    }
}

static void
_muli(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    mulr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_iqmulr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	jit_int32_t r2, jit_int32_t r3, jit_bool_t sign)
{
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if (r2 == r3) {
	    reg = jit_get_reg(jit_class_gpr);
	    movr(rn(reg), r2);
	    if (sign)
		T2_SMULL(r0, r1, rn(reg), r2);
	    else
		T2_UMULL(r0, r1, rn(reg), r2);
	    jit_unget_reg(reg);
	}
	else if (r0 != r2 && r1 != r2) {
	    if (sign)
		T2_SMULL(r0, r1, r2, r3);
	    else
		T2_UMULL(r0, r1, r2, r3);
	}
	else {
	    if (sign)
		T2_SMULL(r0, r1, r3, r2);
	    else
		T2_UMULL(r0, r1, r3, r2);
	}
    }
    else {
	if (r2 == r3) {
	    reg = jit_get_reg(jit_class_gpr);
	    movr(rn(reg), r2);
	    if (sign)
		SMULL(r0, r1, rn(reg), r2);
	    else
		UMULL(r0, r1, rn(reg), r2);
	    jit_unget_reg(reg);
	}
	else if (r0 != r2 && r1 != r2) {
	    if (sign)
		SMULL(r0, r1, r2, r3);
	    else
		UMULL(r0, r1, r2, r3);
	}
	else {
	    if (sign)
		SMULL(r0, r1, r3, r2);
	    else
		UMULL(r0, r1, r3, r2);
	}
    }
}

static void
_iqmuli(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	jit_int32_t r2, jit_word_t i0, jit_bool_t sign)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    iqmulr(r0, r1, r2, rn(reg), sign);
    jit_unget_reg(reg);
}

static void
_divrem(jit_state_t *_jit, int div, int sign,
	jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_word_t		d;
    jit_word_t		w;
    jit_get_reg_args();
    movr(_R0_REGNO, r1);
    movr(_R1_REGNO, r2);
    if (sign)			w = (jit_word_t)__aeabi_idivmod;
    else			w = (jit_word_t)__aeabi_uidivmod;
    if (!jit_exchange_p()) {
	if (jit_thumb_p())	d = ((w - _jit->pc.w) >> 1) - 2;
	else			d = ((w - _jit->pc.w) >> 2) - 2;
	if (_s24P(d)) {
	    if (jit_thumb_p())	T2_BLI(encode_thumb_jump(d));
	    else		BLI(d & 0x00ffffff);
	}
	else			goto fallback;
    }
    else {
    fallback:
	movi(_R2_REGNO, w);
	if (jit_thumb_p())	T1_BLX(_R2_REGNO);
	else			BLX(_R2_REGNO);
    }
    if (div)			movr(r0, _R0_REGNO);
    else			movr(r0, _R1_REGNO);
    jit_unget_reg_args();
}

static void
_divr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_armv7r_p() && jit_thumb_p())
	T2_SDIV(r0, r1, r2);
    else
	divrem(1, 1, r0, r1, r2);
}

static void
_divi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    divr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_divr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_armv7r_p() && jit_thumb_p())
	T2_UDIV(r0, r1, r2);
    else
	divrem(1, 0, r0, r1, r2);
}

static void
_divi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    divr_u(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_iqdivr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	jit_int32_t r2, jit_int32_t r3,	jit_bool_t sign)
{
    jit_word_t		d;
    jit_word_t		w;
    jit_get_reg_args();
    movr(_R0_REGNO, r2);
    movr(_R1_REGNO, r3);
    if (sign)			w = (jit_word_t)__aeabi_idivmod;
    else			w = (jit_word_t)__aeabi_uidivmod;
    if (!jit_exchange_p()) {
	if (jit_thumb_p())	d = ((w - _jit->pc.w) >> 1) - 2;
	else			d = ((w - _jit->pc.w) >> 2) - 2;
	if (_s24P(d)) {
	    if (jit_thumb_p())	T2_BLI(encode_thumb_jump(d));
	    else		BLI(d & 0x00ffffff);
	}
	else			goto fallback;
    }
    else {
    fallback:
	movi(_R2_REGNO, w);
	if (jit_thumb_p())	T1_BLX(_R2_REGNO);
	else			BLX(_R2_REGNO);
    }
    movr(r0, _R0_REGNO);
    movr(r1, _R1_REGNO);
    jit_unget_reg_args();
}

static void
_iqdivi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1,
	jit_int32_t r2, jit_word_t i0, jit_bool_t sign)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    iqdivr(r0, r1, r2, rn(reg), sign);
    jit_unget_reg(reg);
}

static void
_remr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    divrem(0, 1, r0, r1, r2);
}

static void
_remi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    remr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_remr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    divrem(0, 0, r0, r1, r2);
}

static void
_remi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    remr_u(r0, r1,rn(reg));
    jit_unget_reg(reg);
}

static void
_andr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_thumb_p()) {
	if (!jit_no_set_flags() && (r0|r1|r2) < 8 && (r0 == r1 || r0 == r2))
	    T1_AND(r0, r0 == r1 ? r2 : r1);
	else
	    T2_AND(r0, r1, r2);
    }
    else
	AND(r0, r1, r2);
}

static void
_andi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    int			i;
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if ((i = encode_thumb_immediate(i0)) != -1)
	    T2_ANDI(r0, r1, i);
	else if ((i = encode_thumb_immediate(~i0)) != -1)
	    T2_BICI(r0, r1, i);
	else if (r0 != r1) {
	    movi(r0, i0);
	    T2_AND(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    T2_AND(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	if ((i = encode_arm_immediate(i0)) != -1)
	    ANDI(r0, r1, i);
	else if ((i = encode_arm_immediate(~i0)) != -1)
	    BICI(r0, r1, i);
	else if (r0 != r1) {
	    movi(r0, i0);
	    AND(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    AND(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

static void
_orr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_thumb_p()) {
	if (!jit_no_set_flags() && (r0|r1|r2) < 8 && (r0 == r1 || r0 == r2))
	    T1_ORR(r0, r0 == r1 ? r2 : r1);
	else
	    T2_ORR(r0, r1, r2);
    }
    else
	ORR(r0, r1, r2);
}

static void
_ori(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    int			i;
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if ((i = encode_thumb_immediate(i0)) != -1)
	    T2_ORRI(r0, r1, i);
	else if (r0 != r1) {
	    movi(r0, i0);
	    T2_ORR(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    T2_ORR(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	if ((i = encode_arm_immediate(i0)) != -1)
	    ORRI(r0, r1, i);
	else if (r0 != r1) {
	    movi(r0, i0);
	    ORR(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    ORR(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

static void
_xorr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_thumb_p()) {
	if (!jit_no_set_flags() && (r0|r1|r2) < 8 && (r0 == r1 || r0 == r2))
	    T1_EOR(r0, r0 == r1 ? r2 : r1);
	else
	    T2_EOR(r0, r1, r2);
    }
    else
	EOR(r0, r1, r2);
}

static void
_xori(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    int			i;
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if ((i = encode_thumb_immediate(i0)) != -1)
	    T2_EORI(r0, r1, i);
	else if (r0 != r1) {
	    movi(r0, i0);
	    T2_EOR(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    T2_EOR(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	if ((i = encode_arm_immediate(i0)) != -1)
	    EORI(r0, r1, i);
	else if (r0 != r1) {
	    movi(r0, i0);
	    EOR(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    EOR(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

static void
_lshr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_thumb_p()) {
	if (!jit_no_set_flags() && (r0|r1|r2) < 8 && r0 == r1)
	    T1_LSL(r0, r2);
	else
	    T2_LSL(r0, r1, r2);
    }
    else
	LSL(r0, r1, r2);
}

static void
_lshi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    assert(i0 >= 0 && i0 <= 31);
    if (i0 == 0)
	movr(r0, r1);
    else if (jit_thumb_p()) {
	if (!jit_no_set_flags() && (r0|r1) < 8)
	    T1_LSLI(r0, r1, i0);
	else
	    T2_LSLI(r0, r1, i0);
    }
    else
	LSLI(r0, r1, i0);
}

static void
_rshr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_thumb_p()) {
	if (!jit_no_set_flags() && (r0|r1|r2) < 8 && r0 == r1)
	    T1_ASR(r0, r2);
	else
	    T2_ASR(r0, r1, r2);
    }
    else
	ASR(r0, r1, r2);
}

static void
_rshi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    assert(i0 >= 0 && i0 <= 31);
    if (i0 == 0)
	movr(r0, r1);
    else if (jit_thumb_p()) {
	if (!jit_no_set_flags() && (r0|r1) < 8)
	    T1_ASRI(r0, r1, i0);
	else
	    T2_ASRI(r0, r1, i0);
    }
    else
	ASRI(r0, r1, i0);
}

static void
_rshr_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_thumb_p()) {
	if (!jit_no_set_flags() && (r0|r1|r2) < 8 && r0 == r1)
	    T1_LSR(r0, r2);
	else
	    T2_LSR(r0, r1, r2);
    }
    else
	LSR(r0, r1, r2);
}

static void
_rshi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    assert(i0 >= 0 && i0 <= 31);
    if (i0 == 0)
	movr(r0, r1);
    else if (jit_thumb_p()) {
	if (!jit_no_set_flags() && (r0|r1) < 8)
	    T1_LSRI(r0, r1, i0);
	else
	    T2_LSRI(r0, r1, i0);
    }
    else
	LSRI(r0, r1, i0);
}

static void
_ccr(jit_state_t *_jit, int ct, int cf,
     jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_thumb_p()) {
	assert((ct ^ cf) >> 28 == 1);
	if ((r1|r2) < 8)
	    T1_CMP(r1, r2);
	else if ((r1&r2) & 8)
	    T1_CMPX(r1, r2);
	else
	    T2_CMP(r1, r2);
	ITE(ct);
	if (r0 < 8) {
	    T1_MOVI(r0, 1);
	    T1_MOVI(r0, 0);
	}
	else {
	    T2_MOVI(r0, 1);
	    T2_MOVI(r0, 0);
	}
    }
    else {
	CMP(r1, r2);
	CC_MOVI(ct, r0, 1);
	CC_MOVI(cf, r0, 0);
    }
}

static void
_cci(jit_state_t *_jit, int ct, int cf,
     jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    int			i;
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if (r1 < 7 && !(i0 & 0xffffff00))
	    T1_CMPI(r1, i0);
	else if ((i = encode_thumb_immediate(i0)) != -1)
	    T2_CMPI(r1, i);
	else if ((i = encode_thumb_immediate(-i0)) != -1)
	    T2_CMNI(r1, i);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    ccr(ct, cf, r0, r1, rn(reg));
	    jit_unget_reg(reg);
	    return;
	}
	ITE(ct);
	if (r0 < 8) {
	    T1_MOVI(r0, 1);
	    T1_MOVI(r0, 0);
	}
	else {
	    T2_MOVI(r0, 1);
	    T2_MOVI(r0, 0);
	}
    }
    else {
	if ((i = encode_arm_immediate(i0)) != -1)
	    CMPI(r1, i);
	else if ((i = encode_arm_immediate(-i0)) != -1)
	    CMNI(r1, i);
	else if (r0 != r1) {
	    movi(r0, i0);
	    CMP(r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    CMP(r1, rn(reg));
	    jit_unget_reg(reg);
	}
	CC_MOVI(ct, r0, 1);
	CC_MOVI(cf, r0, 0);
    }
}

static void
_ner(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_thumb_p())
	ccr(ARM_CC_NE, ARM_CC_EQ, r0, r1, r2);
    else {
	SUBS(r0, r1, r2);
	CC_MOVI(ARM_CC_NE, r0, 1);
    }
}

static void
_nei(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    int			i;
    jit_int32_t		reg;
    if (jit_thumb_p())
	cci(ARM_CC_NE, ARM_CC_EQ, r0, r1, i0);
    else {
	if ((i = encode_arm_immediate(i0)) != -1)
	    SUBSI(r0, r1, i);
	else if ((i = encode_arm_immediate(-i0)) != -1)
	    ADDSI(r0, r1, i);
	else if (r0 != r1) {
	    movi(r0, i0);
	    SUBS(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    SUBS(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
	CC_MOVI(ARM_CC_NE, r0, 1);
    }
}

static void
_jmpr(jit_state_t *_jit, jit_int32_t r0)
{
    if (jit_thumb_p())
	T1_MOV(_R15_REGNO, r0);
    else
	MOV(_R15_REGNO, r0);
}

static void
_jmpi(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    jit_word_t		d;
    jit_int32_t		reg;
    w = _jit->pc.w;
    /* if thumb and in thumb mode */
    if (jit_thumb_p() && _jitc->thumb) {
	d = ((i0 - w) >> 1) - 2;
	if (d >= -1024 && d <= 1023)
	    T1_B(d & 0x7ff);
	else if (_s24P(d))
	    T2_B(encode_thumb_jump(d));
	else {
	    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	    movi(rn(reg), i0);
	    jmpr(rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	d = ((i0 - w) >> 2) - 2;
	if (_s24P(d))
	    B(d & 0x00ffffff);
	else {
	    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	    movi(rn(reg), i0);
	    jmpr(rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

static jit_word_t
_jmpi_p(jit_state_t *_jit, jit_word_t i0, jit_bool_t i1)
{
    jit_word_t		w;
    jit_word_t		d;
    jit_int32_t		reg;
    if (i1) {
	/* Assume jump is not longer than 23 bits if inside jit */
	w = _jit->pc.w;
	/* if thumb and in thumb mode */
	if (jit_thumb_p() && _jitc->thumb) {
	    d = ((i0 - w) >> 1) - 2;
	    assert(_s24P(d));
	    T2_B(encode_thumb_jump(d));
	}
	else {
	    d = ((i0 - w) >> 2) - 2;
	    assert(_s24P(d));
	    B(d & 0x00ffffff);
	}
    }
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	w = movi_p(rn(reg), i0);
	jmpr(rn(reg));
	jit_unget_reg(reg);
    }
    return (w);
}

static jit_word_t
_bccr(jit_state_t *_jit, int cc, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_word_t		d;
    if (jit_thumb_p()) {
	if ((r0|r1) < 8)
	    T1_CMP(r0, r1);
	else if ((r0&r1) & 8)
	    T1_CMPX(r0, r1);
	else
	    T2_CMP(r0, r1);
	/* use only thumb2 conditional as does not know if will be patched */
	w = _jit->pc.w;
	d = ((i0 - w) >> 1) - 2;
	assert(_s20P(d));
	T2_CC_B(cc, encode_thumb_cc_jump(d));
    }
    else {
	CMP(r0, r1);
	w = _jit->pc.w;
	d = ((i0 - w) >> 2) - 2;
	assert(_s24P(d));
	CC_B(cc, d & 0x00ffffff);
    }
    return (w);
}

static jit_word_t
_bcci(jit_state_t *_jit, int cc, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_word_t		d;
    int			i;
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if (r0 < 7 && !(i1 & 0xffffff00))
	    T1_CMPI(r0, i1);
	else if ((i = encode_thumb_immediate(i1)) != -1)
	    T2_CMPI(r0, i);
	else if ((i = encode_thumb_immediate(-i1)) != -1)
	    T2_CMNI(r0, i);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i1);
	    T2_CMP(r0, rn(reg));
	    jit_unget_reg(reg);
	}
	/* use only thumb2 conditional as does not know if will be patched */
	w = _jit->pc.w;
	d = ((i0 - w) >> 1) - 2;
	assert(_s20P(d));
	T2_CC_B(cc, encode_thumb_cc_jump(d));
    }
    else {
	if ((i = encode_arm_immediate(i1)) != -1)
	    CMPI(r0, i);
	else if ((i = encode_arm_immediate(-i1)) != -1)
	    CMNI(r0, i);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i1);
	    CMP(r0, rn(reg));
	    jit_unget_reg(reg);
	}
	w = _jit->pc.w;
	d = ((i0 - w) >> 2) - 2;
	assert(_s24P(d));
	CC_B(cc, d & 0x00ffffff);
    }
    return (w);
}

static jit_word_t
_baddr(jit_state_t *_jit, int cc, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_word_t		d;
    if (jit_thumb_p()) {
	if ((r0|r1) < 8)
	    T1_ADD(r0, r0, r1);
	else
	    T2_ADDS(r0, r0, r1);
	w = _jit->pc.w;
	d = ((i0 - w) >> 1) - 2;
	assert(_s20P(d));
	T2_CC_B(cc, encode_thumb_cc_jump(d));
    }
    else {
	ADDS(r0, r0, r1);
	w = _jit->pc.w;
	d = ((i0 - w) >> 2) - 2;
	assert(_s24P(d));
	CC_B(cc, d & 0x00ffffff);
    }
    return (w);
}

static jit_word_t
_baddi(jit_state_t *_jit, int cc, jit_word_t i0, jit_int32_t r0, int i1)
{
    int			i;
    jit_word_t		w;
    jit_word_t		d;
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if (r0 < 8 && !(i1 & ~7))
	    T1_ADDI3(r0, r0, i1);
	else if (r0 < 8 && !(-i1 & ~7))
	    T1_SUBI3(r0, r0, -i1);
	else if (r0 < 8 && !(i1 & ~0xff))
	    T1_ADDI8(r0, i1);
	else if (r0 < 8 && !(-i1 & ~0xff))
	    T1_SUBI8(r0, -i1);
	else if ((i = encode_thumb_immediate(i1)) != -1)
	    T2_ADDSI(r0, r0, i);
	else if ((i = encode_thumb_immediate(-i1)) != -1)
	    T2_SUBSI(r0, r0, i);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i1);
	    T2_ADDS(r0, r0, rn(reg));
	    jit_unget_reg(reg);
	}
	w = _jit->pc.w;
	d = ((i0 - w) >> 1) - 2;
	assert(_s20P(d));
	T2_CC_B(cc, encode_thumb_cc_jump(d));
    }
    else {
	if ((i = encode_arm_immediate(i1)) != -1)
	    ADDSI(r0, r0, i);
	else if ((i = encode_arm_immediate(-i1)) != -1)
	    SUBSI(r0, r0, i);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i1);
	    ADDS(r0, r0, rn(reg));
	    jit_unget_reg(reg);
	}
	w = _jit->pc.w;
	d = ((i0 - w) >> 2) - 2;
	assert(_s24P(d));
	CC_B(cc, d & 0x00ffffff);
    }
    return (w);
}

static jit_word_t
_bsubr(jit_state_t *_jit, int cc, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_word_t		d;
    if (jit_thumb_p()) {
	if ((r0|r1) < 8)
	    T1_SUB(r0, r0, r1);
	else
	    T2_SUBS(r0, r0, r1);
	w = _jit->pc.w;
	d = ((i0 - w) >> 1) - 2;
	assert(_s20P(d));
	T2_CC_B(cc, encode_thumb_cc_jump(d));
    }
    else {
	SUBS(r0, r0, r1);
	w = _jit->pc.w;
	d = ((i0 - w) >> 2) - 2;
	assert(_s24P(d));
	CC_B(cc, d & 0x00ffffff);
    }
    return (w);
}

static jit_word_t
_bsubi(jit_state_t *_jit, int cc, jit_word_t i0, jit_int32_t r0, int i1)
{
    int			i;
    jit_word_t		w;
    jit_word_t		d;
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if (r0 < 8 && !(i1 & ~7))
	    T1_SUBI3(r0, r0, i1);
	else if (r0 < 8 && !(-i1 & ~7))
	    T1_ADDI3(r0, r0, -i1);
	else if (r0 < 8 && !(i1 & ~0xff))
	    T1_SUBI8(r0, i1);
	else if (r0 < 8 && !(-i1 & ~0xff))
	    T1_ADDI8(r0, -i1);
	else if ((i = encode_thumb_immediate(i1)) != -1)
	    T2_SUBSI(r0, r0, i);
	else if ((i = encode_thumb_immediate(-i1)) != -1)
	    T2_SUBSI(r0, r0, i);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i1);
	    T2_SUBS(r0, r0, rn(reg));
	    jit_unget_reg(reg);
	}
	w = _jit->pc.w;
	d = ((i0 - w) >> 1) - 2;
	assert(_s20P(d));
	T2_CC_B(cc, encode_thumb_cc_jump(d));
    }
    else {
	if ((i = encode_arm_immediate(i1)) != -1)
	    SUBSI(r0, r0, i);
	else if ((i = encode_arm_immediate(-i1)) != -1)
	    ADDSI(r0, r0, i);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i1);
	    SUBS(r0, r0, rn(reg));
	    jit_unget_reg(reg);
	}
	w = _jit->pc.w;
	d = ((i0 - w) >> 2) - 2;
	assert(_s24P(d));
	CC_B(cc, d & 0x00ffffff);
    }
    return (w);
}

static jit_word_t
_bmxr(jit_state_t *_jit, int cc, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_word_t		d;
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if ((r0|r1) < 8)
	    T1_TST(r0, r1);
	else
	    T2_TST(r0, r1);
	w = _jit->pc.w;
	d = ((i0 - w) >> 1) - 2;
	assert(_s20P(d));
	T2_CC_B(cc, encode_thumb_cc_jump(d));
    }
    else {
	if (jit_armv5_p())
	    TST(r0, r1);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    ANDS(rn(reg), r0, r1);
	    jit_unget_reg(reg);
	}
	w = _jit->pc.w;
	d = ((i0 - w) >> 2) - 2;
	assert(_s24P(d));
	CC_B(cc, d & 0x00ffffff);
    }
    return (w);
}

static jit_word_t
_bmxi(jit_state_t *_jit, int cc, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    int			i;
    jit_word_t		w;
    jit_word_t		d;
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if ((i = encode_thumb_immediate(i1)) != -1)
	    T2_TSTI(r0, i);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i1);
	    T2_TST(r0, rn(reg));
	    jit_unget_reg(reg);
	}
	w = _jit->pc.w;
	d = ((i0 - w) >> 1) - 2;
	assert(_s20P(d));
	T2_CC_B(cc, encode_thumb_cc_jump(d));
    }
    else {
	if (jit_armv5_p()) {
	    if ((i = encode_arm_immediate(i1)) != -1)
		TSTI(r0, i);
	    else {
		reg = jit_get_reg(jit_class_gpr);
		movi(rn(reg), i1);
		TST(r0, rn(reg));
		jit_unget_reg(reg);
	    }
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    if ((i = encode_arm_immediate(i1)) != -1)
		ANDSI(rn(reg), r0, i);
	    else if ((i = encode_arm_immediate(~i1)) != -1)
		BICSI(rn(reg), r0, i);
	    else {
		movi(rn(reg), i1);
		ANDS(rn(reg), r0, rn(reg));
	    }
	    jit_unget_reg(reg);
	}
	w = _jit->pc.w;
	d = ((i0 - w) >> 2) - 2;
	assert(_s24P(d));
	CC_B(cc, d & 0x00ffffff);
    }
    return (w);
}

static void
_ldr_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_thumb_p())
	T2_LDRSBI(r0, r1, 0);
    else
	LDRSBI(r0, r1, 0);
}

static void
_ldi_c(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    if (jit_thumb_p())
	T2_LDRSBI(r0, rn(reg), 0);
    else
	LDRSBI(r0, rn(reg), 0);
    jit_unget_reg(reg);
}

static void
_ldxr_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_thumb_p()) {
	if ((r0|r1|r2) < 8)
	    T1_LDRSB(r0, r1, r2);
	else
	    T2_LDRSB(r0, r1, r2);
    }
    else
	LDRSB(r0, r1, r2);
}

static void
_ldxi_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if (jit_ldrt_strt_p() && i0 >= 0 && i0 <= 255)
	    T2_LDRSBI(r0, r1, i0);
	else if (i0 < 0 && i0 >= -255)
	    T2_LDRSBIN(r0, r1, -i0);
	else if (i0 >= 0 && i0 <= 4095)
	    T2_LDRSBWI(r0, r1, i0);
	else if (r0 != r1) {
	    movi(r0, i0);
	    if ((r0|r1) < 8)
		T1_LDRSB(r0, r1, r0);
	    else
		T2_LDRSB(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    if ((r0|r1|rn(reg)) < 8)
		T1_LDRSB(r0, r1, rn(reg));
	    else
		T2_LDRSB(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	if (i0 >= 0 && i0 <= 255)
	    LDRSBI(r0, r1, i0);
	else if (i0 < 0 && i0 >= -255)
	    LDRSBIN(r0, r1, -i0);
	else if (r0 != r1) {
	    movi(r0, i0);
	    LDRSB(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    LDRSB(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

static void
_ldr_uc(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_thumb_p())
	T2_LDRBI(r0, r1, 0);
    else
	LDRBI(r0, r1, 0);
}

static void
_ldi_uc(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    if (jit_thumb_p())
	T2_LDRBI(r0, rn(reg), 0);
    else
	LDRBI(r0, rn(reg), 0);
    jit_unget_reg(reg);
}

static void
_ldxr_uc(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_thumb_p()) {
	if ((r0|r1|r2) < 8)
	    T1_LDRB(r0, r1, r2);
	else
	    T2_LDRB(r0, r1, r2);
    }
    else
	LDRB(r0, r1, r2);
}

static void
_ldxi_uc(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if ((r0|r1) < 8 && i0 >= 0 && i0 < 0x20)
	    T1_LDRBI(r0, r1, i0);
	else if (jit_ldrt_strt_p() && i0 >= 0 && i0 <= 255)
	    T2_LDRBI(r0, r1, i0);
	else if (i0 < 0 && i0 >= -255)
	    T2_LDRBIN(r0, r1, -i0);
	else if (i0 >= 0 && i0 <= 4095)
	    T2_LDRBWI(r0, r1, i0);
	else if (r0 != r1) {
	    movi(r0, i0);
	    if ((r0|r1) < 8)
		T1_LDRB(r0, r1, r0);
	    else
		T2_LDRB(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    if ((r0|r1|rn(reg)) < 8)
		T1_LDRB(r0, r1, rn(reg));
	    else
		T2_LDRB(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	if (i0 >= 0 && i0 <= 4095)
	    LDRBI(r0, r1, i0);
	else if (i0 < 0 && i0 >= -4095)
	    LDRBIN(r0, r1, -i0);
	else if (r0 != r1) {
	    movi(r0, i0);
	    LDRB(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    LDRB(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

static void
_ldr_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_thumb_p())
	T2_LDRSHI(r0, r1, 0);
    else
	LDRSHI(r0, r1, 0);
}

static void
_ldi_s(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    if (jit_thumb_p())
	T2_LDRSHI(r0, rn(reg), 0);
    else
	LDRSHI(r0, rn(reg), 0);
    jit_unget_reg(reg);
}

static void
_ldxr_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_thumb_p()) {
	if ((r0|r1|r2) < 8)
	    T1_LDRSH(r0, r1, r2);
	else
	    T2_LDRSH(r0, r1, r2);
    }
    else
	LDRSH(r0, r1, r2);
}

static void
_ldxi_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if (jit_ldrt_strt_p() && i0 >= 0 && i0 <= 255)
	    T2_LDRSHI(r0, r1, i0);
	else if (i0 < 0 && i0 >= -255)
	    T2_LDRSHIN(r0, r1, -i0);
	else if (i0 >= 0 && i0 <= 4095)
	    T2_LDRSHWI(r0, r1, i0);
	else if (r0 != r1) {
	    movi(r0, i0);
	    if ((r0|r1) < 8)
		T1_LDRSH(r0, r1, r0);
	    else
		T2_LDRSH(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    if ((r0|r1|rn(reg)) < 8)
		T1_LDRSH(r0, r1, rn(reg));
	    else
		T2_LDRSH(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	if (i0 >= 0 && i0 <= 255)
	    LDRSHI(r0, r1, i0);
	else if (i0 < 0 && i0 >= -255)
	    LDRSHIN(r0, r1, -i0);
	else if (r0 != r1) {
	    movi(r0, i0);
	    LDRSH(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    LDRSH(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

static void
_ldr_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_thumb_p())
	T2_LDRHI(r0, r1, 0);
    else
	LDRHI(r0, r1, 0);
}

static void
_ldi_us(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    if (jit_thumb_p())
	T2_LDRHI(r0, rn(reg), 0);
    else
	LDRHI(r0, rn(reg), 0);
    jit_unget_reg(reg);
}

static void
_ldxr_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_thumb_p()) {
	if ((r0|r1|r2) < 8)
	    T1_LDRH(r0, r1, r2);
	else
	    T2_LDRH(r0, r1, r2);
    }
    else
	LDRH(r0, r1, r2);
}

static void
_ldxi_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if ((r0|r1) < 8 && i0 >= 0 && !(i0 & 1) && (i0 >> 1) < 0x20)
	    T1_LDRHI(r0, r1, i0 >> 1);
	else if (jit_ldrt_strt_p() && i0 >= 0 && i0 <= 255)
	    T2_LDRHI(r0, r1, i0);
	else if (i0 < 0 && i0 >= -255)
	    T2_LDRHIN(r0, r1, -i0);
	else if (i0 >= 0 && i0 <= 4095)
	    T2_LDRHWI(r0, r1, i0);
	else if (r0 != r1) {
	    movi(r0, i0);
	    if ((r0|r1) < 8)
		T1_LDRH(r0, r1, r0);
	    else
		T2_LDRH(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    if ((r0|r1|rn(reg)) < 8)
		T1_LDRH(r0, r1, rn(reg));
	    else
		T2_LDRH(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	if (i0 >= 0 && i0 <= 255)
	    LDRHI(r0, r1, i0);
	else if (i0 < 0 && i0 >= -255)
	    LDRHIN(r0, r1, -i0);
	else if (r0 != r1) {
	    movi(r0, i0);
	    LDRH(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    LDRH(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

static void
_ldr_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_thumb_p())
	T2_LDRI(r0, r1, 0);
    else
	LDRI(r0, r1, 0);
}

static void
_ldi_i(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    if (jit_thumb_p())
	T2_LDRI(r0, rn(reg), 0);
    else
	LDRI(r0, rn(reg), 0);
    jit_unget_reg(reg);
}

static void
_ldxr_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_thumb_p()) {
	if ((r0|r1|r2) < 8)
	    T1_LDR(r0, r1, r2);
	else
	    T2_LDR(r0, r1, r2);
    }
    else
	LDR(r0, r1, r2);
}

static void
_ldxi_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if ((r0|r1) < 8 && i0 >= 0 && !(i0 & 3) && (i0 >> 2) < 0x20)
	    T1_LDRI(r0, r1, i0 >> 2);
	else if (r1 == _R13_REGNO && r0 < 8 &&
		 i0 >= 0 && !(i0 & 3) && (i0 >> 2) <= 255)
	    T1_LDRISP(r0, i0 >> 2);
	else if (jit_ldrt_strt_p() && i0 >= 0 && i0 <= 255)
	    T2_LDRI(r0, r1, i0);
	else if (i0 < 0 && i0 > -255)
	    T2_LDRIN(r0, r1, -i0);
	else if (i0 >= 0 && i0 <= 4095)
	    T2_LDRWI(r0, r1, i0);
	else if (r0 != r1) {
	    movi(r0, i0);
	    if ((r0|r1) < 8)
		T1_LDR(r0, r1, r0);
	    else
		T2_LDR(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    if ((r0|r1|rn(reg)) < 8)
		T1_LDR(r0, r1, rn(reg));
	    else
		T2_LDR(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	if (i0 >= 0 && i0 <= 4095)
	    LDRI(r0, r1, i0);
	else if (i0 < 0 && i0 >= -4095)
	    LDRIN(r0, r1, -i0);
	else if (r0 != r1) {
	    movi(r0, i0);
	    LDR(r0, r1, r0);
	}
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    LDR(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

static void
_str_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_thumb_p())
	T2_STRBI(r1, r0, 0);
    else
	STRBI(r1, r0, 0);
}

static void
_sti_c(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    if (jit_thumb_p())
	T2_STRBI(r0, rn(reg), 0);
    else
	STRBI(r0, rn(reg), 0);
    jit_unget_reg(reg);
}

static void
_stxr_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_thumb_p()) {
	if ((r0|r1|r2) < 8)
	    T1_STRB(r2, r1, r0);
	else
	    T2_STRB(r2, r1, r0);
    }
    else
	STRB(r2, r1, r0);
}

static void
_stxi_c(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if ((r0|r1) < 8 && i0 >= 0 && i0 < 0x20)
	    T1_STRBI(r1, r0, i0);
	else if (jit_ldrt_strt_p() && i0 >= 0 && i0 <= 255)
	    T2_STRBI(r1, r0, i0);
	else if (i0 < 0 && i0 >= -255)
	    T2_STRBIN(r1, r0, -i0);
	else if (i0 >= 0 && i0 <= 4095)
	    T2_STRBWI(r1, r0, i0);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    if ((r0|r1|rn(reg)) < 8)
		T1_STRB(r1, r0, rn(reg));
	    else
		T2_STRB(r1, r0, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	if (i0 >= 0 && i0 <= 4095)
	    STRBI(r1, r0, i0);
	else if (i0 < 0 && i0 >= -4095)
	    STRBIN(r1, r0, -i0);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    STRB(r1, r0, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

static void
_str_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_thumb_p())
	T2_STRHI(r1, r0, 0);
    else
	STRHI(r1, r0, 0);
}

static void
_sti_s(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    if (jit_thumb_p())
	T2_STRHI(r0, rn(reg), 0);
    else
	STRHI(r0, rn(reg), 0);
    jit_unget_reg(reg);
}

static void
_stxr_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_thumb_p()) {
	if ((r0|r1|r2) < 8)
	    T1_STRH(r2, r1, r0);
	else
	    T2_STRH(r2, r1, r0);
    }
    else
	STRH(r2, r1, r0);
}

static void
_stxi_s(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if ((r0|r1) < 8 && i0 >= 0 && !(i0 & 1) && (i0 >> 1) < 0x20)
	    T1_STRHI(r1, r0, i0 >> 1);
	else if (jit_ldrt_strt_p() && i0 >= 0 && i0 <= 255)
	    T2_STRHI(r1, r0, i0);
	else if (i0 < 0 && i0 >= -255)
	    T2_STRHIN(r1, r0, -i0);
	else if (i0 >= 0 && i0 <= 4095)
	    T2_STRHWI(r1, r0, i0);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    if ((r0|r1|rn(reg)) < 8)
		T1_STRH(r1, r0, rn(reg));
	    else
		T2_STRH(r1, r0, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	if (i0 >= 0 && i0 <= 255)
	    STRHI(r1, r0, i0);
	else if (i0 < 0 && i0 >= -255)
	    STRHIN(r1, r0, -i0);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    STRH(r1, r0, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

static void
_str_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_thumb_p())
	T2_STRI(r1, r0, 0);
    else
	STRI(r1, r0, 0);
}

static void
_sti_i(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    if (jit_thumb_p())
	T2_STRI(r0, rn(reg), 0);
    else
	STRI(r0, rn(reg), 0);
    jit_unget_reg(reg);
}

static void
_stxr_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    if (jit_thumb_p()) {
	if ((r0|r1|r2) < 8)
	    T1_STR(r2, r1, r0);
	else
	    T2_STR(r2, r1, r0);
    }
    else
	STR(r2, r1, r0);
}

static void
_stxi_i(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if ((r0|r1) < 8 && i0 >= 0 && !(i0 & 3) && (i0 >> 2) < 0x20)
	    T1_STRI(r1, r0, i0 >> 2);
	else if (r0 == _R13_REGNO && r1 < 8 &&
		 i0 >= 0 && !(i0 & 3) && (i0 >> 2) <= 255)
	    T1_STRISP(r1, i0 >> 2);
	else if (jit_ldrt_strt_p() && i0 >= 0 && i0 <= 255)
	    T2_STRI(r1, r0, i0);
	else if (i0 < 0 && i0 >= -255)
	    T2_STRIN(r1, r0, -i0);
	else if (i0 >= 0 && i0 <= 4095)
	    T2_STRWI(r1, r0, i0);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    if ((r0|r1|rn(reg)) < 8)
		T1_STR(r1, r0, rn(reg));
	    else
		T2_STR(r1, r0, rn(reg));
	    jit_unget_reg(reg);
	}
    }
    else {
	if (i0 >= 0 && i0 <= 4095)
	    STRI(r1, r0, i0);
	else if (i0 < 0 && i0 >= -4095)
	    STRIN(r1, r0, -i0);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    STR(r1, r0, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

#  if __BYTE_ORDER == __LITTLE_ENDIAN
static void
_htonr_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0;
    if (jit_thumb_p()) {
	if ((r0|r1) < 8)
	    T1_REV(r0, r1);
	else
	    T2_REV(r0, r1);
	rshi_u(r0, r0, 16);
    }
    else {
	if (jit_armv6_p()) {
	    REV(r0, r1);
	    rshi_u(r0, r0, 16);
	}
	else {
	    t0 = jit_get_reg(jit_class_gpr);
	    rshi(rn(t0), r1, 8);
	    andi(r0, r1, 0xff);
	    andi(rn(t0), rn(t0), 0xff);
	    lshi(r0, r0, 8);
	    orr(r0, r0, rn(t0));
	    jit_unget_reg(t0);
	}
    }
}

/* inline glibc htonl (without register clobber) */
static void
_htonr_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (jit_thumb_p()) {
	if ((r0|r1) < 8)
	    T1_REV(r0, r1);
	else
	    T2_REV(r0, r1);
    }
    else {
	if (jit_armv6_p())
	    REV(r0, r1);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    EOR_SI(rn(reg), r1, r1, ARM_ROR, 16);
	    LSRI(rn(reg), rn(reg), 8);
	    BICI(rn(reg), rn(reg), encode_arm_immediate(0xff00));
	    EOR_SI(r0, rn(reg), r1, ARM_ROR, 8);
	    jit_unget_reg(reg);
	}
    }
}
#endif

static void
_extr_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_thumb_p()) {
	if ((r0|r1) < 8)
	    T1_SXTB(r0, r1);
	else
	    T2_SXTB(r0, r1);
    }
    else {
	if (jit_armv6_p())
	    SXTB(r0, r1);
	else {
	    LSLI(r0, r1, 24);
	    ASRI(r0, r0, 24);
	}
    }
}

static void
_extr_uc(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_thumb_p()) {
	if ((r0|r1) < 8)
	    T1_UXTB(r0, r1);
	else
	    T2_UXTB(r0, r1);
    }
    else {
	if (jit_armv6_p())
	    UXTB(r0, r1);
	else
	    ANDI(r0, r1, 0xff);
    }
}

static void
_extr_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_thumb_p()) {
	if ((r0|r1) < 8)
	    T1_SXTH(r0, r1);
	else
	    T2_SXTH(r0, r1);
    }
    else {
	if (jit_armv6_p())
	    SXTH(r0, r1);
	else {
	    LSLI(r0, r1, 16);
	    ASRI(r0, r0, 16);
	}
    }
}

static void
_extr_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (jit_thumb_p()) {
	if ((r0|r1) < 8)
	    T1_UXTH(r0, r1);
	else
	    T2_UXTH(r0, r1);
    }
    else {
	if (jit_armv6_p())
	    UXTH(r0, r1);
	else {
	    LSLI(r0, r1, 16);
	    LSRI(r0, r0, 16);
	}
    }
}

static void
_callr(jit_state_t *_jit, jit_int32_t r0)
{
    if (jit_thumb_p())
	T1_BLX(r0);
    else
	BLX(r0);
}

static void
_calli(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		d;
    jit_int32_t		reg;
    d = ((i0 - _jit->pc.w) >> 2) - 2;
    if (!jit_exchange_p() && !jit_thumb_p() && _s24P(d))
	BLI(d & 0x00ffffff);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	if (jit_thumb_p())
	    T1_BLX(rn(reg));
	else
	    BLX(rn(reg));
	jit_unget_reg(reg);
    }
}

static jit_word_t
_calli_p(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    w = _jit->pc.w;
    movi_p(rn(reg), i0);
    if (jit_thumb_p())
	T1_BLX(rn(reg));
    else
	BLX(rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static void
_prolog(jit_state_t *_jit, jit_node_t *node)
{
    jit_int32_t		reg;
    if (_jitc->function->define_frame || _jitc->function->assume_frame) {
	jit_int32_t	frame = -_jitc->function->frame;
	assert(_jitc->function->self.aoff >= frame);
	if (_jitc->function->assume_frame) {
	    if (jit_thumb_p() && !_jitc->thumb)
		_jitc->thumb = _jit->pc.w;
	    return;
	}
	_jitc->function->self.aoff = frame;
    }
    if (_jitc->function->allocar)
	_jitc->function->self.aoff &= -8;
    _jitc->function->stack = ((_jitc->function->self.alen -
			      /* align stack at 8 bytes */
			      _jitc->function->self.aoff) + 7) & -8;

    if (jit_thumb_p()) {
	/*  switch to thumb mode (better approach would be to
	 * ORR 1 address being called, but no clear distinction
	 * of what is a pointer to a jit function, or if patching
	 * a pointer to a jit function) */
	ADDI(_R12_REGNO, _R15_REGNO, 1);
	BX(_R12_REGNO);
	if (!_jitc->thumb)
	    _jitc->thumb = _jit->pc.w;
	if (jit_cpu.abi) {
	    T2_PUSH(0xf);
	    T2_PUSH(0x3f0|(1<<_FP_REGNO)|(1<<_LR_REGNO));
	    VPUSH_F64(_D8_REGNO, 8);
	}
	else {
	    T2_PUSH(0xf);
	    T2_PUSH(0x3f0|(1<<_FP_REGNO)|(1<<_LR_REGNO));
	}
    }
    else {
	if (jit_cpu.abi) {
	    PUSH(0xf);
	    PUSH(0x3f0|(1<<_FP_REGNO)|(1<<_LR_REGNO));
	    VPUSH_F64(_D8_REGNO, 8);
	}
	else {
	    PUSH(0xf);
	    PUSH(0x3f0|(1<<_FP_REGNO)|(1<<_LR_REGNO));
	}
    }
    movr(_FP_REGNO, _SP_REGNO);
    if (_jitc->function->stack)
	subi(_SP_REGNO, _SP_REGNO, _jitc->function->stack);
    if (_jitc->function->allocar) {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), _jitc->function->self.aoff);
	stxi_i(_jitc->function->aoffoff, _FP_REGNO, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_epilog(jit_state_t *_jit, jit_node_t *node)
{
    if (_jitc->function->assume_frame)
	return;

    movr(_SP_REGNO, _FP_REGNO);
    if (jit_cpu.abi)
	VPOP_F64(_D8_REGNO, 8);
    if (jit_thumb_p())
	T2_POP(0x3f0|(1<<_FP_REGNO)|(1<<_LR_REGNO));
    else
	POP(0x3f0|(1<<_FP_REGNO)|(1<<_LR_REGNO));
    addi(_SP_REGNO, _SP_REGNO, 16);
    if (jit_thumb_p())
	T1_BX(_LR_REGNO);
    else
	BX(_LR_REGNO);
    if (jit_thumb_p() && (_jit->pc.w & 2))
	T1_NOP();
}

static void
_vastart(jit_state_t *_jit, jit_int32_t r0)
{
    assert(_jitc->function->self.call & jit_call_varargs);

    /* Initialize stack pointer to the first stack argument.
     * The -16 is to account for the 4 argument registers
     * always saved, and _jitc->function->vagp is to account
     * for declared arguments. */
    addi(r0, _FP_REGNO, _jitc->function->self.size -
	 16 + _jitc->function->vagp);
}

static void
_vaarg(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    assert(_jitc->function->self.call & jit_call_varargs);

    /* Load argument. */
    ldr(r0, r1);

    /* Update stack pointer. */
    addi(r1, r1, sizeof(jit_word_t));
}

static void
_patch_at(jit_state_t *_jit,
	  jit_int32_t kind, jit_word_t instr, jit_word_t label)
{
    jit_word_t		 d;
    jit_thumb_t		 thumb;
    union {
	jit_int16_t	*s;
	jit_int32_t	*i;
	jit_word_t	 w;
    } u;
    u.w = instr;
    if (kind == arm_patch_jump) {
	if (jit_thumb_p() && (jit_uword_t)instr >= _jitc->thumb) {
	    code2thumb(thumb.s[0], thumb.s[1], u.s[0], u.s[1]);
	    if ((thumb.i & THUMB2_B) == THUMB2_B) {
		d = ((label - instr) >> 1) - 2;
		assert(_s24P(d));
		thumb.i = THUMB2_B | encode_thumb_jump(d);
		thumb2code(thumb.s[0], thumb.s[1], u.s[0], u.s[1]);
	    }
	    else if ((thumb.i & THUMB2_B) == THUMB2_CC_B) {
		d = ((label - instr) >> 1) - 2;
		assert(_s20P(d));
		thumb.i = THUMB2_CC_B | (thumb.i & 0x3c00000) |
			  encode_thumb_cc_jump(d);
		thumb2code(thumb.s[0], thumb.s[1], u.s[0], u.s[1]);
	    }
	    else {
		/* for the sake of simplicity in case choose to
		 * movw+movt+[bx|blx], e.g. if changing to instead
		 * of asserting target is reachable, load constant
		 * and do indirect jump if not reachable */
		if ((thumb.i & 0xfbf00000) == THUMB2_MOVWI)
		    goto indirect_jump;
		assert(!"handled branch opcode");
	    }
	}
	else {
	    thumb.i = u.i[0];
	    /* 0x0e000000 because 0x01000000 is (branch&) link modifier */
	    assert((thumb.i & 0x0e000000) == ARM_B);
	    d = ((label - instr) >> 2) - 2;
	    assert(_s24P(d));
	    u.i[0] = (thumb.i & 0xff000000) | (d & 0x00ffffff);
	}
    }
    else if (kind == arm_patch_load) {
	/* offset may be negative for a forward patch because it
	 * is relative to pc + 8, for example:
	 *	    ldr r0, [pc, #-4]
	 *	    bx r0		;; [pc, #-8]
	 *	    .data ...		;; [pc, #-4]
	 *	    ... 		;; [pc]
	 */
	assert(!jit_thumb_p());
	thumb.i = u.i[0];
	assert((thumb.i & 0x0f700000) == ARM_LDRI);
	d = label - (instr + 8);
	if (d < 0) {
	    thumb.i &= ~ARM_P;
	    d = -d;
	}
	else
	    thumb.i |= ARM_P;
	assert(!(d & 0xfffff000));
	u.i[0] = (thumb.i & 0xfffff000) | d;
    }
    else if (kind == arm_patch_word) {
	if (jit_thumb_p()) {
	    code2thumb(thumb.s[0], thumb.s[1], u.s[0], u.s[1]);
	    assert((thumb.i & 0xfbf00000) == THUMB2_MOVWI);
	indirect_jump:
	    thumb.i = ((thumb.i & 0xfbf00f00) |
		       ( (label & 0x0000f000) <<  4) |
		       ( (label & 0x00000800) << 15) |
		       ( (label & 0x00000700) <<  4) |
		       (  label & 0x000000ff));
	    thumb2code(thumb.s[0], thumb.s[1], u.s[0], u.s[1]);
	    label >>= 16;
	    code2thumb(thumb.s[0], thumb.s[1], u.s[2], u.s[3]);
	    assert((thumb.i & 0xfbf00000) == THUMB2_MOVTI);
	    thumb.i = ((thumb.i & 0xfbf00f00) |
		       ( (label & 0x0000f000) <<  4) |
		       ( (label & 0x00000800) << 15) |
		       ( (label & 0x00000700) <<  4) |
		       (  label & 0x000000ff));
	    thumb2code(thumb.s[0], thumb.s[1], u.s[2], u.s[3]);
	}
	else
	    u.i[0] = label;
    }
    else
	assert(!"handled patch");
}
#endif
