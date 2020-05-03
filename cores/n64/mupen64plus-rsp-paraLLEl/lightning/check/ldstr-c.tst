#include "ldst.inc"

#if __WORDSIZE == 64
#  define LDSTL(N, R0, R1)					\
	movi %R0 $(t0 + $offui)					\
	str_i %R0 %R1						\
	movi %R0 $(t0 + $offl)					\
	movi %R1 L##N						\
	str_l %R0 %R1

#  define SI(C, N, x, X, R0)					\
	movi %R0 $(t0 + $off##x)				\
	ldr_##x %R0 %R0						\
	beqi L##x##C %R0 L##X##N				\
	calli @abort						\
L##x##C:

#  define LDRL(C, N, R0, R1)					\
	UI(C, N, i, I, R0)					\
	SI(C, N, l, L, R0)
#else
#  define LDSTL(C, R0, R1)
#  define SI(C, N, x, X, R0)					\
	movi %R0 $(t0 + $off##x)				\
	ldr_##x %R0 %R0						\
	beqi L##x##C %R0 I##X##N				\
	calli @abort						\
L##x##C:

#  define LDRL(C, N, R0, R1)

#endif

#define UI(C, N, x, X, R0)					\
	movi %R0 $(t0 + $offu##x)				\
	ldr_u##x %R0 %R0					\
	beqi Lu##x##C %R0 X##N					\
	calli @abort						\
Lu##x##C:

#define LDST1(X, N, R0, R1)					\
	movi %R0 $(t0 + $offc)					\
	movi %R1 C##N						\
	str_c %R0 %R1						\
	movi %R0 $(t0 + $offuc)					\
	str_c %R0 %R1						\
	movi %R0 $(t0 + $offs)					\
	movi %R1 S##N						\
	str_s %R0 %R1						\
	movi %R0 $(t0 + $offus)					\
	str_s %R0 %R1						\
	movi %R0 $(t0 + $offi)					\
	movi %R1 I##N						\
	str_i %R0 %R1						\
	LDSTL(N, R0, R1)					\
	movi %R0 $(t0 + $offf)					\
	SI(X, N, c, C, R0)					\
	UI(X, N, c, C, R0)					\
	SI(X, N, s, S, R0)					\
	UI(X, N, s, S, R0)					\
	SI(X, N, i, I, R0)					\
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
	LDST0(V1, V0)						\
	LDST0(V1, V2)						\
	LDST0(V1, R0)						\
	LDST0(V1, R1)						\
	LDST0(V1, R2)						\
	LDST0(V2, R0)						\
	LDST0(V2, R1)						\
	LDST0(V2, R2)

.code
	prolog

	/* Simple test to simplify validating encodings before
	 * brute force tests */
	movi %r0 $(t0 + $offc)
	movi %r1 0x81
	str_c %r0 %r1
	movi %r0 $(t0 + $offuc)
	str_c %r0 %r1
	movi %r0 $(t0 + $offs)
	movi %r1 0x8001
	str_s %r0 %r1
	movi %r0 $(t0 + $offus)
	str_s %r0 %r1
	movi %r0 $(t0 + $offi)
	movi %r1 0x80000001
	str_i %r0 %r1
#if __WORDSIZE == 64
	movi %r0 $(t0 + $offui)
	str_i %r0 %r1
	movi %r0 $(t0 + $offl)
	movi %r1 0x8000000000000001
	str_l %r0 %r1
#endif
	movi %r0 $(t0 + $offc)
	ldr_c %r0 %r0
	beqi Lc %r0 XC
	calli @abort
Lc:
	movi %r0 $(t0 + $offuc)
	ldr_uc %r0 %r0
	beqi Luc %r0 0x81
	calli @abort
Luc:
	movi %r0 $(t0 + $offs)
	ldr_s %r0 %r0
	beqi Ls %r0 XS
	calli @abort
Ls:
	movi %r0 $(t0 + $offus)
	ldr_us %r0 %r0
	beqi Lus %r0 0x8001
	calli @abort
Lus:
	movi %r0 $(t0 + $offi)
	ldr_i %r0 %r0
	beqi Li %r0 XI
	calli @abort
Li:
#if __WORDSIZE == 64
	movi %r0 $(t0 + $offui)
	ldr_ui %r0 %r0
	beqi Lui %r0 0x80000001
	calli @abort
Lui:
	movi %r0 $(t0 + $offl)
	ldr_l %r0 %r0
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
