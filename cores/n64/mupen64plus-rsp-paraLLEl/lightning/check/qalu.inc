.data	8
ok:
.c	"ok\n"

/* r0,r1 = r2 op r3 */
#define QALUR(N, T, OP, I0, I1, LO, HI, R0, R1, R2, R3)	\
	movi %R2 I0					\
	movi %R3 I1					\
	OP##r##T %R0 %R1 %R2 %R3			\
	bnei OP##T##N##rlo##R0##R1##R2##R3 %R0 LO	\
	bnei OP##T##N##rlo##R0##R1##R2##R3 %R1 HI	\
	bnei OP##T##N##rlo##R0##R1##R2##R3 %R2 I0	\
	beqi OP##T##N##rhi##R0##R1##R2##R3 %R3 I1	\
OP##T##N##rlo##R0##R1##R2##R3:				\
	calli @abort					\
OP##T##N##rhi##R0##R1##R2##R3:

/* r0,r1 = r2 op i0 */
#define QALUI(N, T, OP, I0, I1, LO, HI, R0, R1, R2, R3)	\
	movi %R2 I0					\
	movi %R3 HI					\
	OP##i##T %R0 %R1 %R2 I1				\
	bnei OP##T##N##ilo##R0##R1##R2##R3 %R0 LO	\
	bner OP##T##N##ilo##R0##R1##R2##R3 %R1 %R3	\
	beqi OP##T##N##ihi##R0##R1##R2##R3 %R2 I0	\
OP##T##N##ilo##R0##R1##R2##R3:				\
	calli @abort					\
OP##T##N##ihi##R0##R1##R2##R3:

/* r0,r1 = r0 op r1 */
#define QALUX(N, T, OP, I0, I1, LO, HI, R0, R1, R2, R3)	\
	movi %R0 I0					\
	movi %R1 I1					\
	movi %R2 LO					\
	movi %R3 HI					\
	OP##r##T %R0 %R1 %R0 %R1			\
	bner OP##T##N##0lo##R0##R1##R2##R3 %R0 %R2	\
	beqr OP##T##N##0hi##R0##R1##R2##R3 %R1 %R3	\
OP##T##N##0lo##R0##R1##R2##R3:				\
	calli @abort					\
OP##T##N##0hi##R0##R1##R2##R3:

/* r0,r1 = r1 op r0 */
#define QALUY(N, T, OP, I0, I1, LO, HI, R0, R1, R2, R3)	\
	movi %R1 I0					\
	movi %R0 I1					\
	movi %R2 LO					\
	movi %R3 HI					\
	OP##r##T %R0 %R1 %R1 %R0			\
	bner OP##T##N##1lo##R0##R1##R2##R3 %R0 %R2	\
	beqr OP##T##N##1hi##R0##R1##R2##R3 %R1 %R3	\
OP##T##N##1lo##R0##R1##R2##R3:				\
	calli @abort					\
OP##T##N##1hi##R0##R1##R2##R3:

/* r0,r1 = r0 op r3 */
#define QALUZ(N, T, OP, I0, I1, LO, HI, R0, R1, R2, R3)	\
	movi %R0 I0					\
	movi %R3 I1					\
	movi %R2 LO					\
	OP##r##T %R0 %R1 %R0 %R3			\
	bner OP##T##N##2lo##R0##R1##R2##R3 %R0 %R2	\
	bnei OP##T##N##2lo##R0##R1##R2##R3 %R1 HI	\
	beqi OP##T##N##2hi##R0##R1##R2##R3 %R3 I1	\
OP##T##N##2lo##R0##R1##R2##R3:				\
	calli @abort					\
OP##T##N##2hi##R0##R1##R2##R3:

/* r0,r1 = r2 op r1 */
#define QALUW(N, T, OP, I0, I1, LO, HI, R0, R1, R2, R3)	\
	movi %R2 I0					\
	movi %R1 I1					\
	movi %R3 LO					\
	OP##r##T %R0 %R1 %R2 %R1			\
	bner OP##T##N##3lo##R0##R1##R2##R3 %R0 %R3	\
	bnei OP##T##N##3lo##R0##R1##R2##R3 %R1 HI	\
	beqi OP##T##N##3hi##R0##R1##R2##R3 %R2 I0	\
OP##T##N##3lo##R0##R1##R2##R3:				\
	calli @abort					\
OP##T##N##3hi##R0##R1##R2##R3:

#define QALU2(N, T, OP, I0, I1, LO, HI, R0, R1, R2, R3)	\
	QALUR(N, T, OP, I0, I1, LO, HI, R0, R1, R2, R3)	\
	QALUI(N, T, OP, I0, I1, LO, HI, R0, R1, R2, R3)	\
	QALUX(N, T, OP, I0, I1, LO, HI, R0, R1, R2, R3)	\
	QALUY(N, T, OP, I0, I1, LO, HI, R0, R1, R2, R3)	\
	QALUZ(N, T, OP, I0, I1, LO, HI, R0, R1, R2, R3)	\
	QALUW(N, T, OP, I0, I1, LO, HI, R0, R1, R2, R3)

#define QALU1(N, T, OP, I0, I1, LO, HI, R0, R1, R2, R3)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R0, R1, R2, R3)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R0, R1, R3, R2)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R0, R2, R1, R3)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R0, R2, R3, R1)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R0, R3, R1, R2)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R0, R3, R2, R1)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R1, R0, R2, R3)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R1, R0, R3, R2)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R1, R2, R0, R3)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R1, R2, R3, R0)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R1, R3, R0, R2)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R1, R3, R2, R0)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R2, R1, R0, R3)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R2, R1, R3, R0)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R2, R0, R1, R3)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R2, R0, R3, R1)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R2, R3, R1, R0)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R2, R3, R0, R1)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R3, R1, R2, R0)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R3, R1, R0, R2)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R3, R2, R1, R0)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R3, R2, R0, R1)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R3, R0, R1, R2)	\
	QALU2(N, T, OP, I0, I1, LO, HI, R3, R0, R2, R1)

#define QALU(N, T, OP, I0, I1, LO, HI)			\
	QALU1(N, T, OP, I0, I1, LO, HI, v0, v1, v2, r0)	\
	QALU1(N, T, OP, I0, I1, LO, HI, v0, v1, v2, r1)	\
	QALU1(N, T, OP, I0, I1, LO, HI, v0, v1, v2, r2)	\
	QALU1(N, T, OP, I0, I1, LO, HI, v1, v2, r0, r1)	\
	QALU1(N, T, OP, I0, I1, LO, HI, v1, v2, r0, r2)	\
	QALU1(N, T, OP, I0, I1, LO, HI, v2, r0, r1, r2)
