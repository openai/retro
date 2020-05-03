#include "ldst.inc"

#if __WORDSIZE == 64
#  define LDSTL(N, R0, R1, R2)					\
	movi %R2 $offui						\
	stxr_i %R2 %R0 %R1					\
	movi %R1 L##N						\
	movi %R2 $offl						\
	stxr_l %R2 %R0 %R1

#  define SI(C, N, x, X, R0, R1, R2)				\
	movi %R2 $off##x					\
	ldxr_##x %R1 %R0 %R2					\
	beqi L##x##C %R1 L##X##N				\
	calli @abort						\
L##x##C:

#  define LDRL(C, N, R0, R1, R2)				\
	UI(C, N, i, I, R0, R1, R2)				\
	SI(C, N, l, L, R0, R1, R2)
#else
#  define LDSTL(C, R0, R1, R2)
#  define SI(C, N, x, X, R0, R1, R2)				\
	movi %R2 $off##x					\
	ldxr_##x %R1 %R0 %R2					\
	beqi L##x##C %R1 I##X##N				\
	calli @abort						\
L##x##C:

#  define LDRL(C, N, R0, R1, R2)

#endif

#define UI(C, N, x, X, R0, R1, R2)				\
	movi %R2 $offu##x					\
	ldxr_u##x %R1 %R0 %R2					\
	beqi Lu##x##C %R1 X##N					\
	calli @abort						\
Lu##x##C:

#define FF(C, N, x, X, R0, R1, F0)				\
	movi %R1 $off##x					\
	ldxr_##x %F0 %R0 %R1					\
	beqi_##x L##x##C %F0 X##N				\
L##x##C:

#define LDST1(X, N, R0, R1, R2, F0)				\
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
	movi_f %F0 F##N						\
	movi %R2 $offf						\
	stxr_f %R2 %R0 %F0					\
	movi_d %F0 D##N						\
	movi %R2 $offd						\
	stxr_d %R2 %R0 %F0					\
	SI(X, N, c, C, R0, R1, R2)				\
	UI(X, N, c, C, R0, R1, R2)				\
	SI(X, N, s, S, R0, R1, R2)				\
	UI(X, N, s, S, R0, R1, R2)				\
	SI(X, N, i, I, R0, R1, R2)				\
	LDRL(X, N, R0, R1, R2)					\
	FF(X, N, f, F, R0, R1, F0)				\
	FF(X, N, d, D, R0, R1, F0)

#define LDST0(R0, R1, R2, F0)					\
	LDST1(0_##R0##_##R1##_##R2##_##F0, 0, R0, R1, R2, F0)	\
	LDST1(1_##R0##_##R1##_##R2##_##F0, 1, R0, R1, R2, F0)	\
	LDST1(2_##R0##_##R1##_##R2##_##F0, 2, R0, R1, R2, F0)	\
	LDST1(3_##R0##_##R1##_##R2##_##F0, 3, R0, R1, R2, F0)

#define LDST(V0, V1, V2, R0, R1, R2, F0, F1, F2, F3, F4, F5)	\
	LDST0(V0, V1, R0, F0)					\
	LDST0(V0, V1, R1, F1)					\
	LDST0(V0, V1, R2, F2)					\
	LDST0(V0, V2, R0, F3)					\
	LDST0(V0, V2, R1, F4)					\
	LDST0(V0, V2, R2, F5)					\
	LDST0(V0, R0, V1, F0)					\
	LDST0(V0, R0, V2, F1)					\
	LDST0(V0, R0, R1, F2)					\
	LDST0(V0, R0, R2, F3)					\
	LDST0(V0, R0, V1, F4)					\
	LDST0(V0, R1, V1, F5)					\
	LDST0(V0, R1, V2, F0)					\
	LDST0(V0, R1, R0, F1)					\
	LDST0(V0, R1, R2, F2)					\
	LDST0(V0, V1, V2, F3)					\
	LDST0(V0, R1, R0, F4)					\
	LDST0(V0, R1, R2, F5)					\
	LDST0(R0, V1, V0, F0)					\
	LDST0(R0, V1, R1, F1)					\
	LDST0(R0, V1, R2, F2)					\
	LDST0(R0, V2, V0, F3)					\
	LDST0(R0, V2, R1, F4)					\
	LDST0(R0, V2, R2, F5)					\
	LDST0(R0, V0, V1, F0)					\
	LDST0(R0, V0, V2, F1)					\
	LDST0(R0, V0, R1, F2)					\
	LDST0(R0, V0, R2, F3)					\
	LDST0(R0, V0, V1, F4)					\
	LDST0(R0, R1, V1, F5)					\
	LDST0(R0, R1, V2, F0)					\
	LDST0(R0, R1, V0, F1)					\
	LDST0(R0, R1, R2, F2)					\
	LDST0(R0, V1, V2, F3)					\
	LDST0(R0, R1, V0, F4)					\
	LDST0(R0, R1, R2, F5)

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
	movi_f %f0 0.5
	movi %r2 $offf
	stxr_f %r2 %r0 %f0
	movi_d %f0 0.25
	movi %r2 $offd
	stxr_d %r2 %r0 %f0
	movi %r2 $offc
	ldxr_c %r1 %r0 %r2
	beqi Lc %r1 XC
	calli @abort
Lc:
	movi %r2 $offuc
	ldxr_uc %r1 %r0 %r2
	beqi Luc %r1 0x81
	calli @abort
Luc:
	movi %r2 $offs
	ldxr_s %r1 %r0 %r2
	beqi Ls %r1 XS
	calli @abort
Ls:
	movi %r2 $offus
	ldxr_us %r1 %r0 %r2
	beqi Lus %r1 0x8001
	calli @abort
Lus:
	movi %r2 $offi
	ldxr_i %r1 %r0 %r2
	beqi Li %r1 XI
	calli @abort
Li:
#if __WORDSIZE == 64
	movi %r2 $offui
	ldxr_ui %r1 %r0 %r2
	beqi Lui %r1 0x80000001
	calli @abort
Lui:
	movi %r2 $offl
	ldxr_l %r1 %r0 %r2
	beqi Ll %r1 0x8000000000000001
	calli @abort
Ll:
#endif
	movi %r2 $offf
	ldxr_f %f0 %r0 %r2
	beqi_f Lf %f0 0.5
	calli @abort
Lf:
	movi %r2 $offd
	ldxr_d %f0 %r0 %r2
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
