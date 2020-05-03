#include "ldst.inc"

#if __WORDSIZE == 64
#  define LDSTL(N, R0)						\
	sti_i ui0 %R0						\
	movi %R0 L##N						\
	sti_l l0 %R0

#  define SI(C, N, x, X, R0)					\
	ldi_##x %R0 x##0					\
	beqi L##x##C %R0 L##X##N				\
	calli @abort						\
L##x##C:

#  define LDRL(C, N, R0)					\
	UI(C, N, i, I, R0)					\
	SI(C, N, l, L, R0)
#else
#  define LDSTL(C, R0)
#  define SI(C, N, x, X, R0)					\
	ldi_##x %R0 x##0					\
	beqi L##x##C %R0 I##X##N				\
	calli @abort						\
L##x##C:

#  define LDRL(C, N, R0)

#endif

#define UI(C, N, x, X, R0)					\
	ldi_u##x %R0 u##x##0					\
	beqi Lu##x##C %R0 X##N					\
	calli @abort						\
Lu##x##C:

#define FF(C, N, x, X, F0)					\
	ldi_##x %F0 x##0					\
	beqi_##x L##x##C %F0 X##N				\
L##x##C:

#define LDST1(X, N, R0, F0)					\
	movi %R0 C##N						\
	sti_c c0 %R0						\
	sti_c uc0 %R0						\
	movi %R0 S##N						\
	sti_s s0 %R0						\
	sti_s us0 %R0						\
	movi %R0 I##N						\
	sti_i i0 %R0						\
	LDSTL(N, R0)						\
	movi_f %F0 F##N						\
	sti_f f0 %F0						\
	movi_d %F0 D##N						\
	sti_d d0 %F0						\
	SI(X, N, c, C, R0)					\
	UI(X, N, c, C, R0)					\
	SI(X, N, s, S, R0)					\
	UI(X, N, s, S, R0)					\
	SI(X, N, i, I, R0)					\
	LDRL(X, N, R0)						\
	FF(X, N, f, F, F0)					\
	FF(X, N, d, D, F0)

#define LDST0(R0, F0)						\
	LDST1(0_##R0##_##F0, 0, R0, F0)				\
	LDST1(1_##R0##_##F0, 1, R0, F0)				\
	LDST1(2_##R0##_##F0, 2, R0, F0)				\
	LDST1(3_##R0##_##F0, 3, R0, F0)

#define LDST(V0, V1, V2, R0, R1, R2, F0, F1, F2, F3, F4, F5)	\
	LDST0(V0, F0)						\
	LDST0(V1, F1)						\
	LDST0(V2, F3)						\
	LDST0(R0, F4)						\
	LDST0(R1, F5)						\
	LDST0(R2, F0)

.code
	prolog

	/* Simple test to simplify validating encodings before
	 * brute force tests */
	movi %r1 0x81
	sti_c c0 %r1
	sti_c uc0 %r1
	movi %r1 0x8001
	sti_s s0 %r1
	sti_s us0 %r1
	movi %r1 0x80000001
	sti_i i0 %r1
#if __WORDSIZE == 64
	sti_i ui0 %r1
	movi %r1 0x8000000000000001
	sti_l l0 %r1
#endif
	movi_f %f0 0.5
	sti_f f0 %f0
	movi_d %f0 0.25
	sti_d d0 %f0
	ldi_c %r1 c0
	beqi Lc %r1 XC
	calli @abort
Lc:
	ldi_uc %r1 uc0
	beqi Luc %r1 0x81
	calli @abort
Luc:
	ldi_s %r1 s0
	beqi Ls %r1 XS
	calli @abort
Ls:
	ldi_us %r1 us0
	beqi Lus %r1 0x8001
	calli @abort
Lus:
	ldi_i %r1 i0
	beqi Li %r1 XI
	calli @abort
Li:
#if __WORDSIZE == 64
	ldi_ui %r1 ui0
	beqi Lui %r1 0x80000001
	calli @abort
Lui:
	ldi_l %r1 l0
	beqi Ll %r1 0x8000000000000001
	calli @abort
Ll:
#endif
	ldi_f %f0 f0
	beqi_f Lf %f0 0.5
	calli @abort
Lf:
	ldi_d %f0 d0
	beqi_d Ld %f0 0.25
	calli @abort
Ld:

	LDST(v0, v1, v2, r0, r1, r2, f0, f1, f2, f3, f4, f5)
	// just to know did not abort
	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
