/*
 * Copyright (C) 2019  Free Software Foundation, Inc.
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
#define _ZERO_REGNO		0
#define _RA_REGNO		1
#define _SP_REGNO		2
#define _FP_REGNO		8
typedef union {
#  define ui			jit_uint32_t
    struct  {
	ui opcode	: 7;
	ui rd		: 5;
	ui funct3	: 3;
	ui rs1		: 5;
	ui rs2		: 5;
	ui funct7	: 7;
    } R;
    struct  {
	ui opcode	: 7;
	ui rd		: 5;
	ui funct3	: 3;
	ui rs1		: 5;
	ui rs2		: 5;
	ui funct2	: 2;
	ui rs3		: 5;
    } R4;
    struct  {
	ui opcode	: 7;
	ui rd		: 5;
	ui funct3	: 3;
	ui rs1		: 5;
	ui imm11_0	: 12;
    } I;
#  if __WORDSIZE == 64
    struct  {
	ui opcode	: 7;
	ui rd		: 5;
	ui funct3	: 3;
	ui rs1		: 5;
	ui shamt	: 6;
	ui imm6_0	: 6;
    } IS;
#  endif
    struct  {
	ui opcode	: 7;
	ui imm4_0	: 5;
	ui funct3	: 3;
	ui rs1		: 5;
	ui rs2		: 5;
	ui imm11_5	: 7;
    } S;
    struct  {
	ui opcode	: 7;
	ui imm11	: 1;
	ui imm4_1	: 4;
	ui funct3	: 3;
	ui rs1		: 5;
	ui rs2		: 5;
	ui imm10_5	: 6;
	ui imm12	: 1;
    } B;
    struct  {
	ui opcode	: 7;
	ui rd		: 5;
	ui imm12_31	: 20;
    } U;
    struct  {
	ui opcode	: 7;
	ui rd		: 5;
	ui imm19_12	: 8;
	ui imm11	: 1;
	ui imm10_1	: 10;
	ui imm20	: 1;
    } J;
    jit_int32_t		w;
#  undef ui
} instr_t;
#  define ii(i)				*_jit->pc.ui++ = i
/* FIXME could jit_rewind_prolog() to only use extra 64 bytes
 * if a variadic jit function that have variadic arguments in
 * registers */
#  define stack_framesize		(200 + 64)
#  define ldr(r0, r1)			ldr_l(r0, r1)
#  define ldi(r0, im)			ldi_l(r0, im)
#  define ldxr(r0, r1, r2)		ldxr_l(r0, r1, r2)
#  define ldxi(r0, r1, im)		ldxi_l(r0, r1, im)
#  define str(r0, r1)			str_l(r0, r1)
#  define sti(im, r0)			sti_l(im, r0)
#  define stxr(r0, r1, r2)		stxr_l(r0, r1, r2)
#  define stxi(im, r0, r1)		stxi_l(im, r0, r1)
#  define simm6_p(im)			((im) <= 31 && (im) >= -32)
#  define simm12_p(im)			((im) <= 2047 && (im) >= -2048)
#  define simm20_p(im)			((im) <= 524287 && (im) >= -524288)
#  define simm32_p(im)			((im) <= 2147483647LL && (im) >= -2147483648LL)

/*
 * RV32I Base Instruction Set
 */
#  define LUI(rd, imm)			Utype(55, rd, imm)
#  define AUIPC(rd, imm)		Utype(23, rd, imm)
#  define JAL(rd, imm)			Jtype(111, rd, imm)
#  define JALR(rd, rs1, imm)		Itype(103, rd, 0, rs1, imm)
#  define BEQ(rs1, rs2, imm)		Btype(99, 0, rs1, rs2, imm)
#  define BNE(rs1, rs2, imm)		Btype(99, 1, rs1, rs2, imm)
#  define BLT(rs1, rs2, imm)		Btype(99, 4, rs1, rs2, imm)
#  define BGE(rs1, rs2, imm)		Btype(99, 5, rs1, rs2, imm)
#  define BLTU(rs1, rs2, imm)		Btype(99, 6, rs1, rs2, imm)
#  define BGEU(rs1, rs2, imm)		Btype(99, 7, rs1, rs2, imm)
#  define LB(rd, rs1, imm)		Itype(3, rd, 0, rs1, imm)
#  define LH(rd, rs1, imm)		Itype(3, rd, 1, rs1, imm)
#  define LW(rd, rs1, imm)		Itype(3, rd, 2, rs1, imm)
#  define LBU(rd, rs1, imm)		Itype(3, rd, 4, rs1, imm)
#  define LHU(rd, rs1, imm)		Itype(3, rd, 5, rs1, imm)
#  define SB(rs1, rs2, imm)		Stype(35, 0, rs1, rs2, imm)
#  define SH(rs1, rs2, imm)		Stype(35, 1, rs1, rs2, imm)
#  define SW(rs1, rs2, imm)		Stype(35, 2, rs1, rs2, imm)
#  define ADDI(rd, rs1, imm)		Itype(19, rd, 0, rs1, imm)
#  define SLTI(rd, rs1, imm)		Itype(19, rd, 2, rs1, imm)
#  define SLTIU(rd, rs1, imm)		Itype(19, rd, 3, rs1, imm)
#  define XORI(rd, rs1, imm)		Itype(19, rd, 4, rs1, imm)
#  define ORI(rd, rs1, imm)		Itype(19, rd, 6, rs1, imm)
#  define ANDI(rd, rs1, imm)		Itype(19, rd, 7, rs1, imm)
#  if __WORDSIZE == 32
#    define SLLI(rd, rs1, imm)		Rtype(19, rd, 1, rs1, imm, 0)
#    define SRLI(rd, rs1, imm)		Rtype(19, rd, 5, rs1, imm, 0)
#    define SRAI(rd, rs1, imm)		Rtype(19, rd, 5, rs1, imm, 32)
#  endif
#  define ADD(rd, rs1, rs2)		Rtype(51, rd, 0, rs1, rs2, 0)
#  define SUB(rd, rs1, rs2)		Rtype(51, rd, 0, rs1, rs2, 32)
#  define SLL(rd, rs1, rs2)		Rtype(51, rd, 1, rs1, rs2, 0)
#  define SLT(rd, rs1, rs2)		Rtype(51, rd, 2, rs1, rs2, 0)
#  define SLTU(rd, rs1, rs2)		Rtype(51, rd, 3, rs1, rs2, 0)
#  define XOR(rd, rs1, rs2)		Rtype(51, rd, 4, rs1, rs2, 0)
#  define SRL(rd, rs1, rs2)		Rtype(51, rd, 5, rs1, rs2, 0)
#  define SRA(rd, rs1, rs2)		Rtype(51, rd, 5, rs1, rs2, 32)
#  define OR(rd, rs1, rs2)		Rtype(51, rd, 6, rs1, rs2, 0)
#  define AND(rd, rs1, rs2)		Rtype(51, rd, 7, rs1, rs2, 0)
#  define FENCE(imm)			Itype(15, 0, 0, 0, im)
#  define FENCE_I(imm)			Itype(15, 0, 1, 0, im)
#  define ECALL()			Itype(115, 0, 0, 0, 0)
#  define EBREAK()			Itype(115, 0, 0, 0, 1)
#  define CSRRW(rd, rs1, csr)		Itype(115, rd, 1, rs1, csr)
#  define CSRRS(rd, rs1, csr)		Itype(115, rd, 2, rs1, csr)
#  define CSRRC(rd, rs1, csr)		Itype(115, rd, 3, rs1, csr)
#  define CSRRWI(rd, zimm, csr)		Itype(115, rd, 5, zimm, csr)
#  define CSRRSI(rd, zimm, csr)		Itype(115, rd, 6, zimm, csr)
#  define CSRRCI(rd, zimm, csr)		Itype(115, rd, 7, zimm, csr)
/*
 * RV64I Base Instruction Set (in addition to RV32I)
 */
#  define LWU(rd, rs1, imm)		Itype(3, rd, 6, rs1, imm)
#  define LD(rd, rs1, imm)		Itype(3, rd, 3, rs1, imm)
#  define SD(rs1, rs2, imm)		Stype(35, 3, rs1, rs2, imm)
#  if __WORDSIZE == 64
#    define SLLI(rd, rs1, sh)		IStype(19, rd, 1, rs1, sh, 0)
#    define SRLI(rd, rs1, sh)		IStype(19, rd, 5, rs1, sh, 0)
#    define SRAI(rd, rs1, sh)		IStype(19, rd, 5, rs1, sh, 16)
#  endif
#  define ADDIW(rd, rs1, imm)		Itype(27, rd, 0, rs1, imm)
#  define SLLIW(rd, rs1, imm)		Rtype(27, rd, 1, rs1, imm, 0)
#  define SRLIW(rd, rs1, imm)		Rtype(27, rd, 3, rs1, imm, 0)
#  define SRAIW(rd, rs1, imm)		Rtype(27, rd, 3, rs1, imm, 32)
#  define ADDW(rd, rs1, imm)		Rtype(59, rd, 0, rs1, imm, 0)
#  define SUBW(rd, rs1, imm)		Rtype(59, rd, 0, rs1, imm, 32)
#  define SLLW(rd, rs1, imm)		Rtype(59, rd, 1, rs1, imm, 0)
#  define SRLW(rd, rs1, imm)		Rtype(59, rd, 5, rs1, imm, 0)
#  define SRAW(rd, rs1, imm)		Rtype(59, rd, 5, rs1, imm, 32)
/*
 * RV32M Standard Extension
 */
#  define MUL(rd, rs1, rs2)		Rtype(51, rd, 0, rs1, rs2, 1)
#  define MULH(rd, rs1, rs2)		Rtype(51, rd, 1, rs1, rs2, 1)
#  define MULHSU(rd, rs1, rs2)		Rtype(51, rd, 2, rs1, rs2, 1)
#  define MULHU(rd, rs1, rs2)		Rtype(51, rd, 3, rs1, rs2, 1)
#  define DIV(rd, rs1, rs2)		Rtype(51, rd, 4, rs1, rs2, 1)
#  define DIVU(rd, rs1, rs2)		Rtype(51, rd, 5, rs1, rs2, 1)
#  define REM(rd, rs1, rs2)		Rtype(51, rd, 6, rs1, rs2, 1)
#  define REMU(rd, rs1, rs2)		Rtype(51, rd, 7, rs1, rs2, 1)
/*
 * RV64M Standard Extension (in addition to RV32M)
 */
#  define MULW(rd, rs1, rs2)		Rtype(59, rd, 0, rs1, rs2, 1)
#  define DIVW(rd, rs1, rs2)		Rtype(59, rd, 4, rs1, rs2, 1)
#  define DIVUW(rd, rs1, rs2)		Rtype(59, rd, 5, rs1, rs2, 1)
#  define REMW(rd, rs1, rs2)		Rtype(59, rd, 6, rs1, rs2, 1)
#  define REMUW(rd, rs1, rs2)		Rtype(59, rd, 7, rs1, rs2, 1)
/*
 * RV32A Standard Extension
 */
#  define LR_W(rd, rs1)			R4type(47, rd, 2, rs1, 0, 0, 2)
#  define SC_W(rd, rs1, rs2)		R4type(47, rd, 2, rs1, rs2, 0, 3)
#  define AMOSWAP_W(rd, rs1, rs2)	R4type(47, rd, 2, rs1, rs2, 0, 1)
#  define AMOADD_W(rd, rs1, rs2)	R4type(47, rd, 2, rs1, rs2, 0, 0)
#  define AMOXOR_W(rd, rs1, rs2)	R4type(47, rd, 2, rs1, rs2, 0, 4)
#  define AMOAND_W(rd, rs1, rs2)	R4type(47, rd, 2, rs1, rs2, 0, 12)
#  define AMOOR_W(rd, rs1, rs2)		R4type(47, rd, 2, rs1, rs2, 0, 8)
#  define AMOMIN_W(rd, rs1, rs2)	R4type(47, rd, 2, rs1, rs2, 0, 16)
#  define AMOMAX_W(rd, rs1, rs2)	R4type(47, rd, 2, rs1, rs2, 0, 20)
#  define AMOMINU_W(rd, rs1, rs2)	R4type(47, rd, 2, rs1, rs2, 0, 24)
#  define AMOMAXU_W(rd, rs1, rs2)	R4type(47, rd, 2, rs1, rs2, 0, 28)
/*
 * RV64A Standard Extension (in addition to RV32A)
 */
#  define LR_D(rd, rs1)			R4type(47, rd, 3, rs1, 0, 0, 2)
#  define SC_D(rd, rs1, rs2)		R4type(47, rd, 3, rs1, rs2, 0, 3)
#  define AMOSWAP_D(rd, rs1, rs2)	R4type(47, rd, 3, rs1, rs2, 0, 1)
#  define AMOADD_D(rd, rs1, rs2)	R4type(47, rd, 3, rs1, rs2, 0, 0)
#  define AMOXOR_D(rd, rs1, rs2)	R4type(47, rd, 3, rs1, rs2, 0, 4)
#  define AMOAND_D(rd, rs1, rs2)	R4type(47, rd, 3, rs1, rs2, 0, 12)
#  define AMOOR_D(rd, rs1, rs2)		R4type(47, rd, 3, rs1, rs2, 0, 8)
#  define AMOMIN_D(rd, rs1, rs2)	R4type(47, rd, 3, rs1, rs2, 0, 16)
#  define AMOMAX_D(rd, rs1, rs2)	R4type(47, rd, 3, rs1, rs2, 0, 20)
#  define AMOMINU_D(rd, rs1, rs2)	R4type(47, rd, 3, rs1, rs2, 0, 24)
#  define AMOMAXU_D(rd, rs1, rs2)	R4type(47, rd, 3, rs1, rs2, 0, 28)
/*
 * Pseudo Instructions
 */
#  define NOP()				ADDI(_ZERO_REGNO, _ZERO_REGNO, 0)
#  define MV(r0, r1)			ADDI(r0, r1, 0)
#  define NOT(r0, r1)			XORI(r0, r1, -1)
#  define NEG(r0, r1)			SUB(r0, _ZERO_REGNO, r1)
#  define NEGW(r0, r1)			SUBW(r0, _ZERO_REGNO, r1)
#  define SEXT_W(r0, r1)		ADDIW(r0, r1, 0)
#  define RET()				JALR(0, 1, 0)

/*
 * Enconding functions
 */
#  define Rtype(op, rd, fct, rs1, rs2, fct2)			\
	_Rtype(_jit, op, rd, fct, rs1, rs2, fct2)
static void _Rtype(jit_state_t*, jit_int32_t, jit_int32_t,
		   jit_int32_t, jit_int32_t, jit_int32_t, jit_int32_t);
#  define R4type(op, rd, fct, rs1,rs2,fct2,rs3)			\
	_R4type(_jit, op, rd, fct, rs1, rs2, fct2, rs3)
static void _R4type(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t,
		    jit_int32_t, jit_int32_t, jit_int32_t, jit_int32_t);
#  define Itype(op, rd, fct, rs1, imm)				\
	_Itype(_jit, op, rd, fct, rs1, imm)
static void _Itype(jit_state_t*, jit_int32_t, jit_int32_t,
		   jit_int32_t, jit_int32_t, jit_int32_t);
#  if __WORDSIZE == 64
#  define IStype(op, rd, fct, rs1, sh, imm)			\
	_IStype(_jit, op, rd, fct, rs1, sh, imm)
static void _IStype(jit_state_t*, jit_int32_t, jit_int32_t,
		   jit_int32_t, jit_int32_t, jit_int32_t,jit_int32_t);
#  endif
#  define Stype(op, fct, rs1, rs2, imm)				\
	_Stype(_jit, op, fct, rs1, rs2, imm)
static void _Stype(jit_state_t*, jit_int32_t, jit_int32_t,
		   jit_int32_t, jit_int32_t, jit_int32_t);
#  define Btype(op, fct, rs1, rs2, imm)				\
	_Btype(_jit, op, fct, rs1, rs2, imm)
static void _Btype(jit_state_t*, jit_int32_t, jit_int32_t,
		   jit_int32_t, jit_int32_t, jit_int32_t);
#  define Utype(op, rd, imm)		_Utype(_jit, op, rd, imm)
static void _Utype(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
#  define Jtype(op, rd, imm)		_Jtype(_jit, op, rd, imm)
static void _Jtype(jit_state_t*, jit_int32_t, jit_int32_t, jit_int32_t);
/*
 * Lightning instructions
 */
#  define nop(im)			_nop(_jit, im)
static void _nop(jit_state_t*, jit_int32_t);
#  define addr(r0, r1, r2)		ADD(r0, r1, r2)
#  define addi(r0, r1, im)		_addi(_jit, r0, r1, im)
static void _addi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define addcr(r0, r1, r2)		_addcr(_jit, r0, r1, r2)
static void _addcr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define addci(r0, r1, im)		_addci(_jit, r0, r1, im)
static void _addci(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define addxr(r0, r1, r2)		_addxr(_jit, r0, r1, r2)
static void _addxr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define addxi(r0, r1, im)		_addxi(_jit, r0, r1, im)
static void _addxi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define subr(r0, r1, r2)		SUB(r0, r1, r2)
#  define subi(r0, r1, im)		_subi(_jit, r0, r1, im)
static void _subi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define subcr(r0, r1, r2)		_subcr(_jit, r0, r1, r2)
static void _subcr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define subci(r0, r1, im)		_subci(_jit, r0, r1, im)
static void _subci(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define subxr(r0, r1, r2)		_subxr(_jit, r0, r1, r2)
static void _subxr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define subxi(r0, r1, im)		_subxi(_jit, r0, r1, im)
static void _subxi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define rsbi(r0, r1, im)		_rsbi(_jit, r0, r1, im)
static void _rsbi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define mulr(r0, r1, r2)		MUL(r0, r1, r2)
#  define muli(r0, r1, im)		_muli(_jit, r0, r1, im)
static void _muli(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define divr(r0, r1, r2)		DIV(r0, r1, r2)
#  define divi(r0, r1, im)		_divi(_jit, r0, r1, im)
static void _divi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define divr_u(r0, r1, r2)		DIVU(r0, r1, r2)
#  define divi_u(r0, r1, im)		_divi_u(_jit, r0, r1, im)
static void _divi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define remr(r0, r1, r2)		REM(r0, r1, r2)
#  define remi(r0, r1, im)		_remi(_jit, r0, r1, im)
static void _remi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define remr_u(r0, r1, r2)		REMU(r0, r1, r2)
#  define remi_u(r0, r1, im)		_remi_u(_jit, r0, r1, im)
static void _remi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define qmulr(r0, r1, r2, r3)		_qmulr(_jit,r0,r1,r2,r3)
static void _qmulr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define qmuli(r0, r1, r2, i0)		_qmuli(_jit,r0,r1,r2,i0)
static void _qmuli(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_word_t);
#  define qmulr_u(r0, r1, r2, r3)	_qmulr_u(_jit,r0,r1,r2,r3)
static void _qmulr_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
#  define qmuli_u(r0, r1, r2, i0)	_qmuli_u(_jit,r0,r1,r2,i0)
static void _qmuli_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t,jit_word_t);
static void _iqdivr(jit_state_t*,jit_bool_t,
		    jit_int32_t,jit_int32_t,jit_int32_t,jit_int32_t);
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
#  define lshr(r0, r1, r2)		SLL(r0, r1, r2)
#  define lshi(r0, r1, im)		_lshi(_jit, r0, r1, im)
static void _lshi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define rshr(r0, r1, r2)		SRA(r0, r1, r2)
#  define rshi(r0, r1, im)		_rshi(_jit, r0, r1, im)
static void _rshi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define rshr_u(r0, r1, r2)		SRL(r0, r1, r2)
#  define rshi_u(r0, r1, im)		_rshi_u(_jit, r0, r1, im)
static void _rshi_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define negr(r0, r1)			NEG(r0, r1)
#  define comr(r0, r1)			NOT(r0, r1)
#  define andr(r0, r1, r2)		AND(r0, r1, r2)
#  define andi(r0, r1, im)		_andi(_jit, r0, r1, im)
static void _andi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define orr(r0, r1, r2)		OR(r0, r1, r2)
#  define ori(r0, r1, im)		_ori(_jit, r0, r1, im)
static void _ori(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define xorr(r0, r1, r2)		XOR(r0, r1, r2)
#  define xori(r0, r1, im)		_xori(_jit, r0, r1, im)
static void _xori(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldr_c(r0, r1)			LB(r0, r1, 0)
#  define ldi_c(r0, im)			_ldi_c(_jit, r0, im)
static void _ldi_c(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_uc(r0, r1)		LBU(r0, r1, 0)
#  define ldi_uc(r0, im)		_ldi_uc(_jit, r0, im)
static void _ldi_uc(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_s(r0, r1)			LH(r0, r1, 0)
#  define ldi_s(r0, im)			_ldi_s(_jit, r0, im)
static void _ldi_s(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_us(r0, r1)		LHU(r0, r1, 0)
#  define ldi_us(r0, im)		_ldi_us(_jit, r0, im)
static void _ldi_us(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_i(r0, r1)			LW(r0, r1, 0)
#  define ldi_i(r0, im)			_ldi_i(_jit, r0, im)
static void _ldi_i(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_ui(r0, r1)		LWU(r0, r1, 0)
#  define ldi_ui(r0, im)		_ldi_ui(_jit, r0, im)
static void _ldi_ui(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldr_l(r0, r1)			LD(r0, r1, 0)
#  define ldi_l(r0, im)			_ldi_l(_jit, r0, im)
static void _ldi_l(jit_state_t*,jit_int32_t,jit_word_t);
#  define ldxr_c(r0, r1, r2)		_ldxr_c(_jit, r0, r1, r2)
static void _ldxr_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_c(r0, r1, im)		_ldxi_c(_jit, r0, r1, im)
static void _ldxi_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_uc(r0, r1, r2)		_ldxr_uc(_jit, r0, r1, r2)
static void _ldxr_uc(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_uc(r0, r1, im)		_ldxi_uc(_jit, r0, r1, im)
static void _ldxi_uc(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_s(r0, r1, r2)		_ldxr_s(_jit, r0, r1, r2)
static void _ldxr_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_s(r0, r1, im)		_ldxi_s(_jit, r0, r1, im)
static void _ldxi_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_us(r0, r1, r2)		_ldxr_us(_jit, r0, r1, r2)
static void _ldxr_us(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_us(r0, r1, im)		_ldxi_us(_jit, r0, r1, im)
static void _ldxi_us(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_i(r0, r1, r2)		_ldxr_i(_jit, r0, r1, r2)
static void _ldxr_i(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_i(r0, r1, im)		_ldxi_i(_jit, r0, r1, im)
static void _ldxi_i(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_ui(r0, r1, r2)		_ldxr_ui(_jit, r0, r1, r2)
static void _ldxr_ui(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_ui(r0, r1, im)		_ldxi_ui(_jit, r0, r1, im)
static void _ldxi_ui(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ldxr_l(r0, r1, r2)		_ldxr_l(_jit, r0, r1, r2)
static void _ldxr_l(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define ldxi_l(r0, r1, im)		_ldxi_l(_jit, r0, r1, im)
static void _ldxi_l(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define str_c(r0, r1)			SB(r0, r1, 0)
#  define sti_c(im, r0)			_sti_c(_jit, im, r0)
static void _sti_c(jit_state_t*,jit_word_t,jit_int32_t);
#  define str_s(r0, r1)			SH(r0, r1, 0)
#  define sti_s(im, r0)			_sti_s(_jit, im, r0)
static void _sti_s(jit_state_t*,jit_word_t,jit_int32_t);
#  define str_i(r0, r1)			SW(r0, r1, 0)
#  define sti_i(im, r0)			_sti_i(_jit, im, r0)
static void _sti_i(jit_state_t*,jit_word_t,jit_int32_t);
#  define str_l(r0, r1)			SD(r0, r1, 0)
#  define sti_l(im, r0)			_sti_l(_jit, im, r0)
static void _sti_l(jit_state_t*,jit_word_t,jit_int32_t);
#  define stxr_c(r0, r1, r2)		_stxr_c(_jit, r0, r1, r2)
static void _stxr_c(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_c(i0, r0, r1)		_stxi_c(_jit, i0, r0, r1)
static void _stxi_c(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define stxr_s(r0, r1, r2)		_stxr_s(_jit, r0, r1, r2)
static void _stxr_s(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_s(i0, r0, r1)		_stxi_s(_jit, i0, r0, r1)
static void _stxi_s(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define stxr_i(r0, r1, r2)		_stxr_i(_jit, r0, r1, r2)
static void _stxr_i(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_i(i0, r0, r1)		_stxi_i(_jit, i0, r0, r1)
static void _stxi_i(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define stxr_l(r0, r1, r2)		_stxr_l(_jit, r0, r1, r2)
static void _stxr_l(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define stxi_l(i0, r0, r1)		_stxi_l(_jit, i0, r0, r1)
static void _stxi_l(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define htonr_us(r0, r1)		_htonr_us(_jit, r0, r1)
static void _htonr_us(jit_state_t*,jit_int32_t,jit_int32_t);
#  define htonr_ui(r0, r1)		_htonr_ui(_jit, r0, r1)
static void _htonr_ui(jit_state_t*,jit_int32_t,jit_int32_t);
#  define htonr_ul(r0, r1)		_htonr_ul(_jit, r0, r1)
static void _htonr_ul(jit_state_t*,jit_int32_t,jit_int32_t);
#  define extr_c(r0, r1)		_extr_c(_jit, r0, r1)
static void _extr_c(jit_state_t*,jit_int32_t,jit_int32_t);
#  define extr_uc(r0, r1)		andi(r0, r1, 0xff)
#  define extr_s(r0, r1)		_extr_s(_jit, r0, r1)
static void _extr_s(jit_state_t*,jit_int32_t,jit_int32_t);
#  define extr_us(r0, r1)		_extr_us(_jit, r0, r1)
static void _extr_us(jit_state_t*,jit_int32_t,jit_int32_t);
#  define extr_i(r0, r1)		SEXT_W(r0, r1)
#  define extr_ui(r0, r1)		_extr_ui(_jit, r0, r1)
static void _extr_ui(jit_state_t*,jit_int32_t,jit_int32_t);
#  define movr(r0, r1)			MV(r0, r1)
#  define movi(r0, im)			_movi(_jit, r0, im)
static void _movi(jit_state_t*,jit_int32_t,jit_word_t);
#  define movi_p(r0, im)		_movi_p(_jit, r0, im)
static jit_word_t _movi_p(jit_state_t*,jit_int32_t,jit_word_t);
#  define ltr(r0, r1, r2)		SLT(r0, r1, r2)
#  define lti(r0, r1, im)		_lti(_jit, r0, r1, im)
static void _lti(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ltr_u(r0, r1, r2)		SLTU(r0, r1, r2)
#  define lti_u(r0, r1, im)		_lti_u(_jit, r0, r1, im)
static void _lti_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ler(r0, r1, r2)		_ler(_jit, r0, r1, r2)
static void _ler(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define lei(r0, r1, im)		_lei(_jit, r0, r1, im)
static void _lei(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ler_u(r0, r1, r2)		_ler_u(_jit, r0, r1, r2)
static void _ler_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define lei_u(r0, r1, im)		_lei_u(_jit, r0, r1, im)
static void _lei_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define eqr(r0, r1, r2)		_eqr(_jit, r0, r1, r2)
static void _eqr(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define eqi(r0, r1, im)		_eqi(_jit, r0, r1, im)
static void _eqi(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ger(r0, r1, r2)		_ger(_jit, r0, r1, r2)
static void _ger(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define gei(r0, r1, r2)		_gei(_jit, r0, r1, r2)
static void _gei(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ger_u(r0, r1, r2)		_ger_u(_jit, r0, r1, r2)
static void _ger_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define gei_u(r0, r1, im)		_gei_u(_jit, r0, r1, im)
static void _gei_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define gtr(r0, r1, r2)		SLT(r0, r2, r1)
#  define gti(r0, r1, im)		_gti(_jit, r0, r1, im)
static void _gti(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define gtr_u(r0, r1, r2)		SLTU(r0, r2, r1)
#  define gti_u(r0, r1, im)		_gti_u(_jit, r0, r1, im)
static void _gti_u(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define ner(r0, r1, r2)		_ner(_jit, r0, r1, r2)
static void _ner(jit_state_t*,jit_int32_t,jit_int32_t,jit_int32_t);
#  define nei(r0, r1, im)		_nei(_jit, r0, r1, im)
static void _nei(jit_state_t*,jit_int32_t,jit_int32_t,jit_word_t);
#  define bltr(br, r0, r1)		_bltr(_jit, br, r0, r1)
static jit_word_t _bltr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define blti(br, r0, im)		_blti(_jit, br, r0, im)
static jit_word_t _blti(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bltr_u(br, r0, r1)		_bltr_u(_jit, br, r0, r1)
static jit_word_t _bltr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define blti_u(br, r0, im)		_blti_u(_jit, br, r0, im)
static jit_word_t _blti_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bler(br, r0, r1)		_bler(_jit, br, r0, r1)
static jit_word_t _bler(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define blei(br, r0, im)		_blei(_jit, br, r0, im)
static jit_word_t _blei(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bler_u(br, r0, r1)		_bler_u(_jit, br, r0, r1)
static jit_word_t _bler_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define blei_u(br, r0, im)		_blei_u(_jit, br, r0, im)
static jit_word_t _blei_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define beqr(br, r0, r1)		_beqr(_jit, br, r0, r1)
static jit_word_t _beqr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define beqi(br, r0, im)		_beqi(_jit, br, r0, im)
static jit_word_t _beqi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bger(br, r0, r1)		_bger(_jit, br, r0, r1)
static jit_word_t _bger(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bgei(br, r0, im)		_bgei(_jit, br, r0, im)
static jit_word_t _bgei(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bger_u(br, r0, r1)		_bger_u(_jit, br, r0, r1)
static jit_word_t _bger_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bgei_u(br, r0, im)		_bgei_u(_jit, br, r0, im)
static jit_word_t _bgei_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bgtr(br, r0, r1)		_bgtr(_jit, br, r0, r1)
static jit_word_t _bgtr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bgti(br, r0, im)		_bgti(_jit, br, r0, im)
static jit_word_t _bgti(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bgtr_u(br, r0, r1)		_bgtr_u(_jit, br, r0, r1)
static jit_word_t _bgtr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bgti_u(br, r0, im)		_bgti_u(_jit, br, r0, im)
static jit_word_t _bgti_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bner(br, r0, r1)		_bner(_jit, br, r0, r1)
static jit_word_t _bner(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
#  define bnei(br, r0, im)		_bnei(_jit, br, r0, im)
static jit_word_t _bnei(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define boaddr(br, r0, r1)		_boaddr(_jit, br, r0, r1)
#  define boaddi(br, r0, im)		_boaddi(_jit, br, r0, im)
static jit_word_t _boaddr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
static jit_word_t _boaddi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define boaddr_u(br, r0, r1)		_boaddr_u(_jit, br, r0, r1)
#  define boaddi_u(br, r0, im)		_boaddi_u(_jit, br, r0, im)
static jit_word_t _boaddr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
static jit_word_t _boaddi_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bxaddr(br, r0, r1)		_bxaddr(_jit, br, r0, r1)
#  define bxaddi(br, r0, im)		_bxaddi(_jit, br, r0, im)
static jit_word_t _bxaddr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
static jit_word_t _bxaddi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bxaddr_u(br, r0, r1)		_bxaddr_u(_jit, br, r0, r1)
#  define bxaddi_u(br, r0, im)		_bxaddi_u(_jit, br, r0, im)
static jit_word_t _bxaddr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
static jit_word_t _bxaddi_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bosubr(br, r0, r1)		_bosubr(_jit, br, r0, r1)
#  define bosubi(br, r0, im)		_bosubi(_jit, br, r0, im)
static jit_word_t _bosubr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
static jit_word_t _bosubi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bosubr_u(br, r0, r1)		_bosubr_u(_jit, br, r0, r1)
#  define bosubi_u(br, r0, im)		_bosubi_u(_jit, br, r0, im)
static jit_word_t _bosubr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
static jit_word_t _bosubi_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bxsubr(br, r0, r1)		_bxsubr(_jit, br, r0, r1)
#  define bxsubi(br, r0, im)		_bxsubi(_jit, br, r0, im)
static jit_word_t _bxsubr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
static jit_word_t _bxsubi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bxsubr_u(br, r0, r1)		_bxsubr_u(_jit, br, r0, r1)
#  define bxsubi_u(br, r0, im)		_bxsubi_u(_jit, br, r0, im)
static jit_word_t _bxsubr_u(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
static jit_word_t _bxsubi_u(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bmsr(br, r0, r1)		_bmsr(_jit, br, r0, r1)
#  define bmsi(br, r0, im)		_bmsi(_jit, br, r0, im)
static jit_word_t _bmsr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
static jit_word_t _bmsi(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define bmcr(br, r0, r1)		_bmcr(_jit, br, r0, r1)
#  define bmci(br, r0, im)		_bmci(_jit, br, r0, im)
static jit_word_t _bmcr(jit_state_t*,jit_word_t,jit_int32_t,jit_int32_t);
static jit_word_t _bmci(jit_state_t*,jit_word_t,jit_int32_t,jit_word_t);
#  define jmpr(r0)			JALR(_ZERO_REGNO, r0, 0)
#  define jmpi(im)			_jmpi(_jit, im)
static void _jmpi(jit_state_t*,jit_word_t);
#  define jmpi_p(im)			_jmpi_p(_jit, im)
static jit_word_t _jmpi_p(jit_state_t*,jit_word_t);
#  define callr(r0)			JALR(_RA_REGNO, r0, 0)
#  define calli(im)			_calli(_jit, im)
static void _calli(jit_state_t*,jit_word_t);
#  define calli_p(im)		_calli_p(_jit, im)
static jit_word_t _calli_p(jit_state_t*,jit_word_t);
#  define prolog(i0)			_prolog(_jit,i0)
static void _prolog(jit_state_t*,jit_node_t*);
#  define epilog(i0)			_epilog(_jit,i0)
static void _epilog(jit_state_t*,jit_node_t*);
#  define vastart(r0)			_vastart(_jit, r0)
static void _vastart(jit_state_t*, jit_int32_t);
#  define vaarg(r0, r1)			_vaarg(_jit, r0, r1)
static void _vaarg(jit_state_t*, jit_int32_t, jit_int32_t);
#define patch_abs(instr,label)		_patch_at(_jit,instr,label)
#define patch_at(instr,label)		_patch_at(_jit,instr,label)
static void _patch_at(jit_state_t*,jit_word_t,jit_word_t);
#endif		/* PROTO */

#if CODE
static void
_Rtype(jit_state_t *_jit, jit_int32_t op, jit_int32_t rd,
       jit_int32_t fct, jit_int32_t rs1, jit_int32_t rs2, jit_int32_t fct2)
{
    instr_t	i;
    assert(!(op   & ~0x7f));
    assert(!(rd   & ~0x1f));
    assert(!(fct  & ~0x07));
    assert(!(rs1  & ~0x1f));
    assert(!(rs2  & ~0x1f));
    assert(!(fct2 & ~0x7f));
    i.R.opcode	= op;
    i.R.rd	= rd;
    i.R.funct3	= fct;
    i.R.rs1	= rs1;
    i.R.rs2	= rs2;
    i.R.funct7	= fct2;
    ii(i.w);
}

static void
_R4type(jit_state_t *_jit, jit_int32_t op, jit_int32_t rd, jit_int32_t fct,
	jit_int32_t rs1, jit_int32_t rs2, jit_int32_t fct2, jit_int32_t rs3)
{
    instr_t	i;
    assert(!(op   & ~0x7f));
    assert(!(rd   & ~0x1f));
    assert(!(fct  & ~0x07));
    assert(!(rs1  & ~0x1f));
    assert(!(rs2  & ~0x1f));
    assert(!(fct2 & ~0x03));
    assert(!(rs3  & ~0x1f));
    i.R4.opcode	= op;
    i.R4.rd	= rd;
    i.R4.funct3	= fct;
    i.R4.rs1	= rs1;
    i.R4.rs2	= rs2;
    i.R4.funct2	= fct2;
    i.R4.rs3	= rs3;
    ii(i.w);
}

static void
_Itype(jit_state_t *_jit, jit_int32_t op, jit_int32_t rd,
       jit_int32_t fct, jit_int32_t rs1, jit_int32_t imm)
{
    instr_t	i;
    assert(!(op  &  ~0x7f));
    assert(!(rd  &  ~0x1f));
    assert(!(fct &  ~0x07));
    assert(!(rs1 &  ~0x1f));
    assert(simm12_p(imm));
    i.I.opcode	= op;
    i.I.rd	= rd;
    i.I.funct3	= fct;
    i.I.rs1	= rs1;
    i.I.imm11_0	= imm;
    ii(i.w);
}

#  if __WORDSIZE == 64
static void
_IStype(jit_state_t *_jit, jit_int32_t op, jit_int32_t rd,
       jit_int32_t fct, jit_int32_t rs1, jit_int32_t sh, jit_int32_t imm)
{
    instr_t	i;
    assert(!(op  &  ~0x7f));
    assert(!(rd  &  ~0x1f));
    assert(!(fct &  ~0x07));
    assert(!(rs1 &  ~0x1f));
    assert(!(sh  &  ~0x3f));
    assert(simm6_p(imm));
    i.IS.opcode	= op;
    i.IS.rd	= rd;
    i.IS.funct3	= fct;
    i.IS.rs1	= rs1;
    i.IS.shamt	= sh;
    i.IS.imm6_0 = imm;
    ii(i.w);
}
#  endif

static void
_Stype(jit_state_t *_jit, jit_int32_t op, jit_int32_t fct,
       jit_int32_t rs1, jit_int32_t rs2, jit_int32_t imm)
{
    instr_t	i;
    assert(!(op  &  ~0x7f));
    assert(!(fct &  ~0x07));
    assert(!(rs1 &  ~0x1f));
    assert(!(rs2 &  ~0x1f));
    assert(simm12_p(imm));
    i.S.opcode	= op;
    i.S.imm4_0	= imm & 0x1f;
    i.S.funct3	= fct;
    i.S.rs1	= rs1;
    i.S.rs2	= rs2;
    i.S.imm11_5	= (imm >> 5) & 0x7f;
    ii(i.w);
}

static void
_Btype(jit_state_t *_jit, jit_int32_t op, jit_int32_t fct,
       jit_int32_t rs1, jit_int32_t rs2, jit_int32_t imm)
{
    instr_t	i;
    assert(!(op  & ~0x7f));
    assert(!(fct & ~0x07));
    assert(!(rs1 & ~0x1f));
    assert(!(rs2 & ~0x1f));
    assert(!(imm & 1) && simm12_p(imm));
    i.B.opcode	= op;
    i.B.imm11	= (imm >> 11) & 0x1;
    i.B.imm4_1	= (imm >>  1) & 0xf;
    i.B.funct3	= fct;
    i.B.rs1	= rs1;
    i.B.rs2	= rs2;
    i.B.imm10_5	= (imm >>  5) & 0x3f;
    i.B.imm12	= (imm >> 12) & 0x1;
    ii(i.w);
}

static void
_Utype(jit_state_t *_jit, jit_int32_t op, jit_int32_t rd, jit_int32_t imm)
{
    instr_t	i;
    assert(!(op	& ~0x7f));
    assert(!(rd	& ~0x1f));
    assert(simm20_p(imm));
    i.U.opcode	= op;
    i.U.rd	= rd;
    i.U.imm12_31= imm;
    ii(i.w);
}

static void
_Jtype(jit_state_t *_jit, jit_int32_t op, jit_int32_t rd, jit_int32_t imm)
{
    instr_t	i;
    assert(!(op & ~0x7f));
    assert(!(rd & ~0x1f));
    assert(!(imm & 1) && imm <= 1048575 && imm >= -1048576);
    i.J.opcode	= op;
    i.J.rd	= rd;
    i.J.imm19_12= (imm >> 12) &  0xff;
    i.J.imm11	= (imm >> 11) &   0x1;
    i.J.imm10_1	= (imm >>  1) & 0x3ff;
    i.J.imm20	= (imm >> 20) &   0x1;
    ii(i.w);
}

static void
_nop(jit_state_t *_jit, jit_int32_t im)
{
    for (; im > 0; im -= 4)
	NOP();
    assert(im == 0);
}

static void
_addi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (simm12_p(i0))
	ADDI(r0, r1, i0);
    else {
	jit_int32_t	t0;
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i0);
	addr(r0, r1, rn(t0));
	jit_unget_reg(t0);
    }
}

static void
_addcr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		t0;
    if (jit_carry == _NOREG)
	jit_carry = jit_get_reg(jit_class_gpr);
    if (r0 == r1) {
	t0 = jit_get_reg(jit_class_gpr);
	addr(rn(t0), r1, r2);
	SLTU(rn(jit_carry), rn(t0), r1);
	movr(r0, rn(t0));
	jit_unget_reg(t0);
    }
    else {
	addr(r0, r1, r2);
	SLTU(rn(jit_carry), r0, r1);
    }
}

static void
_addci(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    if (jit_carry == _NOREG)
	jit_carry = jit_get_reg(jit_class_gpr);
    if (r0 == r1) {
	t0 = jit_get_reg(jit_class_gpr);
	addi(rn(t0), r1, i0);
	SLTU(rn(jit_carry), rn(t0), r1);
	movr(r0, rn(t0));
	jit_unget_reg(t0);
    }
    else {
	addi(r0, r1, i0);
	SLTU(rn(jit_carry), r0, r1);
    }
}

static void
_addxr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		t0;
    assert(jit_carry != _NOREG);
    t0 = jit_get_reg(jit_class_gpr);
    movr(rn(t0), rn(jit_carry));
    addcr(r0, r1, r2);
    addcr(r0, r0, rn(t0));
    jit_unget_reg(t0);
}

static void
_addxi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    assert(jit_carry != _NOREG);
    t0 = jit_get_reg(jit_class_gpr);
    movr(rn(t0), rn(jit_carry));
    addci(r0, r1, i0);
    addcr(r0, r0, rn(t0));
    jit_unget_reg(t0);
}

static void
_subi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (simm12_p(-i0))
	ADDI(r0, r1, -i0);
    else {
	jit_int32_t	t0;
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i0);
	subr(r0, r1, rn(t0));
	jit_unget_reg(t0);
    }
}

static void
_subcr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		t0;
    if (jit_carry == _NOREG)
	jit_carry = jit_get_reg(jit_class_gpr);
    if (r0 == r1) {
	t0 = jit_get_reg(jit_class_gpr);
	subr(rn(t0), r1, r2);
	SLTU(rn(jit_carry), r1, rn(t0));
	movr(r0, rn(t0));
	jit_unget_reg(t0);
    }
    else {
	subr(r0, r1, r2);
	SLTU(rn(jit_carry), r1, r0);
    }
}

static void
_subci(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    if (jit_carry == _NOREG)
	jit_carry = jit_get_reg(jit_class_gpr);
    if (r0 == r1) {
	t0 = jit_get_reg(jit_class_gpr);
	subi(rn(t0), r1, i0);
	SLTU(rn(jit_carry), r1, rn(t0));
	movr(r0, rn(t0));
	jit_unget_reg(t0);
    }
    else {
	subi(r0, r1, i0);
	SLTU(rn(jit_carry), r1, r0);
    }
}

static void
_subxr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    jit_int32_t		t0;
    assert(jit_carry != _NOREG);
    t0 = jit_get_reg(jit_class_gpr);
    movr(rn(t0), rn(jit_carry));
    subcr(r0, r1, r2);
    subcr(r0, r0, rn(t0));
    jit_unget_reg(t0);
}

static void
_subxi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    assert(jit_carry != _NOREG);
    t0 = jit_get_reg(jit_class_gpr);
    movr(rn(t0), rn(jit_carry));
    subci(r0, r1, i0);
    subcr(r0, r0, rn(t0));
    jit_unget_reg(t0);
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
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr);
    movi(rn(t0), i0);
    mulr(r0, r1, rn(t0));
    jit_unget_reg(t0);
}

static void
_divi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr);
    movi(rn(t0), i0);
    divr(r0, r1, rn(t0));
    jit_unget_reg(t0);
}

static void
_divi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr);
    movi(rn(t0), i0);
    divr_u(r0, r1, rn(t0));
    jit_unget_reg(t0);
}

static void
_remi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr);
    movi(rn(t0), i0);
    remr(r0, r1, rn(t0));
    jit_unget_reg(t0);
}

static void
_remi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr);
    movi(rn(t0), i0);
    remr_u(r0, r1, rn(t0));
    jit_unget_reg(t0);
}

static void
_qmulr(jit_state_t *_jit, jit_int32_t r0,
       jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    jit_int32_t		t0;
    if (r0 == r2 || r0 == r3) {
	t0 = jit_get_reg(jit_class_gpr);
	mulr(rn(t0), r2, r3);
    }
    else
	mulr(r0, r2, r3);
    MULH(r1, r2, r3);
    if (r0 == r2 || r0 == r3) {
	movr(r0, rn(t0));
	jit_unget_reg(t0);
    }
}

static void
_qmuli(jit_state_t *_jit, jit_int32_t r0,
       jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr);
    movi(rn(t0), i0);
    qmulr(r0, r1, r2, rn(t0));
    jit_unget_reg(t0);
}

static void
_qmulr_u(jit_state_t *_jit, jit_int32_t r0,
	 jit_int32_t r1, jit_int32_t r2, jit_int32_t r3)
{
    jit_int32_t		t0;
    if (r0 == r2 || r0 == r3) {
	t0 = jit_get_reg(jit_class_gpr);
	mulr(rn(t0), r2, r3);
    }
    else
	mulr(r0, r2, r3);
    MULHU(r1, r2, r3);
    if (r0 == r2 || r0 == r3) {
	movr(r0, rn(t0));
	jit_unget_reg(t0);
    }
}

static void
_qmuli_u(jit_state_t *_jit, jit_int32_t r0,
	 jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr);
    movi(rn(t0), i0);
    qmulr_u(r0, r1, r2, rn(t0));
    jit_unget_reg(t0);
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
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr);
    movi(rn(t0), i0);
    qdivr(r0, r1, r2, rn(t0));
    jit_unget_reg(t0);
}

static void
_qdivi_u(jit_state_t *_jit, jit_int32_t r0,
	 jit_int32_t r1, jit_int32_t r2, jit_word_t i0)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr);
    movi(rn(t0), i0);
    qdivr_u(r0, r1, r2, rn(t0));
    jit_unget_reg(t0);
}

static void
_lshi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (i0 == 0)
	movr(r0, r1);
    else {
	assert(i0 > 0 && i0 < 64);
	SLLI(r0, r1, i0);
    }
}

static void
_rshi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (i0 == 0)
	movr(r0, r1);
    else {
	assert(i0 > 0 && i0 < 64);
	SRAI(r0, r1, i0);
    }
}

static void
_rshi_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (i0 == 0)
	movr(r0, r1);
    else {
	assert(i0 > 0 && i0 < 64);
	SRLI(r0, r1, i0);
    }
}

static void
_andi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (simm12_p(i0))
	ANDI(r0, r1, i0);
    else {
	jit_int32_t	t0;
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i0);
	andr(r0, r1, rn(t0));
	jit_unget_reg(t0);
    }
}

static void
_ori(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (simm12_p(i0))
	ORI(r0, r1, i0);
    else {
	jit_int32_t	t0;
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i0);
	orr(r0, r1, rn(t0));
	jit_unget_reg(t0);
    }
}

static void
_xori(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (simm12_p(i0))
	XORI(r0, r1, i0);
    else {
	jit_int32_t	t0;
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i0);
	xorr(r0, r1, rn(t0));
	jit_unget_reg(t0);
    }
}

#  define DEFLD(T,O)							\
static void								\
_ldi_##T(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)		\
{									\
    if (simm12_p(i0))							\
	L##O(r0, _ZERO_REGNO, i0);					\
    else {								\
	jit_int32_t	t0;						\
	t0 = jit_get_reg(jit_class_gpr);				\
	movi(rn(t0), i0);						\
	ldr_##T(r0, rn(t0));						\
	jit_unget_reg(t0);						\
    }									\
}									\
									\
static void								\
_ldxr_##T(jit_state_t *_jit,jit_int32_t r0,jit_int32_t r1,jit_int32_t r2)\
{									\
    jit_int32_t	t0;							\
    t0 = jit_get_reg(jit_class_gpr);					\
    addr(rn(t0), r1, r2);						\
    ldr_##T(r0, rn(t0));						\
    jit_unget_reg(t0);							\
}									\
									\
static void								\
_ldxi_##T(jit_state_t *_jit,jit_int32_t r0,jit_int32_t r1,jit_word_t i0)\
{									\
    if (simm12_p(i0))							\
	L##O(r0, r1, i0);						\
    else {								\
	jit_int32_t	t0;						\
	t0 = jit_get_reg(jit_class_gpr);				\
	addi(rn(t0), r1, i0);						\
	ldr_##T(r0, rn(t0));						\
	jit_unget_reg(t0);						\
    }									\
}

DEFLD(c,B)
DEFLD(uc,BU)
DEFLD(s,H)
DEFLD(us,HU)
DEFLD(i,W)
DEFLD(ui,WU)
DEFLD(l,D)

#  define DEFST(T, O)							\
static void								\
_sti_##T(jit_state_t *_jit, jit_word_t i0, jit_int32_t r0)		\
{									\
    if (simm12_p(i0))							\
	S##O(_ZERO_REGNO, r0, i0);					\
    else {								\
	jit_int32_t	t0;						\
	t0 = jit_get_reg(jit_class_gpr);				\
	movi(rn(t0), i0);						\
	str_##T(rn(t0), r0);						\
	jit_unget_reg(t0);						\
    }									\
}									\
									\
static void								\
_stxr_##T(jit_state_t *_jit,jit_int32_t r0,jit_int32_t r1,jit_int32_t r2)\
{									\
    jit_int32_t	t0;							\
    t0 = jit_get_reg(jit_class_gpr);					\
    addr(rn(t0), r0, r1);						\
    str_##T(rn(t0), r2);						\
    jit_unget_reg(t0);							\
}									\
									\
static void								\
_stxi_##T(jit_state_t *_jit,jit_word_t i0,jit_int32_t r0,jit_int32_t r1)\
{									\
    if (simm12_p(i0))							\
	S##O(r0, r1, i0);						\
    else {								\
	jit_int32_t	t0;						\
	t0 = jit_get_reg(jit_class_gpr);				\
	addi(rn(t0), r0, i0);						\
	str_##T(rn(t0), r1);						\
	jit_unget_reg(t0);						\
    }									\
}

DEFST(c, B)
DEFST(s, H)
DEFST(i, W)
DEFST(l, D)

static void
_htonr_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr);
    rshi(rn(t0), r1, 8);
    andi(r0, r1, 0xff);
    andi(rn(t0), rn(t0), 0xff);
    lshi(r0, r0, 8);
    orr(r0, r0, rn(t0));
    jit_unget_reg(t0);
}

static void
_htonr_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0;
    jit_int32_t		t1;
    jit_int32_t		t2;
    t0 = jit_get_reg(jit_class_gpr);
    t1 = jit_get_reg(jit_class_gpr);
    t2 = jit_get_reg(jit_class_gpr);
    rshi(rn(t0), r1, 24);
    rshi(rn(t1), r1, 16);
    rshi(rn(t2), r1,  8);
    andi(rn(t0), rn(t0), 0xff);
    andi(rn(t1), rn(t1), 0xff);
    andi(rn(t2), rn(t2), 0xff);
    andi(r0, r1, 0xff);
    lshi(r0, r0, 24);
    lshi(rn(t1), rn(t1), 8);
    orr(r0, r0, rn(t0));
    lshi(rn(t2), rn(t2), 16);
    orr(r0, r0, rn(t1));
    orr(r0, r0, rn(t2));
    jit_unget_reg(t2);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
}

static void
_htonr_ul(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr);
    rshi_u(rn(t0), r1, 32);
    htonr_ui(r0, r1);
    htonr_ui(rn(t0), rn(t0));
    lshi(r0, r0, 32);
    orr(r0, r0, rn(t0));
    jit_unget_reg(t0);
}

static void
_extr_c(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    lshi(r0, r1, 56);
    rshi(r0, r0, 56);
}

static void
_extr_s(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    lshi(r0, r1, 48);
    rshi(r0, r0, 48);
}

static void
_extr_us(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    lshi(r0, r1, 48);
    rshi_u(r0, r0, 48);
}

static void
_extr_ui(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    lshi(r0, r1, 32);
    rshi_u(r0, r0, 32);
}

static void
_movi(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    if (simm32_p(i0)) {
	jit_int32_t	lo = (jit_int32_t)i0 << 20 >> 20;
	jit_int32_t	hi = i0 - lo;
	if (hi) {
	    LUI(r0, hi >> 12);
	    if (lo)
		ADDIW(r0, r0, lo);
	}
	else
	    ADDIW(r0, _ZERO_REGNO, lo);
    }
    else {
	jit_int32_t	lo = i0 << 32 >> 32;
	jit_word_t	hi = i0 - lo;
	jit_int32_t	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), (jit_int32_t)(hi >> 32));
	movi(r0, lo);
	lshi(rn(t0), rn(t0), 32);
	addr(r0, r0, rn(t0));
	jit_unget_reg(t0);
    }
}

static jit_word_t
_movi_p(jit_state_t *_jit, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		t0;
    jit_int32_t		ww = i0 << 32 >> 32;
    jit_int32_t		lo = ww << 20 >> 20;
    jit_int32_t		hi = ww - lo;
    w = _jit->pc.w;
    t0 = jit_get_reg(jit_class_gpr);
    LUI(r0, hi >> 12);
    ADDIW(r0, r0, lo);
    ww = i0 >> 32;
    lo = ww << 20 >> 20;
    hi = ww - lo;
    LUI(rn(t0), hi >> 12);
    ADDIW(rn(t0), rn(t0), lo);
    SLLI(rn(t0), rn(t0), 32);
    ADD(r0, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static void
_lti(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (simm12_p(i0))
	SLTI(r0, r1, i0);
    else {
	jit_int32_t	t0;
	t0 = jit_get_reg(jit_class_gpr);
	movi(r0, i0);
	ltr(r0, r1, rn(t0));
	jit_unget_reg(t0);
    }
}

static void
_lti_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (simm12_p(i0))
	SLTIU(r0, r1, i0);
    else {
	jit_int32_t	t0;
	t0 = jit_get_reg(jit_class_gpr);
	movi(r0, i0);
	ltr_u(r0, r1, rn(t0));
	jit_unget_reg(t0);
    }
}

static void
_ler(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    SLT(r0, r2, r1);
    XORI(r0, r0, 1);
}

static void
_lei(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    if (i0 == 0) {
	SLT(r0, _ZERO_REGNO, r1);
	XORI(r0, r0, 1);
    }
    else {
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i0);
	ler(r0, r1, rn(t0));
	jit_unget_reg(t0);
    }
}

static void
_ler_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    SLTU(r0, r2, r1);
    XORI(r0, r0, 1);
}

static void
_lei_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    if (i0 == 0) {
	SLTU(r0, _ZERO_REGNO, r1);
	XORI(r0, r0, 1);
    }
    else {
	t0 = jit_get_reg(jit_class_gpr);
	movi(rn(t0), i0);
	ler_u(r0, r1, rn(t0));
	jit_unget_reg(t0);
    }
}

static void
_eqr(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    subr(r0, r1, r2);
    SLTU(r0, _ZERO_REGNO, r0);
    XORI(r0, r0, 1);
}

static void
_eqi(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (i0) {
	subi(r0, r1, i0);
	SLTU(r0, _ZERO_REGNO, r0);
    }
    else
	SLTU(r0, _ZERO_REGNO, r1);
    XORI(r0, r0, 1);
}

static void
_ger(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    SLT(r0, r1, r2);
    XORI(r0, r0, 1);
}

static void
_gei(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr);
    movi(rn(t0), i0);
    ger(r0, r1, rn(t0));
    jit_unget_reg(t0);
}

static void
_ger_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    SLTU(r0, r1, r2);
    XORI(r0, r0, 1);
}

static void
_gei_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr);
    movi(rn(t0), i0);
    ger_u(r0, r1, rn(t0));
    jit_unget_reg(t0);
}

static void
_gti(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t	t0;
    t0 = jit_get_reg(jit_class_gpr);
    movi(r0, i0);
    ltr(r0, rn(t0), r1);
    jit_unget_reg(t0);
}

static void
_gti_u(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    jit_int32_t	t0;
    t0 = jit_get_reg(jit_class_gpr);
    movi(r0, i0);
    ltr_u(r0, rn(t0), r1);
    jit_unget_reg(t0);
}

static void
_ner(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_int32_t r2)
{
    subr(r0, r1, r2);
    SLTU(r0, _ZERO_REGNO, r0);
}

static void
_nei(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1, jit_word_t i0)
{
    if (i0) {
	subi(r0, r1, i0);
	SLTU(r0, _ZERO_REGNO, r0);
    }
    else
	SLTU(r0, _ZERO_REGNO, r1);
}

static jit_word_t
_bltr(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    w = _jit->pc.w;
    BLT(r0, r1, br - w);
    return (w);
}

static jit_word_t
_blti(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_reg_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i0);
    w = bltr(br, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bltr_u(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    w = _jit->pc.w;
    BLTU(r0, r1, br - w);
    return (w);
}

static jit_word_t
_blti_u(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_reg_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i0);
    w = bltr_u(br, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bler(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    w = _jit->pc.w;
    BGE(r1, r0, br - w);
    return (w);
}

static jit_word_t
_blei(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_reg_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i0);
    w = bler(br, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bler_u(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    w = _jit->pc.w;
    BGEU(r1, r0, br - w);
    return (w);
}

static jit_word_t
_blei_u(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_reg_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i0);
    w = bler_u(br, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_beqr(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    w = _jit->pc.w;
    BEQ(r1, r0, br - w);
    return (w);
}

static jit_word_t
_beqi(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_reg_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i0);
    w = beqr(br, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bger(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    w = _jit->pc.w;
    BGE(r0, r1, br - w);
    return (w);
}

static jit_word_t
_bgei(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_reg_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i0);
    w = bger(br, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bger_u(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    w = _jit->pc.w;
    BGEU(r0, r1, br - w);
    return (w);
}

static jit_word_t
_bgei_u(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_reg_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i0);
    w = bger_u(br, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bgtr(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    w = _jit->pc.w;
    BLT(r1, r0, br - w);
    return (w);
}

static jit_word_t
_bgti(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_reg_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i0);
    w = bgtr(br, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bgtr_u(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    w = _jit->pc.w;
    BLTU(r1, r0, br - w);
    return (w);
}

static jit_word_t
_bgti_u(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_reg_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i0);
    w = bgtr_u(br, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bner(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    w = _jit->pc.w;
    BNE(r1, r0, br - w);
    return (w);
}

static jit_word_t
_bnei(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_reg_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i0);
    w = bner(br, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_boaddr(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w, jal;
    jit_int32_t		t0, t1;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    /* t0 = r1 < 0; */
    SLT(rn(t0), r1, _ZERO_REGNO);
    /* t1 = r0 */
    movr(rn(t1), r0);
    /* r0 = r0 + r1 */
    addr(r0, r0, r1);
    /* overflow = r1 < 0 ? t1 < r0 : r0 < t1 */
    w = _jit->pc.w;
    BNE(rn(t0), _ZERO_REGNO, 0);
    /* r1 >= 0 */
    SLT(rn(t1), r0, rn(t1));
    jal = _jit->pc.w;
    JAL(_ZERO_REGNO, 0);
    /* r1 < 0 */
    patch_at(w, _jit->pc.w);
    SLT(rn(t1), rn(t1), r0);
    /**/
    patch_at(jal, _jit->pc.w);
    w = _jit->pc.w;
    BNE(rn(t1), _ZERO_REGNO, br - w);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_boaddi(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i0);
    w = boaddr(br, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_boaddr_u(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		t0, t1;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    addr(rn(t0), r0, r1);
    SLTU(rn(t1), rn(t0), r0);
    movr(r0, rn(t0));
    w = _jit->pc.w;
    BNE(_ZERO_REGNO, rn(t1), br - w);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_boaddi_u(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i0);
    w = boaddr_u(br, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bxaddr(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w, jal;
    jit_int32_t		t0, t1;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    /* t0 = r1 < 0; */
    SLT(rn(t0), r1, _ZERO_REGNO);
    /* t1 = r0 */
    movr(rn(t1), r0);
    /* r0 = r0 + r1 */
    addr(r0, r0, r1);
    /* overflow = r1 < 0 ? t1 < r0 : r0 < t1 */
    w = _jit->pc.w;
    BNE(rn(t0), _ZERO_REGNO, 0);
    /* r1 >= 0 */
    SLT(rn(t1), r0, rn(t1));
    jal = _jit->pc.w;
    JAL(_ZERO_REGNO, 0);
    /* r1 < 0 */
    patch_at(w, _jit->pc.w);
    SLT(rn(t1), rn(t1), r0);
    /**/
    patch_at(jal, _jit->pc.w);
    w = _jit->pc.w;
    BEQ(rn(t1), _ZERO_REGNO, br - w);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bxaddi(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i0);
    w = bxaddr(br, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bxaddr_u(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		t0, t1;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    addr(rn(t0), r0, r1);
    SLTU(rn(t1), rn(t0), r0);
    movr(r0, rn(t0));
    w = _jit->pc.w;
    BEQ(_ZERO_REGNO, rn(t1), br - w);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bxaddi_u(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i0);
    w = bxaddr_u(br, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bosubr(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w, jal;
    jit_int32_t		t0, t1;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    /* t0 = 0 < r1; */
    SLT(rn(t0), _ZERO_REGNO, r1);
    /* t1 = r0 */
    movr(rn(t1), r0);
    /* r0 = r0 - r1 */
    subr(r0, r0, r1);
    /* overflow = r1 < 0 ? t1 < r0 : r0 < t1 */
    w = _jit->pc.w;
    BNE(rn(t0), _ZERO_REGNO, 0);
    /* r1 >= 0 */
    SLT(rn(t1), r0, rn(t1));
    jal = _jit->pc.w;
    JAL(_ZERO_REGNO, 0);
    /* r1 < 0 */
    patch_at(w, _jit->pc.w);
    SLT(rn(t1), rn(t1), r0);
    /**/
    patch_at(jal, _jit->pc.w);
    w = _jit->pc.w;
    BNE(rn(t1), _ZERO_REGNO, br - w);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bosubi(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i0);
    w = bosubr(br, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bosubr_u(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		t0, t1;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    subr(rn(t0), r0, r1);
    SLTU(rn(t1), r0, rn(t0));
    movr(r0, rn(t0));
    w = _jit->pc.w;
    BNE(_ZERO_REGNO, rn(t1), br - w);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bosubi_u(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i0);
    w = bosubr_u(br, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bxsubr(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w, jal;
    jit_int32_t		t0, t1;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    /* t0 = r1 < 0; */
    SLT(rn(t0), _ZERO_REGNO, r1);
    /* t1 = r0 */
    movr(rn(t1), r0);
    /* r0 = r0 - r1 */
    subr(r0, r0, r1);
    /* overflow = r1 < 0 ? t1 < r0 : r0 < t1 */
    w = _jit->pc.w;
    BNE(rn(t0), _ZERO_REGNO, 0);
    /* r1 >= 0 */
    SLT(rn(t1), r0, rn(t1));
    jal = _jit->pc.w;
    JAL(_ZERO_REGNO, 0);
    /* r1 < 0 */
    patch_at(w, _jit->pc.w);
    SLT(rn(t1), rn(t1), r0);
    /**/
    patch_at(jal, _jit->pc.w);
    w = _jit->pc.w;
    BEQ(rn(t1), _ZERO_REGNO, br - w);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bxsubi(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i0);
    w = bxsubr(br, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bxsubr_u(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		t0, t1;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    t1 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    subr(rn(t0), r0, r1);
    SLTU(rn(t1), r0, rn(t0));
    movr(r0, rn(t0));
    w = _jit->pc.w;
    BEQ(_ZERO_REGNO, rn(t1), br - w);
    jit_unget_reg(t1);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bxsubi_u(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i0);
    w = bxsubr_u(br, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bmsr(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    AND(rn(t0), r0, r1);
    w = _jit->pc.w;
    BNE(_ZERO_REGNO, rn(t0), br - w);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bmsi(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i0);
    w = bmsr(br, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bmcr(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_int32_t r1)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    AND(rn(t0), r0, r1);
    w = _jit->pc.w;
    BEQ(_ZERO_REGNO, rn(t0), br - w);
    jit_unget_reg(t0);
    return (w);
}

static jit_word_t
_bmci(jit_state_t *_jit, jit_word_t br, jit_int32_t r0, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    movi(rn(t0), i0);
    w = bmcr(br, r0, rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static void
_jmpi(jit_state_t *_jit, jit_word_t i0)
{
    jit_int32_t		t0;
    jit_word_t		dsp;
    dsp = i0 - _jit->pc.w;
    if (simm20_p(dsp))
	JAL(_ZERO_REGNO, dsp);
    else {
	t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(t0), i0);
	jmpr(rn(t0));
	jit_unget_reg(t0);
    }
}

static jit_word_t
_jmpi_p(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    w = movi_p(rn(t0), i0);
    jmpr(rn(t0));
    jit_unget_reg(t0);
    return (w);
}

static void
_calli(jit_state_t *_jit, jit_word_t i0)
{
    jit_int32_t		t0;
    jit_word_t		dsp;
    dsp = i0 - _jit->pc.w;
    if (simm20_p(dsp))
	JAL(_RA_REGNO, dsp);
    else {
	t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
	movi(rn(t0), i0);
	callr(rn(t0));
	jit_unget_reg(t0);
    }
}

static jit_word_t
_calli_p(jit_state_t *_jit, jit_word_t i0)
{
    jit_word_t		w;
    jit_int32_t		t0;
    t0 = jit_get_reg(jit_class_gpr|jit_class_nospill);
    w = movi_p(rn(t0), i0);
    callr(rn(t0));
    jit_unget_reg(t0);
    return (w);
}

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
    subi(_SP_REGNO, _SP_REGNO, stack_framesize);
    stxi(0, _SP_REGNO, _RA_REGNO);
    stxi(8, _SP_REGNO, _FP_REGNO);
    if (jit_regset_tstbit(&_jitc->function->regset, _S1))
	stxi(16, _SP_REGNO, 9);
    if (jit_regset_tstbit(&_jitc->function->regset, _S2))
	stxi(24, _SP_REGNO, 18);
    if (jit_regset_tstbit(&_jitc->function->regset, _S3))
	stxi(32, _SP_REGNO, 19);
    if (jit_regset_tstbit(&_jitc->function->regset, _S4))
	stxi(40, _SP_REGNO, 20);
    if (jit_regset_tstbit(&_jitc->function->regset, _S5))
	stxi(48, _SP_REGNO, 21);
    if (jit_regset_tstbit(&_jitc->function->regset, _S6))
	stxi(56, _SP_REGNO, 22);
    if (jit_regset_tstbit(&_jitc->function->regset, _S7))
	stxi(64, _SP_REGNO, 23);
    if (jit_regset_tstbit(&_jitc->function->regset, _S8))
	stxi(72, _SP_REGNO, 24);
    if (jit_regset_tstbit(&_jitc->function->regset, _S9))
	stxi(80, _SP_REGNO, 25);
    if (jit_regset_tstbit(&_jitc->function->regset, _S10))
	stxi(88, _SP_REGNO, 26);
    if (jit_regset_tstbit(&_jitc->function->regset, _S11))
	stxi(96, _SP_REGNO, 27);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS0))
	stxi_d(104, _SP_REGNO, 8);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS1))
	stxi_d(112, _SP_REGNO, 9);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS2))
	stxi_d(120, _SP_REGNO, 18);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS3))
	stxi_d(128, _SP_REGNO, 19);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS4))
	stxi_d(136, _SP_REGNO, 20);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS5))
	stxi_d(144, _SP_REGNO, 21);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS6))
	stxi_d(152, _SP_REGNO, 22);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS7))
	stxi_d(160, _SP_REGNO, 23);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS8))
	stxi_d(168, _SP_REGNO, 24);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS9))
	stxi_d(176, _SP_REGNO, 25);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS10))
	stxi_d(184, _SP_REGNO, 26);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS11))
	stxi_d(192, _SP_REGNO, 27);
    movr(_FP_REGNO, _SP_REGNO);
    if (_jitc->function->stack)
	subi(_SP_REGNO, _SP_REGNO, _jitc->function->stack);
    if (_jitc->function->allocar) {
	reg = jit_get_reg(jit_class_gpr);
	movi(rn(reg), _jitc->function->self.aoff);
	stxi_i(_jitc->function->aoffoff, _FP_REGNO, rn(reg));
	jit_unget_reg(reg);
    }
    if (_jitc->function->self.call & jit_call_varargs) {
	for (reg = _jitc->function->vagp; jit_arg_reg_p(reg); ++reg)
	    stxi(stack_framesize - ((8 - reg) * 8),
		 _FP_REGNO, rn(JIT_RA0 - reg));
    }
}

static void
_epilog(jit_state_t *_jit, jit_node_t *node)
{
    if (_jitc->function->assume_frame)
	return;
    movr(_SP_REGNO, _FP_REGNO);
    ldxi(_RA_REGNO, _SP_REGNO, 0);
    ldxi(_FP_REGNO, _SP_REGNO, 8);
    if (jit_regset_tstbit(&_jitc->function->regset, _S1))
	ldxi(9, _SP_REGNO, 16);
    if (jit_regset_tstbit(&_jitc->function->regset, _S2))
	ldxi(18, _SP_REGNO, 24);
    if (jit_regset_tstbit(&_jitc->function->regset, _S3))
	ldxi(19, _SP_REGNO, 32);
    if (jit_regset_tstbit(&_jitc->function->regset, _S4))
	ldxi(20, _SP_REGNO, 40);
    if (jit_regset_tstbit(&_jitc->function->regset, _S5))
	ldxi(21, _SP_REGNO, 48);
    if (jit_regset_tstbit(&_jitc->function->regset, _S6))
	ldxi(22, _SP_REGNO, 56);
    if (jit_regset_tstbit(&_jitc->function->regset, _S7))
	ldxi(23, _SP_REGNO, 64);
    if (jit_regset_tstbit(&_jitc->function->regset, _S8))
	ldxi(24, _SP_REGNO, 72);
    if (jit_regset_tstbit(&_jitc->function->regset, _S9))
	ldxi(25, _SP_REGNO, 80);
    if (jit_regset_tstbit(&_jitc->function->regset, _S10))
	ldxi(26, _SP_REGNO, 88);
    if (jit_regset_tstbit(&_jitc->function->regset, _S11))
	ldxi(27, _SP_REGNO, 96);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS0))
	ldxi_d(8, _SP_REGNO, 104);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS1))
	ldxi_d(9, _SP_REGNO, 112);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS2))
	ldxi_d(18, _SP_REGNO, 120);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS3))
	ldxi_d(19, _SP_REGNO, 128);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS4))
	ldxi_d(20, _SP_REGNO, 136);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS5))
	ldxi_d(21, _SP_REGNO, 144);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS6))
	ldxi_d(22, _SP_REGNO, 152);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS7))
	ldxi_d(23, _SP_REGNO, 160);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS8))
	ldxi_d(24, _SP_REGNO, 168);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS9))
	ldxi_d(25, _SP_REGNO, 176);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS10))
	ldxi_d(26, _SP_REGNO, 184);
    if (jit_regset_tstbit(&_jitc->function->regset, _FS11))
	ldxi_d(27, _SP_REGNO, 192);
    addi(_SP_REGNO, _SP_REGNO, stack_framesize);
    RET();
}

static void
_vastart(jit_state_t *_jit, jit_int32_t r0)
{
    assert(_jitc->function->self.call & jit_call_varargs);
    /* Initialize va_list to the first stack argument. */
    if (jit_arg_reg_p(_jitc->function->vagp))
	addi(r0, _FP_REGNO, stack_framesize - ((8 - _jitc->function->vagp) * 8));
    else
	addi(r0, _FP_REGNO, _jitc->function->self.size);
}

static void
_vaarg(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    assert(_jitc->function->self.call & jit_call_varargs);
    /* Load argument. */
    ldr(r0, r1);
    /* Update va_list. */
    addi(r1, r1, sizeof(jit_word_t));
}

static void
_patch_at(jit_state_t *_jit, jit_word_t instr, jit_word_t label)
{
    instr_t		 i;
    union {
	jit_int32_t	*i;
	jit_word_t	 w;
    } u;
    u.w = instr;
    i.w = u.i[0];
    /* movi_p? */
    if (i.U.opcode == 55) {					/* LUI */
	jit_int32_t	ww = label << 32 >> 32;
	jit_int32_t	lo = ww << 20 >> 20;
	jit_int32_t	hi = ww - lo;
	i.U.imm12_31 = hi >> 12;
	u.i[0] = i.w;
	i.w = u.i[1];
	if (i.I.opcode == 27 && i.I.funct3 == 0) {		/* ADDIW */
	    i.I.imm11_0 = lo & 0xfff;
	    u.i[1] = i.w;
	    i.w = u.i[2];
	    if (i.U.opcode == 55) {				/* LUI */
		ww = label >> 32;
		lo = ww << 20 >> 20;
		hi = ww - lo;
		i.U.imm12_31 = hi >> 12;
		u.i[2] = i.w;
		i.w = u.i[3];
		if (i.I.opcode == 27 && i.I.funct3 == 0) {	/* ADDIW */
		    i.I.imm11_0 = lo & 0xfff;
		    u.i[3] = i.w;
		    i.w = u.i[4];
		    assert(i.IS.opcode == 19);			/* SLLI */
		    assert(i.IS.shamt == 32);
		    i.w = u.i[5];
		    assert(i.R.opcode == 51);			/* ADD */
		}
		else
		    abort();
	    }
	    else
		abort();
	}
	else
	    abort();
    }
    /* b{lt,le,eq,ge,gt,ne}{,_u}? */
    else if (i.B.opcode == 99) {		/* B{EQ,NE,LT,GE,LTU,GEU} */
	jit_word_t jmp = label - instr;
	assert(simm12_p(jmp));
	i.B.imm11	= (jmp >> 11) & 0x1;
	i.B.imm4_1	= (jmp >> 1) & 0xf;
	i.B.imm10_5	= (jmp >> 5) & 0x3f;
	i.B.imm12	= (jmp >> 12) & 0x1;
	u.i[0] = i.w;
    }
    else if (i.J.opcode == 111) {		/* JAL */
	jit_word_t jmp = label - instr;
	i.J.imm19_12	= (jmp >> 12) &  0xff;
	i.J.imm11	= (jmp >> 11) &   0x1;
	i.J.imm10_1	= (jmp >>  1) & 0x3ff;
	i.J.imm20	= (jmp >> 20) &   0x1;
	u.i[0] = i.w;
    }
    else
	abort();
}
#endif		/* CODE */
