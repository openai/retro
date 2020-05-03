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

#include <lightning.h>
#include <lightning/jit_private.h>

#if __X32
#  define jit_arg_reg_p(i)		0
#  define jit_arg_f_reg_p(i)		0
#  define stack_framesize		20
#  define stack_adjust			12
#  define CVT_OFFSET			-12
#  define REAL_WORDSIZE			4
#  define va_gp_increment		4
#  define va_fp_increment		8
#else
#  if __CYGWIN__ || _WIN32
#    define jit_arg_reg_p(i)		((i) >= 0 && (i) < 4)
#    define jit_arg_f_reg_p(i)		jit_arg_reg_p(i)
#    define stack_framesize		152
#    define va_fp_increment		8
#  else
#    define jit_arg_reg_p(i)		((i) >= 0 && (i) < 6)
#    define jit_arg_f_reg_p(i)		((i) >= 0 && (i) < 8)
#    define stack_framesize		56
#    define first_gp_argument		rdi
#    define first_gp_offset		offsetof(jit_va_list_t, rdi)
#    define first_gp_from_offset(gp)	((gp) / 8)
#    define last_gp_argument		r9
#    define va_gp_max_offset						\
	(offsetof(jit_va_list_t, r9) - offsetof(jit_va_list_t, rdi) + 8)
#    define first_fp_argument		xmm0
#    define first_fp_offset		offsetof(jit_va_list_t, xmm0)
#    define last_fp_argument		xmm7
#    define va_fp_max_offset						\
	(offsetof(jit_va_list_t, xmm7) - offsetof(jit_va_list_t, rdi) + 16)
#    define va_fp_increment		16
#    define first_fp_from_offset(fp)	(((fp) - va_gp_max_offset) / 16)
#  endif
#  define va_gp_increment		8
#  define stack_adjust			8
#  define CVT_OFFSET			-8
#  define REAL_WORDSIZE			8
#endif

/*
 * Types
 */
#if __X32 || __CYGWIN__ || _WIN32
typedef jit_pointer_t jit_va_list_t;
#else
typedef struct jit_va_list {
    jit_int32_t		gpoff;
    jit_int32_t		fpoff;
    jit_pointer_t	over;
    jit_pointer_t	save;
    /* Declared explicitly as int64 for the x32 abi */
    jit_int64_t		rdi;
    jit_int64_t		rsi;
    jit_int64_t		rdx;
    jit_int64_t		rcx;
    jit_int64_t		r8;
    jit_int64_t		r9;
    jit_float64_t	xmm0;
    jit_float64_t	_up0;
    jit_float64_t	xmm1;
    jit_float64_t	_up1;
    jit_float64_t	xmm2;
    jit_float64_t	_up2;
    jit_float64_t	xmm3;
    jit_float64_t	_up3;
    jit_float64_t	xmm4;
    jit_float64_t	_up4;
    jit_float64_t	xmm5;
    jit_float64_t	_up5;
    jit_float64_t	xmm6;
    jit_float64_t	_up6;
    jit_float64_t	xmm7;
    jit_float64_t	_up7;
} jit_va_list_t;
#endif

/*
 * Prototypes
 */
#define patch(instr, node)		_patch(_jit, instr, node)
static void _patch(jit_state_t*,jit_word_t,jit_node_t*);
#define sse_from_x87_f(r0, r1)		_sse_from_x87_f(_jit, r0, r1)
static void _sse_from_x87_f(jit_state_t*,jit_int32_t,jit_int32_t);
#define sse_from_x87_d(r0, r1)		_sse_from_x87_d(_jit, r0, r1)
static void _sse_from_x87_d(jit_state_t*,jit_int32_t,jit_int32_t);
#define x87_from_sse_f(r0, r1)		_x87_from_sse_f(_jit, r0, r1)
static void _x87_from_sse_f(jit_state_t*,jit_int32_t,jit_int32_t);
#define x87_from_sse_d(r0, r1)		_x87_from_sse_d(_jit, r0, r1)
static void _x87_from_sse_d(jit_state_t*,jit_int32_t,jit_int32_t);

#define PROTO				1
#  include "jit_x86-cpu.c"
#  include "jit_x86-sse.c"
#  include "jit_x86-x87.c"
#undef PROTO

/*
 * Initialization
 */
jit_cpu_t		jit_cpu;
jit_register_t		_rvs[] = {
#if __X32
    { rc(gpr) | rc(rg8) | 0,		"%eax" },
    { rc(gpr) | rc(rg8) | 1,		"%ecx" },
    { rc(gpr) | rc(rg8) | 2,		"%edx" },
    { rc(sav) | rc(rg8) | rc(gpr) | 3,	"%ebx" },
    { rc(sav) | rc(gpr) | 6,		"%esi" },
    { rc(sav) | rc(gpr) | 7,		"%edi" },
    { rc(sav) | 4,			"%esp" },
    { rc(sav) | 5,			"%ebp" },
    { rc(xpr) | rc(fpr) | 0,		"%xmm0" },
    { rc(xpr) | rc(fpr) | 1,		"%xmm1" },
    { rc(xpr) | rc(fpr) | 2,		"%xmm2" },
    { rc(xpr) | rc(fpr) | 3,		"%xmm3" },
    { rc(xpr) | rc(fpr) | 4,		"%xmm4" },
    { rc(xpr) | rc(fpr) | 5,		"%xmm5" },
    { rc(xpr) | rc(fpr) | 6,		"%xmm6" },
    { rc(xpr) | rc(fpr) | 7,		"%xmm7" },
    { rc(fpr) | 0,			"st(0)" },
    { rc(fpr) | 1,			"st(1)" },
    { rc(fpr) | 2,			"st(2)" },
    { rc(fpr) | 3,			"st(3)" },
    { rc(fpr) | 4,			"st(4)" },
    { rc(fpr) | 5,			"st(5)" },
    { rc(fpr) | 6,			"st(6)" },
    { rc(fpr) | 7,			"st(7)" },
#else
#  if __CYGWIN__ || _WIN32
    { rc(gpr) | rc(rg8) | 0,		"%rax" },
    { rc(gpr) | rc(rg8) | rc(rg8) | 10,	"%r10" },
    { rc(gpr) | rc(rg8) | rc(rg8) | 11,	"%r11" },
    { rc(sav) | rc(rg8) | rc(gpr) | 3,	"%rbx" },
    { rc(sav) | rc(gpr) | 7,		"%rdi" },
    { rc(sav) | rc(gpr) | 6,		"%rsi" },
    { rc(sav) | rc(gpr) | 12,		"%r12" },
    { rc(sav) | rc(gpr) | 13,		"%r13" },
    { rc(sav) | rc(gpr) | 14,		"%r14" },
    { rc(sav) | rc(gpr) | 15,		"%r15" },
    { rc(arg) | rc(rg8) | rc(gpr) | 9,	"%r9" },
    { rc(arg) | rc(rg8) | rc(gpr) | 8,	"%r8" },
    { rc(arg) | rc(rg8) | rc(gpr) | 2,	"%rdx" },
    { rc(arg) | rc(rg8) | rc(gpr) | 1,	"%rcx" },
    { rc(sav) | 4,			"%rsp" },
    { rc(sav) | 5,			"%rbp" },
    { rc(xpr) | rc(fpr) | 4,		"%xmm4" },
    { rc(xpr) | rc(fpr) | 5,		"%xmm5" },
    { rc(sav) | rc(xpr) | rc(fpr) | 6,	"%xmm6" },
    { rc(sav) | rc(xpr) | rc(fpr) | 7,	"%xmm7" },
    { rc(sav) | rc(xpr) | rc(fpr) | 8,	"%xmm8" },
    { rc(sav) | rc(xpr) | rc(fpr) | 9,	"%xmm9" },
    { rc(sav) | rc(xpr) | rc(fpr) | 10,	"%xmm10" },
    { rc(sav) | rc(xpr) | rc(fpr) | 11,	"%xmm11" },
    { rc(sav) | rc(xpr) | rc(fpr) | 12,	"%xmm12" },
    { rc(sav) | rc(xpr) | rc(fpr) | 13,	"%xmm13" },
    { rc(sav) | rc(xpr) | rc(fpr) | 14,	"%xmm14" },
    { rc(sav) | rc(xpr) | rc(fpr) | 15,	"%xmm15" },
    { rc(xpr) | rc(arg) | rc(fpr) | 3,	"%xmm3" },
    { rc(xpr) | rc(arg) | rc(fpr) | 2,	"%xmm2" },
    { rc(xpr) | rc(arg) | rc(fpr) | 1,	"%xmm1" },
    { rc(xpr) | rc(arg) | rc(fpr) | 0,	"%xmm0" },
#else
    /* %rax is a pseudo flag argument for varargs functions */
    { rc(arg) | rc(gpr) | rc(rg8) | 0,	"%rax" },
    { rc(gpr) | rc(rg8) | 10,		"%r10" },
    { rc(gpr) | rc(rg8) | 11,		"%r11" },
    { rc(sav) | rc(rg8) | rc(gpr) | 3,	"%rbx" },
    { rc(sav) | rc(rg8) | rc(gpr) | 13,	"%r13" },
    { rc(sav) | rc(rg8) | rc(gpr) | 14,	"%r14" },
    { rc(sav) | rc(rg8) | rc(gpr) | 15,	"%r15" },
    { rc(sav) | rc(gpr) | rc(rg8) | 12,	"%r12" },
    { rc(arg) | rc(rg8) | rc(gpr) | 9,	"%r9" },
    { rc(arg) | rc(rg8) | rc(gpr) | 8,	"%r8" },
    { rc(arg) | rc(rg8) | rc(gpr) | 1,	"%rcx" },
    { rc(arg) | rc(rg8) | rc(gpr) | 2,	"%rdx" },
    { rc(arg) | rc(rg8) | rc(gpr) | 6,	"%rsi" },
    { rc(arg) | rc(rg8) | rc(gpr) | 7,	"%rdi" },
    { rc(sav) | 4,			"%rsp" },
    { rc(sav) | 5,			"%rbp" },
    { rc(xpr) | rc(fpr) | 8,		"%xmm8" },
    { rc(xpr) | rc(fpr) | 9,		"%xmm9" },
    { rc(xpr) | rc(fpr) | 10,		"%xmm10" },
    { rc(xpr) | rc(fpr) | 11,		"%xmm11" },
    { rc(xpr) | rc(fpr) | 12,		"%xmm12" },
    { rc(xpr) | rc(fpr) | 13,		"%xmm13" },
    { rc(xpr) | rc(fpr) | 14,		"%xmm14" },
    { rc(xpr) | rc(fpr) | 15,		"%xmm15" },
    { rc(xpr) | rc(arg) | rc(fpr) | 7,	"%xmm7" },
    { rc(xpr) | rc(arg) | rc(fpr) | 6,	"%xmm6" },
    { rc(xpr) | rc(arg) | rc(fpr) | 5,	"%xmm5" },
    { rc(xpr) | rc(arg) | rc(fpr) | 4,	"%xmm4" },
    { rc(xpr) | rc(arg) | rc(fpr) | 3,	"%xmm3" },
    { rc(xpr) | rc(arg) | rc(fpr) | 2,	"%xmm2" },
    { rc(xpr) | rc(arg) | rc(fpr) | 1,	"%xmm1" },
    { rc(xpr) | rc(arg) | rc(fpr) | 0,	"%xmm0" },
#  endif
    { rc(fpr) | 0,			"st(0)" },
    { rc(fpr) | 1,			"st(1)" },
    { rc(fpr) | 2,			"st(2)" },
    { rc(fpr) | 3,			"st(3)" },
    { rc(fpr) | 4,			"st(4)" },
    { rc(fpr) | 5,			"st(5)" },
    { rc(fpr) | 6,			"st(6)" },
    { rc(fpr) | 7,			"st(7)" },
#endif
    { _NOREG,				"<none>" },
};

/*
 * Implementation
 */
void
jit_get_cpu(void)
{
    union {
	struct {
	    jit_uint32_t sse3		: 1;
	    jit_uint32_t pclmulqdq	: 1;
	    jit_uint32_t dtes64		: 1;	/* amd reserved */
	    jit_uint32_t monitor	: 1;
	    jit_uint32_t ds_cpl		: 1;	/* amd reserved */
	    jit_uint32_t vmx		: 1;	/* amd reserved */
	    jit_uint32_t smx		: 1;	/* amd reserved */
	    jit_uint32_t est		: 1;	/* amd reserved */
	    jit_uint32_t tm2		: 1;	/* amd reserved */
	    jit_uint32_t ssse3		: 1;
	    jit_uint32_t cntx_id	: 1;	/* amd reserved */
	    jit_uint32_t __reserved0	: 1;
	    jit_uint32_t fma		: 1;
	    jit_uint32_t cmpxchg16b	: 1;
	    jit_uint32_t xtpr		: 1;	/* amd reserved */
	    jit_uint32_t pdcm		: 1;	/* amd reserved */
	    jit_uint32_t __reserved1	: 1;
	    jit_uint32_t pcid		: 1;	/* amd reserved */
	    jit_uint32_t dca		: 1;	/* amd reserved */
	    jit_uint32_t sse4_1		: 1;
	    jit_uint32_t sse4_2		: 1;
	    jit_uint32_t x2apic		: 1;	/* amd reserved */
	    jit_uint32_t movbe		: 1;	/* amd reserved */
	    jit_uint32_t popcnt		: 1;
	    jit_uint32_t tsc		: 1;	/* amd reserved */
	    jit_uint32_t aes		: 1;
	    jit_uint32_t xsave		: 1;
	    jit_uint32_t osxsave	: 1;
	    jit_uint32_t avx		: 1;
	    jit_uint32_t __reserved2	: 1;	/* amd F16C */
	    jit_uint32_t __reserved3	: 1;
	    jit_uint32_t __alwayszero	: 1;	/* amd RAZ */
	} bits;
	jit_uword_t	cpuid;
    } ecx;
    union {
	struct {
	    jit_uint32_t fpu		: 1;
	    jit_uint32_t vme		: 1;
	    jit_uint32_t de		: 1;
	    jit_uint32_t pse		: 1;
	    jit_uint32_t tsc		: 1;
	    jit_uint32_t msr		: 1;
	    jit_uint32_t pae		: 1;
	    jit_uint32_t mce		: 1;
	    jit_uint32_t cmpxchg8b	: 1;
	    jit_uint32_t apic		: 1;
	    jit_uint32_t __reserved0	: 1;
	    jit_uint32_t sep		: 1;
	    jit_uint32_t mtrr		: 1;
	    jit_uint32_t pge		: 1;
	    jit_uint32_t mca		: 1;
	    jit_uint32_t cmov		: 1;
	    jit_uint32_t pat		: 1;
	    jit_uint32_t pse36		: 1;
	    jit_uint32_t psn		: 1;	/* amd reserved */
	    jit_uint32_t clfsh		: 1;
	    jit_uint32_t __reserved1	: 1;
	    jit_uint32_t ds		: 1;	/* amd reserved */
	    jit_uint32_t acpi		: 1;	/* amd reserved */
	    jit_uint32_t mmx		: 1;
	    jit_uint32_t fxsr		: 1;
	    jit_uint32_t sse		: 1;
	    jit_uint32_t sse2		: 1;
	    jit_uint32_t ss		: 1;	/* amd reserved */
	    jit_uint32_t htt		: 1;
	    jit_uint32_t tm		: 1;	/* amd reserved */
	    jit_uint32_t __reserved2	: 1;
	    jit_uint32_t pbe		: 1;	/* amd reserved */
	} bits;
	jit_uword_t	cpuid;
    } edx;
#if __X32
    int			ac, flags;
#endif
    jit_uword_t		eax, ebx;

#if __X32
    /* adapted from glibc __sysconf */
    __asm__ volatile ("pushfl;\n\t"
		      "popl %0;\n\t"
		      "movl $0x240000, %1;\n\t"
		      "xorl %0, %1;\n\t"
		      "pushl %1;\n\t"
		      "popfl;\n\t"
		      "pushfl;\n\t"
		      "popl %1;\n\t"
		      "xorl %0, %1;\n\t"
		      "pushl %0;\n\t"
		      "popfl"
		      : "=r" (flags), "=r" (ac));

    /* i386 or i486 without cpuid */
    if ((ac & (1 << 21)) == 0)
	/* probably without x87 as well */
	return;
#endif

    /* query %eax = 1 function */
#if __X32 || __X64_32
    __asm__ volatile ("xchgl %%ebx, %1; cpuid; xchgl %%ebx, %1"
#else
    __asm__ volatile ("xchgq %%rbx, %1; cpuid; xchgq %%rbx, %1"
#endif
		      : "=a" (eax), "=r" (ebx),
		      "=c" (ecx.cpuid), "=d" (edx.cpuid)
		      : "0" (1));

    jit_cpu.fpu		= edx.bits.fpu;
    jit_cpu.cmpxchg8b	= edx.bits.cmpxchg8b;
    jit_cpu.cmov	= edx.bits.cmov;
    jit_cpu.mmx		= edx.bits.mmx;
    jit_cpu.sse		= edx.bits.sse;
    jit_cpu.sse2	= edx.bits.sse2;
    jit_cpu.sse3	= ecx.bits.sse3;
    jit_cpu.pclmulqdq	= ecx.bits.pclmulqdq;
    jit_cpu.ssse3	= ecx.bits.ssse3;
    jit_cpu.fma		= ecx.bits.fma;
    jit_cpu.cmpxchg16b	= ecx.bits.cmpxchg16b;
    jit_cpu.sse4_1	= ecx.bits.sse4_1;
    jit_cpu.sse4_2	= ecx.bits.sse4_2;
    jit_cpu.movbe	= ecx.bits.movbe;
    jit_cpu.popcnt	= ecx.bits.popcnt;
    jit_cpu.aes		= ecx.bits.aes;
    jit_cpu.avx		= ecx.bits.avx;

    /* query %eax = 0x80000001 function */
#if __X64
#  if __X64_32
    __asm__ volatile ("xchgl %%ebx, %1; cpuid; xchgl %%ebx, %1"
#  else
    __asm__ volatile ("xchgq %%rbx, %1; cpuid; xchgq %%rbx, %1"
#  endif
		      : "=a" (eax), "=r" (ebx),
		      "=c" (ecx.cpuid), "=d" (edx.cpuid)
		      : "0" (0x80000001));
    jit_cpu.lahf	= ecx.cpuid & 1;
#endif
}

void
_jit_init(jit_state_t *_jit)
{
#if __X32
    jit_int32_t		regno;
    static jit_bool_t	first = 1;
#endif

    _jitc->reglen = jit_size(_rvs) - 1;
#if __X32
    if (first) {
	if (!jit_cpu.sse2) {
	    for (regno = _jitc->reglen; regno >= 0; regno--) {
		if (_rvs[regno].spec & jit_class_xpr)
		    _rvs[regno].spec = 0;
	    }
	}
	first = 0;
    }
#endif
}

void
_jit_prolog(jit_state_t *_jit)
{
    jit_int32_t		offset;

    if (_jitc->function)
	jit_epilog();
    assert(jit_regset_cmp_ui(&_jitc->regarg, 0) == 0);
    jit_regset_set_ui(&_jitc->regsav, 0);
    offset = _jitc->functions.offset;
    if (offset >= _jitc->functions.length) {
	jit_realloc((jit_pointer_t *)&_jitc->functions.ptr,
		    _jitc->functions.length * sizeof(jit_function_t),
		    (_jitc->functions.length + 16) * sizeof(jit_function_t));
	_jitc->functions.length += 16;
    }
    _jitc->function = _jitc->functions.ptr + _jitc->functions.offset++;
    _jitc->function->self.size = stack_framesize;
    _jitc->function->self.argi = _jitc->function->self.argf =
	_jitc->function->self.aoff = _jitc->function->self.alen = 0;
    /* sse/x87 conversion */
    _jitc->function->self.aoff = CVT_OFFSET;
    _jitc->function->self.call = jit_call_default;
    jit_alloc((jit_pointer_t *)&_jitc->function->regoff,
	      _jitc->reglen * sizeof(jit_int32_t));

    /* _no_link here does not mean the jit_link() call can be removed
     * by rewriting as:
     * _jitc->function->prolog = jit_new_node(jit_code_prolog);
     */
    _jitc->function->prolog = jit_new_node_no_link(jit_code_prolog);
    jit_link(_jitc->function->prolog);
    _jitc->function->prolog->w.w = offset;
    _jitc->function->epilog = jit_new_node_no_link(jit_code_epilog);
    /*	u:	label value
     *	v:	offset in blocks vector
     *	w:	offset in functions vector
     */
    _jitc->function->epilog->w.w = offset;

    jit_regset_new(&_jitc->function->regset);
}

jit_int32_t
_jit_allocai(jit_state_t *_jit, jit_int32_t length)
{
    assert(_jitc->function);
    switch (length) {
	case 0:	case 1:						break;
	case 2:		_jitc->function->self.aoff &= -2;	break;
	case 3:	case 4:	_jitc->function->self.aoff &= -4;	break;
	default:	_jitc->function->self.aoff &= -8;	break;
    }
    _jitc->function->self.aoff -= length;

    /* jit_allocai() may be called from jit_x86-cpu.c, and force a function
     * generation restart on some conditions: div/rem and qmul/qdiv, due
     * to registers constraints.
     * The check is to prevent an assertion of a jit_xyz() being called
     * during code generation, and attempting to add a node to the tail
     * of the current IR generation. */
    if (!_jitc->realize) {
	jit_inc_synth_ww(allocai, _jitc->function->self.aoff, length);
	jit_dec_synth();
    }

    return (_jitc->function->self.aoff);
}

void
_jit_allocar(jit_state_t *_jit, jit_int32_t u, jit_int32_t v)
{
    jit_int32_t		 reg;
    assert(_jitc->function);
    jit_inc_synth_ww(allocar, u, v);
    if (!_jitc->function->allocar) {
	_jitc->function->aoffoff = jit_allocai(sizeof(jit_int32_t));
	_jitc->function->allocar = 1;
    }
    reg = jit_get_reg(jit_class_gpr);
    jit_negr(reg, v);
    jit_andi(reg, reg, -16);
    jit_ldxi_i(u, JIT_FP, _jitc->function->aoffoff);
    jit_addr(u, u, reg);
    jit_addr(JIT_SP, JIT_SP, reg);
    jit_stxi_i(_jitc->function->aoffoff, JIT_FP, u);
    jit_unget_reg(reg);
    jit_dec_synth();
}

void
_jit_ret(jit_state_t *_jit)
{
    jit_node_t		*instr;
    assert(_jitc->function);
    jit_inc_synth(ret);
    /* jump to epilog */
    instr = jit_jmpi();
    jit_patch_at(instr, _jitc->function->epilog);
    jit_dec_synth();
}

void
_jit_retr(jit_state_t *_jit, jit_int32_t u)
{
    jit_inc_synth_w(retr, u);
    /* movr(%ret, %ret) would be optimized out */
    if (JIT_RET != u)
	jit_movr(JIT_RET, u);
    /* explicitly tell it is live */
    jit_live(JIT_RET);
    jit_ret();
    jit_dec_synth();
}

void
_jit_reti(jit_state_t *_jit, jit_word_t u)
{
    jit_inc_synth_w(reti, u);
    jit_movi(JIT_RET, u);
    jit_ret();
    jit_dec_synth();
}

void
_jit_retr_f(jit_state_t *_jit, jit_int32_t u)
{
    jit_inc_synth_w(retr_f, u);
    if (JIT_FRET != u)
	jit_movr_f(JIT_FRET, u);
    else
	jit_live(JIT_FRET);
    jit_ret();
    jit_dec_synth();
}

void
_jit_reti_f(jit_state_t *_jit, jit_float32_t u)
{
    jit_inc_synth_f(reti_f, u);
    jit_movi_f(JIT_FRET, u);
    jit_ret();
    jit_dec_synth();
}

void
_jit_retr_d(jit_state_t *_jit, jit_int32_t u)
{
    jit_inc_synth_w(retr_d, u);
    if (JIT_FRET != u)
	jit_movr_d(JIT_FRET, u);
    else
	jit_live(JIT_FRET);
    jit_ret();
    jit_dec_synth();
}

void
_jit_reti_d(jit_state_t *_jit, jit_float64_t u)
{
    jit_inc_synth_d(reti_d, u);
    jit_movi_d(JIT_FRET, u);
    jit_ret();
    jit_dec_synth();
}

void
_jit_epilog(jit_state_t *_jit)
{
    assert(_jitc->function);
    assert(_jitc->function->epilog->next == NULL);
    jit_link(_jitc->function->epilog);
    _jitc->function = NULL;
}

jit_bool_t
_jit_arg_register_p(jit_state_t *_jit, jit_node_t *u)
{
    if (u->code == jit_code_arg)
	return (jit_arg_reg_p(u->u.w));
    assert(u->code == jit_code_arg_f || u->code == jit_code_arg_d);
    return (jit_arg_f_reg_p(u->u.w));
}

void
_jit_ellipsis(jit_state_t *_jit)
{
    jit_inc_synth(ellipsis);
    if (_jitc->prepare) {
	jit_link_prepare();
	/* Remember that a varargs function call is being constructed. */
	assert(!(_jitc->function->call.call & jit_call_varargs));
	_jitc->function->call.call |= jit_call_varargs;
    }
    else {
	jit_link_prolog();
	/* Remember the current function is varargs. */
	assert(!(_jitc->function->self.call & jit_call_varargs));
	_jitc->function->self.call |= jit_call_varargs;

#if __X64 && !(__CYGWIN__ || _WIN32)
	/* Allocate va_list like object in the stack.
	 * If applicable, with enough space to save all argument
	 * registers, and use fixed offsets for them. */
	_jitc->function->vaoff = jit_allocai(sizeof(jit_va_list_t));

	/* Initialize gp offset in save area. */
	if (jit_arg_reg_p(_jitc->function->self.argi))
	    _jitc->function->vagp = _jitc->function->self.argi * 8;
	else
	    _jitc->function->vagp = va_gp_max_offset;

	/* Initialize fp offset in save area. */
	if (jit_arg_f_reg_p(_jitc->function->self.argf))
	    _jitc->function->vafp = _jitc->function->self.argf * 16 +
				    va_gp_max_offset;
	else
	    _jitc->function->vafp = va_fp_max_offset;
#endif
    }
    jit_dec_synth();
}

void
_jit_va_push(jit_state_t *_jit, jit_int32_t u)
{
    jit_inc_synth_w(va_push, u);
    jit_pushargr(u);
    jit_dec_synth();
}

jit_node_t *
_jit_arg(jit_state_t *_jit)
{
    jit_node_t		*node;
    jit_int32_t		 offset;
    assert(_jitc->function);
    assert(!(_jitc->function->self.call & jit_call_varargs));
#if __X64
    if (jit_arg_reg_p(_jitc->function->self.argi)) {
	offset = _jitc->function->self.argi++;
#  if __CYGWIN__ || _WIN32
	_jitc->function->self.size += sizeof(jit_word_t);
#  endif
    }
    else
#endif
    {
	offset = _jitc->function->self.size;
	_jitc->function->self.size += REAL_WORDSIZE;
    }
    node = jit_new_node_ww(jit_code_arg, offset,
			   ++_jitc->function->self.argn);
    jit_link_prolog();
    return (node);
}

jit_node_t *
_jit_arg_f(jit_state_t *_jit)
{
    jit_node_t		*node;
    jit_int32_t		 offset;
    assert(_jitc->function);
    assert(!(_jitc->function->self.call & jit_call_varargs));
#if __X64
#  if __CYGWIN__ || _WIN32
    if (jit_arg_reg_p(_jitc->function->self.argi)) {
	offset = _jitc->function->self.argi++;
	_jitc->function->self.size += sizeof(jit_word_t);
    }
#  else
    if (jit_arg_f_reg_p(_jitc->function->self.argf))
	offset = _jitc->function->self.argf++;
#  endif
    else
#endif
    {
	offset = _jitc->function->self.size;
	_jitc->function->self.size += REAL_WORDSIZE;
    }
    node = jit_new_node_ww(jit_code_arg_f, offset,
			   ++_jitc->function->self.argn);
    jit_link_prolog();
    return (node);
}

jit_node_t *
_jit_arg_d(jit_state_t *_jit)
{
    jit_node_t		*node;
    jit_int32_t		 offset;
    assert(_jitc->function);
    assert(!(_jitc->function->self.call & jit_call_varargs));
#if __X64
#  if __CYGWIN__ || _WIN32
    if (jit_arg_reg_p(_jitc->function->self.argi)) {
	offset = _jitc->function->self.argi++;
	_jitc->function->self.size += sizeof(jit_word_t);
    }
#  else
    if (jit_arg_f_reg_p(_jitc->function->self.argf))
	offset = _jitc->function->self.argf++;
#  endif
    else
#endif
    {
	offset = _jitc->function->self.size;
	_jitc->function->self.size += sizeof(jit_float64_t);
    }
    node = jit_new_node_ww(jit_code_arg_d, offset,
			   ++_jitc->function->self.argn);
    jit_link_prolog();
    return (node);
}

void
_jit_getarg_c(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(getarg_c, u, v);
#if __X64
    if (jit_arg_reg_p(v->u.w))
	jit_extr_c(u, JIT_RA0 - v->u.w);
    else
#endif
	jit_ldxi_c(u, _RBP, v->u.w);
    jit_dec_synth();
}

void
_jit_getarg_uc(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(getarg_uc, u, v);
#if __X64
    if (jit_arg_reg_p(v->u.w))
	jit_extr_uc(u, JIT_RA0 - v->u.w);
    else
#endif
	jit_ldxi_uc(u, _RBP, v->u.w);
    jit_dec_synth();
}

void
_jit_getarg_s(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(getarg_s, u, v);
#if __X64
    if (jit_arg_reg_p(v->u.w))
	jit_extr_s(u, JIT_RA0 - v->u.w);
    else
#endif
	jit_ldxi_s(u, _RBP, v->u.w);
    jit_dec_synth();
}

void
_jit_getarg_us(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(getarg_us, u, v);
#if __X64
    if (jit_arg_reg_p(v->u.w))
	jit_extr_us(u, JIT_RA0 - v->u.w);
    else
#endif
	jit_ldxi_us(u, _RBP, v->u.w);
    jit_dec_synth();
}

void
_jit_getarg_i(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(getarg_i, u, v);
#if __X64
    if (jit_arg_reg_p(v->u.w)) {
#  if __X64_32
	jit_movr(u, JIT_RA0 - v->u.w);
#  else
	jit_extr_i(u, JIT_RA0 - v->u.w);
#  endif
     }
    else
#endif
	jit_ldxi_i(u, _RBP, v->u.w);
    jit_dec_synth();
}

#if __X64 && !__X64_32
void
_jit_getarg_ui(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(getarg_ui, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_extr_ui(u, JIT_RA0 - v->u.w);
    else
	jit_ldxi_ui(u, _RBP, v->u.w);
    jit_dec_synth();
}

void
_jit_getarg_l(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(getarg_l, u, v);
    if (jit_arg_reg_p(v->u.w))
	jit_movr(u, JIT_RA0 - v->u.w);
    else
	jit_ldxi_l(u, _RBP, v->u.w);
    jit_dec_synth();
}
#endif

void
_jit_putargr(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(putargr, u, v);
#if __X64
    if (jit_arg_reg_p(v->u.w))
	jit_movr(JIT_RA0 - v->u.w, u);
    else
#endif
	jit_stxi(v->u.w, _RBP, u);
    jit_dec_synth();
}

void
_jit_putargi(jit_state_t *_jit, jit_word_t u, jit_node_t *v)
{
    jit_int32_t		regno;
    assert(v->code == jit_code_arg);
    jit_inc_synth_wp(putargi, u, v);
#if __X64
    if (jit_arg_reg_p(v->u.w))
	jit_movi(JIT_RA0 - v->u.w, u);
    else
#endif
    {
	regno = jit_get_reg(jit_class_gpr);
	jit_movi(regno, u);
	jit_stxi(v->u.w, _RBP, regno);
	jit_unget_reg(regno);
    }
    jit_dec_synth();
}

void
_jit_getarg_f(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg_f);
    jit_inc_synth_wp(getarg_f, u, v);
#if __X64
    if (jit_arg_f_reg_p(v->u.w))
	jit_movr_f(u, _XMM0 - v->u.w);
    else
#endif
	jit_ldxi_f(u, _RBP, v->u.w);
    jit_dec_synth();
}

void
_jit_putargr_f(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg_f);
    jit_inc_synth_wp(putargr_f, u, v);
#if __X64
    if (jit_arg_reg_p(v->u.w))
	jit_movr_f(_XMM0 - v->u.w, u);
    else
#endif
	jit_stxi_f(v->u.w, _RBP, u);
    jit_dec_synth();
}

void
_jit_putargi_f(jit_state_t *_jit, jit_float32_t u, jit_node_t *v)
{
    jit_int32_t		regno;
    assert(v->code == jit_code_arg_f);
    jit_inc_synth_fp(putargi_f, u, v);
#if __X64
    if (jit_arg_reg_p(v->u.w))
	jit_movi_f(_XMM0 - v->u.w, u);
    else
#endif
    {
	regno = jit_get_reg(jit_class_gpr);
	jit_movi_f(regno, u);
	jit_stxi_f(v->u.w, _RBP, regno);
	jit_unget_reg(regno);
    }
    jit_dec_synth();
}

void
_jit_getarg_d(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg_d);
    jit_inc_synth_wp(getarg_d, u, v);
#if __X64
    if (jit_arg_f_reg_p(v->u.w))
	jit_movr_d(u, _XMM0 - v->u.w);
    else
#endif
	jit_ldxi_d(u, _RBP, v->u.w);
    jit_dec_synth();
}

void
_jit_putargr_d(jit_state_t *_jit, jit_int32_t u, jit_node_t *v)
{
    assert(v->code == jit_code_arg_d);
    jit_inc_synth_wp(putargr_d, u, v);
#if __X64
    if (jit_arg_reg_p(v->u.w))
	jit_movr_d(_XMM0 - v->u.w, u);
    else
#endif
	jit_stxi_d(v->u.w, _RBP, u);
    jit_dec_synth();
}

void
_jit_putargi_d(jit_state_t *_jit, jit_float64_t u, jit_node_t *v)
{
    jit_int32_t		regno;
    assert(v->code == jit_code_arg_d);
    jit_inc_synth_dp(putargi_d, u, v);
#if __X64
    if (jit_arg_reg_p(v->u.w))
	jit_movi_d(_XMM0 - v->u.w, u);
    else
#endif
    {
	regno = jit_get_reg(jit_class_gpr);
	jit_movi_d(regno, u);
	jit_stxi_d(v->u.w, _RBP, regno);
	jit_unget_reg(regno);
    }
    jit_dec_synth();
}

void
_jit_pushargr(jit_state_t *_jit, jit_int32_t u)
{
    assert(_jitc->function);
    jit_inc_synth_w(pushargr, u);
    jit_link_prepare();
#if __X64
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movr(JIT_RA0 - _jitc->function->call.argi, u);
	++_jitc->function->call.argi;
#  if __CYGWIN__ || _WIN32
	if (_jitc->function->call.call & jit_call_varargs)
	    jit_stxi(_jitc->function->call.size, _RSP, u);
	_jitc->function->call.size += sizeof(jit_word_t);
#  endif
    }
    else
#endif
    {
	jit_stxi(_jitc->function->call.size, _RSP, u);
	_jitc->function->call.size += REAL_WORDSIZE;
    }
    jit_dec_synth();
}

void
_jit_pushargi(jit_state_t *_jit, jit_word_t u)
{
    jit_int32_t		 regno;
    assert(_jitc->function);
    jit_inc_synth_w(pushargi, u);
    jit_link_prepare();
#if __X64
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movi(JIT_RA0 - _jitc->function->call.argi, u);
#  if __CYGWIN__ || _WIN32
	if (_jitc->function->call.call & jit_call_varargs)
	    jit_stxi(_jitc->function->call.size, _RSP,
		     JIT_RA0 - _jitc->function->call.argi);
	_jitc->function->call.size += sizeof(jit_word_t);
#  endif
	++_jitc->function->call.argi;
    }
    else
#endif
    {
	regno = jit_get_reg(jit_class_gpr);
	jit_movi(regno, u);
	jit_stxi(_jitc->function->call.size, _RSP, regno);
	_jitc->function->call.size += REAL_WORDSIZE;
	jit_unget_reg(regno);
    }
    jit_dec_synth();
}

void
_jit_pushargr_f(jit_state_t *_jit, jit_int32_t u)
{
    assert(_jitc->function);
    jit_inc_synth_w(pushargr_f, u);
    jit_link_prepare();
#if __X64
#  if __CYGWIN__ || _WIN32
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movr_f(_XMM0 - _jitc->function->call.argi, u);
	if (_jitc->function->call.call & jit_call_varargs) {
	    jit_stxi_f(_jitc->function->call.size, _RSP,
		       _XMM0 - _jitc->function->call.argi);
	    jit_ldxi_i(JIT_RA0 - _jitc->function->call.argi, _RSP,
		       _jitc->function->call.size);
	}
	++_jitc->function->call.argi;
	_jitc->function->call.size += sizeof(jit_word_t);
    }
#  else
    if (jit_arg_f_reg_p(_jitc->function->self.argf)) {
	jit_movr_f(_XMM0 - _jitc->function->call.argf, u);
	++_jitc->function->call.argf;
    }
#  endif
    else
#endif
    {
	jit_stxi_f(_jitc->function->call.size, _RSP, u);
	_jitc->function->call.size += REAL_WORDSIZE;
    }
    jit_dec_synth();
}

void
_jit_pushargi_f(jit_state_t *_jit, jit_float32_t u)
{
    jit_int32_t		regno;
    assert(_jitc->function);
    jit_inc_synth_f(pushargi_f, u);
    jit_link_prepare();
#if __X64
#  if __CYGWIN__ || _WIN32
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movi_f(_XMM0 - _jitc->function->call.argi, u);
	if (_jitc->function->call.call & jit_call_varargs) {
	    jit_stxi_f(_jitc->function->call.size, _RSP,
		       _XMM0 - _jitc->function->call.argi);
	    jit_ldxi_i(JIT_RA0 - _jitc->function->call.argi, _RSP,
		       _jitc->function->call.size);
	}
	++_jitc->function->call.argi;
	_jitc->function->call.size += sizeof(jit_word_t);
    }
#  else
    if (jit_arg_f_reg_p(_jitc->function->call.argf)) {
	jit_movi_f(_XMM0 - _jitc->function->call.argf, u);
	++_jitc->function->call.argf;
    }
#  endif
    else
#endif
    {
	regno = jit_get_reg(jit_class_fpr);
	jit_movi_f(regno, u);
	jit_stxi_f(_jitc->function->call.size, _RSP, regno);
	_jitc->function->call.size += REAL_WORDSIZE;
	jit_unget_reg(regno);
    }
    jit_dec_synth();
}

void
_jit_pushargr_d(jit_state_t *_jit, jit_int32_t u)
{
    assert(_jitc->function);
    jit_inc_synth_w(pushargr_d, u);
    jit_link_prepare();
#if __X64
#  if __CYGWIN__ || _WIN32
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movr_d(_XMM0 - _jitc->function->call.argi, u);
	if (_jitc->function->call.call & jit_call_varargs) {
	    jit_stxi_d(_jitc->function->call.size, _RSP,
		       _XMM0 - _jitc->function->call.argi);
	    jit_ldxi_l(JIT_RA0 - _jitc->function->call.argi, _RSP,
		       _jitc->function->call.size);
	}
	++_jitc->function->call.argi;
	_jitc->function->call.size += sizeof(jit_word_t);
    }
#  else
    if (jit_arg_f_reg_p(_jitc->function->call.argf)) {
	jit_movr_d(_XMM0 - _jitc->function->call.argf, u);
	++_jitc->function->call.argf;
    }
#  endif
    else
#endif
    {
	jit_stxi_d(_jitc->function->call.size, _RSP, u);
	_jitc->function->call.size += sizeof(jit_float64_t);
    }
    jit_dec_synth();
}

void
_jit_pushargi_d(jit_state_t *_jit, jit_float64_t u)
{
    jit_int32_t		 regno;
    assert(_jitc->function);
    jit_inc_synth_d(pushargi_d, u);
    jit_link_prepare();
#if __X64
#  if __CYGWIN__ || _WIN32
    if (jit_arg_reg_p(_jitc->function->call.argi)) {
	jit_movi_d(_XMM0 - _jitc->function->call.argi, u);
	if (_jitc->function->call.call & jit_call_varargs) {
	    jit_stxi_d(_jitc->function->call.size, _RSP,
		       _XMM0 - _jitc->function->call.argi);
	    jit_ldxi_l(JIT_RA0 - _jitc->function->call.argi, _RSP,
		       _jitc->function->call.size);
	}
	++_jitc->function->call.argi;
	_jitc->function->call.size += sizeof(jit_word_t);
    }
#  else
    if (jit_arg_f_reg_p(_jitc->function->call.argf)) {
	jit_movi_d(_XMM0 - _jitc->function->call.argf, u);
	++_jitc->function->call.argf;
    }
#  endif
    else
#endif
    {
	regno = jit_get_reg(jit_class_fpr);
	jit_movi_d(regno, u);
	jit_stxi_d(_jitc->function->call.size, _RSP, regno);
	_jitc->function->call.size += sizeof(jit_float64_t);
	jit_unget_reg(regno);
    }
    jit_dec_synth();
}

jit_bool_t
_jit_regarg_p(jit_state_t *_jit, jit_node_t *node, jit_int32_t regno)
{
#if __X64
    jit_int32_t		spec;

    spec = jit_class(_rvs[regno].spec);
    if (spec & jit_class_arg) {
	if (spec & jit_class_gpr) {
	    regno = JIT_RA0 - regno;
	    if (regno >= 0 && regno < node->v.w)
		return (1);
	}
	else if (spec & jit_class_fpr) {
	    regno = _XMM0 - regno;
	    if (regno >= 0 && regno < node->w.w)
		return (1);
	}
    }
#endif
    return (0);
}

void
_jit_finishr(jit_state_t *_jit, jit_int32_t r0)
{
    jit_int32_t		 reg;
    jit_node_t		*call;
    assert(_jitc->function);
    reg = r0;
    jit_inc_synth_w(finishr, r0);
    if (_jitc->function->self.alen < _jitc->function->call.size)
	_jitc->function->self.alen = _jitc->function->call.size;
#if __X64
#  if !(__CYGWIN__ || _WIN32)
    if (_jitc->function->call.call & jit_call_varargs) {
	if (jit_regno(reg) == _RAX) {
	    reg = jit_get_reg(jit_class_gpr);
	    jit_movr(reg, _RAX);
	}
	if (_jitc->function->call.argf)
	    jit_movi(_RAX, _jitc->function->call.argf);
	else
	    jit_movi(_RAX, 0);
	if (reg != r0)
	    jit_unget_reg(reg);
    }
#  endif
#endif
    call = jit_callr(reg);
    call->v.w = _jitc->function->call.argi;
    call->w.w = _jitc->function->call.argf;
    _jitc->function->call.argi = _jitc->function->call.argf =
	_jitc->function->call.size = 0;
    _jitc->prepare = 0;
    jit_dec_synth();
}

jit_node_t *
_jit_finishi(jit_state_t *_jit, jit_pointer_t i0)
{
#if __X64
    jit_int32_t		reg;
#endif
    jit_node_t		*node;
    assert(_jitc->function);
    jit_inc_synth_w(finishi, (jit_word_t)i0);
    if (_jitc->function->self.alen < _jitc->function->call.size)
	_jitc->function->self.alen = _jitc->function->call.size;
#if __X64
    /* FIXME preventing %rax allocation is good enough, but for consistency
     * it should automatically detect %rax is dead, in case it has run out
     * registers, and not save/restore it, what would be wrong if using the
     * the return value, otherwise, just a needless noop */
    /* >> prevent %rax from being allocated as the function pointer */
    jit_regset_setbit(&_jitc->regarg, _RAX);
    reg = jit_get_reg(jit_class_gpr);
    node = jit_movi(reg, (jit_word_t)i0);
    jit_finishr(reg);
    jit_unget_reg(reg);
    /* << prevent %rax from being allocated as the function pointer */
    jit_regset_clrbit(&_jitc->regarg, _RAX);
#else
    node = jit_calli(i0);
    node->v.w = _jitc->function->call.argi;
    node->w.w = _jitc->function->call.argf;
#endif
    _jitc->function->call.argi = _jitc->function->call.argf =
	_jitc->function->call.size = 0;
    _jitc->prepare = 0;
    jit_dec_synth();
    return (node);
}

void
_jit_retval_c(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_c, r0);
    jit_extr_c(r0, JIT_RET);
    jit_dec_synth();
}

void
_jit_retval_uc(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_uc, r0);
    jit_extr_uc(r0, JIT_RET);
    jit_dec_synth();
}

void
_jit_retval_s(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_s, r0);
    jit_extr_s(r0, JIT_RET);
    jit_dec_synth();
}

void
_jit_retval_us(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_us, r0);
    jit_extr_us(r0, JIT_RET);
    jit_dec_synth();
}

void
_jit_retval_i(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_i, r0);
#if __X32 || __X64_32
    if (r0 != JIT_RET)
	jit_movr(r0, JIT_RET);
#else
    jit_extr_i(r0, JIT_RET);
#endif
    jit_dec_synth();
}

#if __X64 && !__X64_32
void
_jit_retval_ui(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_ui, r0);
    jit_extr_ui(r0, JIT_RET);
    jit_dec_synth();
}

void
_jit_retval_l(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_l, r0);
    if (r0 != JIT_RET)
	jit_movr(r0, JIT_RET);
    jit_dec_synth();
}
#endif

void
_jit_retval_f(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_f, r0);
#if __X64
    if (r0 != JIT_FRET)
	jit_movr_f(r0, JIT_FRET);
#endif
    jit_dec_synth();
}

void
_jit_retval_d(jit_state_t *_jit, jit_int32_t r0)
{
    jit_inc_synth_w(retval_d, r0);
#if __X64
    if (r0 != JIT_FRET)
	jit_movr_d(r0, JIT_FRET);
#endif
    jit_dec_synth();
}

jit_pointer_t
_emit_code(jit_state_t *_jit)
{
    jit_node_t		*node;
    jit_node_t		*temp;
    jit_word_t		 word;
    jit_int32_t		 value;
    jit_int32_t		 offset;
    struct {
	jit_node_t	*node;
	jit_word_t	 word;
#if DEVEL_DISASSEMBLER
	jit_word_t	 prevw;
#endif
	jit_int32_t	 patch_offset;
    } undo;
#if DEVEL_DISASSEMBLER
    jit_word_t		 prevw;
#endif

    _jitc->function = NULL;

    jit_reglive_setup();

    undo.word = 0;
    undo.node = NULL;
    undo.patch_offset = 0;
#define case_rr(name, type)						\
	    case jit_code_##name##r##type:				\
		name##r##type(rn(node->u.w), rn(node->v.w));		\
		break
#define case_rw(name, type)						\
	    case jit_code_##name##i##type:				\
		name##i##type(rn(node->u.w), node->v.w);		\
		break
#define case_rf(name, type)						\
	    case jit_code_##name##r##type:				\
		if (jit_x87_reg_p(node->v.w))				\
		    x87_##name##r##type(rn(node->u.w), rn(node->v.w));	\
		else							\
		    sse_##name##r##type(rn(node->u.w), rn(node->v.w));	\
		break
#define case_fr(name, type)						\
	    case jit_code_##name##r##type:				\
		if (jit_x87_reg_p(node->u.w))				\
		    x87_##name##r##type(rn(node->u.w), rn(node->v.w));	\
		else							\
		    sse_##name##r##type(rn(node->u.w), rn(node->v.w));	\
		break
#define case_fw(name, type)						\
	    case jit_code_##name##i##type:				\
		if (jit_x87_reg_p(node->u.w))				\
		    x87_##name##i##type(rn(node->u.w), node->v.w);	\
		else							\
		    sse_##name##i##type(rn(node->u.w), node->v.w);	\
		break
#define case_wr(name, type)						\
	    case jit_code_##name##i##type:				\
		name##i##type(node->u.w, rn(node->v.w));		\
		break
#define case_wf(name, type)						\
	    case jit_code_##name##i##type:				\
		if (jit_x87_reg_p(node->v.w))				\
		    x87_##name##i##type(node->u.w, rn(node->v.w));	\
		else							\
		    sse_##name##i##type(node->u.w, rn(node->v.w));	\
		break
#define case_ff(name, type)						\
	    case jit_code_##name##r##type:				\
		if (jit_x87_reg_p(node->u.w) &&				\
		    jit_x87_reg_p(node->v.w))				\
		    x87_##name##r##type(rn(node->u.w), rn(node->v.w));	\
		else							\
		    sse_##name##r##type(rn(node->u.w), rn(node->v.w));	\
		break;
#define case_rrr(name, type)						\
	    case jit_code_##name##r##type:				\
		name##r##type(rn(node->u.w),				\
			      rn(node->v.w), rn(node->w.w));		\
		break
#define case_rrrr(name, type)						\
	    case jit_code_##name##r##type:				\
		name##r##type(rn(node->u.q.l), rn(node->u.q.h),		\
			      rn(node->v.w), rn(node->w.w));		\
		break
#define case_frr(name, type)						\
	    case jit_code_##name##r##type:				\
		if (jit_x87_reg_p(node->u.w))				\
		    x87_##name##r##type(rn(node->u.w),			\
					rn(node->v.w), rn(node->w.w));	\
		else							\
		    sse_##name##r##type(rn(node->u.w),			\
					rn(node->v.w), rn(node->w.w));	\
		break
#define case_rrf(name, type)						\
	    case jit_code_##name##r##type:				\
		if (jit_x87_reg_p(node->w.w))				\
		    x87_##name##r##type(rn(node->u.w),			\
					rn(node->v.w), rn(node->w.w));	\
		else							\
		    sse_##name##r##type(rn(node->u.w),			\
					rn(node->v.w), rn(node->w.w));	\
		break
#define case_rrw(name, type)						\
	    case jit_code_##name##i##type:				\
		name##i##type(rn(node->u.w), rn(node->v.w), node->w.w);	\
		break
#define case_rrrw(name, type)						\
	    case jit_code_##name##i##type:				\
		name##i##type(rn(node->u.q.l), rn(node->u.q.h),		\
			      rn(node->v.w), node->w.w);		\
		break
#define case_frw(name, type)						\
	    case jit_code_##name##i##type:				\
		if (jit_x87_reg_p(node->u.w))				\
		    x87_##name##i##type(rn(node->u.w),			\
					rn(node->v.w), node->w.w);	\
		else							\
		    sse_##name##i##type(rn(node->u.w),			\
					rn(node->v.w), node->w.w);	\
		break
#define case_wrr(name, type)						\
	    case jit_code_##name##i##type:				\
		name##i##type(node->u.w, rn(node->v.w), rn(node->w.w));	\
		break
#define case_wrf(name, type)						\
	    case jit_code_##name##i##type:				\
		if (jit_x87_reg_p(node->w.w))				\
		    x87_##name##i##type(node->u.w,			\
					rn(node->v.w), rn(node->w.w));	\
		else							\
		    sse_##name##i##type(node->u.w,			\
					rn(node->v.w), rn(node->w.w));	\
		break
#define case_brr(name, type)						\
	    case jit_code_##name##r##type:				\
		temp = node->u.n;					\
		assert(temp->code == jit_code_label ||			\
		       temp->code == jit_code_epilog);			\
		if (temp->flag & jit_flag_patch)			\
		    name##r##type(temp->u.w, rn(node->v.w),		\
				  rn(node->w.w));			\
		else {							\
		    word = name##r##type(_jit->pc.w,			\
					 rn(node->v.w), rn(node->w.w));	\
		    patch(word, node);					\
		}							\
		break
#define case_brw(name, type)						\
	    case jit_code_##name##i##type:				\
		temp = node->u.n;					\
		assert(temp->code == jit_code_label ||			\
		       temp->code == jit_code_epilog);			\
		if (temp->flag & jit_flag_patch)			\
		    name##i##type(temp->u.w,				\
				  rn(node->v.w), node->w.w);		\
		else {							\
		    word = name##i##type(_jit->pc.w,			\
					 rn(node->v.w), node->w.w);	\
		    patch(word, node);					\
		}							\
		break
#define case_rff(name, type)						\
	    case jit_code_##name##r##type:				\
		if (jit_x87_reg_p(node->v.w) &&				\
		    jit_x87_reg_p(node->w.w))				\
		    x87_##name##r##type(rn(node->u.w), rn(node->v.w),	\
					rn(node->w.w));			\
		else							\
		    sse_##name##r##type(rn(node->u.w), rn(node->v.w),	\
					rn(node->w.w));			\
		break;
#define case_rfw(name, type, size)					\
	    case jit_code_##name##i##type:				\
		assert(node->flag & jit_flag_data);			\
		if (jit_x87_reg_p(node->v.w))				\
		    x87_##name##i##type(rn(node->u.w), rn(node->v.w),	\
				(jit_float##size##_t *)node->w.n->u.w);	\
		else							\
		    sse_##name##i##type(rn(node->u.w), rn(node->v.w),	\
				(jit_float##size##_t *)node->w.n->u.w);	\
		break
#define case_fff(name, type)						\
	    case jit_code_##name##r##type:				\
		if (jit_x87_reg_p(node->u.w) &&				\
		    jit_x87_reg_p(node->v.w) &&				\
		    jit_x87_reg_p(node->w.w))				\
		    x87_##name##r##type(rn(node->u.w),			\
					rn(node->v.w), rn(node->w.w));	\
		else							\
		    sse_##name##r##type(rn(node->u.w),			\
					rn(node->v.w), rn(node->w.w));	\
		break
#define case_ffw(name, type, size)					\
	    case jit_code_##name##i##type:				\
		assert(node->flag & jit_flag_data);			\
		if (jit_x87_reg_p(node->u.w) &&				\
		    jit_x87_reg_p(node->v.w))				\
		    x87_##name##i##type(rn(node->u.w), rn(node->v.w),	\
				(jit_float##size##_t *)node->w.n->u.w);	\
		else							\
		    sse_##name##i##type(rn(node->u.w), rn(node->v.w),	\
				(jit_float##size##_t *)node->w.n->u.w);	\
		break
#define case_bff(name, type)						\
	    case jit_code_b##name##r##type:				\
		temp = node->u.n;					\
		assert(temp->code == jit_code_label ||			\
		       temp->code == jit_code_epilog);			\
		if (temp->flag & jit_flag_patch) {			\
		    if (jit_x87_reg_p(node->v.w) &&			\
			jit_x87_reg_p(node->w.w))			\
			x87_b##name##r##type(temp->u.w,			\
				rn(node->v.w), rn(node->w.w));		\
		    else						\
			sse_b##name##r##type(temp->u.w,			\
				rn(node->v.w), rn(node->w.w));		\
		}							\
		else {							\
		    if (jit_x87_reg_p(node->v.w) &&			\
			jit_x87_reg_p(node->w.w))			\
			word = x87_b##name##r##type(_jit->pc.w,		\
				rn(node->v.w), rn(node->w.w));		\
		    else						\
			word = sse_b##name##r##type(_jit->pc.w,		\
				rn(node->v.w), rn(node->w.w));		\
		    patch(word, node);					\
		}							\
		break
#define case_bfw(name, type, size)					\
	    case jit_code_b##name##i##type:				\
		temp = node->u.n;					\
		assert(temp->code == jit_code_label ||			\
		       temp->code == jit_code_epilog);			\
		if (temp->flag & jit_flag_patch) {			\
		    if (jit_x87_reg_p(node->v.w))			\
			x87_b##name##i##type(temp->u.w,			\
				rn(node->v.w),				\
				(jit_float##size##_t *)node->w.n->u.w);	\
		    else						\
			sse_b##name##i##type(temp->u.w,			\
				rn(node->v.w),				\
				(jit_float##size##_t *)node->w.n->u.w);	\
		}							\
		else {							\
		    if (jit_x87_reg_p(node->v.w))			\
			word = x87_b##name##i##type(_jit->pc.w,		\
				rn(node->v.w),				\
				(jit_float##size##_t *)node->w.n->u.w);	\
		    else						\
			word = sse_b##name##i##type(_jit->pc.w,		\
				rn(node->v.w),				\
				(jit_float##size##_t *)node->w.n->u.w);	\
		    patch(word, node);					\
		}							\
		break
#if DEVEL_DISASSEMBLER
    prevw = _jit->pc.w;
#endif
    for (node = _jitc->head; node; node = node->next) {
	if (_jit->pc.uc >= _jitc->code.end)
	    return (NULL);

#if DEVEL_DISASSEMBLER
	node->offset = (jit_uword_t)_jit->pc.w - (jit_uword_t)prevw;
	prevw = _jit->pc.w;
#endif
	value = jit_classify(node->code);
	jit_regarg_set(node, value);
	switch (node->code) {
	    case jit_code_align:
		assert(!(node->u.w & (node->u.w - 1)) &&
		       node->u.w <= sizeof(jit_word_t));
		if ((word = _jit->pc.w & (node->u.w - 1)))
		    nop(node->u.w - word);
		break;
	    case jit_code_note:		case jit_code_name:
		node->u.w = _jit->pc.w;
		break;
	    case jit_code_label:
		if ((node->link || (node->flag & jit_flag_use)) &&
		    (word = _jit->pc.w & (sizeof(jit_word_t) - 1)))
		    nop(sizeof(jit_word_t) - word);
		/* remember label is defined */
		node->flag |= jit_flag_patch;
		node->u.w = _jit->pc.w;
		break;
		case_rrr(add,);
		case_rrw(add,);
		case_rrr(addx,);
		case_rrw(addx,);
		case_rrr(addc,);
		case_rrw(addc,);
		case_rrr(sub,);
		case_rrw(sub,);
		case_rrr(subx,);
		case_rrw(subx,);
		case_rrr(subc,);
		case_rrw(subc,);
		case_rrw(rsb,);
		case_rrr(mul,);
		case_rrw(mul,);
		case_rrrr(qmul,);
		case_rrrw(qmul,);
		case_rrrr(qmul, _u);
		case_rrrw(qmul, _u);
		case_rrr(div,);
		case_rrw(div,);
		case_rrr(div, _u);
		case_rrw(div, _u);
		case_rrrr(qdiv,);
		case_rrrw(qdiv,);
		case_rrrr(qdiv, _u);
		case_rrrw(qdiv, _u);
		case_rrr(rem,);
		case_rrw(rem,);
		case_rrr(rem, _u);
		case_rrw(rem, _u);
		case_rrr(and,);
		case_rrw(and,);
		case_rrr(or,);
		case_rrw(or,);
		case_rrr(xor,);
		case_rrw(xor,);
		case_rrr(lsh,);
		case_rrw(lsh,);
		case_rrr(rsh,);
		case_rrw(rsh,);
		case_rrr(rsh, _u);
		case_rrw(rsh, _u);
		case_rr(neg,);
		case_rr(com,);
		case_rrr(lt,);
		case_rrw(lt,);
		case_rrr(lt, _u);
		case_rrw(lt, _u);
		case_rrr(le,);
		case_rrw(le,);
		case_rrr(le, _u);
		case_rrw(le, _u);
		case_rrr(eq,);
		case_rrw(eq,);
		case_rrr(ge,);
		case_rrw(ge,);
		case_rrr(ge, _u);
		case_rrw(ge, _u);
		case_rrr(gt,);
		case_rrw(gt,);
		case_rrr(gt, _u);
		case_rrw(gt, _u);
		case_rrr(ne,);
		case_rrw(ne,);
		case_rr(mov,);
	    case jit_code_movi:
		if (node->flag & jit_flag_node) {
		    temp = node->v.n;
		    if (temp->code == jit_code_data ||
			(temp->code == jit_code_label &&
			 (temp->flag & jit_flag_patch)))
			movi(rn(node->u.w), temp->u.w);
		    else {
			assert(temp->code == jit_code_label ||
			       temp->code == jit_code_epilog);
			word = movi_p(rn(node->u.w), node->v.w);
			patch(word, node);
		    }
		}
		else
		    movi(rn(node->u.w), node->v.w);
		break;
		case_rr(hton, _us);
		case_rr(hton, _ui);
#if __X64 && !__X64_32
		case_rr(hton, _ul);
#endif
		case_rr(ext, _c);
		case_rr(ext, _uc);
		case_rr(ext, _s);
		case_rr(ext, _us);
#if __X64 && !__X64_32
		case_rr(ext, _i);
		case_rr(ext, _ui);
#endif
		case_rf(trunc, _f_i);
		case_rf(trunc, _d_i);
#if __X64
		case_rf(trunc, _f_l);
		case_rf(trunc, _d_l);
#endif
		case_rr(ld, _c);
		case_rw(ld, _c);
		case_rr(ld, _uc);
		case_rw(ld, _uc);
		case_rr(ld, _s);
		case_rw(ld, _s);
		case_rr(ld, _us);
		case_rw(ld, _us);
		case_rr(ld, _i);
		case_rw(ld, _i);
#if __X64 && !__X64_32
		case_rr(ld, _ui);
		case_rw(ld, _ui);
		case_rr(ld, _l);
		case_rw(ld, _l);
#endif
		case_rrr(ldx, _c);
		case_rrw(ldx, _c);
		case_rrr(ldx, _uc);
		case_rrw(ldx, _uc);
		case_rrr(ldx, _s);
		case_rrw(ldx, _s);
		case_rrr(ldx, _us);
		case_rrw(ldx, _us);
		case_rrr(ldx, _i);
		case_rrw(ldx, _i);
#if __X64 && !__X64_32
		case_rrr(ldx, _ui);
		case_rrw(ldx, _ui);
		case_rrr(ldx, _l);
		case_rrw(ldx, _l);
#endif
		case_rr(st, _c);
		case_wr(st, _c);
		case_rr(st, _s);
		case_wr(st, _s);
		case_rr(st, _i);
		case_wr(st, _i);
#if __X64 && !__X64_32
		case_rr(st, _l);
		case_wr(st, _l);
#endif
		case_rrr(stx, _c);
		case_wrr(stx, _c);
		case_rrr(stx, _s);
		case_wrr(stx, _s);
		case_rrr(stx, _i);
		case_wrr(stx, _i);
#if __X64 && !__X64_32
		case_rrr(stx, _l);
		case_wrr(stx, _l);
#endif
		case_brr(blt,);
		case_brw(blt,);
		case_brr(blt, _u);
		case_brw(blt, _u);
		case_brr(ble,);
		case_brw(ble,);
		case_brr(ble, _u);
		case_brw(ble, _u);
		case_brr(beq,);
		case_brw(beq,);
		case_brr(bge,);
		case_brw(bge,);
		case_brr(bge, _u);
		case_brw(bge, _u);
		case_brr(bgt,);
		case_brw(bgt,);
		case_brr(bgt, _u);
		case_brw(bgt, _u);
		case_brr(bne,);
		case_brw(bne,);
		case_brr(bms,);
		case_brw(bms,);
		case_brr(bmc,);
		case_brw(bmc,);
		case_brr(boadd,);
		case_brw(boadd,);
		case_brr(boadd, _u);
		case_brw(boadd, _u);
		case_brr(bxadd,);
		case_brw(bxadd,);
		case_brr(bxadd, _u);
		case_brw(bxadd, _u);
		case_brr(bosub,);
		case_brw(bosub,);
		case_brr(bosub, _u);
		case_brw(bosub, _u);
		case_brr(bxsub,);
		case_brw(bxsub,);
		case_brr(bxsub, _u);
		case_brw(bxsub, _u);
		case_fff(add, _f);
		case_ffw(add, _f, 32);
		case_fff(sub, _f);
		case_ffw(sub, _f, 32);
		case_ffw(rsb, _f, 32);
		case_fff(mul, _f);
		case_ffw(mul, _f, 32);
		case_fff(div, _f);
		case_ffw(div, _f, 32);
		case_ff(abs, _f);
		case_ff(neg, _f);
		case_ff(sqrt, _f);
		case_fr(ext, _f);
		case_fr(ext, _d_f);
		case_rff(lt, _f);
		case_rfw(lt, _f, 32);
		case_rff(le, _f);
		case_rfw(le, _f, 32);
		case_rff(eq, _f);
		case_rfw(eq, _f, 32);
		case_rff(ge, _f);
		case_rfw(ge, _f, 32);
		case_rff(gt, _f);
		case_rfw(gt, _f, 32);
		case_rff(ne, _f);
		case_rfw(ne, _f, 32);
		case_rff(unlt, _f);
		case_rfw(unlt, _f, 32);
		case_rff(unle, _f);
		case_rfw(unle, _f, 32);
		case_rff(uneq, _f);
		case_rfw(uneq, _f, 32);
		case_rff(unge, _f);
		case_rfw(unge, _f, 32);
		case_rff(ungt, _f);
		case_rfw(ungt, _f, 32);
		case_rff(ltgt, _f);
		case_rfw(ltgt, _f, 32);
		case_rff(ord, _f);
		case_rfw(ord, _f, 32);
		case_rff(unord, _f);
		case_rfw(unord, _f, 32);
	    case jit_code_movr_f:
		if (jit_x87_reg_p(node->u.w)) {
		    if (jit_x87_reg_p(node->v.w))
			x87_movr_f(rn(node->u.w), rn(node->v.w));
		    else
			x87_from_sse_f(rn(node->u.w), rn(node->v.w));
		}
		else {
		    if (jit_sse_reg_p(node->v.w))
			sse_movr_f(rn(node->u.w), rn(node->v.w));
		    else
			sse_from_x87_f(rn(node->u.w), rn(node->v.w));
		}
		break;
	    case jit_code_movi_f:
		assert(node->flag & jit_flag_data);
		if (jit_x87_reg_p(node->u.w))
		    x87_movi_f(rn(node->u.w), (jit_float32_t *)node->v.n->u.w);
		else
		    sse_movi_f(rn(node->u.w), (jit_float32_t *)node->v.n->u.w);
		break;
		case_fr(ld, _f);
		case_fw(ld, _f);
		case_frr(ldx, _f);
		case_frw(ldx, _f);
		case_rf(st, _f);
		case_wf(st, _f);
		case_rrf(stx, _f);
		case_wrf(stx, _f);
		case_bff(lt, _f);
		case_bfw(lt, _f, 32);
		case_bff(le, _f);
		case_bfw(le, _f, 32);
		case_bff(eq, _f);
		case_bfw(eq, _f, 32);
		case_bff(ge, _f);
		case_bfw(ge, _f, 32);
		case_bff(gt, _f);
		case_bfw(gt, _f, 32);
		case_bff(ne, _f);
		case_bfw(ne, _f, 32);
		case_bff(unlt, _f);
		case_bfw(unlt, _f, 32);
		case_bff(unle, _f);
		case_bfw(unle, _f, 32);
		case_bff(uneq, _f);
		case_bfw(uneq, _f, 32);
		case_bff(unge, _f);
		case_bfw(unge, _f, 32);
		case_bff(ungt, _f);
		case_bfw(ungt, _f, 32);
		case_bff(ltgt, _f);
		case_bfw(ltgt, _f, 32);
		case_bff(ord, _f);
		case_bfw(ord, _f, 32);
		case_bff(unord, _f);
		case_bfw(unord, _f, 32);
		case_fff(add, _d);
		case_ffw(add, _d, 64);
		case_fff(sub, _d);
		case_ffw(sub, _d, 64);
		case_ffw(rsb, _d, 64);
		case_fff(mul, _d);
		case_ffw(mul, _d, 64);
		case_fff(div, _d);
		case_ffw(div, _d, 64);
		case_ff(abs, _d);
		case_ff(neg, _d);
		case_ff(sqrt, _d);
		case_fr(ext, _d);
		case_fr(ext, _f_d);
		case_rff(lt, _d);
		case_rfw(lt, _d, 64);
		case_rff(le, _d);
		case_rfw(le, _d, 64);
		case_rff(eq, _d);
		case_rfw(eq, _d, 64);
		case_rff(ge, _d);
		case_rfw(ge, _d, 64);
		case_rff(gt, _d);
		case_rfw(gt, _d, 64);
		case_rff(ne, _d);
		case_rfw(ne, _d, 64);
		case_rff(unlt, _d);
		case_rfw(unlt, _d, 64);
		case_rff(unle, _d);
		case_rfw(unle, _d, 64);
		case_rff(uneq, _d);
		case_rfw(uneq, _d, 64);
		case_rff(unge, _d);
		case_rfw(unge, _d, 64);
		case_rff(ungt, _d);
		case_rfw(ungt, _d, 64);
		case_rff(ltgt, _d);
		case_rfw(ltgt, _d, 64);
		case_rff(ord, _d);
		case_rfw(ord, _d, 64);
		case_rff(unord, _d);
		case_rfw(unord, _d, 64);
	    case jit_code_movr_d:
		if (jit_x87_reg_p(node->u.w)) {
		    if (jit_x87_reg_p(node->v.w))
			x87_movr_d(rn(node->u.w), rn(node->v.w));
		    else
			x87_from_sse_d(rn(node->u.w), rn(node->v.w));
		}
		else {
		    if (jit_sse_reg_p(node->v.w))
			sse_movr_d(rn(node->u.w), rn(node->v.w));
		    else
			sse_from_x87_d(rn(node->u.w), rn(node->v.w));
		}
		break;
	    case jit_code_movi_d:
		assert(node->flag & jit_flag_data);
		if (jit_x87_reg_p(node->u.w))
		    x87_movi_d(rn(node->u.w), (jit_float64_t *)node->v.n->u.w);
		else
		    sse_movi_d(rn(node->u.w), (jit_float64_t *)node->v.n->u.w);
		break;
		case_fr(ld, _d);
		case_fw(ld, _d);
		case_frr(ldx, _d);
		case_frw(ldx, _d);
		case_rf(st, _d);
		case_wf(st, _d);
		case_rrf(stx, _d);
		case_wrf(stx, _d);
		case_bff(lt, _d);
		case_bfw(lt, _d, 64);
		case_bff(le, _d);
		case_bfw(le, _d, 64);
		case_bff(eq, _d);
		case_bfw(eq, _d, 64);
		case_bff(ge, _d);
		case_bfw(ge, _d, 64);
		case_bff(gt, _d);
		case_bfw(gt, _d, 64);
		case_bff(ne, _d);
		case_bfw(ne, _d, 64);
		case_bff(unlt, _d);
		case_bfw(unlt, _d, 64);
		case_bff(unle, _d);
		case_bfw(unle, _d, 64);
		case_bff(uneq, _d);
		case_bfw(uneq, _d, 64);
		case_bff(unge, _d);
		case_bfw(unge, _d, 64);
		case_bff(ungt, _d);
		case_bfw(ungt, _d, 64);
		case_bff(ltgt, _d);
		case_bfw(ltgt, _d, 64);
		case_bff(ord, _d);
		case_bfw(ord, _d, 64);
		case_bff(unord, _d);
		case_bfw(unord, _d, 64);
	    case jit_code_jmpr:
		jmpr(rn(node->u.w));
		break;
	    case jit_code_jmpi:
		if (node->flag & jit_flag_node) {
		    temp = node->u.n;
		    assert(temp->code == jit_code_label ||
			   temp->code == jit_code_epilog);
		    if (temp->flag & jit_flag_patch)
			jmpi(temp->u.w);
		    else {
			word = jmpi(_jit->pc.w);
			patch(word, node);
		    }
		}
		else
		    jmpi(node->u.w);
		break;
	    case jit_code_callr:
		callr(rn(node->u.w));
		break;
	    case jit_code_calli:
		if (node->flag & jit_flag_node) {
		    temp = node->u.n;
		    assert(temp->code == jit_code_label ||
			   temp->code == jit_code_epilog);
		    word = calli(temp->u.w);
		    if (!(temp->flag & jit_flag_patch))
			patch(word, node);
		}
		else
		    calli(node->u.w);
		break;
	    case jit_code_prolog:
		_jitc->function = _jitc->functions.ptr + node->w.w;
		undo.node = node;
		undo.word = _jit->pc.w;
#if DEVEL_DISASSEMBLER
		undo.prevw = prevw;
#endif
		undo.patch_offset = _jitc->patches.offset;
	    restart_function:
		_jitc->again = 0;
		prolog(node);
		break;
	    case jit_code_epilog:
		assert(_jitc->function == _jitc->functions.ptr + node->w.w);
		if (_jitc->again) {
		    for (temp = undo.node->next;
			 temp != node; temp = temp->next) {
			if (temp->code == jit_code_label ||
			    temp->code == jit_code_epilog)
			    temp->flag &= ~jit_flag_patch;
		    }
		    temp->flag &= ~jit_flag_patch;
		    node = undo.node;
		    _jit->pc.w = undo.word;
#if DEVEL_DISASSEMBLER
		    prevw = undo.prevw;
#endif
		    _jitc->patches.offset = undo.patch_offset;
		    goto restart_function;
		}
		if (node->link &&
		    (word = _jit->pc.w & (sizeof(jit_word_t) - 1)))
		    nop(sizeof(jit_word_t) - word);
		/* remember label is defined */
		node->flag |= jit_flag_patch;
		node->u.w = _jit->pc.w;
		epilog(node);
		_jitc->function = NULL;
		break;
	    case jit_code_va_start:
		vastart(rn(node->u.w));
		break;
	    case jit_code_va_arg:
		vaarg(rn(node->u.w), rn(node->v.w));
		break;
	    case jit_code_va_arg_d:
		vaarg_d(rn(node->u.w), rn(node->v.w), jit_x87_reg_p(node->u.w));
		break;
	    case jit_code_live:			case jit_code_ellipsis:
	    case jit_code_va_push:
	    case jit_code_allocai:		case jit_code_allocar:
	    case jit_code_arg:
	    case jit_code_arg_f:		case jit_code_arg_d:
	    case jit_code_va_end:
	    case jit_code_ret:
	    case jit_code_retr:			case jit_code_reti:
	    case jit_code_retr_f:		case jit_code_reti_f:
	    case jit_code_retr_d:		case jit_code_reti_d:
	    case jit_code_getarg_c:		case jit_code_getarg_uc:
	    case jit_code_getarg_s:		case jit_code_getarg_us:
	    case jit_code_getarg_i:
#if __X64 && !__X64_32
	    case jit_code_getarg_ui:		case jit_code_getarg_l:
#endif
	    case jit_code_getarg_f:		case jit_code_getarg_d:
	    case jit_code_putargr:		case jit_code_putargi:
	    case jit_code_putargr_f:		case jit_code_putargi_f:
	    case jit_code_putargr_d:		case jit_code_putargi_d:
	    case jit_code_pushargr:		case jit_code_pushargi:
	    case jit_code_pushargr_f:		case jit_code_pushargi_f:
	    case jit_code_pushargr_d:		case jit_code_pushargi_d:
	    case jit_code_retval_c:		case jit_code_retval_uc:
	    case jit_code_retval_s:		case jit_code_retval_us:
	    case jit_code_retval_i:
#if __X64 && !__X32
	    case jit_code_retval_ui:		case jit_code_retval_l:
#endif
	    case jit_code_prepare:
	    case jit_code_finishr:		case jit_code_finishi:
		break;
	    case jit_code_retval_f:
#if __X32
		if (jit_sse_reg_p(node->u.w)) {
		    fstpr(_ST1_REGNO);
		    sse_from_x87_f(rn(node->u.w), _ST0_REGNO);
		}
		else
		    fstpr(rn(node->u.w) + 1);
#endif
		break;
	    case jit_code_retval_d:
#if __X32
		if (jit_sse_reg_p(node->u.w)) {
		    fstpr(_ST1_REGNO);
		    sse_from_x87_d(rn(node->u.w), _ST0_REGNO);
		}
		else
		    fstpr(rn(node->u.w) + 1);
#endif
		break;
	    default:
		abort();
	}
	jit_regarg_clr(node, value);
	assert(_jitc->regarg == 0 && _jitc->synth == 0);
	/* update register live state */
	jit_reglive(node);
    }
#undef case_bfw
#undef case_bff
#undef case_ffw
#undef case_rfw
#undef case_rff
#undef case_brw
#undef case_brr
#undef case_wrf
#undef case_wrr
#undef case_frw
#undef case_rrf
#undef case_rrw
#undef case_frr
#undef case_rrr
#undef case_wf
#undef case_fw
#undef case_fr
#undef case_rr

    for (offset = 0; offset < _jitc->patches.offset; offset++) {
	node = _jitc->patches.ptr[offset].node;
	word = node->code == jit_code_movi ? node->v.n->u.w : node->u.n->u.w;
	patch_at(node, _jitc->patches.ptr[offset].inst, word);
    }

    jit_flush(_jit->code.ptr, _jit->pc.uc);

    return (_jit->code.ptr);
}

#define CODE				1
#  include "jit_x86-cpu.c"
#  include "jit_x86-sse.c"
#  include "jit_x86-x87.c"
#undef CODE

void
jit_flush(void *fptr, void *tptr)
{
}

void
_emit_ldxi(jit_state_t *_jit, jit_gpr_t r0, jit_gpr_t r1, jit_word_t i0)
{
    ldxi(rn(r0), rn(r1), i0);
}

void
_emit_stxi(jit_state_t *_jit, jit_word_t i0, jit_gpr_t r0, jit_gpr_t r1)
{
    stxi(i0, rn(r0), rn(r1));
}

void
_emit_ldxi_d(jit_state_t *_jit, jit_fpr_t r0, jit_gpr_t r1, jit_word_t i0)
{
    if (jit_x87_reg_p(r0))
	x87_ldxi_d(rn(r0), rn(r1), i0);
    else
	sse_ldxi_d(rn(r0), rn(r1), i0);
}

void
_emit_stxi_d(jit_state_t *_jit, jit_word_t i0, jit_gpr_t r0, jit_fpr_t r1)
{
    if (jit_x87_reg_p(r1))
	x87_stxi_d(i0, rn(r0), rn(r1));
    else
	sse_stxi_d(i0, rn(r0), rn(r1));
}

static void
_patch(jit_state_t *_jit, jit_word_t instr, jit_node_t *node)
{
    jit_int32_t		flag;

    assert(node->flag & jit_flag_node);
    if (node->code == jit_code_movi)
	flag = node->v.n->flag;
    else
	flag = node->u.n->flag;
    assert(!(flag & jit_flag_patch));
    if (_jitc->patches.offset >= _jitc->patches.length) {
	jit_realloc((jit_pointer_t *)&_jitc->patches.ptr,
		    _jitc->patches.length * sizeof(jit_patch_t),
		    (_jitc->patches.length + 1024) * sizeof(jit_patch_t));
	_jitc->patches.length += 1024;
    }
    _jitc->patches.ptr[_jitc->patches.offset].inst = instr;
    _jitc->patches.ptr[_jitc->patches.offset].node = node;
    ++_jitc->patches.offset;
}

static void
_sse_from_x87_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    x87_stxi_f(CVT_OFFSET, _RBP_REGNO, r1);
    sse_ldxi_f(r0, _RBP_REGNO, CVT_OFFSET);
}

static void
_sse_from_x87_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    x87_stxi_d(CVT_OFFSET, _RBP_REGNO, r1);
    sse_ldxi_d(r0, _RBP_REGNO, CVT_OFFSET);
}

static void
_x87_from_sse_f(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    sse_stxi_f(CVT_OFFSET, _RBP_REGNO, r1);
    x87_ldxi_f(r0, _RBP_REGNO, CVT_OFFSET);
}

static void
_x87_from_sse_d(jit_state_t *_jit, jit_int32_t r0, jit_int32_t r1)
{
    sse_stxi_d(CVT_OFFSET, _RBP_REGNO, r1);
    x87_ldxi_d(r0, _RBP_REGNO, CVT_OFFSET);
}
