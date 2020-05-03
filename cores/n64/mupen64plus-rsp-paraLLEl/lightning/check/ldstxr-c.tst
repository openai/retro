#include "ldst.inc"

#if __WORDSIZE == 64
#  define LDSTL(N, R0, R1, R2)					\
	movi %R2 $offui						\
	stxr_i %R2 %R0 %R1					\
	movi %R1 L##N						\
	movi %R2 $offl						\
	stxr_l %R2 %R0 %R1

#  define SI(C, N, x, X, R0, R1)				\
	movi %R1 $off##x					\
	ldxr_##x %R1 %R0 %R1					\
	beqi L##x##C##0 %R1 L##X##N				\
	calli @abort						\
L##x##C##0:							\
	movi %R1 $off##x					\
	ldxr_##x %R0 %R0 %R1					\
	beqi L##x##C##1 %R0 L##X##N				\
	calli @abort						\
L##x##C##1:

#  define LDRL(C, N, R0, R1, R2)				\
	UI(C, N, i, I, R0, R1)					\
	movi %R0 t0						\
	SI(C, N, l, L, R0, R1)
#else
#  define LDSTL(C, R0, R1, R2)
#  define SI(C, N, x, X, R0, R1)				\
	movi %R1 $off##x					\
	ldxr_##x %R1 %R0 %R1					\
	beqi L##x##C##0 %R1 I##X##N				\
	calli @abort						\
L##x##C##0:							\
	movi %R1 $off##x					\
	ldxr_##x %R0 %R0 %R1					\
	beqi L##x##C##1 %R0 I##X##N				\
	calli @abort						\
L##x##C##1:

#  define LDRL(C, N, R0, R1, R2)

#endif

#define UI(C, N, x, X, R0, R1)					\
	movi %R1 $offu##x					\
	ldxr_u##x %R1 %R0 %R1					\
	beqi Lu##x##C##0 %R1 X##N				\
	calli @abort						\
Lu##x##C##0:							\
	movi %R1 $offu##x					\
	ldxr_u##x %R0 %R0 %R1					\
	beqi Lu##x##C##1 %R0 X##N				\
	calli @abort						\
Lu##x##C##1:

#define LDST1(X, N, R0, R1, R2)					\
	movi %R0 t0						\
	movi %R1 C##N						\
	movi %R2 $offc						\
	stxr_c %R2 %R0 %R1					\
	movi %R2 $offuc						\
	stxr_c %R2 %R0 %R1					\
	movi %R1 S##N						\
	movi %R2 $offs						\
	stxr_s %R2 %R0 %R1					\
	movi %R2 $offus						\
	stxr_s %R2 %R0 %R1					\
	movi %R1 I##N						\
	movi %R2 $offi						\
	stxr_i %R2 %R0 %R1					\
	LDSTL(N, R0, R1, R2)					\
	SI(X, N, c, C, R0, R1)					\
	movi %R0 t0						\
	UI(X, N, c, C, R0, R1)					\
	movi %R0 t0						\
	SI(X, N, s, S, R0, R1)					\
	movi %R0 t0						\
	UI(X, N, s, S, R0, R1)					\
	movi %R0 t0						\
	SI(X, N, i, I, R0, R1)					\
	movi %R0 t0						\
	LDRL(X, N, R0, R1, R2)					\

#define LDST0(R0, R1, R2)					\
	LDST1(0_##R0##_##R1##_##R2, 0, R0, R1, R2)		\
	LDST1(1_##R0##_##R1##_##R2, 1, R0, R1, R2)		\
	LDST1(2_##R0##_##R1##_##R2, 2, R0, R1, R2)		\
	LDST1(3_##R0##_##R1##_##R2, 3, R0, R1, R2)

#define LDST(V0, V1, V2, R0, R1, R2)				\
	LDST0(V1, V2, V0)					\
	LDST0(V1, R0, V0)					\
	LDST0(V1, R1, V0)					\
	LDST0(V1, R2, V0)					\
	LDST0(V0, R0, V1)					\
	LDST0(V0, R1, V1)					\
	LDST0(V0, R2, V1)					\
	LDST0(V0, V2, V1)					\
	LDST0(V2, V0, V1)					\
	LDST0(V2, R0, V1)					\
	LDST0(V2, R1, V1)					\
	LDST0(V2, R2, V1)					\
	LDST0(R0, R1, V2)					\
	LDST0(R0, R2, V2)

.code
	prolog

	/* Simple test to simplify validating encodings before
	 * brute force tests */
	movi %r0 t0
	movi %r1 0x81
	movi %r2 $offc
	stxr_c %r2 %r0 %r1
	movi %r2 $offuc
	stxr_c %r2 %r0 %r1
	movi %r1 0x8001
	movi %r2 $offs
	stxr_s %r2 %r0 %r1
	movi %r2 $offus
	stxr_s %r2 %r0 %r1
	movi %r1 0x80000001
	movi %r2 $offi
	stxr_i %r2 %r0 %r1
#if __WORDSIZE == 64
	movi %r2 $offui
	stxr_i %r2 %r0 %r1
	movi %r1 0x8000000000000001
	movi %r2 $offl
	stxr_l %r2 %r0 %r1
#endif
	movi %r1 $offc
	ldxr_c %r1 %r0 %r1
	beqi Lc0 %r1 XC
	calli @abort
Lc0:
	movi %r1 $offc
	ldxr_c %r0 %r0 %r1
	beqi Lc1 %r0 XC
	calli @abort
Lc1:
	movi %r0 t0
	movi %r1 $offuc
	ldxr_uc %r1 %r0 %r1
	beqi Luc0 %r1 0x81
	calli @abort
Luc0:
	movi %r1 $offuc
	ldxr_uc %r0 %r0 %r1
	beqi Luc1 %r0 0x81
	calli @abort
Luc1:
	movi %r0 t0
	movi %r1 $offs
	ldxr_s %r1 %r0 %r1
	beqi Ls0 %r1 XS
	calli @abort
Ls0:
	movi %r1 $offs
	ldxr_s %r0 %r0 %r1
	beqi Ls1 %r0 XS
	calli @abort
Ls1:
	movi %r0 t0
	movi %r1 $offus
	ldxr_us %r1 %r0 %r1
	beqi Lus0 %r1 0x8001
	calli @abort
Lus0:
	movi %r1 $offus
	ldxr_us %r0 %r0 %r1
	beqi Lus1 %r0 0x8001
	calli @abort
Lus1:
	movi %r0 t0
	movi %r1 $offi
	ldxr_i %r1 %r0 %r1
	beqi Li0 %r1 XI
	calli @abort
Li0:
	movi %r1 $offi
	ldxr_i %r0 %r0 %r1
	beqi Li1 %r0 XI
	calli @abort
Li1:
#if __WORDSIZE == 64
	movi %r0 t0
	movi %r1 $offui
	ldxr_ui %r1 %r0 %r1
	beqi Lui0 %r1 0x80000001
	calli @abort
Lui0:
	movi %r1 $offui
	ldxr_ui %r0 %r0 %r1
	beqi Lui1 %r0 0x80000001
	calli @abort
Lui1:
	movi %r0 t0
	movi %r1 $offl
	ldxr_l %r1 %r0 %r1
	beqi Ll0 %r1 0x8000000000000001
	calli @abort
Ll0:
	movi %r1 $offl
	ldxr_l %r0 %r0 %r1
	beqi Ll1 %r0 0x8000000000000001
	calli @abort
Ll1:
#endif

	LDST(v0, v1, v2, r0, r1, r2)
	// just to know did not abort
	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
