#if __WORDSIZE == 64
#  define I7f		0x7fffffffffffffff
#  define I80		0x8000000000000000
#  define I81		0x8000000000000001
#  define Iff		0xffffffffffffffff
#else
#  define I7f		0x7fffffff
#  define I80		0x80000000
#  define I81		0x80000001
#  define Iff		0xffffffff
#endif

.data	12
ok:
.c	"ok\n"
.	$($NaN  =  0.0 / 0.0)

#define BOP(N, Ls, Rs, Lu, Ru, R0, R1)		\
	movi %R0 Ls				\
	movi %R1 Rs				\
	b##N##r N##r_##R0##_##R1 %R0 %R1	\
	calli @abort				\
N##r_##R0##_##R1:				\
	b##N##i N##i_##R0##_##R1 %R0 Rs		\
	calli @abort				\
N##i_##R0##_##R1:				\
	movi %R0 Lu				\
	movi %R1 Ru				\
	b##N##r_u N##r_u_##R0##_##R1 %R0 %R1	\
	calli @abort				\
N##r_u_##R0##_##R1:				\
	b##N##i_u N##i_u_##R0##_##R1 %R0 Ru	\
	calli @abort				\
N##i_u_##R0##_##R1:				\
	movi %R0 Ls				\
	movi %R1 Rs				\
	N##r %R0 %R0 %R1			\
	beqi _##N##r_##R0##_##R1 %R0 1		\
	calli @abort				\
_##N##r_##R0##_##R1:				\
	movi %R0 Ls				\
	N##i %R1 %R0 Rs				\
	beqi _##N##i_##R0##_##R1 %R1 1		\
	calli @abort				\
_##N##i_##R0##_##R1:				\
	movi %R0 Lu				\
	movi %R1 Ru				\
	N##r_u %R0 %R0 %R1			\
	beqi _##N##r_u_##R0##_##R1 %R0 1	\
	calli @abort				\
_##N##r_u_##R0##_##R1:				\
	movi %R0 Lu				\
	N##i_u %R1 %R0 Ru			\
	beqi _##N##i_u_##R0##_##R1 %R1 1	\
	calli @abort				\
_##N##i_u_##R0##_##R1:

#define EB(N, L, R, R0, R1)			\
	movi %R0 L				\
	movi %R1 R				\
	b##N##r N##r_##R0##_##R1 %R0 %R1	\
	calli @abort				\
N##r_##R0##_##R1:				\
	b##N##i N##i_##R0##_##R1 %R0 R		\
	calli @abort				\
N##i_##R0##_##R1:				\
	movi %R0 L				\
	movi %R1 R				\
	N##r %R0 %R0 %R1			\
	beqi _##N##r_##R0##_##R1 %R0 1		\
	calli @abort				\
_##N##r_##R0##_##R1:				\
	movi %R0 L				\
	N##i %R1 %R0 R				\
	beqi _##N##i_##R0##_##R1 %R1 1		\
	calli @abort				\
_##N##i_##R0##_##R1:

#define XEB(N, L, R, R0, R1)			\
	movi %R0 L				\
	movi %R1 R				\
	b##N##r N##r_##R0##_##R1 %R0 %R1	\
	calli @abort				\
N##r_##R0##_##R1:				\
	b##N##i N##i_##R0##_##R1 %R0 R		\
	calli @abort				\
N##i_##R0##_##R1:

#define XBOP(N, Ls, Rs, Lu, Ru, R0, R1)		\
	movi %R0 Ls				\
	movi %R1 Rs				\
	b##N##r N##r_##R0##_##R1 %R0 %R1	\
	calli @abort				\
N##r_##R0##_##R1:				\
	movi %R0 Ls				\
	b##N##i N##i_##R0##_##R1 %R0 Rs		\
	calli @abort				\
N##i_##R0##_##R1:				\
	movi %R0 Lu				\
	movi %R1 Ru				\
	b##N##r_u N##r_u_##R0##_##R1 %R0 %R1	\
	calli @abort				\
N##r_u_##R0##_##R1:				\
	movi %R0 Lu				\
	b##N##i_u N##i_u_##R0##_##R1 %R0 Ru	\
	calli @abort				\
N##i_u_##R0##_##R1:

#define BOPI(N, Ls, Rs, Lu, Ru)			\
	BOP(N, Ls, Rs, Lu, Ru, v0, v1)		\
	BOP(N, Ls, Rs, Lu, Ru, v0, v2)		\
	BOP(N, Ls, Rs, Lu, Ru, v0, r0)		\
	BOP(N, Ls, Rs, Lu, Ru, v0, r1)		\
	BOP(N, Ls, Rs, Lu, Ru, v0, r2)		\
	BOP(N, Ls, Rs, Lu, Ru, v1, v0)		\
	BOP(N, Ls, Rs, Lu, Ru, v1, v2)		\
	BOP(N, Ls, Rs, Lu, Ru, v1, r0)		\
	BOP(N, Ls, Rs, Lu, Ru, v1, r1)		\
	BOP(N, Ls, Rs, Lu, Ru, v1, r2)		\
	BOP(N, Ls, Rs, Lu, Ru, v2, v0)		\
	BOP(N, Ls, Rs, Lu, Ru, v2, v1)		\
	BOP(N, Ls, Rs, Lu, Ru, v2, r0)		\
	BOP(N, Ls, Rs, Lu, Ru, v2, r1)		\
	BOP(N, Ls, Rs, Lu, Ru, v2, r2)		\
	BOP(N, Ls, Rs, Lu, Ru, r0, v0)		\
	BOP(N, Ls, Rs, Lu, Ru, r0, v1)		\
	BOP(N, Ls, Rs, Lu, Ru, r0, v2)		\
	BOP(N, Ls, Rs, Lu, Ru, r0, r1)		\
	BOP(N, Ls, Rs, Lu, Ru, r0, r2)		\
	BOP(N, Ls, Rs, Lu, Ru, r1, v0)		\
	BOP(N, Ls, Rs, Lu, Ru, r1, v1)		\
	BOP(N, Ls, Rs, Lu, Ru, r1, v2)		\
	BOP(N, Ls, Rs, Lu, Ru, r1, r0)		\
	BOP(N, Ls, Rs, Lu, Ru, r1, r2)		\
	BOP(N, Ls, Rs, Lu, Ru, r2, v0)		\
	BOP(N, Ls, Rs, Lu, Ru, r2, v1)		\
	BOP(N, Ls, Rs, Lu, Ru, r2, v2)		\
	BOP(N, Ls, Rs, Lu, Ru, r2, r0)		\
	BOP(N, Ls, Rs, Lu, Ru, r2, r1)

#define EBI(N, L, R)				\
	EB(N, L, R, v0, v1)			\
	EB(N, L, R, v0, v2)			\
	EB(N, L, R, v0, r0)			\
	EB(N, L, R, v0, r1)			\
	EB(N, L, R, v0, r2)			\
	EB(N, L, R, v1, v0)			\
	EB(N, L, R, v1, v2)			\
	EB(N, L, R, v1, r0)			\
	EB(N, L, R, v1, r1)			\
	EB(N, L, R, v1, r2)			\
	EB(N, L, R, v2, v0)			\
	EB(N, L, R, v2, v1)			\
	EB(N, L, R, v2, r0)			\
	EB(N, L, R, v2, r1)			\
	EB(N, L, R, v2, r2)			\
	EB(N, L, R, r0, v0)			\
	EB(N, L, R, r0, v1)			\
	EB(N, L, R, r0, v2)			\
	EB(N, L, R, r0, r1)			\
	EB(N, L, R, r0, r2)			\
	EB(N, L, R, r1, v0)			\
	EB(N, L, R, r1, v1)			\
	EB(N, L, R, r1, v2)			\
	EB(N, L, R, r1, r0)			\
	EB(N, L, R, r1, r2)			\
	EB(N, L, R, r2, v0)			\
	EB(N, L, R, r2, v1)			\
	EB(N, L, R, r2, v2)			\
	EB(N, L, R, r2, r0)			\
	EB(N, L, R, r2, r1)


#define XEBI(N, L, R)				\
	XEB(N, L, R, v0, v1)			\
	XEB(N, L, R, v0, v2)			\
	XEB(N, L, R, v0, r0)			\
	XEB(N, L, R, v0, r1)			\
	XEB(N, L, R, v0, r2)			\
	XEB(N, L, R, v1, v0)			\
	XEB(N, L, R, v1, v2)			\
	XEB(N, L, R, v1, r0)			\
	XEB(N, L, R, v1, r1)			\
	XEB(N, L, R, v1, r2)			\
	XEB(N, L, R, v2, v0)			\
	XEB(N, L, R, v2, v1)			\
	XEB(N, L, R, v2, r0)			\
	XEB(N, L, R, v2, r1)			\
	XEB(N, L, R, v2, r2)			\
	XEB(N, L, R, r0, v0)			\
	XEB(N, L, R, r0, v1)			\
	XEB(N, L, R, r0, v2)			\
	XEB(N, L, R, r0, r1)			\
	XEB(N, L, R, r0, r2)			\
	XEB(N, L, R, r1, v0)			\
	XEB(N, L, R, r1, v1)			\
	XEB(N, L, R, r1, v2)			\
	XEB(N, L, R, r1, r0)			\
	XEB(N, L, R, r1, r2)			\
	XEB(N, L, R, r2, v0)			\
	XEB(N, L, R, r2, v1)			\
	XEB(N, L, R, r2, v2)			\
	XEB(N, L, R, r2, r0)			\
	XEB(N, L, R, r2, r1)

#define XBOPI(N, Ls, Rs, Lu, Ru)		\
	XBOP(N, Ls, Rs, Lu, Ru, v0, v1)		\
	XBOP(N, Ls, Rs, Lu, Ru, v0, v2)		\
	XBOP(N, Ls, Rs, Lu, Ru, v0, r0)		\
	XBOP(N, Ls, Rs, Lu, Ru, v0, r1)		\
	XBOP(N, Ls, Rs, Lu, Ru, v0, r2)		\
	XBOP(N, Ls, Rs, Lu, Ru, v1, v0)		\
	XBOP(N, Ls, Rs, Lu, Ru, v1, v2)		\
	XBOP(N, Ls, Rs, Lu, Ru, v1, r0)		\
	XBOP(N, Ls, Rs, Lu, Ru, v1, r1)		\
	XBOP(N, Ls, Rs, Lu, Ru, v1, r2)		\
	XBOP(N, Ls, Rs, Lu, Ru, v2, v0)		\
	XBOP(N, Ls, Rs, Lu, Ru, v2, v1)		\
	XBOP(N, Ls, Rs, Lu, Ru, v2, r0)		\
	XBOP(N, Ls, Rs, Lu, Ru, v2, r1)		\
	XBOP(N, Ls, Rs, Lu, Ru, v2, r2)		\
	XBOP(N, Ls, Rs, Lu, Ru, r0, v0)		\
	XBOP(N, Ls, Rs, Lu, Ru, r0, v1)		\
	XBOP(N, Ls, Rs, Lu, Ru, r0, v2)		\
	XBOP(N, Ls, Rs, Lu, Ru, r0, r1)		\
	XBOP(N, Ls, Rs, Lu, Ru, r0, r2)		\
	XBOP(N, Ls, Rs, Lu, Ru, r1, v0)		\
	XBOP(N, Ls, Rs, Lu, Ru, r1, v1)		\
	XBOP(N, Ls, Rs, Lu, Ru, r1, v2)		\
	XBOP(N, Ls, Rs, Lu, Ru, r1, r0)		\
	XBOP(N, Ls, Rs, Lu, Ru, r1, r2)		\
	XBOP(N, Ls, Rs, Lu, Ru, r2, v0)		\
	XBOP(N, Ls, Rs, Lu, Ru, r2, v1)		\
	XBOP(N, Ls, Rs, Lu, Ru, r2, v2)		\
	XBOP(N, Ls, Rs, Lu, Ru, r2, r0)		\
	XBOP(N, Ls, Rs, Lu, Ru, r2, r1)

#define TBOPF(N, T, L, R)			\
	movi_##T %f0 L				\
	movi_##T %f1 R				\
	b##N##r##_##T N##r_##T %f0 %f1		\
	calli @abort				\
N##r_##T:					\
	b##N##i##_##T N##i_##T %f0 R		\
	calli @abort				\
N##i_##T:					\
	movi_##T %f1 $NaN			\
	b##N##r##_##T N##r_##T##_##u %f0 %f1	\
	jmpi N##r_##T##_##u0			\
N##r_##T##_##u:					\
	calli @abort				\
N##r##_##T##_##u0:				\
	b##N##i##_##T N##i_##T##_##u %f0 $NaN	\
	jmpi N##i_##T##_##u0			\
N##i##_##T##_##u:				\
	calli @abort				\
N##i##_##T##_##u0:
#define BOPF(N, L, R)				\
	TBOPF(N, f, L, R)			\
	TBOPF(N, d, L, R)

#define TUBOPF(N, T, L, R)			\
	movi_##T %f0 L				\
	movi_##T %f1 R				\
	b##N##r##_##T N##r_##T %f0 %f1		\
	calli @abort				\
N##r_##T:					\
	b##N##i##_##T N##i_##T %f0 R		\
	calli @abort				\
N##i_##T:					\
	movi_##T %f1 $NaN			\
	b##N##r##_##T N##r_##T##_##u %f0 %f1	\
	calli @abort				\
N##r_##T##_##u:					\
	b##N##i##_##T N##i_##T##_##u %f0 $NaN	\
	calli @abort				\
N##i##_##T##_##u:

#define UBOPF(N, L, R)				\
	TUBOPF(N, f, L, R)			\
	TUBOPF(N, d, L, R)

.code
	prolog

	movi %r0 -1
	movi %r1 1
	bltr xltr_r0_r1 %r0 %r1
	calli @abort
xltr_r0_r1:
	blti xlti_r0_r1 %r0 1
	calli @abort
xlti_r0_r1:
	movi %r0 1
	movi %r1 -1
	bltr_u xltru_r0_r1 %r0 %r1
	calli @abort
xltru_r0_r1:
	blti_u xltiu_r0_r1 %r0 -1
	calli @abort
xltiu_r0_r1:
	movi %r0 -1
	movi %r1 -1
	bler xler_r0_r1 %r0 %r1
	calli @abort
xler_r0_r1:
	blti xlei_r0_r1 %r0 1
	calli @abort
xlei_r0_r1:
	movi %r0 1
	movi %r1 -1
	bltr_u xlteu_r0_r1 %r0 %r1
	calli @abort
xlteu_r0_r1:
	blei_u xleiu_r0_r1 %r0 -1
	calli @abort
xleiu_r0_r1:
	movi %r0 32
	movi %r1 32
	beqr xeqr_r0_r1 %r0 %r1
	calli @abort
xeqr_r0_r1:
	beqi xeqi_r0_r1 %r0 32
	calli @abort
xeqi_r0_r1:
	movi %r0 -2
	movi %r1 -2
	bger xger_r0_r1 %r0 %r1
	calli @abort
xger_r0_r1:
	bgei xgei_r0_r1 %r0 -2
	calli @abort
xgei_r0_r1:
	movi %r0 2
	movi %r1 2
	bger_u xgeru_r0_r1 %r0 %r1
	calli @abort
xgeru_r0_r1:
	bgei_u xgeiu_r0_r1 %r0 2
	calli @abort
xgeiu_r0_r1:
	movi %r0 2
	movi %r1 -2
	bgtr xgtr_r0_r1 %r0 %r1
	calli @abort
xgtr_r0_r1:
	bgti xgti_r0_r1 %r0 -2
	calli @abort
xgti_r0_r1:
	movi %r0 -2
	movi %r1 2
	bgtr_u xgtru_r0_r1 %r0 %r1
	calli @abort
xgtru_r0_r1:
	bgti_u xgtiu_r0_r1 %r0 2
	calli @abort
xgtiu_r0_r1:
	movi %r0 -3
	movi %r1 3
	bner xner_r0_r1 %r0 %r1
	calli @abort
xner_r0_r1:
	bnei xnei_r0_r1 %r0 3
	calli @abort
xnei_r0_r1:
	movi %r0 1
	movi %r1 3
	bmsr xmsr_r0_r1 %r0 %r1
	calli @abort
xmsr_r0_r1:
	bmsi xmsi_r0_r1 %r0 3
	calli @abort
xmsi_r0_r1:
	movi %r0 1
	movi %r1 2
	bmcr xmcr_r0_r1 %r0 %r1
	calli @abort
xmcr_r0_r1:
	bmci xmci_r0_r1 %r0 2
	calli @abort
xmci_r0_r1:
	movi %r0 I7f
	movi %r1 1
	boaddr xoaddr_r0_r1 %r0 %r1
	calli @abort
xoaddr_r0_r1:
	movi %r0 Iff
	movi %r1 1
	boaddr_u xoaddr_u_r0_r1 %r0 %r1
	calli @abort
xoaddr_u_r0_r1:
	movi %r0 I7f
	boaddi xoaddi_r0_r1 %r0 1
	calli @abort
xoaddi_r0_r1:
	movi %r0 Iff
	boaddi_u xoaddi_u_r0_r1 %r0 1
	calli @abort
xoaddi_u_r0_r1:
	movi %r0 I80
	movi %r1 1
	bxaddr xxaddr_r0_r1 %r0 %r1
	calli @abort
xxaddr_r0_r1:
	movi %r0 I80
	bxaddi xxaddi_r0_r1 %r0 1
	calli @abort
xxaddi_r0_r1:
	movi %r0 I7f
	movi %r1 1
	bxaddr_u xxaddr_u_r0_r1 %r0 %r1
	calli @abort
xxaddr_u_r0_r1:
	movi %r0 I7f
	bxaddi_u xxaddi_u_r0_r1 %r0 1
	calli @abort
xxaddi_u_r0_r1:
	movi %r0 I80
	movi %r1 1
	bosubr xosubr_r0_r1 %r0 %r1
	calli @abort
xosubr_r0_r1:
	movi %r0 0
	movi %r1 1
	bosubr_u xosubr_u_r0_r1 %r0 %r1
	calli @abort
xosubr_u_r0_r1:
	movi %r0 I80
	bosubi xosubi_r0_r1 %r0 1
	calli @abort
xosubi_r0_r1:
	movi %r0 0
	bosubi_u xosubi_u_r0_r1 %r0 1
	calli @abort
xosubi_u_r0_r1:
	movi %r0 I81
	movi %r1 1
	bxsubr xxsubr_r0_r1 %r0 %r1
	calli @abort
xxsubr_r0_r1:
	movi %r0 I81
	bxsubi xxsubi_r0_r1 %r0 1
	calli @abort
xxsubi_r0_r1:
	movi %r0 I80
	movi %r1 1
	bxsubr_u xxsubr_u_r0_r1 %r0 %r1
	calli @abort
xxsubr_u_r0_r1:
	movi %r0 I80
	bxsubi_u xxsubi_u_r0_r1 %r0 1
	calli @abort
xxsubi_u_r0_r1:
	movi_f %f0 1
	movi_f %f1 2
	bltr_f xltr_f_f0_f1 %f0 %f1
	calli @abort
xltr_f_f0_f1:
	blti_f xlti_f_f0_f1 %f0 2
	calli @abort
xlti_f_f0_f1:
	movi_f %f0 -1
	movi_f %f1 -1
	bler_f xler_f_f0_f1 %f0 %f1
	calli @abort
xler_f_f0_f1:
	blei_f xlei_f_f0_f1 %f0 -1
	calli @abort
xlei_f_f0_f1:
	movi_f %f0 -2
	movi_f %f1 -2
	beqr_f xeqr_f_f0_f1 %f0 %f1
	calli @abort
xeqr_f_f0_f1:
	beqi_f xeqi_f_f0_f1 %f0 -2
	calli @abort
xeqi_f_f0_f1:
	movi_f %f0 -3
	movi_f %f1 -3
	bger_f xger_f_f0_f1 %f0 %f1
	calli @abort
xger_f_f0_f1:
	bgei_f xgei_f_f0_f1 %f0 -3
	calli @abort
xgei_f_f0_f1:
	movi_f %f0 2
	movi_f %f1 1
	bgtr_f xgtr_f_f0_f1 %f0 %f1
	calli @abort
xgtr_f_f0_f1:
	bgti_f xgti_f_f0_f1 %f0 1
	calli @abort
xgti_f_f0_f1:
	movi_f %f0 0
	movi_f %f1 2
	bner_f xner_f_f0_f1 %f0 %f1
	calli @abort
xner_f_f0_f1:
	bnei_f xnei_f_f0_f1 %f0 2
	calli @abort
xnei_f_f0_f1:

	BOPI(lt, -1, 1, 1, -1)
	BOPI(le, -1, -1, 1, 1)
	EBI(eq, 32, 32)
	BOPI(ge, -2, -2, 2, 2)
	BOPI(gt, 2, -2, -2, 2)
	EBI(ne, 3, -3)
	XEBI(ms, 1, 3)
	XEBI(mc, 1, 2)
	XBOPI(oadd, I7f, 1, Iff, 1)
	XBOPI(xadd, I80, 1, I7f, 1)
	XBOPI(osub, I80, 1, 0, 1)
	XBOPI(xsub, I81, 1, I80, 1)
	BOPF(lt, 1, 2)
	BOPF(le, 2, 2)
	BOPF(eq, 3, 3)
	BOPF(ge, 3, 3)
	BOPF(gt, 4, 3)
	UBOPF(ne, 4, 3)
	UBOPF(unlt, 1, 2)
	UBOPF(unle, 2, 2)
	UBOPF(uneq, 3, 3)
	UBOPF(unge, 3, 3)
	UBOPF(ungt, 4, 3)
	BOPF(ltgt, 4, 3)
	movi_f %f0 5
	movi_f %f1 5
	bordr_f ordr_f %f0 %f1
	calli @abort
ordr_f:
	bordi_f ordi_f %f0 1
	calli @abort
ordi_f:
	bordi_f ordi_f_u %f0 $NaN
	jmpi ordi_f_u0
ordi_f_u:
	calli @abort
ordi_f_u0:
	movi_f %f0 5
	movi_f %f1 5
	bunordr_f unordr_f %f0 %f1
	jmpi unordr_f_0
unordr_f:
	calli @abort
unordr_f_0:
	bunordi_f unordi_f %f0 1
	jmpi unordi_f_0
unordi_f:
	calli @abort
unordi_f_0:
	bunordi_f unordi_f_1 %f0 $NaN
	calli @abort
unordi_f_1:

	// just to know did not crash or abort
	prepare
		pushargi ok
		ellipsis
	finishi @printf

	ret
	epilog
