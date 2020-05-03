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

#ifndef _jit_private_h
#define _jit_private_h

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include <assert.h>
#include <limits.h>
#include <stdio.h>

#ifdef STDC_HEADERS
#  include <stddef.h>
#else
#  if !defined(offsetof)
#    define offsetof(type, field) ((char *)&((type *)0)->field - (char *)0)
#  endif
#endif

#if defined(__GNUC__)
#  define maybe_unused		__attribute__ ((unused))
#  define unlikely(exprn)	__builtin_expect(!!(exprn), 0)
#  define likely(exprn)		__builtin_expect(!!(exprn), 1)
#  if (__GNUC__ >= 4)
#    define PUBLIC		__attribute__ ((visibility("default")))
#    define HIDDEN		__attribute__ ((visibility("hidden")))
#  else
#    define PUBLIC		/**/
#    define HIDDEN		/**/
#  endif
#else
#  define maybe_unused		/**/
#  define unlikely(exprn)	exprn
#  define likely(exprn)		exprn
#  define PUBLIC		/**/
#  define HIDDEN		/**/
#endif

#define rc(value)		jit_class_##value
#define rn(reg)			(jit_regno(_rvs[jit_regno(reg)].spec))

#if defined(__i386__) || defined(__x86_64__)
#  define JIT_SP		_RSP
#  define JIT_RET		_RAX
#  if __X32
#    define JIT_FRET		_ST0
typedef jit_uint32_t		jit_regset_t;
#  else
#    if __CYGWIN__ || _WIN32
#      define JIT_RA0		_RCX
#    else
#      define JIT_RA0		_RDI
#    endif
#    define JIT_FA0		_XMM0
#    define JIT_FRET		_XMM0
typedef jit_uint64_t		jit_regset_t;
#  endif
#elif defined(__mips__)
#  define JIT_RA0		_A0
#  define JIT_FA0		_F12
#  define JIT_SP		_SP
#  define JIT_RET		_V0
#  define JIT_FRET		_F0
typedef jit_uint64_t		jit_regset_t;
#elif defined(__arm__)
#  define JIT_RA0		_R0
#  define JIT_FA0		_D0
#  define JIT_SP		_R13
#  define JIT_RET		_R0
#  if defined(__ARM_PCS_VFP)
#    define JIT_FRET		_D0
#  else
#    define JIT_FRET		_R0
#  endif
typedef jit_uint64_t		jit_regset_t;
#elif defined(__powerpc__)
#  define JIT_RA0		_R3
#  define JIT_FA0		_F1
#  define JIT_SP		_R1
#  define JIT_RET		_R3
#  define JIT_FRET		_F1
typedef jit_uint64_t		jit_regset_t;
#elif defined(__sparc__)
#  define JIT_SP		_SP
#  define JIT_RET		_I0
#  define JIT_FRET		_F0
#  if __WORDSIZE == 32
typedef jit_uint64_t		jit_regset_t;
#  else
typedef struct {
    jit_uint64_t	rl;
    jit_uint64_t	rh;
} jit_regset_t;
#  endif
#elif defined(__ia64__)
#  define JIT_SP		_R12
#  define JIT_RET		_R8
#  define JIT_FRET		_F8
typedef struct {
    jit_uint64_t	rl;
    jit_uint64_t	rh;
    jit_uint64_t	fl;
    jit_uint64_t	fh;
} jit_regset_t;
#elif defined(__hppa__)
#  define JIT_SP		_R30
#  define JIT_RET		_R28
#  define JIT_FRET		_F4
typedef jit_uint64_t		jit_regset_t;
#elif defined(__aarch64__)
#  define JIT_RA0		_R0
#  define JIT_FA0		_V0
#  define JIT_SP		_SP
#  define JIT_RET		_R0
#  define JIT_FRET		_V0
typedef jit_uint64_t		jit_regset_t;
#elif defined(__s390__) || defined(__s390x__)
#  define JIT_SP		_R15
#  define JIT_RET		_R2
#  define JIT_FRET		_F0
typedef jit_uint32_t		jit_regset_t;
#elif defined(__alpha__)
#  define JIT_SP		_SP
#  define JIT_RET		_V0
#  define JIT_FRET		_F0
typedef jit_uint64_t		jit_regset_t;
#elif defined(__riscv)
#  define JIT_RA0		_A0
#  define JIT_FA0		_FA0
#  define JIT_SP		_SP
#  define JIT_RET		_A0
#  define JIT_FRET		_FA0
typedef jit_uint64_t		jit_regset_t;
#endif

#define jit_data(u,v,w)		_jit_data(_jit,u,v,w)
extern jit_node_t *_jit_data(jit_state_t*, const void*,
			     jit_word_t, jit_int32_t);

#define jit_size(vector)	(sizeof(vector) / sizeof((vector)[0]))

#define jit_reg_free_p(regno)						\
    (!jit_regset_tstbit(&_jitc->reglive, regno) &&			\
     !jit_regset_tstbit(&_jitc->regarg, regno) &&			\
     !jit_regset_tstbit(&_jitc->regsav, regno))

#define jit_reg_free_if_spill_p(regno)					\
    (!jit_regset_tstbit(&_jitc->regarg, regno) &&			\
     !jit_regset_tstbit(&_jitc->regsav, regno))

#define jit_inc_synth(code)						\
    do {								\
	(void)jit_new_node(jit_code_##code);				\
	jit_synth_inc();						\
    } while (0)
#define jit_inc_synth_w(code, u)					\
    do {								\
	(void)jit_new_node_w(jit_code_##code, u);			\
	jit_synth_inc();						\
    } while (0)
#define jit_inc_synth_f(code, u)					\
    do {								\
	(void)jit_new_node_f(jit_code_##code, u);			\
	jit_synth_inc();						\
    } while (0)
#define jit_inc_synth_d(code, u)					\
    do {								\
	(void)jit_new_node_d(jit_code_##code, u);			\
	jit_synth_inc();						\
    } while (0)
#define jit_inc_synth_ww(code, u, v)					\
    do {								\
	(void)jit_new_node_ww(jit_code_##code, u, v);			\
	jit_synth_inc();						\
    } while (0)
#define jit_inc_synth_wp(code, u, v)					\
    do {								\
	(void)jit_new_node_wp(jit_code_##code, u, v);			\
	jit_synth_inc();						\
    } while (0)
#define jit_inc_synth_fp(code, u, v)					\
    do {								\
	(void)jit_new_node_fp(jit_code_##code, u, v);			\
	jit_synth_inc();						\
    } while (0)
#define jit_inc_synth_dp(code, u, v)					\
    do {								\
	(void)jit_new_node_dp(jit_code_##code, u, v);			\
	jit_synth_inc();						\
    } while (0)
#define jit_dec_synth()		jit_synth_dec()

#define jit_link_prolog()						\
    do {								\
	_jitc->tail->link = _jitc->function->prolog->link;		\
	_jitc->function->prolog->link = _jitc->tail;			\
    } while (0)
#define jit_link_prepare()						\
    do {								\
	_jitc->tail->link = _jitc->prepare->link;			\
	_jitc->prepare->link = _jitc->tail;				\
    } while (0)
#define jit_link_reverse(where)						\
    do {								\
	jit_node_t	*tmp, *tail = 0;				\
	while (where) {							\
	    tmp = (where)->link;					\
	    (where)->link = tail;					\
	    tail = where;						\
	    where = tmp;						\
	}								\
	where = tail;							\
    } while (0);

/*
 * Private jit_class bitmasks
 */
#define jit_class_named		0x00400000	/* hit must be the named reg */
#define jit_class_nospill	0x00800000	/* hint to fail if need spill */
#define jit_class_sft		0x01000000	/* not a hardware register */
#define jit_class_rg8		0x04000000	/* x86 8 bits */
#define jit_class_xpr		0x80000000	/* float / vector */
/* Used on sparc64 where %f0-%f31 can be encode for single float
 * but %f32 to %f62 only as double precision */
#define jit_class_sng		0x10000000	/* Single precision float */
#define jit_class_dbl		0x20000000	/* Only double precision float */
#define jit_regno_patch		0x00008000	/* this is a register
						 * returned by a "user" call
						 * to jit_get_reg() */

#define jit_call_default	0
#define jit_call_varargs	1

#define jit_kind_register	1
#define jit_kind_code		2
#define jit_kind_word		3
#define jit_kind_float32	4
#define jit_kind_float64	5

#define jit_cc_a0_reg		0x00000001	/* arg0 is a register */
#define jit_cc_a0_chg		0x00000002	/* arg0 is modified */
#define jit_cc_a0_jmp		0x00000004	/* arg0 is a jump target */
#define jit_cc_a0_rlh		0x00000008	/* arg0 is a register pair */
#define jit_cc_a0_int		0x00000010	/* arg0 is immediate word */
#define jit_cc_a0_flt		0x00000020	/* arg0 is immediate float */
#define jit_cc_a0_dbl		0x00000040	/* arg0 is immediate double */
#define jit_cc_a0_arg		0x00000080	/* arg1 is an argument int id */
#define jit_cc_a1_reg		0x00000100	/* arg1 is a register */
#define jit_cc_a1_chg		0x00000200	/* arg1 is modified */
#define jit_cc_a1_int		0x00001000	/* arg1 is immediate word */
#define jit_cc_a1_flt		0x00002000	/* arg1 is immediate float */
#define jit_cc_a1_dbl		0x00004000	/* arg1 is immediate double */
#define jit_cc_a1_arg		0x00008000	/* arg1 is an argument node */
#define jit_cc_a2_reg		0x00010000	/* arg2 is a register */
#define jit_cc_a2_chg		0x00020000	/* arg2 is modified */
#define jit_cc_a2_int		0x00100000	/* arg2 is immediate word */
#define jit_cc_a2_flt		0x00200000	/* arg2 is immediate float */
#define jit_cc_a2_dbl		0x00400000	/* arg2 is immediate double */

#if __ia64__ || (__sparc__ && __WORDSIZE == 64)
extern void
jit_regset_com(jit_regset_t*, jit_regset_t*);

extern void
jit_regset_and(jit_regset_t*, jit_regset_t*, jit_regset_t*);

extern void
jit_regset_ior(jit_regset_t*, jit_regset_t*, jit_regset_t*);

extern void
jit_regset_xor(jit_regset_t*, jit_regset_t*, jit_regset_t*);

extern void
jit_regset_set(jit_regset_t*, jit_regset_t*);

extern void
jit_regset_set_mask(jit_regset_t*, jit_int32_t);

extern jit_bool_t
jit_regset_cmp_ui(jit_regset_t*, jit_word_t);

extern void
jit_regset_set_ui(jit_regset_t*, jit_word_t);

extern jit_bool_t
jit_regset_set_p(jit_regset_t*);

extern void
jit_regset_clrbit(jit_regset_t*, jit_int32_t);

extern void
jit_regset_setbit(jit_regset_t*, jit_int32_t);

extern jit_bool_t
jit_regset_tstbit(jit_regset_t*, jit_int32_t);
#  if __sparc__ && __WORDSIZE == 64
#    define jit_regset_new(set)						\
    do { (set)->rl = (set)->rh = 0; } while (0)
#    define jit_regset_del(set)						\
    do { (set)->rl = (set)->rh = 0; } while (0)
#  else
#    define jit_regset_new(set)						\
    do { (set)->rl = (set)->rh = (set)->fl = (set)->fh = 0; } while (0)
#    define jit_regset_del(set)						\
    do { (set)->rl = (set)->rh = (set)->fl = (set)->fh = 0; } while (0)
#  endif
#else
#  define jit_regset_com(u, v)		(*(u) = ~*(v))
#  define jit_regset_and(u, v, w)	(*(u) = *(v) & *(w))
#  define jit_regset_ior(u, v, w)	(*(u) = *(v) | *(w))
#  define jit_regset_xor(u, v, w)	(*(u) = *(v) ^ *(w))
#  define jit_regset_set(u, v)		(*(u) = *(v))
#  define jit_regset_set_mask(u, v)	(*(u) = (1LL << (v)) - 1)
#  define jit_regset_cmp_ui(u, v)	(*(u) != (v))
#  define jit_regset_set_ui(u, v)	(*(u) = (v))
#  define jit_regset_set_p(set)		(*set)
#  define jit_regset_clrbit(set, bit)	(*(set) &= ~(1LL << (bit)))
#  define jit_regset_setbit(set, bit)	(*(set) |= 1LL << (bit))
#  define jit_regset_tstbit(set, bit)	(*(set) & (1LL << (bit)))
#  define jit_regset_new(set)		(*(set) = 0)
#  define jit_regset_del(set)		(*(set) = 0)
#endif
extern unsigned long
jit_regset_scan1(jit_regset_t*, jit_int32_t);

#define jit_reglive_setup()						\
    do {								\
	jit_regset_set_ui(&_jitc->reglive, 0);				\
	jit_regset_set_ui(&_jitc->regmask, 0);				\
    } while (0)

/*
 * Types
 */
typedef union jit_data		jit_data_t;
typedef struct jit_note		jit_note_t;
typedef struct jit_line		jit_line_t;
typedef struct jit_block	jit_block_t;
typedef struct jit_value	jit_value_t;
typedef struct jit_compiler	jit_compiler_t;
typedef struct jit_function	jit_function_t;
typedef struct jit_register	jit_register_t;
#if __arm__
#  if DISASSEMBLER
typedef struct jit_data_info	jit_data_info_t;
#  endif
#endif

union jit_data {
    struct {
#if __BYTE_ORDER == __LITTLE_ENDIAN
	jit_int32_t	 l;
	jit_int32_t	 h;
#else
	jit_int32_t	 h;
	jit_int32_t	 l;
#endif
    } q;
    jit_word_t		 w;
    jit_float32_t	 f;
    jit_float64_t	 d;
    jit_pointer_t	 p;
    jit_node_t		*n;
};

struct jit_note {
    jit_uint8_t		*code;
    char		*name;
    jit_line_t		*lines;
    jit_word_t		 length;
    jit_word_t		 size;		/* of code */
};

struct jit_line {
    char		*file;
    jit_int32_t		*linenos;
    jit_int32_t		*offsets;
    jit_word_t		 length;
};

struct jit_node {
    jit_node_t		*next;
    jit_code_t		 code;
    jit_uint16_t	 flag;
    jit_uint16_t	 offset;	/* Used if DEVEL_DISASSEMBLER */
    jit_data_t		 u;
    jit_data_t		 v;
    jit_data_t		 w;
    jit_node_t		*link;
};

struct jit_block {
    jit_node_t		*label;
    jit_regset_t	 reglive;
    jit_regset_t	 regmask;
};

struct jit_value {
    jit_int32_t		kind;
    jit_code_t		code;
    jit_data_t		base;
    jit_data_t		disp;
};

typedef struct {
#if __arm__
    jit_word_t		 kind;
#endif
    jit_word_t		 inst;
    jit_node_t		*node;
} jit_patch_t;

#if __arm__ && DISASSEMBLER
struct jit_data_info {
    jit_uword_t		  code;		/* pointer in code buffer */
    jit_word_t		  length;	/* length of constant vector */
};
#endif

struct jit_function {
    struct {
	jit_int32_t	 argi;
	jit_int32_t	 argf;
	jit_int32_t	 size;
	jit_int32_t	 aoff;
	jit_int32_t	 alen;
	jit_int32_t	 call;
	jit_int32_t	 argn;		/* for debug output */
    } self;
    struct {
	jit_int32_t	 argi;
	jit_int32_t	 argf;
	jit_int32_t	 size;
	jit_int32_t	 call;
    } call;
    jit_node_t		*prolog;
    jit_node_t		*epilog;
    jit_int32_t		*regoff;
    jit_regset_t	 regset;
    jit_int32_t		 stack;

    /* Helper for common jit generation pattern, used in GNU Smalltalk
     * and possibly others, where a static frame layout is required or
     * assumed. */
    jit_int32_t		 frame;
    jit_uint32_t	 define_frame : 1;
    jit_uint32_t	 assume_frame : 1;

    /* alloca offset offset */
    jit_int32_t		 aoffoff;
    /* uses allocar flag */
    jit_uint32_t	 allocar : 1;

    /* varargs state offsets */
    jit_int32_t		 vaoff;		/* offset of jit_va_list */
    jit_int32_t		 vagp;		/* first gp va argument */
    jit_int32_t		 vafp;		/* first fp va argument */
};

/* data used only during jit generation */
struct jit_compiler {
#if __ia64__
    struct {
	jit_uint64_t	  i : 41;
	jit_uint64_t	  t :  4;
    } inst[3];
    jit_regset_t	  regs;		/* changed regs since last stop */
    jit_int32_t		  pred;		/* changed preds last stop */
    jit_int32_t		  ioff;		/* offset in inst vector */
    jit_int32_t		  rout;		/* first output register */
    jit_int32_t		  breg;		/* base register for prolog/epilog */
#endif
#if __mips__ || __ia64__ || __alpha__ || \
	(__sparc__ && __WORDSIZE == 64) || __riscv
    jit_int32_t		  carry;
#define jit_carry	  _jitc->carry
#endif
    jit_node_t		 *head;
    jit_node_t		 *tail;
    jit_node_t		 *prepare;	/* inside prepare/finish* block */
    jit_uint32_t	  realize : 1;	/* jit_realize() called? */
    jit_uint32_t	  dataset : 1;	/* jit_dataset() called? */
    jit_uint32_t	  done	: 1;	/* emit state finished */
    jit_uint32_t	  emit	: 1;	/* emit state entered */
    jit_uint32_t	  again	: 1;	/* start over emiting function */
    jit_uint32_t	  synth : 8;	/* emiting synthesized instructions */
#if DEBUG
    jit_uint32_t	  getreg : 1;
#endif
    jit_uint32_t	  no_data : 1;
    jit_uint32_t	  no_note : 1;
    jit_int32_t		  reglen;	/* number of registers */
    jit_regset_t	  regarg;	/* cannot allocate */
    jit_regset_t	  regsav;	/* automatic spill only once */
    jit_regset_t	  reglive;	/* known live registers at some point */
    jit_regset_t	  regmask;	/* register mask to update reglive */
    struct {
	jit_uint8_t	 *end;
    } code;
    struct {
	jit_uint8_t	 *ptr;
	jit_node_t	**table;	/* very simple hash table */
	jit_word_t	  size;		/* number of vectors in table */
	jit_word_t	  count;	/* number of hash table entries */
	jit_word_t	  offset;	/* offset in bytes in ptr */
    } data;
    jit_node_t		**spill;
    jit_int32_t		 *gen;		/* ssa like "register version" */
    jit_value_t		 *values;	/* temporary jit_value_t vector */
    struct {
	jit_block_t	 *ptr;
	jit_word_t	  offset;
	jit_word_t	  length;
    } blocks;				/* basic blocks */
    struct {
	jit_patch_t	 *ptr;
	jit_word_t	  offset;
	jit_word_t	  length;
    } patches;				/* forward patch information */
    jit_function_t	 *function;	/* current function */
    struct {
	jit_function_t	 *ptr;
	jit_word_t	  offset;
	jit_word_t	  length;
    } functions;			/* prolog/epilogue offsets in code */
    struct {
	jit_node_t	**ptr;
	jit_word_t	  offset;
	jit_word_t	  length;
    } pool;
    jit_node_t		 *list;
    struct {
	jit_node_t	 *head;		/* first note node */
	jit_node_t	 *tail;		/* linked list insertion */
	/* fields to store temporary state information */
	jit_word_t	  size;
	jit_node_t	 *name;
	jit_node_t	 *note;
	jit_uint8_t	 *base;
    } note;
#if __arm__
    /* prevent using thumb instructions that set flags? */
    jit_uint32_t	  no_set_flags : 1;
#  if DISASSEMBLER
    struct {
	jit_data_info_t	 *ptr;
	jit_word_t	  offset;
	jit_word_t	  length;
    } data_info;			/* constant pools information */
#  endif
    /* Note that this field is somewhat hackish, but required by most
     * ways to implement jit, unless implementing a pure one function
     * per jit, as most times it needs to start the jit buffer with a
     * jump where the "main" prolog starts, and because the initial
     * code is in "arm mode", need to make an "arm mode" patch on that
     * jump. A good example is the test suite assembler, where most
     * test cases start with a "jmpi main" call. */
    jit_uword_t		  thumb;
    struct {
	jit_uint8_t	 *data;		/* pointer to code */
	jit_word_t	  size;		/* size data */
	jit_word_t	  offset;	/* pending patches */
	jit_word_t	  length;	/* number of pending constants */
	jit_int32_t	  values[1024];	/* pending constants */
	jit_word_t	  patches[2048];
    } consts;
#elif (__powerpc__ && _CALL_AIXDESC) || __ia64__
    /* Keep track of prolog addresses, just for the sake of making
     * jit that starts with a jump to a "main" label work like other
     * backends. */
    struct {
	jit_word_t	 *ptr;
	jit_word_t	  offset;
	jit_word_t	  length;
    } prolog;
    jit_bool_t		  jump;
#endif
#if GET_JIT_SIZE
    /* Temporary storage to calculate instructions length */
    jit_word_t		  size;
    /* Global flag for code buffer heuristic size computation */
    jit_word_t		  mult;
    /* Pointer to code to prevent miscalculation if reallocating buffer */
    jit_uint8_t		 *cptr;
#endif
};

#define _jitc				_jit->comp
struct jit_state {
    union {
	jit_uint8_t	 *uc;
	jit_uint16_t	 *us;
	jit_uint32_t	 *ui;
	jit_uint64_t	 *ul;
	jit_word_t	  w;
    } pc;
    struct {
	jit_uint8_t	*ptr;
	jit_word_t	 length;
    } code;
    struct {
	jit_uint8_t	*ptr;
	jit_word_t	 length;
    } data;
    struct {
	jit_note_t	*ptr;
	jit_word_t	 length;
    } note;
    jit_compiler_t	*comp;
    /* Flags to know if user did set the code and data buffers */
    jit_uint32_t	 user_code	: 1;
    jit_uint32_t	 user_data	: 1;
};

struct jit_register {
    jit_reg_t		 spec;
    char		*name;
};

/*
 * Prototypes
 */
extern void jit_get_cpu(void);

#define jit_init()			_jit_init(_jit)
extern void _jit_init(jit_state_t*);

#define jit_synth_inc()			_jit_synth_inc(_jit)
extern void _jit_synth_inc(jit_state_t*);

#define jit_new_node_no_link(u)		_jit_new_node_no_link(_jit, u)
extern jit_node_t *_jit_new_node_no_link(jit_state_t*, jit_code_t);

#define jit_link_node(u)		_jit_link_node(_jit, u)
extern void _jit_link_node(jit_state_t*, jit_node_t*);

#define jit_link_label(l)	_jit_link_label(_jit,l)
extern void
_jit_link_label(jit_state_t*,jit_node_t*);

#define jit_synth_dec()			_jit_synth_dec(_jit)
extern void _jit_synth_dec(jit_state_t*);

#define jit_reglive(node)	_jit_reglive(_jit, node)
extern void
_jit_reglive(jit_state_t*, jit_node_t*);

#define jit_regarg_set(n,v)	_jit_regarg_set(_jit,n,v)
extern void
_jit_regarg_set(jit_state_t*, jit_node_t*, jit_int32_t);

#define jit_regarg_clr(n,v)	_jit_regarg_clr(_jit,n,v)
extern void
_jit_regarg_clr(jit_state_t*, jit_node_t*, jit_int32_t);

#define jit_get_reg(s)		_jit_get_reg(_jit,s)
extern jit_int32_t
_jit_get_reg(jit_state_t*, jit_int32_t);

#define jit_unget_reg(r)	_jit_unget_reg(_jit,r)
extern void
_jit_unget_reg(jit_state_t*, jit_int32_t);

#define jit_save(reg)		_jit_save(_jit, reg)
extern void
_jit_save(jit_state_t*, jit_int32_t);

#define jit_load(reg)		_jit_load(_jit, reg)
extern void
_jit_load(jit_state_t*, jit_int32_t);

#define jit_trampoline(u,v)	_jit_trampoline(_jit, u, v)
extern void _jit_trampoline(jit_state_t*, jit_int32_t, jit_bool_t);

#define jit_optimize()		_jit_optimize(_jit)
extern void
_jit_optimize(jit_state_t*);

#define jit_classify(code)	_jit_classify(_jit, code)
extern jit_int32_t
_jit_classify(jit_state_t*, jit_code_t);

#define jit_regarg_p(n, r)	_jit_regarg_p(_jit, n, r)
extern jit_bool_t
_jit_regarg_p(jit_state_t*, jit_node_t*, jit_int32_t);

#define emit_code()		_emit_code(_jit)
extern jit_pointer_t
_emit_code(jit_state_t*);

extern void
jit_flush(void *fptr, void *tptr);

#define emit_ldxi(r0, r1, i0)	_emit_ldxi(_jit, r0, r1, i0)
extern void
_emit_ldxi(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);

#define emit_stxi(i0, r0, r1)	_emit_stxi(_jit, i0, r0, r1)
extern void
_emit_stxi(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);

#define emit_ldxi_d(r0, r1, i0)	_emit_ldxi_d(_jit, r0, r1, i0)
extern void
_emit_ldxi_d(jit_state_t*, jit_int32_t, jit_int32_t, jit_word_t);

#define emit_stxi_d(i0, r0, r1)	_emit_stxi_d(_jit, i0, r0, r1)
extern void
_emit_stxi_d(jit_state_t*, jit_word_t, jit_int32_t, jit_int32_t);

extern void jit_init_debug(const char*);
extern void jit_finish_debug(void);

extern void jit_init_note(void);
extern void jit_finish_note(void);
#define jit_set_note(n,u,v,w)	_jit_set_note(_jit, n, u, v, w)
extern void _jit_set_note(jit_state_t*, jit_note_t*, char*, int, jit_int32_t);
#define jit_annotate()		_jit_annotate(_jit)
extern void _jit_annotate(jit_state_t*);

#define jit_print_node(u)	_jit_print_node(_jit,u)
extern void _jit_print_node(jit_state_t*,jit_node_t*);

extern jit_pointer_t jit_memcpy(jit_pointer_t,const void*,jit_word_t);
extern jit_pointer_t jit_memmove(jit_pointer_t,const void*,jit_word_t);
extern void jit_alloc(jit_pointer_t*, jit_word_t);
extern void jit_realloc(jit_pointer_t*, jit_word_t, jit_word_t);
void jit_free(jit_pointer_t*);

extern void jit_init_size(void);
extern void jit_finish_size(void);

#if GET_JIT_SIZE
#  define jit_size_prepare()		_jit_size_prepare(_jit)
extern void
_jit_size_prepare(jit_state_t*);

#  define jit_size_collect(node)	_jit_size_collect(_jit, node)
extern void
_jit_size_collect(jit_state_t*, jit_node_t*);
#else
#  define jit_get_size()		_jit_get_size(_jit)
extern jit_word_t
_jit_get_size(jit_state_t*);
#endif

extern jit_word_t
jit_get_max_instr(void);

/*
 * Externs
 */
extern jit_register_t	 _rvs[];

#endif /* _jit_private_h */
