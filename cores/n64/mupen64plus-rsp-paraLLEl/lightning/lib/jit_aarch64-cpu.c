/*
 * Copyright (C) 2013-2019  Free Software Foundation, Inc.
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
typedef union {
/* aarch64-opc.c */
#  define ui			jit_uint32_t
#  if __BYTE_ORDER == __LITTLE_ENDIAN
    /* cond2: condition in truly conditional-executed inst.  */
    struct {		ui b:  4; } cond2;
    /* nzcv: flag bit specifier, encoded in the "nzcv" field.  */
    struct {		ui b:  4; } nzcv;
    /* defgh: d:e:f:g:h bits in AdvSIMD modified immediate.  */
    struct { ui _:  5;	ui b:  5; } defgh;
    /* abc: a:b:c bits in AdvSIMD modified immediate.  */
    struct { ui _: 16;	ui b:  3; } abc;
    /* imm19: e.g. in CBZ.  */
    struct { ui _:  5;	ui b: 19; } imm19;
    /* immhi: e.g. in ADRP.  */
    struct { ui _:  5;	ui b: 19; } immhi;
    /* immlo: e.g. in ADRP.  */
    struct { ui _: 29;	ui b:  2; } immlo;
    /* size: in most AdvSIMD and floating-point instructions.  */
    struct { ui _: 22;	ui b:  2; } size;
    /* vldst_size: size field in the AdvSIMD load/store inst.  */
    struct { ui _: 10;	ui b:  2; } vldst_size;
    /* op: in AdvSIMD modified immediate instructions.  */
    struct { ui _: 29;	ui b:  1; } op;
    /* Q: in most AdvSIMD instructions.  */
    struct { ui _: 30;	ui b:  1; } Q;
    /* Rt: in load/store instructions.  */
    struct { 		ui b:  5; } Rt;
    /* Rd: in many integer instructions.  */
    struct { 		ui b:  5; } Rd;
    /* Rn: in many integer instructions.  */
    struct { ui _:  5;	ui b:  5; } Rn;
    /* Rt2: in load/store pair instructions.  */
    struct { ui _: 10;	ui b:  5; } Rt2;
    /* Ra: in fp instructions.  */
    struct { ui _: 10;	ui b:  5; } Ra;
    /* op2: in the system instructions.  */
    struct { ui _:  5;	ui b:  3; } op2;
    /* CRm: in the system instructions.  */
    struct { ui _:  8;	ui b:  4; } CRm;
    /* CRn: in the system instructions.  */
    struct { ui _: 12;	ui b:  4; } CRn;
    /* op1: in the system instructions.  */
    struct { ui _: 16;	ui b:  3; } op1;
    /* op0: in the system instructions.  */
    struct { ui _: 19;	ui b:  2; } op0;
    /* imm3: in add/sub extended reg instructions.  */
    struct { ui _: 10;	ui b:  3; } imm3;
    /* cond: condition flags as a source operand.  */
    struct { ui _: 12;	ui b:  4; } cond;
    /* opcode: in advsimd load/store instructions.  */
    struct { ui _: 12;	ui b:  4; } opcode;
    /* cmode: in advsimd modified immediate instructions.  */
    struct { ui _: 12;	ui b:  4; } cmode;
    /* asisdlso_opcode: opcode in advsimd ld/st single element.  */
    struct { ui _: 13;	ui b:  3; } asisdlso_opcode;
    /* len: in advsimd tbl/tbx instructions.  */
    struct { ui _: 13;	ui b:  2; } len;
    /* Rm: in ld/st reg offset and some integer inst.  */
    struct { ui _: 16;	ui b:  5; } Rm;
    /* Rs: in load/store exclusive instructions.  */
    struct { ui _: 16;	ui b:  5; } Rs;
    /* option: in ld/st reg offset + add/sub extended reg inst.  */
    struct { ui _: 13;	ui b:  3; } option;
    /* S: in load/store reg offset instructions.  */
    struct { ui _: 12;	ui b:  1; } S;
    /* hw: in move wide constant instructions.  */
    struct { ui _: 21;	ui b:  2; } hw;
    /* opc: in load/store reg offset instructions.  */
    struct { ui _: 22;	ui b:  2; } opc;
    /* opc1: in load/store reg offset instructions.  */
    struct { ui _: 23;	ui b:  1; } opc1;
    /* shift: in add/sub reg/imm shifted instructions.  */
    struct { ui _: 22;	ui b:  2; } shift;
    /* type: floating point type field in fp data inst.  */
    struct { ui _: 22;	ui b:  2; } type;
    /* ldst_size: size field in ld/st reg offset inst.  */
    struct { ui _: 30;	ui b:  2; } ldst_size;
    /* imm6: in add/sub reg shifted instructions.  */
    struct { ui _: 10;	ui b:  6; } imm6;
    /* imm4: in advsimd ext and advsimd ins instructions.  */
    struct { ui _: 11;	ui b:  4; } imm4;
    /* imm5: in conditional compare (immediate) instructions.  */
    struct { ui _: 16;	ui b:  5; } imm5;
    /* imm7: in load/store pair pre/post index instructions.  */
    struct { ui _: 15;	ui b:  7; } imm7;
    /* imm8: in floating-point scalar move immediate inst.  */
    struct { ui _: 13;	ui b:  8; } imm8;
    /* imm9: in load/store pre/post index instructions.  */
    struct { ui _: 12;	ui b:  9; } imm9;
    /* imm12: in ld/st unsigned imm or add/sub shifted inst.  */
    struct { ui _: 10;	ui b: 12; } imm12;
    /* imm14: in test bit and branch instructions.  */
    struct { ui _:  5;	ui b: 14; } imm14;
    /* imm16: in exception instructions.  */
    struct { ui _:  5;	ui b: 16; } imm16;
    /* imm26: in unconditional branch instructions.  */
    struct {		ui b: 26; } imm26;
    /* imms: in bitfield and logical immediate instructions.  */
    struct { ui _: 10;	ui b:  6; } imms;
    /* immr: in bitfield and logical immediate instructions.  */
    struct { ui _: 16;	ui b:  6; } immr;
    /* immb: in advsimd shift by immediate instructions.  */
    struct { ui _: 16;	ui b:  3; } immb;
    /* immh: in advsimd shift by immediate instructions.  */
    struct { ui _: 19;	ui b:  4; } immh;
    /* N: in logical (immediate) instructions.  */
    struct { ui _: 22;	ui b:  1; } N;
    /* index: in ld/st inst deciding the pre/post-index.  */
    struct { ui _: 11;	ui b:  1; } index;
    /* index2: in ld/st pair inst deciding the pre/post-index.  */
    struct { ui _: 24;	ui b:  1; } index2;
    /* sf: in integer data processing instructions.  */
    struct { ui _: 31;	ui b:  1; } sf;
    /* H: in advsimd scalar x indexed element instructions.  */
    struct { ui _: 11;	ui b:  1; } H;
    /* L: in advsimd scalar x indexed element instructions.  */
    struct { ui _: 21;	ui b:  1; } L;
    /* M: in advsimd scalar x indexed element instructions.  */
    struct { ui _: 20;	ui b:  1; } M;
    /* b5: in the test bit and branch instructions.  */
    struct { ui _: 31;	ui b:  1; } b5;
    /* b40: in the test bit and branch instructions.  */
    struct { ui _: 19;	ui b:  5; } b40;
    /* scale: in the fixed-point scalar to fp converting inst.  */
    struct { ui _: 10;	ui b:  6; } scale;
#  else
    struct { ui _: 28;	ui b:  4; } cond2;
    struct { ui _: 28;	ui b:  4; } nzcv;
    struct { ui _: 22;	ui b:  5; } defgh;
    struct { ui _: 13;	ui b:  3; } abc;
    struct { ui _:  8;	ui b: 19; } imm19;
    struct { ui _:  8;	ui b: 19; } immhi;
    struct { ui _:  1;	ui b: 29; } immlo;
    struct { ui _:  8;	ui b:  2; } size;
    struct { ui _: 20;	ui b:  2; } vldst_size;
    struct { ui _:  2;	ui b:  1; } op;
    struct { ui _:  1;	ui b:  1; } Q;
    struct { ui _: 27;	ui b:  1; } Rt;
    struct { ui _: 27;	ui b:  1; } Rd;
    struct { ui _: 22;	ui b:  5; } Rn;
    struct { ui _: 17;	ui b:  5; } Rt2;
    struct { ui _: 17;	ui b:  5; } Ra;
    struct { ui _: 24;	ui b:  3; } op2;
    struct { ui _: 20;	ui b:  4; } CRm;
    struct { ui _: 16;	ui b:  4; } CRn;
    struct { ui _: 13;	ui b:  3; } op1;
    struct { ui _: 11;	ui b:  2; } op0;
    struct { ui _: 19;	ui b:  3; } imm3;
    struct { ui _: 16;	ui b:  4; } cond;
    struct { ui _: 16;	ui b:  4; } opcode;
    struct { ui _: 16;	ui b:  4; } cmode;
    struct { ui _: 16;	ui b:  3; } asisdlso_opcode;
    struct { ui _: 17;	ui b:  2; } len;
    struct { ui _: 11;	ui b:  5; } Rm;
    struct { ui _: 11;	ui b:  5; } Rs;
    struct { ui _: 16;	ui b:  3; } option;
    struct { ui _: 19;	ui b:  1; } S;
    struct { ui _:  9;	ui b:  2; } hw;
    struct { ui _:  8;	ui b:  2; } opc;
    struct { ui _:  8;	ui b:  1; } opc1;
    struct { ui _:  8;	ui b:  2; } shift;
    struct { ui _:  8;	ui b:  2; } type;
    struct {		ui b:  2; } ldst_size;
    struct { ui _: 16;	ui b:  6; } imm6;
    struct { ui _: 17;	ui b:  4; } imm4;
    struct { ui _: 11;	ui b:  5; } imm5;
    struct { ui _: 10;	ui b:  7; } imm7;
    struct { ui _: 11;	ui b:  8; } imm8;
    struct { ui _: 11;	ui b:  9; } imm9;
    struct { ui _: 10;	ui b: 12; } imm12;
    struct { ui _: 13;	ui b: 14; } imm14;
    struct { ui _: 11;	ui b: 16; } imm16;
    struct { ui _:  6;	ui b: 26; } imm26;
    struct { ui _: 16;	ui b:  6; } imms;
    struct { ui _: 10;	ui b:  6; } immr;
    struct { ui _: 13;	ui b:  3; } immb;
    struct { ui _:  9;	ui b:  4; } immh;
    struct { ui _:  9;	ui b:  1; } N;
    struct { ui _: 20;	ui b:  1; } index;
    struct { ui _:  7;	ui b:  1; } index2;
    struct {		ui b:  1; } sf;
    struct { ui _: 20;	ui b:  1; } H;
    struct { ui _: 10;	ui b:  1; } L;
    struct { ui _: 11;	ui b:  1; } M;
    struct {		ui b:  1; } b5;
    struct { ui _:  8;	ui b:  5; } b40;
    struct { ui _: 16;	ui b:  6; } scale;
#  endif
    jit_int32_t		w;
#  undef ui
} instr_t;
#  define stack_framesize		160
#  define ii(i)				*_jit->pc.ui++ = i
#  define ldr(r0,r1)			ldr_l(r0,r1)
#  define ldxr(r0,r1,r2)		ldxr_l(r0,r1,r2)
#  define ldxi(r0,r1,i0)		ldxi_l(r0,r1,i0)
#  define stxi(i0,r0,r1)		stxi_l(i0,r0,r1)
#  define FP_REGNO			0x1d
#  define LR_REGNO			0x1e
#  define SP_REGNO			0x1f
#  define XZR_REGNO			0x1f
#  define WZR_REGNO			XZR_REGNO
#  define LSL_12			0x00400000
#  define MOVI_LSL_16			0x00200000
#  define MOVI_LSL_32			0x00400000
#  define MOVI_LSL_48			0x00600000
#  define XS				0x80000000	/* Wn -> Xn */
#  define DS				0x00400000	/* Sn -> Dn */
#  define CC_NE				0x0
#  define CC_EQ				0x1
#  define CC_CC				0x2
#  define CC_LO				CC_CC
#  define CC_CS				0x3
#  define CC_HS				CC_CS
#  define CC_PL				0x4
#  define CC_MI				0x5
#  define CC_VC				0x6
#  define CC_VS				0x7
#  define CC_LS				0x8
#  define CC_HI				0x9
#  define CC_LT				0xa
#  define CC_GE				0xb
#  define CC_LE				0xc
#  define CC_GT				0xd
#  define CC_NV				0xe
#  define CC_AL				0xf
/* Branches need inverted condition */
#  define BCC_EQ			0x0
#  define BCC_NE			0x1
#  define BCC_CS			0x2
#  define BCC_HS			BCC_CS
#  define BCC_CC			0x3
#  define BCC_LO			BCC_CC
#  define BCC_MI			0x4
#  define BCC_PL			0x5
#  define BCC_VS			0x6
#  define BCC_VC			0x7
#  define BCC_HI			0x8
#  define BCC_LS			0x9
#  define BCC_GE			0xa
#  define BCC_LT			0xb
#  define BCC_GT			0xc
#  define BCC_LE			0xd
#  define BCC_AL			0xe
#  define BCC_NV			0xf
/* adapted and cut down to only tested and required by lightning,
 * from data in binutils/aarch64-tbl.h */
#  define A64_ADCS			0x3a000000
#  define A64_SBCS			0x7a000000
#  define A64_ADDI			0x11000000
#  define A64_ADDSI			0xb1000000
#  define A64_SUBI			0x51000000
#  define A64_SUBSI			0x71000000
#  define A64_ADD			0x0b000000
#  define A64_ADDS			0x2b000000
#  define A64_SUB			0x4b000000
#  define A64_NEG			0x4b0003e0
#  define A64_SUBS			0x6b000000
#  define A64_CMP			0x6b00001f
#  define A64_SBFM			0x93400000
#  define A64_UBFM			0x53400000
#  define A64_UBFX			0x53000000
#  define A64_B				0x14000000
#  define A64_BL			0x94000000
#  define A64_BR			0xd61f0000
#  define A64_BLR			0xd63f0000
#  define A64_RET			0xd65f0000
#  define A64_CBZ			0x34000000
#  define A64_CBNZ			0x35000000
#  define A64_B_C			0x54000000
#  define A64_CSINC			0x1a800400
#  define A64_REV			0xdac00c00
#  define A64_UDIV			0x1ac00800
#  define A64_SDIV			0x1ac00c00
#  define A64_LSL			0x1ac02000
#  define A64_LSR			0x1ac02400
#  define A64_ASR			0x1ac02800
#  define A64_MUL			0x1b007c00
#  define A64_SMULL			0x9b207c00
#  define A64_SMULH			0x9b407c00
#  define A64_UMULL			0x9ba07c00
#  define A64_UMULH			0x9bc07c00
#  define A64_STRBI			0x39000000
#  define A64_LDRBI			0x39400000
#  define A64_LDRSBI			0x39800000
#  define A64_STRI			0xf9000000
#  define A64_LDRI			0xf9400000
#  define A64_STRHI			0x79000000
#  define A64_LDRHI			0x79400000
#  define A64_LDRSHI			0x79800000
#  define A64_STRWI			0xb9000000
#  define A64_LDRWI			0xb9400000
#  define A64_LDRSWI			0xb9800000
#  define A64_STRB			0x38206800
#  define A64_LDRB			0x38606800
#  define A64_LDRSB			0x38e06800
#  define A64_STR			0xf8206800
#  define A64_LDR			0xf8606800
#  define A64_STRH			0x78206800
#  define A64_LDRH			0x78606800
#  define A64_LDRSH			0x78a06800
#  define A64_STRW			0xb8206800
#  define A64_LDRW			0xb8606800
#  define A64_LDRSW			0xb8a06800
#  define A64_STURB			0x38000000
#  define A64_LDURB			0x38400000
#  define A64_LDURSB			0x38800000
#  define A64_STUR			0xf8000000
#  define A64_LDUR			0xf8400000
#  define A64_STURH			0x78000000
#  define A64_LDURH			0x78400000
#  define A64_LDURSH			0x78800000
#  define A64_STURW			0xb8000000
#  define A64_LDURW			0xb8400000
#  define A64_LDURSW			0xb8800000
#  define A64_STP			0x29000000
#  define A64_LDP			0x29400000
#  define A64_STP_POS			0x29800000
#  define A64_LDP_PRE			0x28c00000
#  define A64_ANDI			0x12400000
#  define A64_ORRI			0x32400000
#  define A64_EORI			0x52400000
#  define A64_ANDSI			0x72000000
#  define A64_AND			0x0a000000
#  define A64_ORR			0x2a000000
#  define A64_MOV			0x2a0003e0	/* AKA orr Rd,xzr,Rm */
#  define A64_MVN			0x2a2003e0
#  define A64_UXTW			0x2a0003e0	/* AKA MOV */
#  define A64_EOR			0x4a000000
#  define A64_ANDS			0x6a000000
#  define A64_MOVN			0x12800000
#  define A64_MOVZ			0x52800000
#  define A64_MOVK			0x72800000
#  define SBFM(Rd,Rn,ImmR,ImmS)		oxxrs(A64_SBFM|XS,Rd,Rn,ImmR,ImmS)
#  define UBFM(Rd,Rn,ImmR,ImmS)		oxxrs(A64_UBFM|XS,Rd,Rn,ImmR,ImmS)
#  define UBFX(Rd,Rn,ImmR,ImmS)		oxxrs(A64_UBFX,Rd,Rn,ImmR,ImmS)
#  define CMP(Rn,Rm)			oxx_(A64_CMP|XS,Rn,Rm)
#  define CMPI(Rn,Imm12)		oxxi(A64_SUBSI|XS,XZR_REGNO,Rn,Imm12)
#  define CMPI_12(Rn,Imm12)		oxxi(A64_SUBSI|XS|LSL_12,XZR_REGNO,Rn,Imm12)
#  define CMNI(Rn,Imm12)		oxxi(A64_ADDSI|XS,XZR_REGNO,Rn,Imm12)
#  define CMNI_12(Rn,Imm12)		oxxi(A64_ADDSI|XS|LSL_12,XZR_REGNO,Rn,Imm12)
#  define CSINC(Rd,Rn,Rm,Cc)		oxxxc(A64_CSINC|XS,Rd,Rn,Rm,Cc)
#  define TST(Rn,Rm)			oxxx(A64_ANDS|XS,XZR_REGNO,Rn,Rm)
/* actually should use oxxrs but logical_immediate returns proper encoding */
#  define TSTI(Rn,Imm12)		oxxi(A64_ANDSI,XZR_REGNO,Rn,Imm12)
#  define MOV(Rd,Rm)			ox_x(A64_MOV|XS,Rd,Rm)
#  define MVN(Rd,Rm)			ox_x(A64_MVN|XS,Rd,Rm)
#  define NEG(Rd,Rm)			ox_x(A64_NEG|XS,Rd,Rm)
#  define MOVN(Rd,Imm16)		ox_h(A64_MOVN|XS,Rd,Imm16)
#  define MOVN_16(Rd,Imm16)		ox_h(A64_MOVN|XS|MOVI_LSL_16,Rd,Imm16)
#  define MOVN_32(Rd,Imm16)		ox_h(A64_MOVN|XS|MOVI_LSL_32,Rd,Imm16)
#  define MOVN_48(Rd,Imm16)		ox_h(A64_MOVN|XS|MOVI_LSL_48,Rd,Imm16)
#  define MOVZ(Rd,Imm16)		ox_h(A64_MOVZ|XS,Rd,Imm16)
#  define MOVZ_16(Rd,Imm16)		ox_h(A64_MOVZ|XS|MOVI_LSL_16,Rd,Imm16)
#  define MOVZ_32(Rd,Imm16)		ox_h(A64_MOVZ|XS|MOVI_LSL_32,Rd,Imm16)
#  define MOVZ_48(Rd,Imm16)		ox_h(A64_MOVZ|XS|MOVI_LSL_48,Rd,Imm16)
#  define MOVK(Rd,Imm16)		ox_h(A64_MOVK|XS,Rd,Imm16)
#  define MOVK_16(Rd,Imm16)		ox_h(A64_MOVK|XS|MOVI_LSL_16,Rd,Imm16)
#  define MOVK_32(Rd,Imm16)		ox_h(A64_MOVK|XS|MOVI_LSL_32,Rd,Imm16)
#  define MOVK_48(Rd,Imm16)		ox_h(A64_MOVK|XS|MOVI_LSL_48,Rd,Imm16)
#  define ADD(Rd,Rn,Rm)			oxxx(A64_ADD|XS,Rd,Rn,Rm)
#  define ADDI(Rd,Rn,Imm12)		oxxi(A64_ADDI|XS,Rd,Rn,Imm12)
#  define ADDI_12(Rd,Rn,Imm12)		oxxi(A64_ADDI|XS|LSL_12,Rd,Rn,Imm12)
#  define MOV_XSP(Rd,Rn)		ADDI(Rd,Rn,0)
#  define ADDS(Rd,Rn,Rm)		oxxx(A64_ADDS|XS,Rd,Rn,Rm)
#  define ADDSI(Rd,Rn,Imm12)		oxxi(A64_ADDSI|XS,Rd,Rn,Imm12)
#  define ADDSI_12(Rd,Rn,Imm12)		oxxi(A64_ADDSI|XS|LSL_12,Rd,Rn,Imm12)
#  define ADCS(Rd,Rn,Rm)		oxxx(A64_ADCS|XS,Rd,Rn,Rm)
#  define SUB(Rd,Rn,Rm)			oxxx(A64_SUB|XS,Rd,Rn,Rm)
#  define SUBI(Rd,Rn,Imm12)		oxxi(A64_SUBI|XS,Rd,Rn,Imm12)
#  define SUBI_12(Rd,Rn,Imm12)		oxxi(A64_SUBI|XS|LSL_12,Rd,Rn,Imm12)
#  define SUBS(Rd,Rn,Rm)		oxxx(A64_SUBS|XS,Rd,Rn,Rm)
#  define SUBSI(Rd,Rn,Imm12)		oxxi(A64_SUBSI|XS,Rd,Rn,Imm12)
#  define SUBSI_12(Rd,Rn,Imm12)		oxxi(A64_SUBSI|XS|LSL_12,Rd,Rn,Imm12)
#  define SBCS(Rd,Rn,Rm)		oxxx(A64_SBCS|XS,Rd,Rn,Rm)
#  define MUL(Rd,Rn,Rm)			oxxx(A64_MUL|XS,Rd,Rn,Rm)
#  define SMULL(Rd,Rn,Rm)		oxxx(A64_SMULL,Rd,Rn,Rm)
#  define SMULH(Rd,Rn,Rm)		oxxx(A64_SMULH,Rd,Rn,Rm)
#  define UMULL(Rd,Rn,Rm)		oxxx(A64_UMULL,Rd,Rn,Rm)
#  define UMULH(Rd,Rn,Rm)		oxxx(A64_UMULH,Rd,Rn,Rm)
#  define SDIV(Rd,Rn,Rm)		oxxx(A64_SDIV|XS,Rd,Rn,Rm)
#  define UDIV(Rd,Rn,Rm)		oxxx(A64_UDIV|XS,Rd,Rn,Rm)
#  define LSL(Rd,Rn,Rm)			oxxx(A64_LSL|XS,Rd,Rn,Rm)
#  define LSLI(r0,r1,i0)		UBFM(r0,r1,(64-i0)&63,63-i0)
#  define ASR(Rd,Rn,Rm)			oxxx(A64_ASR|XS,Rd,Rn,Rm)
#  define ASRI(r0,r1,i0)		SBFM(r0,r1,i0,63)
#  define LSR(Rd,Rn,Rm)			oxxx(A64_LSR|XS,Rd,Rn,Rm)
#  define LSRI(r0,r1,i0)		UBFM(r0,r1,i0,63)
#  define AND(Rd,Rn,Rm)			oxxx(A64_AND|XS,Rd,Rn,Rm)
/* actually should use oxxrs but logical_immediate returns proper encoding */
#  define ANDI(Rd,Rn,Imm12)		oxxi(A64_ANDI|XS,Rd,Rn,Imm12)
#  define ORR(Rd,Rn,Rm)			oxxx(A64_ORR|XS,Rd,Rn,Rm)
/* actually should use oxxrs but logical_immediate returns proper encoding */
#  define ORRI(Rd,Rn,Imm12)		oxxi(A64_ORRI|XS,Rd,Rn,Imm12)
#  define EOR(Rd,Rn,Rm)			oxxx(A64_EOR|XS,Rd,Rn,Rm)
/* actually should use oxxrs but logical_immediate returns proper encoding */
#  define EORI(Rd,Rn,Imm12)		oxxi(A64_EORI|XS,Rd,Rn,Imm12)
#  define SXTB(Rd,Rn)			SBFM(Rd,Rn,0,7)
#  define SXTH(Rd,Rn)			SBFM(Rd,Rn,0,15)
#  define SXTW(Rd,Rn)			SBFM(Rd,Rn,0,31)
#  define UXTB(Rd,Rn)			UBFX(Rd,Rn,0,7)
#  define UXTH(Rd,Rn)			UBFX(Rd,Rn,0,15)
#  define UXTW(Rd,Rm)			ox_x(A64_UXTW,Rd,Rm)
#  define REV(Rd,Rn)			o_xx(A64_REV,Rd,Rn)
#  define LDRSB(Rt,Rn,Rm)		oxxx(A64_LDRSB,Rt,Rn,Rm)
#  define LDRSBI(Rt,Rn,Imm12)		oxxi(A64_LDRSBI,Rt,Rn,Imm12)
#  define LDURSB(Rt,Rn,Imm9)		oxx9(A64_LDURSB,Rt,Rn,Imm9)
#  define LDRB(Rt,Rn,Rm)		oxxx(A64_LDRB,Rt,Rn,Rm)
#  define LDRBI(Rt,Rn,Imm12)		oxxi(A64_LDRBI,Rt,Rn,Imm12)
#  define LDURB(Rt,Rn,Imm9)		oxx9(A64_LDURB,Rt,Rn,Imm9)
#  define LDRSH(Rt,Rn,Rm)		oxxx(A64_LDRSH,Rt,Rn,Rm)
#  define LDRSHI(Rt,Rn,Imm12)		oxxi(A64_LDRSHI,Rt,Rn,Imm12)
#  define LDURSH(Rt,Rn,Imm9)		oxx9(A64_LDURSH,Rt,Rn,Imm9)
#  define LDRH(Rt,Rn,Rm)		oxxx(A64_LDRH,Rt,Rn,Rm)
#  define LDRHI(Rt,Rn,Imm12)		oxxi(A64_LDRHI,Rt,Rn,Imm12)
#  define LDURH(Rt,Rn,Imm9)		oxx9(A64_LDURH,Rt,Rn,Imm9)
#  define LDRSW(Rt,Rn,Rm)		oxxx(A64_LDRSW,Rt,Rn,Rm)
#  define LDRSWI(Rt,Rn,Imm12)		oxxi(A64_LDRSWI,Rt,Rn,Imm12)
#  define LDURSW(Rt,Rn,Imm9)		oxx9(A64_LDURSW,Rt,Rn,Imm9)
#  define LDRW(Rt,Rn,Rm)		oxxx(A64_LDRW,Rt,Rn,Rm)
#  define LDRWI(Rt,Rn,Imm12)		oxxi(A64_LDRWI,Rt,Rn,Imm12)
#  define LDURW(Rt,Rn,Imm9)		oxx9(A64_LDURW,Rt,Rn,Imm9)
#  define LDR(Rt,Rn,Rm)			oxxx(A64_LDR,Rt,Rn,Rm)
#  define LDRI(Rt,Rn,Imm12)		oxxi(A64_LDRI,Rt,Rn,Imm12)
#  define LDUR(Rt,Rn,Imm9)		oxx9(A64_LDUR,Rt,Rn,Imm9)
#  define STRB(Rt,Rn,Rm)		oxxx(A64_STRB,Rt,Rn,Rm)
#  define STRBI(Rt,Rn,Imm12)		oxxi(A64_STRBI,Rt,Rn,Imm12)
#  define STURB(Rt,Rn,Imm9)		oxx9(A64_STURB,Rt,Rn,Imm9)
#  define STRH(Rt,Rn,Rm)		oxxx(A64_STRH,Rt,Rn,Rm)
#  define STRHI(Rt,Rn,Imm12)		oxxi(A64_STRHI,Rt,Rn,Imm12)
#  define STURH(Rt,Rn,Imm9)		oxx9(A64_STURH,Rt,Rn,Imm9)
#  define STRW(Rt,Rn,Rm)		oxxx(A64_STRW,Rt,Rn,Rm)
#  define STRWI(Rt,Rn,Imm12)		oxxi(A64_STRWI,Rt,Rn,Imm12)
#  define STURW(Rt,Rn,Imm9)		oxx9(A64_STURW,Rt,Rn,Imm9)
#  define STR(Rt,Rn,Rm)			oxxx(A64_STR,Rt,Rn,Rm)
#  define STRI(Rt,Rn,Imm12)		oxxi(A64_STRI,Rt,Rn,Imm12)
#  define STUR(Rt,Rn,Imm9)		oxx9(A64_STUR,Rt,Rn,Imm9)
#  define LDPI(Rt,Rt2,Rn,Simm7)		oxxx7(A64_LDP|XS,Rt,Rt2,Rn,Simm7)
#  define STPI(Rt,Rt2,Rn,Simm7)		oxxx7(A64_STP|XS,Rt,Rt2,Rn,Simm7)
#  define LDPI_PRE(Rt,Rt2,Rn,Simm7)	oxxx7(A64_LDP_PRE|XS,Rt,Rt2,Rn,Simm7)
#  define STPI_POS(Rt,Rt2,Rn,Simm7)	oxxx7(A64_STP_POS|XS,Rt,Rt2,Rn,Simm7)
#  define CSET(Rd,Cc)			CSINC(Rd,XZR_REGNO,XZR_REGNO,Cc)
#  define B(Simm26)			o26(A64_B,Simm26)
#  define BL(Simm26)			o26(A64_BL,Simm26)
#  define BR(Rn)			o_x_(A64_BR,Rn)
#  define BLR(Rn)			o_x_(A64_BLR,Rn)
#  define RET()				o_x_(A64_RET,LR_REGNO)
#  define B_C(Cc,Simm19)		oc19(A64_B_C,Cc,Simm19)
#  define CBZ(Rd,Simm19)		ox19(A64_CBZ|XS,Rd,Simm19)
#  define CBNZ(Rd,Simm19)		ox19(A64_CBNZ|XS,Rd,Simm19)
#  define NOP()				ii(0xd503201f)
static jit_int32_t logical_immediate(jit_word_t);
#  define oxxx(Op,Rd,Rn,Rm)		_oxxx(_jit,Op,Rd,Rn,Rm)
static void _oxxx(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define oxxi(Op,Rd,Rn,Imm12)		_oxxi(_jit,Op,Rd,Rn,Imm12)
static void _oxxi(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define oxx9(Op,Rd,Rn,Imm9)		_oxx9(_jit,Op,Rd,Rn,Imm9)
static void _oxx9(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ox19(Op,Rd,Simm19)		_ox19(_jit,Op,Rd,Simm19)
static void _ox19(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define oc19(Op,Cc,Simm19)		_oc19(_jit,Op,Cc,Simm19)
static void _oc19(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define o26(Op,Simm26)		_o26(_jit,Op,Simm26)
static void _oc26(jit_state_t*,jit_int32_t,jit_int32_t);
#  define ox_x(Op,Rd,Rn)		_ox_x(_jit,Op,Rd,Rn)
static void _ox_x(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define o_xx(Op,Rd,Rn)		_o_xx(_jit,Op,Rd,Rn)
static void _o_xx(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define oxx_(Op,Rn,Rm)		_oxx_(_jit,Op,Rn,Rm)
static void _oxx_(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define o_x_(Op,Rn)			_o_x_(_jit,Op,Rn)
static void _o_x_(jit_state_t*,jit_int32_t,jit_int32_t);
#  define ox_h(Op,Rd,Imm16)		_ox_h(_jit,Op,Rd,Imm16)
static void _ox_h(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define oxxrs(Op,Rd,Rn,R,S)		_oxxrs(_jit,Op,Rd,Rn,R,S)
static void _oxxrs(jit_state_t*,jit_int32_t,jit_int32_t,
		   jit_int32_t,jit_int32_t,jit_int32_t);
#  define oxxxc(Op,Rd,Rn,Rm,Cc)		_oxxxc(_jit,Op,Rd,Rn,Rm,Cc)
static void _oxxxc(jit_state_t*,jit_int32_t,jit_int32_t,
		   jit_int32_t,jit_int32_t,jit_int32_t);
#  define oxxx7(Op,Rt,Rt2,Rn,Simm7)	_oxxx7(_jit,Op,Rt,Rt2,Rn,Simm7)
static void _oxxx7(jit_state_t*,jit_int32_t,
		   jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define nop(i0)			_nop(_jit,i0)
static void _nop(jit_state_t*,jit_int32_t);
#  define addr(r0,r1,r2)		ADD(r0,r1,r2)
#  define addi(r0,r1,i0)		_addi(_jit,r0,r1,i0)
static void _addi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define addcr(r0,r1,r2)		ADDS(r0,r1,r2)
#  define addci(r0,r1,i0)		_addci(_jit,r0,r1,i0)
static void _addci(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define addxr(r0,r1,r2)		ADCS(r0,r1,r2)
#  define addxi(r0,r1,i0)		_addxi(_jit,r0,r1,i0)
static void _addxi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define subr(r0,r1,r2)		SUB(r0,r1,r2)
#  define subi(r0,r1,i0)		_subi(_jit,r0,r1,i0)
static void _subi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define subcr(r0,r1,r2)		SUBS(r0,r1,r2)
#  define subci(r0,r1,i0)		_subci(_jit,r0,r1,i0)
static void _subci(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define subxr(r0,r1,r2)		SBCS(r0,r1,r2)
#  define subxi(r0,r1,i0)		_subxi(_jit,r0,r1,i0)
static void _subxi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define rsbi(r0, r1, i0)		_rsbi(_jit, r0, r1, i0)
static void _rsbi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define mulr(r0,r1,r2)		MUL(r0,r1,r2)
#  define muli(r0,r1,i0)		_muli(_jit,r0,r1,i0)
static void _muli(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define qmulr(r0,r1,r2,r3)		_qmulr(_jit,r0,r1,r2,r3)
static void _qmulr(jit_state_t*,jit_int32_t,
		   jit_int32_t,jit_int32_t,jit_int32_t);
#  define qmuli(r0,r1,r2,i0)		_qmuli(_jit,r0,r1,r2,i0)
static void _qmuli(jit_state_t*,jit_int32_t,
		   jit_int32_t,jit_int32_t,jit_word_t);
#  define qmulr_u(r0,r1,r2,r3)		_qmulr_u(_jit,r0,r1,r2,r3)
static void _qmulr_u(jit_state_t*,jit_int32_t,
		     jit_int32_t,jit_int32_t,jit_int32_t);
#  define qmuli_u(r0,r1,r2,i0)		_qmuli_u(_jit,r0,r1,r2,i0)
static void _qmuli_u(jit_state_t*,jit_int32_t,
		     jit_int32_t,jit_int32_t,jit_word_t);
#  define divr(r0,r1,r2)		SDIV(r0,r1,r2)
#  define divi(r0,r1,i0)		_divi(_jit,r0,r1,i0)
static void _divi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define divr_u(r0,r1,r2)		UDIV(r0,r1,r2)
#  define divi_u(r0,r1,i0)		_divi_u(_jit,r0,r1,i0)
static void _divi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define qdivr(r0,r1,r2,r3)		_iqdivr(_jit,1,r0,r1,r2,r3)
#  define qdivr_u(r0,r1,r2,r3)		_iqdivr(_jit,0,r0,r1,r2,r3)
static void _iqdivr(jit_state_t*,jit_bool_t,
		    jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define qdivi(r0,r1,r2,i0)		_qdivi(_jit,r0,r1,r2,i0)
static void _qdivi(jit_state_t*,jit_int32_t,
		   jit_int32_t,jit_int32_t,jit_word_t);
#  define qdivi_u(r0,r1,r2,i0)		_qdivi_u(_jit,r0,r1,r2,i0)
static void _qdivi_u(jit_state_t*,jit_int32_t,
		     jit_int32_t,jit_int32_t,jit_word_t);
#  define remr(r0,r1,r2)		_remr(_jit,r0,r1,r2)
static void _remr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define remi(r0,r1,i0)		_remi(_jit,r0,r1,i0)
static void _remi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define remr_u(r0,r1,r2)		_remr_u(_jit,r0,r1,r2)
static void _remr_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define remi_u(r0,r1,i0)		_remi_u(_jit,r0,r1,i0)
static void _remi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define lshr(r0,r1,r2)		LSL(r0,r1,r2)
#  define lshi(r0,r1,i0)		_lshi(_jit,r0,r1,i0)
static void _lshi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define rshr(r0,r1,r2)		ASR(r0,r1,r2)
#  define rshi(r0,r1,i0)		_rshi(_jit,r0,r1,i0)
static void _rshi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define rshr_u(r0,r1,r2)		LSR(r0,r1,r2)
#  define rshi_u(r0,r1,i0)		_rshi_u(_jit,r0,r1,i0)
static void _rshi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define negr(r0,r1)			NEG(r0,r1)
#  define comr(r0,r1)			MVN(r0,r1)
#  define andr(r0,r1,r2)		AND(r0,r1,r2)
#  define andi(r0,r1,i0)		_andi(_jit,r0,r1,i0)
static void _andi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define orr(r0,r1,r2)			ORR(r0,r1,r2)
#  define ori(r0,r1,i0)			_ori(_jit,r0,r1,i0)
static void _ori(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define xorr(r0,r1,r2)		EOR(r0,r1,r2)
#  define xori(r0,r1,i0)		_xori(_jit,r0,r1,i0)
static void _xori(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldr_c(r0,r1)			LDRSBI(r0,r1,0)
#  define ldi_c(r0,i0)			_ldi_c(_jit,r0,i0)
static void _ldi_c(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_uc(r0,r1)			_ldr_uc(_jit,r0,r1)
static void _ldr_uc(jit_state_t*,jit_int32_t,jit_int32_t);
#  define ldi_uc(r0,i0)			_ldi_uc(_jit,r0,i0)
static void _ldi_uc(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_s(r0,r1)			LDRSHI(r0,r1,0)
#  define ldi_s(r0,i0)			_ldi_s(_jit,r0,i0)
static void _ldi_s(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_us(r0,r1)			_ldr_us(_jit,r0,r1)
static void _ldr_us(jit_state_t*,jit_int32_t,jit_int32_t);
#  define ldi_us(r0,i0)			_ldi_us(_jit,r0,i0)
static void _ldi_us(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_i(r0,r1)			LDRSWI(r0,r1,0)
#  define ldi_i(r0,i0)			_ldi_i(_jit,r0,i0)
static void _ldi_i(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_ui(r0,r1)			_ldr_ui(_jit,r0,r1)
static void _ldr_ui(jit_state_t*,jit_int32_t,jit_int32_t);
#  define ldi_ui(r0,i0)			_ldi_ui(_jit,r0,i0)
static void _ldi_ui(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_l(r0,r1)			LDRI(r0,r1,0)
static void _ldr_l(jit_state_t*,jit_int32_t,jit_int32_t);
#  define ldi_l(r0,i0)			_ldi_l(_jit,r0,i0)
static void _ldi_l(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_c(r0,r1,r2)		_ldxr_c(_jit,r0,r1,r2)
static void _ldxr_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_c(r0,r1,i0)		_ldxi_c(_jit,r0,r1,i0)
static void _ldxi_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_uc(r0,r1,r2)		_ldxr_uc(_jit,r0,r1,r2)
static void _ldxr_uc(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_uc(r0,r1,i0)		_ldxi_uc(_jit,r0,r1,i0)
static void _ldxi_uc(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_s(r0,r1,r2)		LDRSH(r0,r1,r2)
#  define ldxi_s(r0,r1,i0)		_ldxi_s(_jit,r0,r1,i0)
static void _ldxi_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_us(r0,r1,r2)		_ldxr_us(_jit,r0,r1,r2)
static void _ldxr_us(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_us(r0,r1,i0)		_ldxi_us(_jit,r0,r1,i0)
static void _ldxi_us(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_i(r0,r1,r2)		LDRSW(r0,r1,r2)
#  define ldxi_i(r0,r1,i0)		_ldxi_i(_jit,r0,r1,i0)
static void _ldxi_i(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_ui(r0,r1,r2)		_ldxr_ui(_jit,r0,r1,r2)
static void _ldxr_ui(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_ui(r0,r1,i0)		_ldxi_ui(_jit,r0,r1,i0)
static void _ldxi_ui(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_l(r0,r1,r2)		LDR(r0,r1,r2)
#  define ldxi_l(r0,r1,i0)		_ldxi_l(_jit,r0,r1,i0)
static void _ldxi_l(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define str_c(r0,r1)			STRBI(r1,r0,0)
#  define sti_c(i0,r0)			_sti_c(_jit,i0,r0)
static void _sti_c(jit_state_t*,jit_word_t,jit_int32_t);
#  define str_s(r0,r1)			STRHI(r1,r0,0)
#  define sti_s(i0,r0)			_sti_s(_jit,i0,r0)
static void _sti_s(jit_state_t*,jit_word_t,jit_int32_t);
#  define str_i(r0,r1)			STRWI(r1,r0,0)
#  define sti_i(i0,r0)			_sti_i(_jit,i0,r0)
static void _sti_i(jit_state_t*,jit_word_t,jit_int32_t);
#  define str_l(r0,r1)			STRI(r1,r0,0)
#  define sti_l(i0,r0)			_sti_l(_jit,i0,r0)
static void _sti_l(jit_state_t*,jit_word_t,jit_int32_t);
#  define stxr_c(r0,r1,r2)		STRB(r2,r1,r0)
#  define stxi_c(i0,r0,r1)		_stxi_c(_jit,i0,r0,r1)
static void _stxi_c(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define stxr_s(r0,r1,r2)		STRH(r2,r1,r0)
#  define stxi_s(i0,r0,r1)		_stxi_s(_jit,i0,r0,r1)
static void _stxi_s(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define stxr_i(r0,r1,r2)		STRW(r2,r1,r0)
#  define stxi_i(i0,r0,r1)		_stxi_i(_jit,i0,r0,r1)
static void _stxi_i(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define stxr_l(r0,r1,r2)		STR(r2,r1,r0)
#  define stxi_l(i0,r0,r1)		_stxi_l(_jit,i0,r0,r1)
static void _stxi_l(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  if __BYTE_ORDER == __LITTLE_ENDIAN
#  define htonr_us(r0,r1)		_htonr_us(_jit,r0,r1)
static void _htonr_us(jit_state_t*,jit_int32_t,jit_int32_t);
#  define htonr_ui(r0,r1)		_htonr_ui(_jit,r0,r1)
static void _htonr_ui(jit_state_t*,jit_int32_t,jit_int32_t);
#    define htonr_ul(r0,r1)		REV(r0,r1)
#  else
#    define htonr_us(r0,r1)		extr_us(r0,r1)
#    define htonr_ui(r0,r1)		extr_ui(r0,r1)
#    define htonr_ul(r0,r1)		movr(r0,r1)
#  endif
#  define extr_c(r0,r1)			SXTB(r0,r1)
#  define extr_uc(r0,r1)		UXTB(r0,r1)
#  define extr_s(r0,r1)			SXTH(r0,r1)
#  define extr_us(r0,r1)		UXTH(r0,r1)
#  define extr_i(r0,r1)			SXTW(r0,r1)
#  define extr_ui(r0,r1)		UXTW(r0,r1)
#  define movr(r0,r1)			_movr(_jit,r0,r1)
static void _movr(jit_state_t*,jit_int32_t,jit_int32_t);
#  define movi(r0,i0)			_movi(_jit,r0,i0)
static void _movi(jit_state_t*,jit_int32_t,jit_word_t);
#  define movi_p(r0,i0)			_movi_p(_jit,r0,i0)
static jit_word_t _movi_p(jit_state_t*,jit_int32_t,jit_word_t);
#  define ccr(cc,r0,r1,r2)		_ccr(_jit,cc,r0,r1,r2)
static void _ccr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define cci(cc,r0,r1,i0)		_cci(_jit,cc,r0,r1,i0)
static void _cci(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_word_t);
#  define ltr(r0,r1,r2)			ccr(CC_LT,r0,r1,r2)
#  define lti(r0,r1,i0)			cci(CC_LT,r0,r1,i0)
#  define ltr_u(r0,r1,r2)		ccr(CC_CC,r0,r1,r2)
#  define lti_u(r0,r1,i0)		cci(CC_CC,r0,r1,i0)
#  define ler(r0,r1,r2)			ccr(CC_LE,r0,r1,r2)
#  define lei(r0,r1,i0)			cci(CC_LE,r0,r1,i0)
#  define ler_u(r0,r1,r2)		ccr(CC_LS,r0,r1,r2)
#  define lei_u(r0,r1,i0)		cci(CC_LS,r0,r1,i0)
#  define eqr(r0,r1,r2)			ccr(CC_EQ,r0,r1,r2)
#  define eqi(r0,r1,i0)			cci(CC_EQ,r0,r1,i0)
#  define ger(r0,r1,r2)			ccr(CC_GE,r0,r1,r2)
#  define gei(r0,r1,i0)			cci(CC_GE,r0,r1,i0)
#  define ger_u(r0,r1,r2)		ccr(CC_CS,r0,r1,r2)
#  define gei_u(r0,r1,i0)		cci(CC_CS,r0,r1,i0)
#  define gtr(r0,r1,r2)			ccr(CC_GT,r0,r1,r2)
#  define gti(r0,r1,i0)			cci(CC_GT,r0,r1,i0)
#  define gtr_u(r0,r1,r2)		ccr(CC_HI,r0,r1,r2)
#  define gti_u(r0,r1,i0)		cci(CC_HI,r0,r1,i0)
#  define ner(r0,r1,r2)			ccr(CC_NE,r0,r1,r2)
#  define nei(r0,r1,i0)			cci(CC_NE,r0,r1,i0)
#  define bccr(cc,i0,r0,r1)		_bccr(_jit,cc,i0,r0,r1)
static jit_word_t
_bccr(jit_state_t*,jit_int32_t,jit_word_t,jit_int32_t,jit_int32_t);
#  define bcci(cc,i0,r0,i1)		_bcci(_jit,cc,i0,r0,i1)
static jit_word_t
_bcci(jit_state_t*,jit_int32_t,jit_word_t,jit_int32_t,jit_word_t);
#  define bltr(i0,r0,r1)		bccr(BCC_LT,i0,r0,r1)
#  define blti(i0,r0,i1)		bcci(BCC_LT,i0,r0,i1)
#  define bltr_u(i0,r0,r1)		bccr(BCC_CC,i0,r0,r1)
#  define blti_u(i0,r0,i1)		bcci(BCC_CC,i0,r0,i1)
#  define bler(i0,r0,r1)		bccr(BCC_LE,i0,r0,r1)
#  define blei(i0,r0,i1)		bcci(BCC_LE,i0,r0,i1)
#  define bler_u(i0,r0,r1)		bccr(BCC_LS,i0,r0,r1)
#  define blei_u(i0,r0,i1)		bcci(BCC_LS,i0,r0,i1)
#  define beqr(i0,r0,r1)		bccr(BCC_EQ,i0,r0,r1)
#  define beqi(i0,r0,i1)		_beqi(_jit,i0,r0,i1)
static jit_word_t _beqi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bger(i0,r0,r1)		bccr(BCC_GE,i0,r0,r1)
#  define bgei(i0,r0,i1)		bcci(BCC_GE,i0,r0,i1)
#  define bger_u(i0,r0,r1)		bccr(BCC_CS,i0,r0,r1)
#  define bgei_u(i0,r0,i1)		bcci(BCC_CS,i0,r0,i1)
#  define bgtr(i0,r0,r1)		bccr(BCC_GT,i0,r0,r1)
#  define bgti(i0,r0,i1)		bcci(BCC_GT,i0,r0,i1)
#  define bgtr_u(i0,r0,r1)		bccr(BCC_HI,i0,r0,r1)
#  define bgti_u(i0,r0,i1)		bcci(BCC_HI,i0,r0,i1)
#  define bner(i0,r0,r1)		bccr(BCC_NE,i0,r0,r1)
#  define bnei(i0,r0,i1)		_bnei(_jit,i0,r0,i1)
static jit_word_t _bnei(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define baddr(cc,i0,r0,r1)		_baddr(_jit,cc,i0,r0,r1)
static jit_word_t
_baddr(jit_state_t*,jit_int32_t,jit_word_t,jit_int32_t,jit_int32_t);
#  define baddi(cc,i0,r0,i1)		_baddi(_jit,cc,i0,r0,i1)
static jit_word_t
_baddi(jit_state_t*,jit_int32_t,jit_word_t,jit_int32_t,jit_word_t);
#  define boaddr(i0,r0,r1)		baddr(BCC_VS,i0,r0,r1)
#  define boaddi(i0,r0,i1)		baddi(BCC_VS,i0,r0,i1)
#  define boaddr_u(i0,r0,r1)		baddr(BCC_HS,i0,r0,r1)
#  define boaddi_u(i0,r0,i1)		baddi(BCC_HS,i0,r0,i1)
#  define bxaddr(i0,r0,r1)		baddr(BCC_VC,i0,r0,r1)
#  define bxaddi(i0,r0,i1)		baddi(BCC_VC,i0,r0,i1)
#  define bxaddr_u(i0,r0,r1)		baddr(BCC_LO,i0,r0,r1)
#  define bxaddi_u(i0,r0,i1)		baddi(BCC_LO,i0,r0,i1)
#  define bsubr(cc,i0,r0,r1)		_bsubr(_jit,cc,i0,r0,r1)
static jit_word_t
_bsubr(jit_state_t*,jit_int32_t,jit_word_t,jit_int32_t,jit_int32_t);
#  define bsubi(cc,i0,r0,i1)		_bsubi(_jit,cc,i0,r0,i1)
static jit_word_t
_bsubi(jit_state_t*,jit_int32_t,jit_word_t,jit_int32_t,jit_word_t);
#  define bosubr(i0,r0,r1)		bsubr(BCC_VS,i0,r0,r1)
#  define bosubi(i0,r0,i1)		bsubi(BCC_VS,i0,r0,i1)
#  define bosubr_u(i0,r0,r1)		bsubr(BCC_LO,i0,r0,r1)
#  define bosubi_u(i0,r0,i1)		bsubi(BCC_LO,i0,r0,i1)
#  define bxsubr(i0,r0,r1)		bsubr(BCC_VC,i0,r0,r1)
#  define bxsubi(i0,r0,i1)		bsubi(BCC_VC,i0,r0,i1)
#  define bxsubr_u(i0,r0,r1)		bsubr(BCC_HS,i0,r0,r1)
#  define bxsubi_u(i0,r0,i1)		bsubi(BCC_HS,i0,r0,i1)
#  define bmxr(cc,i0,r0,r1)		_bmxr(_jit,cc,i0,r0,r1)
static jit_word_t
_bmxr(jit_state_t*,jit_int32_t,jit_word_t,jit_int32_t,jit_int32_t);
#  define bmxi(cc,i0,r0,r1)		_bmxi(_jit,cc,i0,r0,r1)
static jit_word_t
_bmxi(jit_state_t*,jit_int32_t,jit_word_t,jit_int32_t,jit_word_t);
#  define bmsr(i0,r0,r1)		bmxr(BCC_NE,i0,r0,r1)
#  define bmsi(i0,r0,i1)		bmxi(BCC_NE,i0,r0,i1)
#  define bmcr(i0,r0,r1)		bmxr(BCC_EQ,i0,r0,r1)
#  define bmci(i0,r0,i1)		bmxi(BCC_EQ,i0,r0,i1)
#  define jmpr(r0)			BR(r0)
#  define jmpi(i0)			_jmpi(_jit,i0)
static void _jmpi(jit_state_t*,jit_word_t);
#  define jmpi_p(i0)			_jmpi_p(_jit,i0)
static jit_word_t _jmpi_p(jit_state_t*,jit_word_t);
#  define callr(r0)			BLR(r0)
#  define calli(i0)			_calli(_jit,i0)
static void _calli(jit_state_t*,jit_word_t);
#  define calli_p(i0)			_calli_p(_jit,i0)
static jit_word_t _calli_p(jit_state_t*,jit_word_t);
#  define prolog(i0)			_prolog(_jit,i0)
static void _prolog(jit_state_t*,jit_node_t*);
#  define epilog(i0)			_epilog(_jit,i0)
static void _epilog(jit_state_t*,jit_node_t*);
#  define vastart(r0)			_vastart(_jit, r0)
static void _vastart(jit_state_t*, jit_int32_t);
#  define vaarg(r0, r1)			_vaarg(_jit, r0, r1)
static void _vaarg(jit_state_t*, jit_int32_t, jit_int32_t);
#  define patch_at(jump,label)		_patch_at(_jit,jump,label)
static void _patch_at(jit_state_t*,jit_word_t,jit_word_t);
#endif

#if CODE
static jit_int32_t
logical_immediate(jit_word_t imm)
{
    /* There are 5334 possible immediate values, but to avoid the
     * need of either too complex code or large lookup tables,
     * only check for (simply) encodable common/small values */
    switch (imm) {
	case -16:	return (0xf3b);
	case -15:	return (0xf3c);
	case -13:	return (0xf3d);
	case -9:	return (0xf3e);
	case -8:	return (0xf7c);
	case -7:	return (0xf7d);
	case -5:	return (0xf7e);
	case -4:	return (0xfbd);
	case -3:	return (0xfbe);
	case -2:	return (0xffe);
	case 1:		return (0x000);
	case 2:		return (0xfc0);
	case 3:		return (0x001);
	case 4:		return (0xf80);
	case 6:		return (0xfc1);
	case 7:		return (0x002);
	case 8:		return (0xf40);
	case 12:	return (0xf81);
	case 14:	return (0xfc2);
	case 15:	return (0x003);
	case 16:	return (0xf00);
	default:	return (-1);
    }
}

static void
_oxxx(jit_state_t *_jit, jit_int32_t Op,
      jit_int32_t Rd, jit_int32_t Rn, jit_int32_t Rm)
{
    instr_t	i;
    assert(!(Rd &       ~0x1f));
    assert(!(Rn &       ~0x1f));
    assert(!(Rm &       ~0x1f));
    assert(!(Op & ~0xffe0fc00));
    i.w = Op;
    i.Rd.b = Rd;
    i.Rn.b = Rn;
    i.Rm.b = Rm;
    ii(i.w);
}

static void
_oxxi(jit_state_t *_jit, jit_int32_t Op,
      jit_int32_t Rd, jit_int32_t Rn, jit_int32_t Imm12)
{
    instr_t	i;
    assert(!(Rd    &       ~0x1f));
    assert(!(Rn    &       ~0x1f));
    assert(!(Imm12 &      ~0xfff));
    assert(!(Op    & ~0xffe00000));
    i.w = Op;
    i.Rd.b = Rd;
    i.Rn.b = Rn;
    i.imm12.b = Imm12;
    ii(i.w);
}

static void
_oxx9(jit_state_t *_jit, jit_int32_t Op,
      jit_int32_t Rd, jit_int32_t Rn, jit_int32_t Imm9)
{
    instr_t	i;
    assert(!(Rd   &       ~0x1f));
    assert(!(Rn   &       ~0x1f));
    assert(!(Imm9 &      ~0x1ff));
    assert(!(Op   & ~0xffe00000));
    i.w = Op;
    i.Rd.b = Rd;
    i.Rn.b = Rn;
    i.imm9.b = Imm9;
    ii(i.w);
}

static void
_ox19(jit_state_t *_jit, jit_int32_t Op, jit_int32_t Rd, jit_int32_t Simm19)
{
    instr_t	i;
    assert(!(Rd &         ~0x1f));
    assert(Simm19 >= -262148 && Simm19 <= 262143);
    assert(!(Op   & ~0xff000000));
    i.w = Op;
    i.Rd.b = Rd;
    i.imm19.b = Simm19;
    ii(i.w);
}

static void
_oc19(jit_state_t *_jit, jit_int32_t Op, jit_int32_t Cc, jit_int32_t Simm19)
{
    instr_t	i;
    assert(!(Cc &          ~0xf));
    assert(Simm19 >= -262148 && Simm19 <= 262143);
    assert(!(Op   & ~0xff000000));
    i.w = Op;
    i.cond2.b = Cc;
    i.imm19.b = Simm19;
    ii(i.w);
}

static void
_o26(jit_state_t *_jit, jit_int32_t Op, jit_int32_t Simm26)
{
    instr_t	i;
    assert(Simm26 >= -33554432 && Simm26 <= 33554431);
    assert(!(Op   & ~0xfc000000));
    i.w = Op;
    i.imm26.b = Simm26;
    ii(i.w);
}

static void
_ox_x(jit_state_t *_jit, jit_int32_t Op, jit_int32_t Rd, jit_int32_t Rm)
{
    instr_t	i;
    assert(!(Rd &       ~0x1f));
    assert(!(Rm &       ~0x1f));
    assert(!(Op & ~0xffe0ffe0));
    i.w = Op;
    i.Rd.b = Rd;
    i.Rm.b = Rm;
    ii(i.w);
}

static void
_o_xx(jit_state_t *_jit, jit_int32_t Op, jit_int32_t Rd, jit_int32_t Rn)
{
    instr_t	i;
    assert(!(Rd &       ~0x1f));
    assert(!(Rn &       ~0x1f));
    assert(!(Op & ~0xfffffc00));
    i.w = Op;
    i.Rd.b = Rd;
    i.Rn.b = Rn;
    ii(i.w);
}

static void
_oxx_(jit_state_t *_jit, jit_int32_t Op, jit_int32_t Rn, jit_int32_t Rm)
{
    instr_t	i;
    assert(!(Rn &       ~0x1f));
    assert(!(Rm &       ~0x1f));
    assert(!(Op & ~0xffc0fc1f));
    i.w = Op;
    i.Rn.b = Rn;
    i.Rm.b = Rm;
    ii(i.w);
}

static void
_o_x_(jit_state_t *_jit, jit_int32_t Op, jit_int32_t Rn)
{
    instr_t	i;
    assert(!(Rn & ~0x1f));
    assert(!(Op & 0x3e0));
    i.w = Op;
    i.Rn.b = Rn;
    ii(i.w);
}

static void
_ox_h(jit_state_t *_jit, jit_int32_t Op, jit_int32_t Rd, jit_int32_t Imm16)
{
    instr_t	i;
    assert(!(Rd    &       ~0x1f));
    assert(!(Imm16 &     ~0xffff));
    assert(!(Op    & ~0xffe00000));
    i.w = Op;
    i.Rd.b = Rd;
    i.imm16.b = Imm16;
    ii(i.w);
}

static void
_oxxrs(jit_state_t *_jit, jit_int32_t Op,
       jit_int32_t Rd, jit_int32_t Rn, jit_int32_t R, jit_int32_t S)
{
    instr_t	i;
    assert(!(Rd &       ~0x1f));
    assert(!(Rn &       ~0x1f));
    assert(!(R  &       ~0x3f));
    assert(!(S  &       ~0x3f));
    assert(!(Op & ~0xffc00000));
    i.w = Op;
    i.Rd.b = Rd;
    i.Rn.b = Rn;
    i.immr.b = R;
    i.imms.b = S;
    ii(i.w);
}

static void
_oxxxc(jit_state_t *_jit, jit_int32_t Op,
       jit_int32_t Rd, jit_int32_t Rn, jit_int32_t Rm, jit_int32_t Cc)
{
    instr_t	i;
    assert(!(Rd &       ~0x1f));
    assert(!(Rn &       ~0x1f));
    assert(!(Rm &       ~0x1f));
    assert(!(Cc  &       ~0xf));
    assert(!(Op & ~0xffc00c00));
    i.w = Op;
    i.Rd.b = Rd;
    i.Rn.b = Rn;
    i.Rm.b = Rm;
    i.cond.b = Cc;
    ii(i.w);
}

static void
_oxxx7(jit_state_t *_jit, jit_int32_t Op,
       jit_int32_t Rt, jit_int32_t Rt2, jit_int32_t Rn, jit_int32_t Simm7)
{
    instr_t	i;
    assert(!(Rt  &       ~0x1f));
    assert(!(Rt2 &       ~0x1f));
    assert(!(Rn  &       ~0x1f));
    assert(Simm7 >= -128 && Simm7 <= 127);
    assert(!(Op & ~0xffc003e0));
    i.w = Op;
    i.Rt.b = Rt;
    i.Rt2.b = Rt2;
    i.Rn.b = Rn;
    i.imm7.b = Simm7;
    ii(i.w);
}

static void
_nop(jit_state_t *_jit, jit_int32_t i0)
{
    for (; i0 > 0; i0 -= 4)
	NOP();
    assert(i0 == 0);
}

static void
_addi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    jit_word_t		is =  i0 >> 12;
    jit_word_t		in = -i0;
    jit_word_t		iS =  in >> 12;
    if (      i0 >= 0 && i0 <= 0xfff)
	ADDI   (r0, r1, i0);
    else if ((is << 12) == i0 && is >= 0 && is <= 0xfff)
	ADDI_12(r0, r1, is);
    else if ( in >= 0 && in <= 0xfff)
	SUBI   (r0, r1, in);
    else if ((iS << 12) == is && iS >= 0 && iS <= 0xfff)
	SUBI_12(r0, r1, iS);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	addr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_addci(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    jit_word_t		is =  i0 >> 12;
    jit_word_t		in = -i0;
    jit_word_t		iS =  in >> 12;
    if (      i0 >= 0 && i0 <= 0xfff)
	ADDSI   (r0, r1, i0);
    else if ((is << 12) == i0 && is >= 0 && is <= 0xfff)
	ADDSI_12(r0, r1, is);
    else if ( in >= 0 && in <= 0xfff)
	SUBSI   (r0, r1, in);
    else if ((iS << 12) == is && iS >= 0 && iS <= 0xfff)
	SUBSI_12(r0, r1, iS);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	addcr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_addxi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    addxr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_subi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    jit_word_t		is = i0 >> 12;
    if (      i0 >= 0 && i0 <= 0xfff)
	SUBI   (r0, r1, i0);
    else if ((is << 12) == i0 && is >= 0 && is <= 0xfff)
	SUBI_12(r0, r1, is);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	subr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_subci(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    jit_word_t		is = i0 >> 12;
    if (      i0 >= 0 && i0 <= 0xfff)
	SUBSI   (r0, r1, i0);
    else if ((is << 12) == i0 && is >= 0 && is <= 0xfff)
	SUBSI_12(r0, r1, is);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	subcr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_subxi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    subxr(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_rsbi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    subi(r0, r1, i0);
    negr(r0, r0);
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
_qmulr(jit_state_t *_jit, jit_int32_t r0,
       jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    jit_int32_t		reg;
    if (r0 == r2 || r0 == r3) {
	reg = jit_get_reg(jit_class_gpr);
	mulr(rn(reg), r2, r3);
    }
    else
	mulr(r0, r2, r3);
    SMULH(r1, r2, r3);
    if (r0 == r2 || r0 == r3) {
	movr(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_qmuli(jit_state_t *_jit, jit_int32_t r0,
       jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    qmulr(r0, r1, r2, rn(reg));
    jit_unget_reg(reg);
}

static void
_qmulr_u(jit_state_t *_jit, jit_int32_t r0,
	 jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    jit_int32_t		reg;
    if (r0 == r2 || r0 == r3) {
	reg = jit_get_reg(jit_class_gpr);
	mulr(rn(reg), r2, r3);
    }
    else
	mulr(r0, r2, r3);
    UMULH(r1, r2, r3);
    if (r0 == r2 || r0 == r3) {
	movr(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_qmuli_u(jit_state_t *_jit, jit_int32_t r0,
	 jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    qmulr_u(r0, r1, r2, rn(reg));
    jit_unget_reg(reg);
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
_divi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    divr_u(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_iqdivr(jit_state_t *_jit, jit_bool_t sign,
	jit_int32_t r0, jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    jit_int32_t		sv0, rg0;
    jit_int32_t		sv1, rg1;
    if (r0 == r2 || r0 == r3) {
	sv0 = jit_get_reg(jit_class_gpr);
	rg0 = rn(sv0);
    }
    else
	rg0 = r0;
    if (r1 == r2 || r1 == r3) {
	sv1 = jit_get_reg(jit_class_gpr);
	rg1 = rn(sv1);
    }
    else
	rg1 = r1;
    if (sign)
	divr(rg0, r2, r3);
    else
	divr_u(rg0, r2, r3);
    mulr(rg1, r3, rg0);
    subr(rg1, r2, rg1);
    if (rg0 != r0) {
	movr(r0, rg0);
	jit_unget_reg(sv0);
    }
    if (rg1 != r1) {
	movr(r1, rg1);
	jit_unget_reg(sv1);
    }
}

static void
_qdivi(jit_state_t *_jit, jit_int32_t r0,
       jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    qdivr(r0, r1, r2, rn(reg));
    jit_unget_reg(reg);
}

static void
_qdivi_u(jit_state_t *_jit, jit_int32_t r0,
	 jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    qdivr_u(r0, r1, r2, rn(reg));
    jit_unget_reg(reg);
}

static void
_remr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		reg;
    if (r0 == r1 || r0 == r2) {
	reg = jit_get_reg(jit_class_gpr);
	divr(rn(reg), r1, r2);
	mulr(rn(reg), r2, rn(reg));
	subr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	divr(r0, r1, r2);
	mulr(r0, r2, r0);
	subr(r0, r1, r0);
    }
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
    jit_int32_t		reg;
    if (r0 == r1 || r0 == r2) {
	reg = jit_get_reg(jit_class_gpr);
	divr_u(rn(reg), r1, r2);
	mulr(rn(reg), r2, rn(reg));
	subr(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
    else {
	divr_u(r0, r1, r2);
	mulr(r0, r2, r0);
	subr(r0, r1, r0);
    }
}

static void
_remi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    remr_u(r0, r1, rn(reg));
    jit_unget_reg(reg);
}

static void
_lshi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (i0 == 0)
	movr(r0, r1);
    else {
	assert(i0 > 0 && i0 < 64);
	LSLI(r0, r1, i0);
    }
}

static void
_rshi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (i0 == 0)
	movr(r0, r1);
    else {
	assert(i0 > 0 && i0 < 64);
	ASRI(r0, r1, i0);
    }
}

static void
_rshi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (i0 == 0)
	movr(r0, r1);
    else {
	assert(i0 > 0 && i0 < 64);
	LSRI(r0, r1, i0);
    }
}

static void
_andi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    jit_int32_t		imm;
    if (i0 == 0)
	movi(r0, 0);
    else if (i0 == -1)
	movr(r0, r1);
    else {
	imm = logical_immediate(i0);
	if (imm != -1)
	    ANDI(r0, r1, imm);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    andr(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

static void
_ori(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    jit_int32_t		imm;
    if (i0 == 0)
	movr(r0, r1);
    else if (i0 == -1)
	movi(r0, -1);
    else {
	imm = logical_immediate(i0);
	if (imm != -1)
	    ORRI(r0, r1, imm);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    orr(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

static void
_xori(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    jit_int32_t		imm;
    if (i0 == 0)
	movr(r0, r1);
    else if (i0 == -1)
	comr(r0, r1);
    else {
	imm = logical_immediate(i0);
	if (imm != -1)
	    EORI(r0, r1, imm);
	else {
	    reg = jit_get_reg(jit_class_gpr);
	    movi(rn(reg), i0);
	    xorr(r0, r1, rn(reg));
	    jit_unget_reg(reg);
	}
    }
}

#if __BYTE_ORDER == __LITTLE_ENDIAN
static void
_htonr_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    htonr_ul(r0, r1);
    rshi_u(r0, r0, 48);
}

static void
_htonr_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    htonr_ul(r0, r1);
    rshi_u(r0, r0, 32);
}
#endif

static void
_ldi_c(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    ldr_c(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldr_uc(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    LDRBI(r0, r1, 0);
#if 0
    extr_uc(r0, r0);
#endif
}

static void
_ldi_uc(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    ldr_uc(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldi_s(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    ldr_s(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldr_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    LDRHI(r0, r1, 0);
#if 0
    extr_us(r0, r0);
#endif
}

static void
_ldi_us(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    ldr_us(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldi_i(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    ldr_i(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldr_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    LDRWI(r0, r1, 0);
#if 0
    extr_ui(r0, r0);
#endif
}

static void
_ldi_ui(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    ldr_ui(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldi_l(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    ldr_l(r0, rn(reg));
    jit_unget_reg(reg);
}

static void
_ldxr_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    LDRSB(r0, r1, r2);
    extr_c(r0, r0);
}

static void
_ldxi_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= 0 && i0 <= 4095)
	LDRSBI(r0, r1, i0);
    else if (i0 > -256 && i0 < 0)
	LDURSB(r0, r1, i0 & 0x1ff);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	LDRSB(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
    extr_c(r0, r0);
}

static void
_ldxr_uc(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    LDRB(r0, r1, r2);
#if 0
    extr_uc(r0, r0);
#endif
}

static void
_ldxi_uc(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    if (i0 >= 0 && i0 <= 4095)
	LDRBI(r0, r1, i0);
    else if (i0 > -256 && i0 < 0)
	LDURB(r0, r1, i0 & 0x1ff);
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r1, i0);
	ldr_uc(r0, rn(reg));
	jit_unget_reg(reg);
    }
#if 0
    extr_uc(r0, r0);
#endif
}

static void
_ldxi_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    assert(!(i0 & 1));
    if (i0 >= 0 && i0 <= 8191)
	LDRSHI(r0, r1, i0 >> 1);
    else if (i0 > -256 && i0 < 0)
	LDURSH(r0, r1, i0 & 0x1ff);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	LDRSH(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxr_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    LDRH(r0, r1, r2);
#if 0
    extr_us(r0, r0);
#endif
}

static void
_ldxi_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    assert(!(i0 & 1));
    if (i0 >= 0 && i0 <= 8191)
	LDRHI(r0, r1, i0 >> 1);
    else if (i0 > -256 && i0 < 0)
	LDURH(r0, r1, i0 & 0x1ff);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	LDRH(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
#if 0
    extr_us(r0, r0);
#endif
}

static void
_ldxi_i(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    assert(!(i0 & 3));
    if (i0 >= 0 && i0 <= 16383)
	LDRSWI(r0, r1, i0 >> 2);
    else if (i0 > -256 && i0 < 0)
	LDURSW(r0, r1, i0 & 0x1ff);
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r1, i0);
	ldr_i(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_ldxr_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    LDRW(r0, r1, r2);
#if 0
    extr_ui(r0, r0);
#endif
}

static void
_ldxi_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    assert(!(i0 & 3));
    if (i0 >= 0 && i0 <= 16383)
	LDRWI(r0, r1, i0 >> 2);
    else if (i0 > -256 && i0 < 0)
	LDURW(r0, r1, i0 & 0x1ff);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	LDRW(r0, r1, rn(reg));
	jit_unget_reg(reg);
    }
#if 0
    extr_ui(r0, r0);
#endif
}

static void
_ldxi_l(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    assert(!(i0 & 7));
    if (i0 >= 0 && i0 <= 32767)
	LDRI(r0, r1, i0 >> 3);
    else if (i0 > -256 && i0 < 0)
	LDUR(r0, r1, i0 & 0x1ff);
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r1, i0);
	ldr_l(r0, rn(reg));
	jit_unget_reg(reg);
    }
}

static void
_sti_c(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    str_c(rn(reg), r0);
    jit_unget_reg(reg);
}

static void
_sti_s(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    str_s(rn(reg), r0);
    jit_unget_reg(reg);
}

static void
_sti_i(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    str_i(rn(reg), r0);
    jit_unget_reg(reg);
}

static void
_sti_l(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)
{
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    movi(rn(reg), i0);
    str_l(rn(reg), r0);
    jit_unget_reg(reg);
}

static void
_stxi_c(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    if (i0 >= 0 && i0 <= 4095)
	STRBI(r1, r0, i0);
    else if (i0 > -256 && i0 < 0)
	STURB(r1, r0, i0 & 0x1ff);
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_c(rn(reg), r1);
	jit_unget_reg(reg);
    }
}

static void
_stxi_s(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    assert(!(i0 & 1));
    if (i0 >= 0 && i0 <= 8191)
	STRHI(r1, r0, i0 >> 1);
    else if (i0 > -256 && i0 < 0)
	STURH(r1, r0, i0 & 0x1ff);
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_s(rn(reg), r1);
	jit_unget_reg(reg);
    }
}

static void
_stxi_i(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    assert(!(i0 & 3));
    if (i0 >= 0 && i0 <= 16383)
	STRWI(r1, r0, i0 >> 2);
    else if (i0 > -256 && i0 < 0)
	STURW(r1, r0, i0 & 0x1ff);
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_i(rn(reg), r1);
	jit_unget_reg(reg);
    }
}

static void
_stxi_l(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		reg;
    assert(!(i0 & 7));
    if (i0 >= 0 && i0 <= 32767)
	STRI(r1, r0, i0 >> 3);
    else if (i0 > -256 && i0 < 0)
	STUR(r1, r0, i0 & 0x1ff);
    else {
	reg = jit_get_reg(jit_class_gpr);
	addi(rn(reg), r0, i0);
	str_l(rn(reg), r1);
	jit_unget_reg(reg);
    }
}

static void
_movr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    if (r0 != r1)
	MOV(r0, r1);
}

static void
_movi(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		n0, ibit, nbit;
    n0 = ~i0;
    ibit = nbit = 0;
    if (i0 & 0x000000000000ffffL)	ibit |= 1;
    if (i0 & 0x00000000ffff0000L)	ibit |= 2;
    if (i0 & 0x0000ffff00000000L)	ibit |= 4;
    if (i0 & 0xffff000000000000L)	ibit |= 8;
    if (n0 & 0x000000000000ffffL)	nbit |= 1;
    if (n0 & 0x00000000ffff0000L)	nbit |= 2;
    if (n0 & 0x0000ffff00000000L)	nbit |= 4;
    if (n0 & 0xffff000000000000L)	nbit |= 8;
    switch (ibit) {
	case 0:
	    MOVZ   (r0,  0);
	    break;
	case 1:
	    MOVZ   (r0,  i0        & 0xffff);
	    break;
	case 2:
	    MOVZ_16(r0, (i0 >> 16) & 0xffff);
	    break;
	case 3:
	    MOVZ   (r0,  i0        & 0xffff);
	    MOVK_16(r0, (i0 >> 16) & 0xffff);
	    break;
	case 4:
	    MOVZ_32(r0, (i0 >> 32) & 0xffff);
	    break;
	case 5:
	    MOVZ   (r0,  i0        & 0xffff);
	    MOVK_32(r0, (i0 >> 32) & 0xffff);
	    break;
	case 6:
	    MOVZ_16(r0, (i0 >> 16) & 0xffff);
	    MOVK_32(r0, (i0 >> 32) & 0xffff);
	    break;
	case 7:
	    if (nbit == 8)
		MOVN_48(r0, (n0 >> 48) & 0xffff);
	    else {
		MOVZ   (r0,  i0        & 0xffff);
		MOVK_16(r0, (i0 >> 16) & 0xffff);
		MOVK_32(r0, (i0 >> 32) & 0xffff);
	    }
	    break;
	case 8:
	    MOVZ_48(r0, (i0 >> 48) & 0xffff);
	    break;
	case 9:
	    MOVZ   (r0,  i0        & 0xffff);
	    MOVK_48(r0, (i0 >> 48) & 0xffff);
	    break;
	case 10:
	    MOVZ_16(r0, (i0 >> 16) & 0xffff);
	    MOVK_48(r0, (i0 >> 48) & 0xffff);
	    break;
	case 11:
	    if (nbit == 4)
		MOVN_32(r0, (n0 >> 32) & 0xffff);
	    else {
		MOVZ   (r0,  i0        & 0xffff);
		MOVK_16(r0, (i0 >> 16) & 0xffff);
		MOVK_48(r0, (i0 >> 48) & 0xffff);
	    }
	    break;
	case 12:
	    MOVZ_32(r0, (i0 >> 32) & 0xffff);
	    MOVK_48(r0, (i0 >> 48) & 0xffff);
	    break;
	case 13:
	    if (nbit == 2)
		MOVN_16(r0, (n0 >> 16) & 0xffff);
	    else {
		MOVZ   (r0,  i0        & 0xffff);
		MOVK_32(r0, (i0 >> 32) & 0xffff);
		MOVK_48(r0, (i0 >> 48) & 0xffff);
	    }
	    break;
	case 14:
	    if (nbit == 1)
		MOVN   (r0, (n0)       & 0xffff);
	    else {
		MOVZ_16(r0, (i0 >> 16) & 0xffff);
		MOVK_32(r0, (i0 >> 32) & 0xffff);
		MOVK_48(r0, (i0 >> 48) & 0xffff);
	    }
	    break;
	case 15:
	    if (nbit == 0)
		MOVN   (r0,  0);
	    else if (nbit == 1)
		MOVN   (r0,  n0        & 0xffff);
	    else if (nbit == 8)
		MOVN_48(r0, (n0 >> 48) & 0xffff);
	    else {
		MOVZ   (r0,  i0        & 0xffff);
		MOVK_16(r0, (i0 >> 16) & 0xffff);
		MOVK_32(r0, (i0 >> 32) & 0xffff);
		MOVK_48(r0, (i0 >> 48) & 0xffff);
	    }
	    break;
	default:
	    abort();
    }
}

static jit_word_t
_movi_p(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    w = _jit->pc.w;
    MOVZ   (r0,  i0        & 0xffff);
    MOVK_16(r0, (i0 >> 16) & 0xffff);
    MOVK_32(r0, (i0 >> 32) & 0xffff);
    MOVK_48(r0, (i0 >> 48) & 0xffff);
    return (w);
}

static void
_ccr(jit_state_t *_jit, jit_int32_t cc,
     jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    CMP(r1, r2);
    CSET(r0, cc);
}

static void
_cci(jit_state_t *_jit, jit_int32_t cc,
     jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		reg;
    jit_word_t		is =  i0 >> 12;
    jit_word_t		in = -i0;
    jit_word_t		iS =  in >> 12;
    if (      i0 >= 0 && i0 <= 0xfff)
	CMPI   (r1, i0);
    else if ((is << 12) == i0 && is >= 0 && is <= 0xfff)
	CMPI_12(r1, is);
    else if ( in >= 0 && in <= 0xfff)
	CMNI   (r1, in);
    else if ((iS << 12) == is && iS >= 0 && iS <= 0xfff)
	CMNI_12(r1, iS);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	CMP(r1, rn(reg));
	jit_unget_reg(reg);
    }
    CSET(r0, cc);
}

static jit_word_t
_bccr(jit_state_t *_jit, jit_int32_t cc,
      jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w, d;
    CMP(r0, r1);
    w = _jit->pc.w;
    d = (i0 - w) >> 2;
    B_C(cc, d);
    return (w);
}

static jit_word_t
_bcci(jit_state_t *_jit, jit_int32_t cc,
      jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_int32_t		reg;
    jit_word_t		w, d;
    jit_word_t		is =  i1 >> 12;
    jit_word_t		in = -i1;
    jit_word_t		iS =  in >> 12;
    if (      i1 >= 0 && i1 <= 0xfff)
	CMPI   (r0, i1);
    else if ((is << 12) == i1 && is >= 0 && is <= 0xfff)
	CMPI_12(r0, is);
    else if ( in >= 0 && in <= 0xfff)
	CMNI   (r0, in);
    else if ((iS << 12) == is && iS >= 0 && iS <= 0xfff)
	CMNI_12(r0, iS);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i1);
	CMP(r0, rn(reg));
	jit_unget_reg(reg);
    }
    w = _jit->pc.w;
    d = (i0 - w) >> 2;
    B_C(cc, d);
    return (w);
}

static jit_word_t
_beqi(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    if (i1 == 0) {
	w = _jit->pc.w;
	CBZ(r0, (i0 - w) >> 2);
    }
    else
	w = bcci(BCC_EQ, i0, r0, i1);
    return (w);
}

static jit_word_t
_bnei(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    if (i1 == 0) {
	w = _jit->pc.w;
	CBNZ(r0, (i0 - w) >> 2);
    }
    else
	w = bcci(BCC_NE, i0, r0, i1);
    return (w);
}

static jit_word_t
_baddr(jit_state_t *_jit, jit_int32_t cc,
       jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    addcr(r0, r0, r1);
    w = _jit->pc.w;
    B_C(cc, (i0 - w) >> 2);
    return (w);
}

static jit_word_t
_baddi(jit_state_t *_jit, jit_int32_t cc,
       jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    addci(r0, r0, i1);
    w = _jit->pc.w;
    B_C(cc, (i0 - w) >> 2);
    return (w);
}

static jit_word_t
_bsubr(jit_state_t *_jit, jit_int32_t cc,
       jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    subcr(r0, r0, r1);
    w = _jit->pc.w;
    B_C(cc, (i0 - w) >> 2);
    return (w);
}

static jit_word_t
_bsubi(jit_state_t *_jit, jit_int32_t cc,
       jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    subci(r0, r0, i1);
    w = _jit->pc.w;
    B_C(cc, (i0 - w) >> 2);
    return (w);
}

static jit_word_t
_bmxr(jit_state_t *_jit, jit_int32_t cc,
      jit_word_t i0, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    TST(r0, r1);
    w = _jit->pc.w;
    B_C(cc, (i0 - w) >> 2);
    return (w);
}

static jit_word_t
_bmxi(jit_state_t *_jit, jit_int32_t cc,
      jit_word_t i0, jit_int32_t r0, jit_word_t i1)
{
    jit_word_t		w;
    jit_int32_t		reg;
    jit_int32_t		imm;
    imm = logical_immediate(i1);
    if (imm != -1)
	TSTI(r0, imm);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i1);
	TST(r0, rn(reg));
	jit_unget_reg(reg);
    }
    w = _jit->pc.w;
    B_C(cc, (i0 - w) >> 2);
    return (w);
}

static void
_jmpi(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		reg;
    w = (i0 - _jit->pc.w) >> 2;
    if (w >= -33554432 && w <= 33554431)
	B(w);
    else {
	reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(reg), i0);
	jmpr(rn(reg));
	jit_unget_reg(reg);
    }
}

static jit_word_t
_jmpi_p(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr|jit_class_nospill);
    w = movi_p(rn(reg), i0);
    jmpr(rn(reg));
    jit_unget_reg(reg);
    return (w);
}

static void
_calli(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		reg;
    w = (i0 - _jit->pc.w) >> 2;
    if (w >= -33554432 && w <= 33554431)
	BL(w);
    else {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), i0);
	callr(rn(reg));
	jit_unget_reg(reg);
    }
}

static jit_word_t
_calli_p(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		reg;
    reg = jit_get_reg(jit_class_gpr);
    w = movi_p(rn(reg), i0);
    callr(rn(reg));
    jit_unget_reg(reg);
    return (w);
}

/*
 * prolog and epilog not as "optimized" as one would like, but the
 * problem of overallocating stack space to save callee save registers
 * exists on all ports, and is still a todo to use a variable
 *	stack_framesize
 * value, what would cause needing to patch some calls, most likely
 * the offset of jit_arg* of stack arguments.
 */
static void
_prolog(jit_state_t *_jit, jit_node_t *node)
{
    jit_int32_t		reg;
    if (_jitc->function->define_frame || _jitc->function->assume_frame) {
	jit_int32_t	frame = -_jitc->function->frame;
	assert(_jitc->function->self.aoff >= frame);
	if (_jitc->function->assume_frame)
	    return;
	_jitc->function->self.aoff = frame;
    }
    if (_jitc->function->allocar)
	_jitc->function->self.aoff &= -16;
    _jitc->function->stack = ((_jitc->function->self.alen -
			      /* align stack at 16 bytes */
			      _jitc->function->self.aoff) + 15) & -16;
    STPI_POS(FP_REGNO, LR_REGNO, SP_REGNO, -(stack_framesize >> 3));
    MOV_XSP(FP_REGNO, SP_REGNO);
#define SPILL(L, R, O)							\
    do {								\
	if (jit_regset_tstbit(&_jitc->function->regset, _R##L)) {	\
	    if (jit_regset_tstbit(&_jitc->function->regset, _R##R))	\
		STPI(L, R, SP_REGNO, O);				\
	    else							\
		STRI(L, SP_REGNO, O);					\
	}								\
	else if (jit_regset_tstbit(&_jitc->function->regset, _R##R))	\
	    STRI(R, SP_REGNO, O + 1);					\
    } while (0)
    SPILL(19, 20,  2);
    SPILL(21, 22,  4);
    SPILL(23, 24,  6);
    SPILL(25, 26,  8);
    SPILL(27, 28, 10);
#undef SPILL
#define SPILL(R, O)							\
    do {								\
	if (jit_regset_tstbit(&_jitc->function->regset, _V##R))		\
		stxi_d(O, SP_REGNO, R);					\
    } while (0)
    SPILL( 8,  96);
    SPILL( 9, 104);
    SPILL(10, 112);
    SPILL(11, 120);
    SPILL(12, 128);
    SPILL(13, 136);
    SPILL(14, 144);
    SPILL(15, 152);
#undef SPILL
    if (_jitc->function->stack)
	subi(SP_REGNO, SP_REGNO, _jitc->function->stack);
    if (_jitc->function->allocar) {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), _jitc->function->self.aoff);
	stxi_i(_jitc->function->aoffoff, FP_REGNO, rn(reg));
	jit_unget_reg(reg);
    }

    if (_jitc->function->self.call & jit_call_varargs) {
	/* Save gp registers in the save area, if any is a vararg */
	for (reg = 8 - _jitc->function->vagp / -8;
	     jit_arg_reg_p(reg); ++reg)
	    stxi(_jitc->function->vaoff + offsetof(jit_va_list_t, x0) +
		 reg * 8, FP_REGNO, rn(JIT_RA0 - reg));

	for (reg = 8 - _jitc->function->vafp / -16;
	     jit_arg_f_reg_p(reg); ++reg)
	    /* Save fp registers in the save area, if any is a vararg */
	    /* Note that the full 16 byte register is not saved, because
	     * lightning only handles float and double, and, while
	     * attempting to provide a va_list compatible pointer as
	     * jit_va_start return, does not guarantee it (on all ports). */
	    stxi_d(_jitc->function->vaoff + offsetof(jit_va_list_t, q0) +
		   reg * 16 + offsetof(jit_qreg_t, l), FP_REGNO, rn(_V0 - reg));
    }
}

static void
_epilog(jit_state_t *_jit, jit_node_t *node)
{
    if (_jitc->function->assume_frame)
	return;
    if (_jitc->function->stack)
	MOV_XSP(SP_REGNO, FP_REGNO);
#define LOAD(L, R, O)							\
    do {								\
	if (jit_regset_tstbit(&_jitc->function->regset, _R##L)) {	\
	    if (jit_regset_tstbit(&_jitc->function->regset, _R##R))	\
		LDPI(L, R, SP_REGNO, O);				\
	    else							\
		LDRI(L, SP_REGNO, O);					\
	}								\
	else if (jit_regset_tstbit(&_jitc->function->regset, _R##R))	\
	    LDRI(R, SP_REGNO, O + 1);					\
    } while (0)
    LOAD(19, 20,  2);
    LOAD(21, 22,  4);
    LOAD(23, 24,  6);
    LOAD(25, 26,  8);
    LOAD(27, 28, 10);
#undef LOAD
#define LOAD(R, O)							\
    do {								\
	if (jit_regset_tstbit(&_jitc->function->regset, _V##R))		\
		ldxi_d(R, SP_REGNO, O);					\
    } while (0)
    LOAD( 8,  96);
    LOAD( 9, 104);
    LOAD(10, 112);
    LOAD(11, 120);
    LOAD(12, 128);
    LOAD(13, 136);
    LOAD(14, 144);
    LOAD(15, 152);
#undef LOAD
    LDPI_PRE(FP_REGNO, LR_REGNO, SP_REGNO, stack_framesize >> 3);
    RET();
}

static void
_vastart(jit_state_t *_jit, jit_int32_t r0)
{
    jit_int32_t		reg;

    assert(_jitc->function->self.call & jit_call_varargs);

    /* Return jit_va_list_t in the register argument */
    addi(r0, FP_REGNO, _jitc->function->vaoff);

    reg = jit_get_reg(jit_class_gpr);

    /* Initialize stack pointer to the first stack argument. */
    addi(rn(reg), FP_REGNO, _jitc->function->self.size);
    stxi(offsetof(jit_va_list_t, stack), r0, rn(reg));

    /* Initialize gp top pointer to the first stack argument. */
    addi(rn(reg), r0, va_gp_top_offset);
    stxi(offsetof(jit_va_list_t, gptop), r0, rn(reg));

    /* Initialize fp top pointer to the first stack argument. */
    addi(rn(reg), r0, va_fp_top_offset);
    stxi(offsetof(jit_va_list_t, fptop), r0, rn(reg));

    /* Initialize gp offset in the save area. */
    movi(rn(reg), _jitc->function->vagp);
    stxi_i(offsetof(jit_va_list_t, gpoff), r0, rn(reg));

    /* Initialize fp offset in the save area. */
    movi(rn(reg), _jitc->function->vafp);
    stxi_i(offsetof(jit_va_list_t, fpoff), r0, rn(reg));

    jit_unget_reg(reg);
}

static void
_vaarg(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		ge_code;
    jit_word_t		lt_code;
    jit_int32_t		rg0, rg1;

    assert(_jitc->function->self.call & jit_call_varargs);

    rg0 = jit_get_reg(jit_class_gpr);
    rg1 = jit_get_reg(jit_class_gpr);

    /* Load the gp offset in save area in the first temporary. */
    ldxi_i(rn(rg0), r1, offsetof(jit_va_list_t, gpoff));

    /* Jump over if there are no remaining arguments in the save area. */
    ge_code = bgei(_jit->pc.w, rn(rg0), 0);

    /* Load the gp save pointer in the second temporary. */
    ldxi(rn(rg1), r1, offsetof(jit_va_list_t, gptop));

    /* Load the vararg argument in the first argument. */
    ldxr(r0, rn(rg1), rn(rg0));

    /* Update the gp offset. */
    addi(rn(rg0), rn(rg0), 8);
    stxi_i(offsetof(jit_va_list_t, gpoff), r1, rn(rg0));

    /* Will only need one temporary register below. */
    jit_unget_reg(rg1);

    /* Jump over overflow code. */
    lt_code = jmpi_p(_jit->pc.w);

    /* Where to land if argument is in overflow area. */
    patch_at(ge_code, _jit->pc.w);

    /* Load stack pointer. */
    ldxi(rn(rg0), r1, offsetof(jit_va_list_t, stack));

    /* Load argument. */
    ldr(r0, rn(rg0));

    /* Update stack pointer. */
    addi(rn(rg0), rn(rg0), 8);
    stxi(offsetof(jit_va_list_t, stack), r1, rn(rg0));

    /* Where to land if argument is in gp save area. */
    patch_at(lt_code, _jit->pc.w);

    jit_unget_reg(rg0);
}

static void
_patch_at(jit_state_t *_jit, jit_word_t instr, jit_word_t label)
{
    instr_t		 i;
    jit_word_t		 d;
    jit_int32_t		 fc, ff, ffc;
    union {
	jit_int32_t	*i;
	jit_word_t	 w;
    } u;
    u.w = instr;
    i.w = u.i[0];
    fc  = i.w & 0xfc000000;
    ff  = i.w & 0xff000000;
    ffc = i.w & 0xffc00000;
    if (fc == A64_B || fc == A64_BL) {
	d = (label - instr) >> 2;
	assert(d >= -33554432 && d <= 33554431);
	i.imm26.b = d;
	u.i[0] = i.w;
    }
    else if (ff == A64_B_C || ff == (A64_CBZ|XS) || ff == (A64_CBNZ|XS)) {
	d = (label - instr) >> 2;
	assert(d >= -262148 && d <= 262143);
	i.imm19.b = d;
	u.i[0] = i.w;
    }
    else if (ffc == (A64_MOVZ|XS)) {
	i.imm16.b = label;
	u.i[0] = i.w;
	i.w = u.i[1];
	assert((i.w & 0xffe00000) == (A64_MOVK|XS|MOVI_LSL_16));
	i.imm16.b = label >> 16;
	u.i[1] = i.w;
	i.w = u.i[2];
	assert((i.w & 0xffe00000) == (A64_MOVK|XS|MOVI_LSL_32));
	i.imm16.b = label >> 32;
	u.i[2] = i.w;
	i.w = u.i[3];
	assert((i.w & 0xffe00000) == (A64_MOVK|XS|MOVI_LSL_48));
	i.imm16.b = label >> 48;
	u.i[3] = i.w;
    }
    else
	abort();
}
#endif
