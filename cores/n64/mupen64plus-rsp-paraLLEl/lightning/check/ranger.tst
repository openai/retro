#define M64	67108864

#define aB1	(1<<1)
#define aB2	(1<<2)
#define aB3	(1<<3)
#define aB4	(1<<4)
#define aB5	(1<<5)
#define aB6	(1<<6)
#define aB7	(1<<7)
#define aB8	(1<<8)
#define aB9	(1<<9)
#define aB10	(1<<10)
#define aB11	(1<<11)
#define aB12	(1<<12)
#define aB13	(1<<13)
#define aB14	(1<<14)
#define aB15	(1<<15)
#define aB16	(1<<16)
#define aB17	(1<<17)
#define aB18	(1<<18)
#define aB19	(1<<19)
#define aB20	(1<<20)
#define aB21	(1<<21)
#define aB22	(1<<22)
#define aB23	(1<<23)
#define aB24	(1<<24)
#define aB25	(1<<25)
#define aB26	(1<<26)
#define bB1	(-aB1)
#define bB2	(-aB2)
#define bB3	(-aB3)
#define bB4	(-aB4)
#define bB5	(-aB5)
#define bB6	(-aB6)
#define bB7	(-aB7)
#define bB8	(-aB8)
#define bB9	(-aB9)
#define bB10	(-aB10)
#define bB11	(-aB11)
#define bB12	(-aB12)
#define bB13	(-aB13)
#define bB14	(-aB14)
#define bB15	(-aB15)
#define bB16	(-aB16)
#define bB17	(-aB17)
#define bB18	(-aB18)
#define bB19	(-aB19)
#define bB20	(-aB20)
#define bB21	(-aB21)
#define bB22	(-aB22)
#define bB23	(-aB23)
#define bB24	(-aB24)
#define bB25	(-aB25)
#define bB26	(-aB26)
#define cB1	(aB1-1)
#define cB2	(aB2-1)
#define cB3	(aB3-1)
#define cB4	(aB4-1)
#define cB5	(aB5-1)
#define cB6	(aB6-1)
#define cB7	(aB7-1)
#define cB8	(aB8-1)
#define cB9	(aB9-1)
#define cB10	(aB10-1)
#define cB11	(aB11-1)
#define cB12	(aB12-1)
#define cB13	(aB13-1)
#define cB14	(aB14-1)
#define cB15	(aB15-1)
#define cB16	(aB16-1)
#define cB17	(aB17-1)
#define cB18	(aB18-1)
#define cB19	(aB19-1)
#define cB20	(aB20-1)
#define cB21	(aB21-1)
#define cB22	(aB22-1)
#define cB23	(aB23-1)
#define cB24	(aB24-1)
#define cB25	(aB25-1)
#define cB26	(aB26-1)
#define dB1	(-aB1+1)
#define dB2	(-aB2+1)
#define dB3	(-aB3+1)
#define dB4	(-aB4+1)
#define dB5	(-aB5+1)
#define dB6	(-aB6+1)
#define dB7	(-aB7+1)
#define dB8	(-aB8+1)
#define dB9	(-aB9+1)
#define dB10	(-aB10+1)
#define dB11	(-aB11+1)
#define dB12	(-aB12+1)
#define dB13	(-aB13+1)
#define dB14	(-aB14+1)
#define dB15	(-aB15+1)
#define dB16	(-aB16+1)
#define dB17	(-aB17+1)
#define dB18	(-aB18+1)
#define dB19	(-aB19+1)
#define dB20	(-aB20+1)
#define dB21	(-aB21+1)
#define dB22	(-aB22+1)
#define dB23	(-aB23+1)
#define dB24	(-aB24+1)
#define dB25	(-aB25+1)
#define dB26	(-aB26+1)

#define add(a, b)		$(a + b)
#define sub(a, b)		$(a - b)
#define rsb(a, b)		$(b - a)
#define mul(a, b)		$(a * b)
#define div(a, b)		$(a / b)
#define rem(a, b)		$(a % b)
#define and(a, b)		$(a & b)
#define or(a, b)		$(a | b)
#define xor(a, b)		$(a ^ b)

#define alu2(N, X, L, R, V)					\
	movi %r1 L						\
	movi %r2 R						\
	N##r %r0 %r1 %r2					\
	beqi X %r0 V						\
	calli @abort						\
X:
#define alu1(N, M)						\
	alu2(N, N##M##1, 3, $(M##1), N(3, M##1))		\
	alu2(N, N##M##2, 3, $(M##2), N(3, M##2))		\
	alu2(N, N##M##3, 3, $(M##3), N(3, M##3))		\
	alu2(N, N##M##4, 3, $(M##4), N(3, M##4))		\
	alu2(N, N##M##5, 3, $(M##5), N(3, M##5))		\
	alu2(N, N##M##6, 3, $(M##6), N(3, M##6))		\
	alu2(N, N##M##7, 3, $(M##7), N(3, M##7))		\
	alu2(N, N##M##8, 3, $(M##8), N(3, M##8))		\
	alu2(N, N##M##9, 3, $(M##9), N(3, M##9))		\
	alu2(N, N##M##10, 3, $(M##10), N(3, M##10))		\
	alu2(N, N##M##11, 3, $(M##11), N(3, M##11))		\
	alu2(N, N##M##12, 3, $(M##12), N(3, M##12))		\
	alu2(N, N##M##13, 3, $(M##13), N(3, M##13))		\
	alu2(N, N##M##14, 3, $(M##14), N(3, M##14))		\
	alu2(N, N##M##15, 3, $(M##15), N(3, M##15))		\
	alu2(N, N##M##16, 3, $(M##16), N(3, M##16))		\
	alu2(N, N##M##17, 3, $(M##17), N(3, M##17))		\
	alu2(N, N##M##18, 3, $(M##18), N(3, M##18))		\
	alu2(N, N##M##19, 3, $(M##19), N(3, M##19))		\
	alu2(N, N##M##20, 3, $(M##20), N(3, M##20))		\
	alu2(N, N##M##21, 3, $(M##21), N(3, M##21))		\
	alu2(N, N##M##22, 3, $(M##22), N(3, M##22))		\
	alu2(N, N##M##23, 3, $(M##23), N(3, M##23))		\
	alu2(N, N##M##24, 3, $(M##24), N(3, M##24))		\
	alu2(N, N##M##25, 3, $(M##25), N(3, M##25))		\
	alu2(N, N##M##26, 3, $(M##26), N(3, M##26))

#define alu(N)							\
	alu1(N, aB)						\
	alu1(N, bB)						\
	alu1(N, cB)						\
	alu1(N, dB)

#define _lsh(N)							\
	alu2(lsh, L##N, 3, N, $(3<<N))
#define _rsh(N)							\
	alu2(rsh, R##N, $(1<<63), N, $((1<<63)>>N))

#if __WORDSIZE == 32
#  define xsh64(X)			/**/
#else
#  define xsh64(X)						\
	_##X##sh(32)						\
	_##X##sh(33)						\
	_##X##sh(34)						\
	_##X##sh(35)						\
	_##X##sh(36)						\
	_##X##sh(37)						\
	_##X##sh(38)						\
	_##X##sh(39)						\
	_##X##sh(40)						\
	_##X##sh(41)						\
	_##X##sh(42)						\
	_##X##sh(43)						\
	_##X##sh(44)						\
	_##X##sh(45)						\
	_##X##sh(46)						\
	_##X##sh(47)						\
	_##X##sh(48)						\
	_##X##sh(49)						\
	_##X##sh(50)						\
	_##X##sh(51)						\
	_##X##sh(52)						\
	_##X##sh(53)						\
	_##X##sh(54)						\
	_##X##sh(55)						\
	_##X##sh(56)						\
	_##X##sh(57)						\
	_##X##sh(58)						\
	_##X##sh(59)						\
	_##X##sh(60)						\
	_##X##sh(61)						\
	_##X##sh(62)						\
	_##X##sh(63)
#endif

#define xsh(X)							\
	_##X##sh(0)						\
	_##X##sh(1)						\
	_##X##sh(2)						\
	_##X##sh(3)						\
	_##X##sh(4)						\
	_##X##sh(5)						\
	_##X##sh(6)						\
	_##X##sh(7)						\
	_##X##sh(8)						\
	_##X##sh(9)						\
	_##X##sh(10)						\
	_##X##sh(11)						\
	_##X##sh(12)						\
	_##X##sh(13)						\
	_##X##sh(14)						\
	_##X##sh(15)						\
	_##X##sh(16)						\
	_##X##sh(17)						\
	_##X##sh(18)						\
	_##X##sh(19)						\
	_##X##sh(20)						\
	_##X##sh(21)						\
	_##X##sh(22)						\
	_##X##sh(23)						\
	_##X##sh(24)						\
	_##X##sh(25)						\
	_##X##sh(26)						\
	_##X##sh(27)						\
	_##X##sh(28)						\
	_##X##sh(29)						\
	_##X##sh(30)						\
	_##X##sh(31)						\
	xsh64(X)

#define lsh()							\
	xsh(l)
#define rsh()							\
	xsh(r)

#define reset(V)						\
	prepare							\
		pushargi buf					\
		pushargi V					\
		pushargi $(M64 + 8)				\
	finishi @memset

#define stx(T, N, O, V)						\
	movi %r0 V						\
	movi %r1 O						\
	stxr##T %r1 %v0 %r0
#define stx8(T, M, V)						\
	stx(T, 3, $(M##B3), V)					\
	stx(T, 4, $(M##B4), V)					\
	stx(T, 5, $(M##B5), V)					\
	stx(T, 6, $(M##B6), V)					\
	stx(T, 7, $(M##B7), V)					\
	stx(T, 8, $(M##B8), V)					\
	stx(T, 9, $(M##B9), V)					\
	stx(T, 10, $(M##B10), V)				\
	stx(T, 11, $(M##B11), V)				\
	stx(T, 12, $(M##B12), V)				\
	stx(T, 13, $(M##B13), V)				\
	stx(T, 14, $(M##B14), V)				\
	stx(T, 15, $(M##B15), V)				\
	stx(T, 16, $(M##B16), V)				\
	stx(T, 17, $(M##B17), V)				\
	stx(T, 18, $(M##B18), V)				\
	stx(T, 19, $(M##B19), V)				\
	stx(T, 20, $(M##B20), V)				\
	stx(T, 21, $(M##B21), V)				\
	stx(T, 22, $(M##B22), V)				\
	stx(T, 23, $(M##B23), V)				\
	stx(T, 24, $(M##B24), V)				\
	stx(T, 25, $(M##B25), V)				\
	stx(T, 26, $(M##B26), V)
#define stx4(T, M, V)						\
	stx(T, 2, $(M##B2), V)					\
	stx8(T, M, V)
#define stx2(T, M, V)						\
	stx(T, 1, $(M##B1), V)					\
	stx4(T, M, V)
#define ldx(T, N, M, O, V)					\
	movi %r0 0						\
	ldxi##T %r0 %v0 O					\
	beqi ldx##T##N##M %r0 V					\
	calli @abort						\
ldx##T##N##M:
#define ldx8(T, M, V)						\
	ldx(T, 3, M, $(M##B3), V)				\
	ldx(T, 4, M, $(M##B4), V)				\
	ldx(T, 5, M, $(M##B5), V)				\
	ldx(T, 6, M, $(M##B6), V)				\
	ldx(T, 7, M, $(M##B7), V)				\
	ldx(T, 8, M, $(M##B8), V)				\
	ldx(T, 9, M, $(M##B9), V)				\
	ldx(T, 10, M, $(M##B10), V)				\
	ldx(T, 11, M, $(M##B11), V)				\
	ldx(T, 12, M, $(M##B12), V)				\
	ldx(T, 13, M, $(M##B13), V)				\
	ldx(T, 14, M, $(M##B14), V)				\
	ldx(T, 15, M, $(M##B15), V)				\
	ldx(T, 16, M, $(M##B16), V)				\
	ldx(T, 17, M, $(M##B17), V)				\
	ldx(T, 18, M, $(M##B18), V)				\
	ldx(T, 19, M, $(M##B19), V)				\
	ldx(T, 20, M, $(M##B20), V)				\
	ldx(T, 21, M, $(M##B21), V)				\
	ldx(T, 22, M, $(M##B22), V)				\
	ldx(T, 23, M, $(M##B23), V)				\
	ldx(T, 24, M, $(M##B24), V)				\
	ldx(T, 25, M, $(M##B25), V)				\
	ldx(T, 26, M, $(M##B26), V)
#define ldx4(T, M, V)						\
	ldx(T, 2, M, $(M##B2), V)				\
	ldx8(T, M, V)
#define ldx2(T, M, V)						\
	ldx(T, 1, M, $(M##B1), V)				\
	ldx4(T, M, V)

#define stf(T, N, O, V)						\
	movi##T %f0 V						\
	movi %r0 O						\
	stxr##T %r0 %v0 %f0
#define stf8(T, M, V)						\
	stf(T, 3, $(M##B3), V)					\
	stf(T, 4, $(M##B4), V)					\
	stf(T, 5, $(M##B5), V)					\
	stf(T, 6, $(M##B6), V)					\
	stf(T, 7, $(M##B7), V)					\
	stf(T, 8, $(M##B8), V)					\
	stf(T, 9, $(M##B9), V)					\
	stf(T, 10, $(M##B10), V)				\
	stf(T, 11, $(M##B11), V)				\
	stf(T, 12, $(M##B12), V)				\
	stf(T, 13, $(M##B13), V)				\
	stf(T, 14, $(M##B14), V)				\
	stf(T, 15, $(M##B15), V)				\
	stf(T, 16, $(M##B16), V)				\
	stf(T, 17, $(M##B17), V)				\
	stf(T, 18, $(M##B18), V)				\
	stf(T, 19, $(M##B19), V)				\
	stf(T, 20, $(M##B20), V)				\
	stf(T, 21, $(M##B21), V)				\
	stf(T, 22, $(M##B22), V)				\
	stf(T, 23, $(M##B23), V)				\
	stf(T, 24, $(M##B24), V)				\
	stf(T, 25, $(M##B25), V)				\
	stf(T, 26, $(M##B26), V)
#define stf4(T, M, V)						\
	stf(T, 2, $(M##B2), V)					\
	stf8(T, M, V)
#define ldf(T, N, M, O, V)					\
	movi##T %f0 0						\
	ldxi##T %f0 %v0 O					\
	beqi##T ldf##T##N##M %f0 V				\
	calli @abort						\
ldf##T##N##M:
#define ldf8(T, M, V)						\
	ldf(T, 3, M, $(M##B3), V)				\
	ldf(T, 4, M, $(M##B4), V)				\
	ldf(T, 5, M, $(M##B5), V)				\
	ldf(T, 6, M, $(M##B6), V)				\
	ldf(T, 7, M, $(M##B7), V)				\
	ldf(T, 8, M, $(M##B8), V)				\
	ldf(T, 9, M, $(M##B9), V)				\
	ldf(T, 10, M, $(M##B10), V)				\
	ldf(T, 11, M, $(M##B11), V)				\
	ldf(T, 12, M, $(M##B12), V)				\
	ldf(T, 13, M, $(M##B13), V)				\
	ldf(T, 14, M, $(M##B14), V)				\
	ldf(T, 15, M, $(M##B15), V)				\
	ldf(T, 16, M, $(M##B16), V)				\
	ldf(T, 17, M, $(M##B17), V)				\
	ldf(T, 18, M, $(M##B18), V)				\
	ldf(T, 19, M, $(M##B19), V)				\
	ldf(T, 20, M, $(M##B20), V)				\
	ldf(T, 21, M, $(M##B21), V)				\
	ldf(T, 22, M, $(M##B22), V)				\
	ldf(T, 23, M, $(M##B23), V)				\
	ldf(T, 24, M, $(M##B24), V)				\
	ldf(T, 25, M, $(M##B25), V)				\
	ldf(T, 26, M, $(M##B26), V)
#define ldf4(T, M, V)						\
	ldf(T, 2, M, $(M##B2), V)				\
	ldf8(T, M, V)

#define ldst_c()						\
	reset(0xa5)						\
	movi %v0 buf						\
	stx2(_c, a, 0x5a)					\
	ldx2(_c, a, 0x5a)					\
	reset(0xa5)						\
	movi %v0 $(buf + M64)					\
	stx2(_c, b, 0x5a)					\
	ldx2(_c, b, 0x5a)
#define ldst_uc()						\
	reset(0xa5)						\
	movi %v0 buf						\
	stx2(_c, a, 0x5a)					\
	ldx2(_uc, a, 0x5a)					\
	movi %v0 $(buf + M64)					\
	stx2(_c, b, 0x5a)					\
	ldx2(_uc, b, 0x5a)
#define ldst_s()						\
	reset(0xa5)						\
	movi %v0 buf						\
	stx2(_s, a, 0x5a5a)					\
	ldx2(_s, a, 0x5a5a)					\
	reset(0xa5)						\
	movi %v0 $(buf + M64)					\
	stx2(_s, b, 0x5a5a)					\
	ldx2(_s, b, 0x5a5a)
#define ldst_us()						\
	reset(0xa5)						\
	movi %v0 buf						\
	stx2(_s, a, 0x5a5a)					\
	ldx2(_us, a, 0x5a5a)					\
	reset(0xa5)						\
	movi %v0 $(buf + M64)					\
	stx2(_s, b, 0x5a5a)					\
	ldx2(_us, b, 0x5a5a)
#define ldst_i()						\
	reset(0xa5)						\
	movi %v0 buf						\
	stx4(_i, a, 0x5a5a5a5a)					\
	ldx4(_i, a, 0x5a5a5a5a)					\
	reset(0xa5)						\
	movi %v0 $(buf + M64)					\
	stx4(_i, b, 0x5a5a5a5a)					\
	ldx4(_i, b, 0x5a5a5a5a)
#define ldst_ui()						\
	reset(0xa5)						\
	movi %v0 buf						\
	stx4(_i, a, 0x5a5a5a5a)					\
	ldx4(_ui, a, 0x5a5a5a5a)				\
	reset(0xa5)						\
	movi %v0 $(buf + M64)					\
	stx4(_i, b, 0x5a5a5a5a)					\
	ldx4(_ui, b, 0x5a5a5a5a)
#define ldst_l()						\
	reset(0xa5)						\
	movi %v0 buf						\
	stx8(_l, a, 0x5a5a5a5a5a5a5a5a)				\
	ldx8(_l, a, 0x5a5a5a5a5a5a5a5a)				\
	reset(0xa5)						\
	movi %v0 $(buf + M64)					\
	stx8(_l, b, 0x5a5a5a5a5a5a5a5a)				\
	ldx8(_l, b, 0x5a5a5a5a5a5a5a5a)
#define ldst_f()						\
	reset(0xa5)						\
	movi %v0 buf						\
	stf4(_f, a, 0.5)					\
	ldf4(_f, a, 0.5)					\
	reset(0xa5)						\
	movi %v0 $(buf + M64)					\
	stf4(_f, b, 0.5)					\
	ldf4(_f, b, 0.5)
#define ldst_d()						\
	reset(0xa5)						\
	movi %v0 buf						\
	stf8(_d, a, 0.5)					\
	ldf8(_d, a, 0.5)					\
	reset(0xa5)						\
	movi %v0 $(buf + M64)					\
	stf8(_d, b, 0.5)					\
	ldf8(_d, b, 0.5)

.data		67112960
buf:
.size		M64
.size		8
ok:
.c		"ok"

.code
	prolog

	alu(add)
	alu(sub)
	alu(rsb)
	alu(mul)
	alu(div)
	alu(rem)
	lsh()
	rsh()
	alu(and)
	alu(or)
	alu(xor)
	ldst_c()
	ldst_uc()
	ldst_s()
	ldst_us()
	ldst_i()
#if __WORDSIZE == 64
	ldst_ui()
	ldst_l()
#endif
	ldst_f()
	ldst_d()

	prepare
		pushargi ok
	finishi @puts
	ret
	epilog
