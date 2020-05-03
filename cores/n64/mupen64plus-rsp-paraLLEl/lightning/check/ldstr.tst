#include "ldst.inc"

#if __WORDSIZE == 64
#  define LDSTL(N, R0, R1)					\
	movi %R0 $(t0 + $offui)					\
	str_i %R0 %R1						\
	movi %R0 $(t0 + $offl)					\
	movi %R1 L##N						\
	str_l %R0 %R1

#  define SI(C, N, x, X, R0, R1)				\
	movi %R0 $(t0 + $off##x)				\
	ldr_##x %R1 %R0						\
	beqi L##x##C %R1 L##X##N				\
	calli @abort						\
L##x##C:

#  define LDRL(C, N, R0, R1)					\
	UI(C, N, i, I, R0, R1)					\
	SI(C, N, l, L, R0, R1)
#else
#  define LDSTL(C, R0, R1)
#  define SI(C, N, x, X, R0, R1)				\
	movi %R0 $(t0 + $off##x)				\
	ldr_##x %R1 %R0						\
	beqi L##x##C %R1 I##X##N				\
	calli @abort						\
L##x##C:

#  define LDRL(C, N, R0, R1)

#endif

#define UI(C, N, x, X, R0, R1)					\
	movi %R0 $(t0 + $offu##x)				\
	ldr_u##x %R1 %R0					\
	beqi Lu##x##C %R1 X##N					\
	calli @abort						\
Lu##x##C:

#define FF(C, N, x, X, R0, F0)					\
	movi %R0 $(t0 + $off##x)				\
	ldr_##x %F0 %R0						\
	beqi_##x L##x##C %F0 X##N				\
L##x##C:

#define LDST1(X, N, R0, R1, F0)					\
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
	movi_f %F0 F##N						\
	str_f %R0 %F0						\
	movi %R0 $(t0 + $offd)					\
	movi_d %F0 D##N						\
	str_d %R0 %F0						\
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
	movi %r0 $(t0 + $offf)
	movi_f %f0 0.5
	str_f %r0 %f0
	movi %r0 $(t0 + $offd)
	movi_d %f0 0.25
	str_d %r0 %f0
	movi %r0 $(t0 + $offc)
	ldr_c %r1 %r0
	beqi Lc %r1 XC
	calli @abort
Lc:
	movi %r0 $(t0 + $offuc)
	ldr_uc %r1 %r0
	beqi Luc %r1 0x81
	calli @abort
Luc:
	movi %r0 $(t0 + $offs)
	ldr_s %r1 %r0
	beqi Ls %r1 XS
	calli @abort
Ls:
	movi %r0 $(t0 + $offus)
	ldr_us %r1 %r0
	beqi Lus %r1 0x8001
	calli @abort
Lus:
	movi %r0 $(t0 + $offi)
	ldr_i %r1 %r0
	beqi Li %r1 XI
	calli @abort
Li:
#if __WORDSIZE == 64
	movi %r0 $(t0 + $offui)
	ldr_ui %r1 %r0
	beqi Lui %r1 0x80000001
	calli @abort
Lui:
	movi %r0 $(t0 + $offl)
	ldr_l %r1 %r0
	beqi Ll %r1 0x8000000000000001
	calli @abort
Ll:
#endif
	movi %r0 $(t0 + $offf)
	ldr_f %f0 %r0
	beqi_f Lf %f0 0.5
	calli @abort
Lf:
	movi %r0 $(t0 + $offd)
	ldr_d %f0 %r0
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
