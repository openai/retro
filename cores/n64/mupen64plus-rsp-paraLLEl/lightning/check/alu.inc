.data	8
ok:
.c	"ok\n"

/* ia64 code generation is not optimized for size, and also some
 * codes generate quite long sequences due to need for stops causing
 * no code template match and needing to add nops, and other cases
 * are division/remainder that needs function calls, or float division
 * that requires a quite long sequence.
 * (the brute force tests of all register combinations can easily
 *  generate several GB of jit).
 */

/* 3 operand */

/* reg0 = reg1 op reg2 */
#define ALUR(N, T, OP, I0, I1, V, R0, R1, R2)		\
	movi %R1 I0					\
	movi %R2 I1					\
	OP##r##T %R0 %R1 %R2				\
	beqi OP##T##N##r_##R0##R1##R2 %R0 V		\
	calli @abort					\
OP##T##N##r_##R0##R1##R2:

/* reg0 = reg1 op im */
#define ALUI(N, T, OP, I0, I1, V, R0, R1, R2)		\
	movi %R1 I0					\
	movi %R2 V					\
	OP##i##T %R0 %R1 I1				\
	beqr OP##T##N##i_##R0##R1##R2 %R0 %R2		\
	calli @abort					\
OP##T##N##i_##R0##R1##R2:

/* reg0 = reg0 op reg1 */
#define ALUR0(N, T, OP, I0, I1, V, R0, R1, R2)		\
	movi %R0 I0					\
	movi %R1 I1					\
	movi %R2 V					\
	OP##r##T %R0 %R0 %R1				\
	beqr OP##T##N##r_0##R0##R1##R2 %R0 %R2		\
	calli @abort					\
OP##T##N##r_0##R0##R1##R2:

/* reg0 = reg1 op reg0 */
#define ALUR1(N, T, OP, I0, I1, V, R0, R1, R2)		\
	movi %R0 I1					\
	movi %R1 I0					\
	movi %R2 V					\
	OP##r##T %R0 %R1 %R0				\
	beqr OP##T##N##r_1##R0##R1##R2 %R0 %R2		\
	calli @abort					\
OP##T##N##r_1##R0##R1##R2:

/* reg0 = reg0 op im */
#define ALUI0(N, T, OP, I0, I1, V, R0, R1, R2)		\
	movi %R0 I0					\
	movi %R1 V					\
	OP##i##T %R0 %R0 I1				\
	beqr OP##T##N##i_0##R0##R1##R2 %R0 %R1		\
	calli @abort					\
OP##T##N##i_0##R0##R1##R2:

#define ALU3(N, T, OP, I0, I1, V, R0, R1, R2)		\
	ALUR(N, T, OP, I0, I1, V, R0, R1, R2)		\
	ALUI(N, T, OP, I0, I1, V, R0, R1, R2)		\
	ALUR0(N, T, OP, I0, I1, V, R0, R1, R2)		\
	ALUR1(N, T, OP, I0, I1, V, R0, R1, R2)		\
	ALUI0(N, T, OP, I0, I1, V, R0, R1, R2)

#define ALU2(N, T, OP, I0, I1, V, R0, R1, R2)		\
	ALU3(N, T, OP, I0, I1, V, R0, R1, R2)		\
	ALU3(N, T, OP, I0, I1, V, R0, R2, R1)

#define ALU1(N, T, OP, I0, I1, V, R0, R1, R2)		\
	ALU2(N, T, OP, I0, I1, V, R0, R1, R2)		\
	ALU2(N, T, OP, I0, I1, V, R1, R0, R2)		\
	ALU2(N, T, OP, I0, I1, V, R2, R1, R0)

#if __ia64__
#  define ALU(N, T, OP, I0, I1, V)			\
	ALU3(N, T, OP, I0, I1, V, r0, r1, r2)		\
	ALU3(N, T, OP, I0, I1, V, v0, v1, v2)
#else
#  define ALU(N, T, OP, I0, I1, V)			\
	ALU1(N, T, OP, I0, I1, V, v0, v1, v2)		\
	ALU1(N, T, OP, I0, I1, V, v0, v1, r0)		\
	ALU1(N, T, OP, I0, I1, V, v0, v1, r1)		\
	ALU1(N, T, OP, I0, I1, V, v0, v1, r2)		\
	ALU1(N, T, OP, I0, I1, V, v1, v2, r1)		\
	ALU1(N, T, OP, I0, I1, V, v1, v2, r2)		\
	ALU1(N, T, OP, I0, I1, V, v2, r0, r1)		\
	ALU1(N, T, OP, I0, I1, V, v2, r0, r2)		\
	ALU1(N, T, OP, I0, I1, V, r0, r1, r2)
#endif

/* 3 carry set/propagate */

/*
 * r0 = i0
 * r1 = i1
 * r2 = 0
 * r0 = r0 opc r1	<only want carry>
 * r2 = r2 opx r2	<r2 must match v>
 */
#define ALUXII(N, OP, I0, I1, V, R0, R1, R2)		\
	movi %R0 I0					\
	movi %R2 0					\
	OP##ci %R0 %R0 I1				\
	OP##xi %R2 %R2 0				\
	beqi OP##N##ii##R0##R1##R2 %R2 V		\
	calli @abort					\
OP##N##ii##R0##R1##R2:

#define ALUXIR(N, OP, I0, I1, V, R0, R1, R2)		\
	movi %R0 I0					\
	movi %R2 0					\
	OP##ci %R0 %R0 I1				\
	OP##xr %R2 %R2 %R2				\
	beqi OP##N##ir##R0##R1##R2 %R2 V		\
	calli @abort					\
OP##N##ir##R0##R1##R2:

#define ALUXRI(N, OP, I0, I1, V, R0, R1, R2)		\
	movi %R0 I0					\
	movi %R1 I1					\
	movi %R2 0					\
	OP##cr %R0 %R0 %R1				\
	OP##xi %R2 %R2 0				\
	beqi OP##N##ri##R0##R1##R2 %R2 V		\
	calli @abort					\
OP##N##ri##R0##R1##R2:

#define ALUXRR(N, OP, I0, I1, V, R0, R1, R2)		\
	movi %R0 I0					\
	movi %R1 I1					\
	movi %R2 0					\
	OP##cr %R0 %R0 %R1				\
	OP##xr %R2 %R2 %R2				\
	beqi OP##N##rr##R0##R1##R2 %R2 V		\
	calli @abort					\
OP##N##rr##R0##R1##R2:

#define ALUX2(N, OP, I0, I1, V, R0, R1, R2)		\
	 ALUXII(N, OP, I0, I1, V, R0, R1, R2)		\
	 ALUXIR(N, OP, I0, I1, V, R0, R1, R2)		\
	 ALUXRI(N, OP, I0, I1, V, R0, R1, R2)		\
	 ALUXRR(N, OP, I0, I1, V, R0, R1, R2)

#define ALUX1(N, OP, I0, I1, V, R0, R1, R2)		\
	ALUX2(N, OP, I0, I1, V, R0, R1, R2)		\
	ALUX2(N, OP, I0, I1, V, R0, R2, R1)

#define ALUX0(N, OP, I0, I1, V, R0, R1, R2)		\
	ALUX1(N, OP, I0, I1, V, R0, R1, R2)		\
	ALUX1(N, OP, I0, I1, V, R1, R0, R2)		\
	ALUX1(N, OP, I0, I1, V, R2, R1, R0)

#if __ia64__
#  define ALUX(N, OP, I0, I1, V)			\
	ALUX2(N, OP, I0, I1, V, r0, r1, r2)		\
	ALUX2(N, OP, I0, I1, V, v0, v1, v2)
#else
#  define ALUX(N, OP, I0, I1, V)			\
	ALUX0(N, OP, I0, I1, V, v0, v1, v2)		\
	ALUX0(N, OP, I0, I1, V, v0, v1, r0)		\
	ALUX0(N, OP, I0, I1, V, v0, v1, r1)		\
	ALUX0(N, OP, I0, I1, V, v0, v1, r2)		\
	ALUX0(N, OP, I0, I1, V, v1, v2, r0)		\
	ALUX0(N, OP, I0, I1, V, v1, v2, r1)		\
	ALUX0(N, OP, I0, I1, V, v1, v2, r2)		\
	ALUX0(N, OP, I0, I1, V, v2, r0, r1)		\
	ALUX0(N, OP, I0, I1, V, v2, r0, r2)		\
	ALUX0(N, OP, I0, I1, V, r0, r1, r2)
#endif

/* unary int */

#define UNR(N, OP, I, V, R0, R1)			\
	movi %R1 I					\
	OP##r %R0 %R1					\
	beqi OP##N##R0##R1 %R0 V			\
	calli @abort					\
OP##N##R0##R1:

#define UNRC(N, OP, I, V, R0, R1)			\
	movi %R0 I					\
	OP##r %R0 %R0					\
	beqi OP##N##c##R0##R1 %R0 V			\
	calli @abort					\
OP##N##c##R0##R1:

#define UN2(N, OP, I, V, R0, R1)			\
	UNR(N, OP, I, V, R0, R1)			\
	UNRC(N, OP, I, V, R0, R1)

#define UN1(N, OP, I, V, R0, R1)			\
	UN2(N, OP, I, V, R0, R1)			\
	UN2(N, OP, I, V, R1, R0)

#if __ia64__
#  define UN(N, OP, I, V)				\
	UN2(N, OP, I, V, r0, r1)			\
	UN2(N, OP, I, V, v0, v1)
#else
#  define UN(N, OP, I, V)				\
	UN1(N, OP, I, V, v0, v1)			\
	UN1(N, OP, I, V, v0, v2)			\
	UN1(N, OP, I, V, v0, r0)			\
	UN1(N, OP, I, V, v0, r1)			\
	UN1(N, OP, I, V, v0, r2)			\
	UN1(N, OP, I, V, v1, v2)			\
	UN1(N, OP, I, V, v1, r0)			\
	UN1(N, OP, I, V, v1, r1)			\
	UN1(N, OP, I, V, v1, r2)			\
	UN1(N, OP, I, V, v2, r0)			\
	UN1(N, OP, I, V, v2, r1)			\
	UN1(N, OP, I, V, v2, r2)			\
	UN1(N, OP, I, V, r0, r1)			\
	UN1(N, OP, I, V, r0, r2)			\
	UN1(N, OP, I, V, r1, r2)
#endif

/* reg0 = reg1 op reg2 */
#define FOPR(N, T, OP, I0, I1, V, F0, F1, F2)		\
	movi##T %F1 I0					\
	movi##T %F2 I1					\
	OP##r##T %F0 %F1 %F2				\
	beqi##T OP##T##N##F0##F1##F2 %F0 V		\
	calli @abort					\
OP##T##N##F0##F1##F2:

/* reg0 = reg0 op reg1 */
#define FOPR0(N, T, OP, I0, I1, V, F0, F1, F2)		\
	movi##T %F0 I0					\
	movi##T %F1 I1					\
	OP##r##T %F0 %F0 %F1				\
	beqi##T OP##T##N##0##F0##F1##F2 %F0 V		\
	calli @abort					\
OP##T##N##0##F0##F1##F2:

/* reg1 = reg0 op reg1 */
#define FOPR1(N, T, OP, I0, I1, V, F0, F1, F2)		\
	movi##T %F0 I0					\
	movi##T %F1 I1					\
	OP##r##T %F1 %F0 %F1				\
	beqi##T OP##T##N##1##F0##F1##F2 %F1 V		\
	calli @abort					\
OP##T##N##1##F0##F1##F2:

/* reg0 = reg1 op im */
#define FOPI(N, T, OP, I0, I1, V, F0, F1, F2)		\
	movi##T %F1 I0					\
	movi##T %F2 V					\
	OP##i##T %F0 %F1 I1				\
	beqr##T OP##T##N##i##F0##F1##F2 %F0 %F2		\
	calli @abort					\
OP##T##N##i##F0##F1##F2:

/* reg0 = reg0 op im */
#define FOPI0(N, T, OP, I0, I1, V, F0, F1, F2)		\
	movi##T %F0 I0					\
	movi##T %F2 V					\
	OP##i##T %F0 %F0 I1				\
	beqr##T OP##T##N##i0##F0##F1##F2 %F0 %F2	\
	calli @abort					\
OP##T##N##i0##F0##F1##F2:

#define FOP1(N, T, OP, I0, I1, V, F0, F1, F2)		\
	FOPR(N, T, OP, I0, I1, V, F0, F1, F2)		\
	FOPR0(N, T, OP, I0, I1, V, F0, F1, F2)		\
	FOPR1(N, T, OP, I0, I1, V, F0, F1, F2)		\
	FOPI(N, T, OP, I0, I1, V, F0, F1, F2)		\
	FOPI0(N, T, OP, I0, I1, V, F0, F1, F2)

#if __ia64__
#  define  FOP(N, T, OP, I0, I1, V)			\
	FOP1(N, T, OP, I0, I1, V, f0, f1, f2)
#else
#  define  FOP(N, T, OP, I0, I1, V)			\
	FOP1(N, T, OP, I0, I1, V, f0, f1, f2)		\
	FOP1(N, T, OP, I0, I1, V, f0, f2, f3)		\
	FOP1(N, T, OP, I0, I1, V, f0, f3, f4)		\
	FOP1(N, T, OP, I0, I1, V, f0, f5, f1)
#endif

/* unary float */

#define FUNR(N, T, OP, I, V, R0, R1)			\
	movi##T %R1 I					\
	OP##r##T %R0 %R1				\
	beqi##T OP##N##T##R0##R1 %R0 V			\
	calli @abort					\
OP##N##T##R0##R1:

#define FUNRC(N, T, OP, I, V, R0, R1)			\
	movi##T %R0 I					\
	OP##r##T %R0 %R0				\
	beqi##T OP##N##T##c##R0##R1 %R0 V		\
	calli @abort					\
OP##N##T##c##R0##R1:

#define FUN2(N, T, OP, I, V, R0, R1)			\
	FUNR(N, T, OP, I, V, R0, R1)			\
	FUNRC(N, T, OP, I, V, R0, R1)

#define FUN1(N, T, OP, I, V, R0, R1)			\
	FUN2(N, T, OP, I, V, R0, R1)			\
	FUN2(N, T, OP, I, V, R1, R0)

#if __ia64__
#  define FUN(N, T, OP, I, V)				\
	FUN2(N, T, OP, I, V, f0, f1)
#else
#  define FUN(N, T, OP, I, V)				\
	FUN1(N, T, OP, I, V, f0, f1)			\
	FUN1(N, T, OP, I, V, f0, f2)			\
	FUN1(N, T, OP, I, V, f0, f3)			\
	FUN1(N, T, OP, I, V, f0, f4)			\
	FUN1(N, T, OP, I, V, f0, f5)
#endif

/* unordered comparison unary float */

#define UFUNR(N, T, OP, I, V, R0, R1)			\
	movi##T %R1 I					\
	OP##r##T %R0 %R1				\
	buneqi##T OP##N##T##u##R0##R1 %R0 V		\
	calli @abort					\
OP##N##T##u##R0##R1:

#define UFUNRC(N, T, OP, I, V, R0, R1)			\
	movi##T %R0 I					\
	OP##r##T %R0 %R0				\
	buneqi##T OP##N##T##uc##R0##R1 %R0 V		\
	calli @abort					\
OP##N##T##uc##R0##R1:

#define UFUN2(N, T, OP, I, V, R0, R1)			\
	UFUNR(N, T, OP, I, V, R0, R1)			\
	UFUNRC(N, T, OP, I, V, R0, R1)

#define UFUN1(N, T, OP, I, V, R0, R1)			\
	UFUN2(N, T, OP, I, V, R0, R1)			\
	UFUN2(N, T, OP, I, V, R1, R0)

#if __ia64__
#  define UFUN(N, T, OP, I, V)				\
	UFUN2(N, T, OP, I, V, f0, f1)
#else
#  define UFUN(N, T, OP, I, V)				\
	UFUN1(N, T, OP, I, V, f0, f1)			\
	UFUN1(N, T, OP, I, V, f0, f2)			\
	UFUN1(N, T, OP, I, V, f0, f3)			\
	UFUN1(N, T, OP, I, V, f0, f4)			\
	UFUN1(N, T, OP, I, V, f0, f5)
#endif

.	$( $NaN =  0.0/0.0)
.	$( $Inf =  1.0/0.0)
.	$($nInf = -1.0/0.0)
