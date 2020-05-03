#include "ldst.inc"

#if __WORDSIZE == 64
#  define LDSTL(N, R0, R1)					\
	stxi_i $offui %R0 %R1					\
	movi %R1 L##N						\
	stxi_l $offl %R0 %R1

#  define SI(C, N, x, X, R0)					\
	ldxi_##x %R0 %R0 $off##x				\
	beqi L##x##C %R0 L##X##N				\
	calli @abort						\
L##x##C:

#  define LDRL(C, N, R0, R1)					\
	UI(C, N, i, I, R0)					\
	movi %R0 t0						\
	SI(C, N, l, L, R0)
#else
#  define LDSTL(C, R0, R1)
#  define SI(C, N, x, X, R0)					\
	ldxi_##x %R0 %R0 $off##x				\
	beqi L##x##C %R0 I##X##N				\
	calli @abort						\
L##x##C:

#  define LDRL(C, N, R0, R1)

#endif

#define UI(C, N, x, X, R0)					\
	ldxi_u##x %R0 %R0 $offu##x				\
	beqi Lu##x##C %R0 X##N					\
	calli @abort						\
Lu##x##C:

#define LDST1(X, N, R0, R1)					\
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
	SI(X, N, c, C, R0)					\
	movi %R0 t0						\
	UI(X, N, c, C, R0)					\
	movi %R0 t0						\
	SI(X, N, s, S, R0)					\
	movi %R0 t0						\
	UI(X, N, s, S, R0)					\
	movi %R0 t0						\
	SI(X, N, i, I, R0)					\
	movi %R0 t0						\
	LDRL(X, N, R0, R1)					\

#define LDST0(R0, R1)						\
	LDST1(0_##R0##_##R1, 0, R0, R1)				\
	LDST1(1_##R0##_##R1, 1, R0, R1)				\
	LDST1(2_##R0##_##R1, 2, R0, R1)				\
	LDST1(3_##R0##_##R1, 3, R0, R1)

#define LDST(V0, V1, V2, R0, R1, R2)				\
	LDST0(V0, V1)						\
	LDST0(V0, V2)						\
	LDST0(V0, R0)						\
	LDST0(V0, R1)						\
	LDST0(V0, R2)						\
	LDST0(V1, V2)						\
	LDST0(V1, R0)						\
	LDST0(V1, R1)						\
	LDST0(V1, R2)						\
	LDST0(V2, R0)						\
	LDST0(V2, R1)						\
	LDST0(V2, R2)						\
	LDST0(R0, V0)						\
	LDST0(R0, V1)						\
	LDST0(R0, V2)						\
	LDST0(R0, R1)						\
	LDST0(R0, R2)						\
	LDST0(R1, V0)						\
	LDST0(R1, V1)						\
	LDST0(R1, V2)						\
	LDST0(R1, R0)						\
	LDST0(R1, R2)						\
	LDST0(R2, V0)						\
	LDST0(R2, V1)						\
	LDST0(R2, V2)						\
	LDST0(R2, R0)						\
	LDST0(R2, R1)

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
	ldxi_c %r0 %r0 $offc
	beqi Lc %r0 XC
	calli @abort
Lc:
	movi %r0 t0
	ldxi_uc %r0 %r0 $offuc
	beqi Luc %r0 0x81
	calli @abort
Luc:
	movi %r0 t0
	ldxi_s %r0 %r0 $offs
	beqi Ls %r0 XS
	calli @abort
Ls:
	movi %r0 t0
	ldxi_us %r0 %r0 $offus
	beqi Lus %r0 0x8001
	calli @abort
Lus:
	movi %r0 t0
	ldxi_i %r0 %r0 $offi
	beqi Li %r0 XI
	calli @abort
Li:
#if __WORDSIZE == 64
	movi %r0 t0
	ldxi_ui %r0 %r0 $offui
	beqi Lui %r0 0x80000001
	calli @abort
Lui:
	movi %r0 t0
	ldxi_l %r0 %r0 $offl
	beqi Ll %r0 0x8000000000000001
	calli @abort
Ll:
#endif

	LDST(v0, v1, v2, r0, r1, r2)
	// just to know did not abort
	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
