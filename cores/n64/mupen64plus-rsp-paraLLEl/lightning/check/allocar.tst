#define szof_c			1
#define szof_uc			szof_c
#define szof_s			2
#define szof_us			szof_s
#define szof_i			4
#if __WORDSIZE == 64
#  define szof_ui		szof_i
#  define szof_l		8
#endif
#define szof_f			4
#define szof_d			8

#define FILL(T)							\
	name fill##T						\
fill##T:							\
	prolog							\
	arg $argp						\
	getarg %v0 $argp					\
	arg $argi						\
	getarg %r0 $argi					\
	muli %r0 %r0 szof##T					\
	addr %v1 %v0 %r0					\
	movi %r0 0						\
fill##T##loop:							\
	bger fill##T##done %v0 %v1				\
	str##T %v0 %r0						\
	addi %r0 %r0 1						\
	addi %v0 %v0 szof##T					\
	jmpi fill##T##loop					\
fill##T##done:							\
	ret							\
	epilog
#define FILLF(T)						\
	name fill##T						\
fill##T:							\
	prolog							\
	arg $argp						\
	getarg %v0 $argp					\
	arg $argi						\
	getarg %r0 $argi					\
	muli %r0 %r0 szof##T					\
	addr %v1 %v0 %r0					\
	movi##T %f0 0.0						\
fill##T##loop:							\
	bger fill##T##done %v0 %v1				\
	str##T %v0 %f0						\
	addi##T %f0 %f0 1.0					\
	addi %v0 %v0 szof##T					\
	jmpi fill##T##loop					\
fill##T##done:							\
	ret							\
	epilog

#define fill_uc		fill_c
#define fill_us		fill_s
#define fill_ui		fill_i

#define ARG(  T, N)			arg    $arg##T##N
#define ARGF( T, N)			arg##T $arg##T##N
#define ARG1( K, T)			ARG##K(T, 0)
#define ARG2( K, T)	ARG1( K, T)	ARG##K(T, 1)
#define ARG3( K, T)	ARG2( K, T)	ARG##K(T, 2)
#define ARG4( K, T)	ARG3( K, T)	ARG##K(T, 3)
#define ARG5( K, T)	ARG4( K, T)	ARG##K(T, 4)
#define ARG6( K, T)	ARG5( K, T)	ARG##K(T, 5)
#define ARG7( K, T)	ARG6( K, T)	ARG##K(T, 6)
#define ARG8( K, T)	ARG7( K, T)	ARG##K(T, 7)
#define ARG9( K, T)	ARG8( K, T)	ARG##K(T, 8)
#define ARG10(K, T)	ARG9( K, T)	ARG##K(T, 9)
#define ARG11(K, T)	ARG10(K, T)	ARG##K(T, 10)
#define ARG12(K, T)	ARG11(K, T)	ARG##K(T, 11)
#define ARG13(K, T)	ARG12(K, T)	ARG##K(T, 12)
#define ARG14(K, T)	ARG13(K, T)	ARG##K(T, 13)
#define ARG15(K, T)	ARG14(K, T)	ARG##K(T, 14)
#define ARG16(K, T)	ARG15(K, T)	ARG##K(T, 15)
#define ARG_c(N)			ARG##N( , _c)
#define ARG_uc(N)			ARG##N( , _uc)
#define ARG_s(N)			ARG##N( , _s)
#define ARG_us(N)			ARG##N( , _us)
#define ARG_i(N)			ARG##N( , _i)
#define ARG_ui(N)			ARG##N( , _ui)
#define ARG_l(N)			ARG##N( , _l)
#define ARG_f(N)			ARG##N(F, _f)
#define ARG_d(N)			ARG##N(F, _d)

#define CHK(N, T, V)						\
	getarg %r0 $arg##T##V					\
	ldxi##T %r1 %v0 $(V * szof##T)				\
	beqr N##T##V %r0 %r1					\
	calli @abort						\
N##T##V:
#define CHKF(N, T, V)						\
	getarg##T %f0 $arg##T##V				\
	ldxi##T %f1 %v0 $(V * szof##T)				\
	beqr##T N##T##V %f0 %f1					\
	calli @abort						\
N##T##V:

#define GET1( K, N, T, V)				CHK##K(N, T, 0)
#define GET2( K, N, T, V)	GET1( K, N, T, V)	CHK##K(N, T, 1)
#define GET3( K, N, T, V)	GET2( K, N, T, V)	CHK##K(N, T, 2)
#define GET4( K, N, T, V)	GET3( K, N, T, V)	CHK##K(N, T, 3)
#define GET5( K, N, T, V)	GET4( K, N, T, V)	CHK##K(N, T, 4)
#define GET6( K, N, T, V)	GET5( K, N, T, V)	CHK##K(N, T, 5)
#define GET7( K, N, T, V)	GET6( K, N, T, V)	CHK##K(N, T, 6)
#define GET8( K, N, T, V)	GET7( K, N, T, V)	CHK##K(N, T, 7)
#define GET9( K, N, T, V)	GET8( K, N, T, V)	CHK##K(N, T, 8)
#define GET10(K, N, T, V)	GET9( K, N, T, V)	CHK##K(N, T, 9)
#define GET11(K, N, T, V)	GET10(K, N, T, V)	CHK##K(N, T, 10)
#define GET12(K, N, T, V)	GET11(K, N, T, V)	CHK##K(N, T, 11)
#define GET13(K, N, T, V)	GET12(K, N, T, V)	CHK##K(N, T, 12)
#define GET14(K, N, T, V)	GET13(K, N, T, V)	CHK##K(N, T, 13)
#define GET15(K, N, T, V)	GET14(K, N, T, V)	CHK##K(N, T, 14)
#define GET16(K, N, T, V)	GET15(K, N, T, V)	CHK##K(N, T, 15)

#define GET_c(N, M)		GET##N( , c##N,  _c,  M)
#define GET_uc(N, M)		GET##N( , uc##N, _uc, M)
#define GET_s(N, M)		GET##N( , s##N,  _s,  M)
#define GET_us(N, M)		GET##N( , us##N, _us, M)
#define GET_i(N, M)		GET##N( , i##N,  _i,  M)
#define GET_ui(N, M)		GET##N( , ui##N, _ui, M)
#define GET_l(N, M)		GET##N( , l##N,  _l,  M)
#define GET_f(N, M)		GET##N(F, f##N,  _f,  M)
#define GET_d(N, M)		GET##N(F, d##N,  _d,  M)

#define PUSH(  T, V)		pushargi    V
#define PUSHF( T, V)		pushargi##T V
#define PUSH0( K, T)		/**/
#define PUSH1( K, T)					PUSH##K(T, 0)
#define PUSH2( K, T)		PUSH1( K, T)		PUSH##K(T, 1)
#define PUSH3( K, T)		PUSH2( K, T)		PUSH##K(T, 2)
#define PUSH4( K, T)		PUSH3( K, T)		PUSH##K(T, 3)
#define PUSH5( K, T)		PUSH4( K, T)		PUSH##K(T, 4)
#define PUSH6( K, T)		PUSH5( K, T)		PUSH##K(T, 5)
#define PUSH7( K, T)		PUSH6( K, T)		PUSH##K(T, 6)
#define PUSH8( K, T)		PUSH7( K, T)		PUSH##K(T, 7)
#define PUSH9( K, T)		PUSH8( K, T)		PUSH##K(T, 8)
#define PUSH10(K, T)		PUSH9( K, T)		PUSH##K(T, 9)
#define PUSH11(K, T)		PUSH10(K, T)		PUSH##K(T, 10)
#define PUSH12(K, T)		PUSH11(K, T)		PUSH##K(T, 11)
#define PUSH13(K, T)		PUSH12(K, T)		PUSH##K(T, 12)
#define PUSH14(K, T)		PUSH13(K, T)		PUSH##K(T, 13)
#define PUSH15(K, T)		PUSH14(K, T)		PUSH##K(T, 14)
#define PUSH16(K, T)		PUSH15(K, T)		PUSH##K(T, 15)

#define PUSH_c( N)		PUSH##N( , _c)
#define PUSH_uc(N)		PUSH##N( , _uc)
#define PUSH_s( N)		PUSH##N( , _s)
#define PUSH_us(N)		PUSH##N( , _us)
#define PUSH_i( N)		PUSH##N( , _i)
#define PUSH_ui(N)		PUSH##N( , _ui)
#define PUSH_l( N)		PUSH##N( , _l)
#define PUSH_f( N)		PUSH##N(F, _f)
#define PUSH_d( N)		PUSH##N(F, _d)

/* bottom function */
#define DEF0(T)							\
	name test##T##_0					\
test##T##_0:							\
	prolog							\
	ret							\
	epilog

#define DEFN(N, M, T)						\
	name test##T##_##N					\
test##T##_##N:							\
	prolog							\
	arg $argp						\
	/* stack buffer in %v0 */				\
	getarg %v0 $argp					\
	ARG##T(N)						\
	/* validate arguments */				\
	GET##T(N, M)						\
	/* heap buffer in %v1 */				\
	prepare							\
		pushargi $(N * szof##T)				\
	finishi @malloc						\
	retval %v1						\
	/* copy stack bufer to heap buffer */			\
	prepare							\
		pushargr %v1					\
		pushargr %v0					\
		pushargi $(N * szof##T)				\
	finishi MEMCPY						\
	/* stack buffer for next function in %v2 */		\
	movi %r0 $(M * szof##T)					\
	allocar %v2 %r0						\
	addr %v2 %v2 %fp					\
	/* fill stack buffer for next function */		\
	prepare							\
		pushargr %v2					\
		pushargi M					\
	finishi fill##T						\
	/* call next function */				\
	prepare							\
		pushargr %v2					\
		PUSH##T(M)					\
	finishi test##T##_##M					\
	/* validate stack buffer */				\
	prepare							\
		pushargr %v1					\
		pushargr %v0					\
		pushargi $(N * szof##T)				\
	finishi @memcmp						\
	retval %r0						\
	beqi test##T##_##N##_done %r0 0				\
	calli @abort						\
test##T##_##N##_done:						\
	/* release heap bufer */				\
	prepare							\
		pushargr %v1					\
	finishi @free						\
	ret							\
	epilog

/* top function */
#define DEFX(T)							\
	name test##T##_17					\
test##T##_17:							\
	prolog							\
	/* heap buffer in %v1 */				\
	prepare							\
		pushargi $(16 * szof##T)			\
	finishi @malloc						\
	retval %v1						\
	/* stack buffer for next function in %v2 */		\
	movi %r0 $(16 * szof##T)				\
	allocar %v2 %r0						\
	addr %v2 %v2 %fp					\
	/* fill stack buffer for next function */		\
	prepare							\
		pushargr %v2					\
		pushargi 16					\
	finishi fill##T						\
	/* copy stack buffer to heap buffer */			\
	prepare							\
		pushargr %v1					\
		pushargr %v2					\
		pushargi $(16 * szof##T)			\
	finishi MEMCPY						\
	/* call next function */				\
	prepare							\
		pushargr %v2					\
		PUSH##T(16)					\
	finishi test##T##_16					\
	/* validate stack buffer */				\
	prepare							\
		pushargr %v1					\
		pushargr %v2					\
		pushargi $(16 * szof##T)			\
	finishi @memcmp						\
	retval %r0						\
	beqi test##T##_17_done %r0 0				\
	calli @abort						\
test##T##_17_done:						\
	/* release heap bufer */				\
	prepare							\
		pushargr %v1					\
	finishi @free						\
	ret							\
	epilog

#define DEF(  T)						\
	DEF0( T)						\
	DEFN( 1,  0, T)						\
	DEFN( 2,  1, T)						\
	DEFN( 3,  2, T)						\
	DEFN( 4,  3, T)						\
	DEFN( 5,  4, T)						\
	DEFN( 6,  5, T)						\
	DEFN( 7,  6, T)						\
	DEFN( 8,  7, T)						\
	DEFN( 9,  8, T)						\
	DEFN(10,  9, T)						\
	DEFN(11, 10, T)						\
	DEFN(12, 11, T)						\
	DEFN(13, 12, T)						\
	DEFN(14, 13, T)						\
	DEFN(15, 14, T)						\
	DEFN(16, 15, T)						\
	DEFX(T)

#define CALL(T)			calli test##T##_17

.data	16
fmt:
.c	"%d %d %d\n"
.code
	jmpi main

#if _AIX
#  define MEMCPY		memcpy
/* error: Function not implemented (memcpy) */
	name memcpy
memcpy:
	prolog
	arg $dst
	arg $src
	arg $len
	getarg %r0 $dst
	getarg %r1 $src
	getarg %r2 $len
	movr %v1 %r0
	blti memcpy_done %r2 1
memcpy_loop:
	subi %r2 %r2 1
	ldxr_c %v0 %r1 %r2
	stxr_c %r2 %r0 %v0
	bgti memcpy_loop %r2 0
memcpy_done:
	retr %v1
	epilog
#else
#  define MEMCPY		@memcpy
#endif

	FILL(_c)
	FILL(_s)
	FILL(_i)
#if __WORDSIZE == 64
	FILL(_l)
#endif
	FILLF(_f)
	FILLF(_d)

	DEF(_c)
	DEF(_uc)
	DEF(_s)
	DEF(_us)
	DEF(_i)
#if __WORDSIZE == 64
	DEF(_ui)
	DEF(_l)
#endif
	DEF(_f)
	DEF(_d)

	name main
main:
	prolog

	CALL(_c)
	CALL(_uc)
	CALL(_s)
	CALL(_us)
	CALL(_i)
#if __WORDSIZE == 64
	CALL(_ui)
	CALL(_l)
#endif
	CALL(_f)
	CALL(_d)

	// loop control
	movi %v2 1

	// loop a few times calling allocar
loop:
	// allocate 12 bytes
	movi %r0 12
	allocar %v0 %r0

	// offset
	movr %v1 %v0

	// 1
	stxr_i %v1 %fp %v2

	// 2
	addi %v2 %v2 1
	addi %v1 %v1 4
	stxr_i %v1 %fp %v2

	// 3
	addi %v2 %v2 1
	addi %v1 %v1 4
	stxr_i %v1 %fp %v2

	// reload
	movr %v1 %v0

	// 1
	ldxr_i %r0 %fp %v1

	// 2
	addi %v1 %v1 4
	ldxr_i %r1 %fp %v1

	// 3
	addi %v1 %v1 4
	ldxr_i %r2 %fp %v1

	prepare
		pushargi fmt
		ellipsis
		pushargr %r0
		pushargr %r1
		pushargr %r2
	finishi @printf
	blti loop %v2 9

	ret
	epilog
