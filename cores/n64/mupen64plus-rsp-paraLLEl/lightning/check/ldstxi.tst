#include "ldst.inc"

#if __WORDSIZE == 64
#  define LDSTL(N, R0, R1)					\
	stxi_i $offui %R0 %R1					\
	movi %R1 L##N						\
	stxi_l $offl %R0 %R1

#  define SI(C, N, x, X, R0, R1)				\
	ldxi_##x %R1 %R0 $off##x				\
	beqi L##x##C %R1 L##X##N				\
	calli @abort						\
L##x##C:

#  define LDRL(C, N, R0, R1)					\
	UI(C, N, i, I, R0, R1)					\
	SI(C, N, l, L, R0, R1)
#else
#  define LDSTL(C, R0, R1)
#  define SI(C, N, x, X, R0, R1)				\
	ldxi_##x %R1 %R0 $off##x				\
	beqi L##x##C %R1 I##X##N				\
	calli @abort						\
L##x##C:

#  define LDRL(C, N, R0, R1)

#endif

#define UI(C, N, x, X, R0, R1)					\
	ldxi_u##x %R1 %R0 $offu##x				\
	beqi Lu##x##C %R1 X##N					\
	calli @abort						\
Lu##x##C:

#define FF(C, N, x, X, R0, F0)					\
	ldxi_##x %F0 %R0 $off##x				\
	beqi_##x L##x##C %F0 X##N				\
L##x##C:

#define LDST1(X, N, R0, R1, F0)					\
	movi %R0 t0						\
	movi %R1 C##N						\
	stxi_c $offc %R0 %R1					\
	stxi_c $offuc %R0 %R1					\
	movi %R1 S##N						\
	stxi_s $offs %R0 %R1					\
	stxi_s $offus %R0 %R1					\
	movi %R1 I##N						\
	stxi_i $offi %R0 %R1					\
	LDSTL(N, R0, R1)					\
	movi_f %F0 F##N						\
	stxi_f $offf %R0 %F0					\
	movi_d %F0 D##N						\
	stxi_d $offd %R0 %F0					\
	SI(X, N, c, C, R0, R1)					\
	UI(X, N, c, C, R0, R1)					\
	SI(X, N, s, S, R0, R1)					\
	UI(X, N, s, S, R0, R1)					\
	SI(X, N, i, I, R0, R1)					\
	LDRL(X, N, R0, R1)					\
	FF(X, N, f, F, R0, F0)					\
	FF(X, N, d, D, R0, F0)

#define LDST0(R0, R1, F0)					\
	LDST1(0_##R0##_##R1##_##F0, 0, R0, R1, F0)		\
	LDST1(1_##R0##_##R1##_##F0, 1, R0, R1, F0)		\
	LDST1(2_##R0##_##R1##_##F0, 2, R0, R1, F0)		\
	LDST1(3_##R0##_##R1##_##F0, 3, R0, R1, F0)

#define LDST(V0, V1, V2, R0, R1, R2, F0, F1, F2, F3, F4, F5)	\
	LDST0(V0, V1, F0)					\
	LDST0(V0, V2, F1)					\
	LDST0(V0, R0, F3)					\
	LDST0(V0, R1, F4)					\
	LDST0(V0, R2, F5)					\
	LDST0(V1, V2, F0)					\
	LDST0(V1, R0, F1)					\
	LDST0(V1, R1, F2)					\
	LDST0(V1, R2, F3)					\
	LDST0(V2, R0, F4)					\
	LDST0(V2, R1, F5)					\
	LDST0(V2, R2, F0)

.code
	prolog

	/* Simple test to simplify validating encodings before
	 * brute force tests */
	movi %r0 t0
	movi %r1 0x81
	stxi_c $offc %r0 %r1
	stxi_c $offuc %r0 %r1
	movi %r1 0x8001
	stxi_s $offs %r0 %r1
	stxi_s $offus %r0 %r1
	movi %r1 0x80000001
	stxi_i $offi %r0 %r1
#if __WORDSIZE == 64
	stxi_i $offui %r0 %r1
	movi %r1 0x8000000000000001
	stxi_l $offl %r0 %r1
#endif
	movi_f %f0 0.5
	stxi_f $offf %r0 %f0
	movi_d %f0 0.25
	stxi_d $offd %r0 %f0
	ldxi_c %r1 %r0 $offc
	beqi Lc %r1 XC
	calli @abort
Lc:
	ldxi_uc %r1 %r0 $offuc
	beqi Luc %r1 0x81
	calli @abort
Luc:
	ldxi_s %r1 %r0 $offs
	beqi Ls %r1 XS
	calli @abort
Ls:
	ldxi_us %r1 %r0 $offus
	beqi Lus %r1 0x8001
	calli @abort
Lus:
	ldxi_i %r1 %r0 $offi
	beqi Li %r1 XI
	calli @abort
Li:
#if __WORDSIZE == 64
	ldxi_ui %r1 %r0 $offui
	beqi Lui %r1 0x80000001
	calli @abort
Lui:
	ldxi_l %r1 %r0 $offl
	beqi Ll %r1 0x8000000000000001
	calli @abort
Ll:
#endif
	ldxi_f %f0 %r0 $offf
	beqi_f Lf %f0 0.5
	calli @abort
Lf:
	ldxi_d %f0 %r0 $offd
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
