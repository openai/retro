.data	8
ok:
.c	"ok\n"
.code
	jmpi main

#define BEGIN(L)						\
L:								\
	prolog
#define VA_START()						\
	ellipsis						\
	va_start %v0
#define VARG(L,N)						\
	va_arg %r0 %v0						\
	beqi L##N %r0 N						\
	calli @abort						\
L##N:
#define VARGD(L,N)						\
	va_arg_d %f0 %v0					\
	beqi_d L##N %f0 N					\
	calli @abort						\
L##N:
#define VA_END()						\
	va_end %v0						\
	ret							\
	epilog
#define ARG(N)			arg $arg##N
#define ARGD(N)			arg_d $arg##N
#define GET(L,N)						\
	getarg %r0 $arg##N					\
	beqi L##N %r0 N						\
	calli @abort						\
L##N:
#define GETD(L,N)						\
	getarg_d %f0 $arg##N					\
	beqi_d L##N %f0 N					\
	calli @abort						\
L##N:

#define ARG1()				ARG(1)
#define ARG2()		ARG1()		ARG(2)
#define ARG3()		ARG2()		ARG(3)
#define ARG4()		ARG3()		ARG(4)
#define ARG5()		ARG4()		ARG(5)
#define ARG6()		ARG5()		ARG(6)
#define ARG7()		ARG6()		ARG(7)
#define ARG8()		ARG7()		ARG(8)
#define ARG9()		ARG8()		ARG(9)
#define GET1(L)				GET(L,1)
#define GET2(L)		GET1(L)		GET(L,2)
#define GET3(L)		GET2(L)		GET(L,3)
#define GET4(L)		GET3(L)		GET(L,4)
#define GET5(L)		GET4(L)		GET(L,5)
#define GET6(L)		GET5(L)		GET(L,6)
#define GET7(L)		GET6(L)		GET(L,7)
#define GET8(L)		GET7(L)		GET(L,8)
#define GET9(L)		GET8(L)		GET(L,9)
#define ARGD1()				ARGD(1)
#define ARGD2()		ARGD1()		ARGD(2)
#define ARGD3()		ARGD2()		ARGD(3)
#define ARGD4()		ARGD3()		ARGD(4)
#define ARGD5()		ARGD4()		ARGD(5)
#define ARGD6()		ARGD5()		ARGD(6)
#define ARGD7()		ARGD6()		ARGD(7)
#define ARGD8()		ARGD7()		ARGD(8)
#define ARGD9()		ARGD8()		ARGD(9)
#define GETD1(L)			GETD(L,1)
#define GETD2(L)	GETD1(L)	GETD(L,2)
#define GETD3(L)	GETD2(L)	GETD(L,3)
#define GETD4(L)	GETD3(L)	GETD(L,4)
#define GETD5(L)	GETD4(L)	GETD(L,5)
#define GETD6(L)	GETD5(L)	GETD(L,6)
#define GETD7(L)	GETD6(L)	GETD(L,7)
#define GETD8(L)	GETD7(L)	GETD(L,8)
#define GETD9(L)	GETD8(L)	GETD(L,9)
#define IDARG1()			ARG(1)
#define IDARG2()	IDARG1()	ARGD(2)
#define IDARG3()	IDARG2()	ARG(3)
#define IDARG4()	IDARG3()	ARGD(4)
#define IDARG5()	IDARG4()	ARG(5)
#define IDARG6()	IDARG5()	ARGD(6)
#define IDARG7()	IDARG6()	ARG(7)
#define IDARG8()	IDARG7()	ARGD(8)
#define IDARG9()	IDARG8()	ARG(9)
#define IDGET1(L)			GET(L,1)
#define IDGET2(L)	IDGET1(L)	GETD(L,2)
#define IDGET3(L)	IDGET2(L)	GET(L,3)
#define IDGET4(L)	IDGET3(L)	GETD(L,4)
#define IDGET5(L)	IDGET4(L)	GET(L,5)
#define IDGET6(L)	IDGET5(L)	GETD(L,6)
#define IDGET7(L)	IDGET6(L)	GET(L,7)
#define IDGET8(L)	IDGET7(L)	GETD(L,8)
#define IDGET9(L)	IDGET8(L)	GET(L,9)
#define DIARG1()			ARGD(1)
#define DIARG2()	DIARG1()	ARG(2)
#define DIARG3()	DIARG2()	ARGD(3)
#define DIARG4()	DIARG3()	ARG(4)
#define DIARG5()	DIARG4()	ARGD(5)
#define DIARG6()	DIARG5()	ARG(6)
#define DIARG7()	DIARG6()	ARGD(7)
#define DIARG8()	DIARG7()	ARG(8)
#define DIARG9()	DIARG8()	ARGD(9)
#define DIGET1(L)			GETD(L,1)
#define DIGET2(L)	DIGET1(L)	GET(L,2)
#define DIGET3(L)	DIGET2(L)	GETD(L,3)
#define DIGET4(L)	DIGET3(L)	GET(L,4)
#define DIGET5(L)	DIGET4(L)	GETD(L,5)
#define DIGET6(L)	DIGET5(L)	GET(L,6)
#define DIGET7(L)	DIGET6(L)	GETD(L,7)
#define DIGET8(L)	DIGET7(L)	GET(L,8)
#define DIGET9(L)	DIGET8(L)	GETD(L,9)

#define VARG1(L)						\
	VARG(L, 10)
#define VARG2(L)						\
	VARG(L, 9)						\
	VARG1(L)
#define VARG3(L)						\
	VARG(L, 8)						\
	VARG2(L)
#define VARG4(L)						\
	VARG(L, 7)						\
	VARG3(L)
#define VARG5(L)						\
	VARG(L, 6)						\
	VARG4(L)
#define VARG6(L)						\
	VARG(L, 5)						\
	VARG5(L)
#define VARG7(L)						\
	VARG(L, 4)						\
	VARG6(L)
#define VARG8(L)						\
	VARG(L, 3)						\
	VARG7(L)
#define VARG9(L)						\
	VARG(L, 2)						\
	VARG8(L)
#define VARG10(L)						\
	VARG(L, 1)						\
	VARG9(L)
#define VARGD1(L)						\
	VARGD(L, 10)
#define VARGD2(L)						\
	VARGD(L, 9)						\
	VARGD1(L)
#define VARGD3(L)						\
	VARGD(L, 8)						\
	VARGD2(L)
#define VARGD4(L)						\
	VARGD(L, 7)						\
	VARGD3(L)
#define VARGD5(L)						\
	VARGD(L, 6)						\
	VARGD4(L)
#define VARGD6(L)						\
	VARGD(L, 5)						\
	VARGD5(L)
#define VARGD7(L)						\
	VARGD(L, 4)						\
	VARGD6(L)
#define VARGD8(L)						\
	VARGD(L, 3)						\
	VARGD7(L)
#define VARGD9(L)						\
	VARGD(L, 2)						\
	VARGD8(L)
#define VARGD10(L)						\
	VARGD(L, 1)						\
	VARGD9(L)
#define IDVARG1(L)						\
	VARGD(L, 10)
#define IDVARG2(L)						\
	VARG(L, 9)						\
	IDVARG1(L)
#define IDVARG3(L)						\
	VARGD(L, 8)						\
	IDVARG2(L)
#define IDVARG4(L)						\
	VARG(L, 7)						\
	IDVARG3(L)
#define IDVARG5(L)						\
	VARGD(L, 6)						\
	IDVARG4(L)
#define IDVARG6(L)						\
	VARG(L, 5)						\
	IDVARG5(L)
#define IDVARG7(L)						\
	VARGD(L, 4)						\
	IDVARG6(L)
#define IDVARG8(L)						\
	VARG(L, 3)						\
	IDVARG7(L)
#define IDVARG9(L)						\
	VARGD(L, 2)						\
	IDVARG8(L)
#define IDVARG10(L)						\
	VARG(L, 1)						\
	IDVARG9(L)
#define DIVARG1(L)						\
	VARG(L, 10)
#define DIVARG2(L)						\
	VARGD(L, 9)						\
	DIVARG1(L)
#define DIVARG3(L)						\
	VARG(L, 8)						\
	DIVARG2(L)
#define DIVARG4(L)						\
	VARGD(L, 7)						\
	DIVARG3(L)
#define DIVARG5(L)						\
	VARG(L, 6)						\
	DIVARG4(L)
#define DIVARG6(L)						\
	VARGD(L, 5)						\
	DIVARG5(L)
#define DIVARG7(L)						\
	VARG(L, 4)						\
	DIVARG6(L)
#define DIVARG8(L)						\
	VARGD(L, 3)						\
	DIVARG7(L)
#define DIVARG9(L)						\
	VARG(L, 2)						\
	DIVARG8(L)
#define DIVARG10(L)						\
	VARGD(L, 1)						\
	DIVARG9(L)

BEGIN(_iiiiiiiiii)
	VA_START()
	VARG10(_iiiiiiiiii)
	VA_END()
BEGIN(i_iiiiiiiii)
	ARG1()
	GET1(i_iiiiiiiii)
	VA_START()
	VARG9(i_iiiiiiiii)
	VA_END()
BEGIN(ii_iiiiiiii)
	ARG2()
	GET2(ii_iiiiiiii)
	VA_START()
	VARG8(ii_iiiiiiii)
	VA_END()
BEGIN(iii_iiiiiii)
	ARG3()
	GET3(iii_iiiiiii)
	VA_START()
	VARG7(iii_iiiiiii)
	VA_END()
BEGIN(iiii_iiiiii)
	ARG4()
	GET4(iiii_iiiiii)
	VA_START()
	VARG6(iiii_iiiiii)
	VA_END()
BEGIN(iiiii_iiiii)
	ARG5()
	GET5(iiiii_iiiii)
	VA_START()
	VARG5(iiiii_iiiii)
	VA_END()
BEGIN(iiiiii_iiii)
	ARG6()
	GET6(iiiiii_iiii)
	VA_START()
	VARG4(iiiiii_iiii)
	VA_END()
BEGIN(iiiiiii_iii)
	ARG7()
	GET7(iiiiiii_iii)
	VA_START()
	VARG3(iiiiiii_iii)
	VA_END()
BEGIN(iiiiiiii_ii)
	ARG8()
	GET8(iiiiiiii_ii)
	VA_START()
	VARG2(iiiiiiii_ii)
	VA_END()
BEGIN(iiiiiiiii_i)
	ARG9()
	GET9(iiiiiiiii_i)
	VA_START()
	VARG1(iiiiiiiii_i)
	VA_END()
BEGIN(_dddddddddd)
	VA_START()
	VARGD10(_dddddddddd)
	VA_END()
BEGIN(d_ddddddddd)
	ARGD1()
	GETD1(d_ddddddddd)
	VA_START()
	VARGD9(d_ddddddddd)
	VA_END()
BEGIN(dd_dddddddd)
	ARGD2()
	GETD2(dd_dddddddd)
	VA_START()
	VARGD8(dd_dddddddd)
	VA_END()
BEGIN(ddd_ddddddd)
	ARGD3()
	GETD3(ddd_ddddddd)
	VA_START()
	VARGD7(ddd_ddddddd)
	VA_END()
BEGIN(dddd_dddddd)
	ARGD4()
	GETD4(dddd_dddddd)
	VA_START()
	VARGD6(dddd_dddddd)
	VA_END()
BEGIN(ddddd_ddddd)
	ARGD5()
	GETD5(ddddd_ddddd)
	VA_START()
	VARGD5(ddddd_ddddd)
	VA_END()
BEGIN(dddddd_dddd)
	ARGD6()
	GETD6(dddddd_dddd)
	VA_START()
	VARGD4(dddddd_dddd)
	VA_END()
BEGIN(ddddddd_ddd)
	ARGD7()
	GETD7(ddddddd_ddd)
	VA_START()
	VARGD3(ddddddd_ddd)
	VA_END()
BEGIN(dddddddd_dd)
	ARGD8()
	GETD8(dddddddd_dd)
	VA_START()
	VARGD2(dddddddd_dd)
	VA_END()
BEGIN(ddddddddd_d)
	ARGD9()
	GETD9(ddddddddd_d)
	VA_START()
	VARGD1(ddddddddd_d)
	VA_END()
BEGIN(_ididididid)
	VA_START()
	IDVARG10(_ididididid)
	VA_END()
BEGIN(i_didididid)
	IDARG1()
	IDGET1(i_didididid)
	VA_START()
	IDVARG9(i_didididid)
	VA_END()
BEGIN(id_idididid)
	IDARG2()
	IDGET2(id_idididid)
	VA_START()
	IDVARG8(id_idididid)
	VA_END()
BEGIN(idi_dididid)
	IDARG3()
	IDGET3(idi_dididid)
	VA_START()
	IDVARG7(idi_dididid)
	VA_END()
BEGIN(idid_ididid)
	IDARG4()
	IDGET4(idid_ididid)
	VA_START()
	IDVARG6(idid_ididid)
	VA_END()
BEGIN(ididi_didid)
	IDARG5()
	IDGET5(ididi_didid)
	VA_START()
	IDVARG5(ididi_didid)
	VA_END()
BEGIN(ididid_idid)
	IDARG6()
	IDGET6(ididid_idid)
	VA_START()
	IDVARG4(ididid_idid)
	VA_END()
BEGIN(idididi_did)
	IDARG7()
	IDGET7(idididi_did)
	VA_START()
	IDVARG3(idididi_did)
	VA_END()
BEGIN(idididid_id)
	IDARG8()
	IDGET8(idididid_id)
	VA_START()
	IDVARG2(idididid_id)
	VA_END()
BEGIN(ididididi_d)
	IDARG9()
	IDGET9(ididididi_d)
	VA_START()
	IDVARG1(ididididi_d)
	VA_END()
BEGIN(_dididididi)
	VA_START()
	DIVARG10(_dididididi)
	VA_END()
BEGIN(d_ididididi)
	DIARG1()
	DIGET1(d_ididididi)
	VA_START()
	DIVARG9(d_ididididi)
	VA_END()
BEGIN(di_didididi)
	DIARG2()
	DIGET2(di_didididi)
	VA_START()
	DIVARG8(di_didididi)
	VA_END()
BEGIN(did_idididi)
	DIARG3()
	DIGET3(did_idididi)
	VA_START()
	DIVARG7(did_idididi)
	VA_END()
BEGIN(didi_dididi)
	DIARG4()
	DIGET4(didi_dididi)
	VA_START()
	DIVARG6(didi_dididi)
	VA_END()
BEGIN(didid_ididi)
	DIARG5()
	DIGET5(didid_ididi)
	VA_START()
	DIVARG5(didid_ididi)
	VA_END()
BEGIN(dididi_didi)
	DIARG6()
	DIGET6(dididi_didi)
	VA_START()
	DIVARG4(dididi_didi)
	VA_END()
BEGIN(dididid_idi)
	DIARG7()
	DIGET7(dididid_idi)
	VA_START()
	DIVARG3(dididid_idi)
	VA_END()
BEGIN(didididi_di)
	DIARG8()
	DIGET8(didididi_di)
	VA_START()
	DIVARG2(didididi_di)
	VA_END()
BEGIN(didididid_i)
	DIARG9()
	DIGET9(didididid_i)
	VA_START()
	DIVARG1(didididid_i)
	VA_END()

#define PUSH1()		pushargi 1
#define PUSH2()		PUSH1()		pushargi 2
#define PUSH3()		PUSH2()		pushargi 3
#define PUSH4()		PUSH3()		pushargi 4
#define PUSH5()		PUSH4()		pushargi 5
#define PUSH6()		PUSH5()		pushargi 6
#define PUSH7()		PUSH6()		pushargi 7
#define PUSH8()		PUSH7()		pushargi 8
#define PUSH9()		PUSH8()		pushargi 9
#define VPUSH1()	pushargi 1	VPUSH2()
#define VPUSH2()	pushargi 2	VPUSH3()
#define VPUSH3()	pushargi 3	VPUSH4()
#define VPUSH4()	pushargi 4	VPUSH5()
#define VPUSH5()	pushargi 5	VPUSH6()
#define VPUSH6()	pushargi 6	VPUSH7()
#define VPUSH7()	pushargi 7	VPUSH8()
#define VPUSH8()	pushargi 8	VPUSH9()
#define VPUSH9()	pushargi 9	VPUSH10()
#define VPUSH10()	pushargi 10
#define PUSHD1()	pushargi_d 1
#define PUSHD2()	PUSHD1()	pushargi_d 2
#define PUSHD3()	PUSHD2()	pushargi_d 3
#define PUSHD4()	PUSHD3()	pushargi_d 4
#define PUSHD5()	PUSHD4()	pushargi_d 5
#define PUSHD6()	PUSHD5()	pushargi_d 6
#define PUSHD7()	PUSHD6()	pushargi_d 7
#define PUSHD8()	PUSHD7()	pushargi_d 8
#define PUSHD9()	PUSHD8()	pushargi_d 9
#define VPUSHD1()	pushargi_d 1	VPUSHD2()
#define VPUSHD2()	pushargi_d 2	VPUSHD3()
#define VPUSHD3()	pushargi_d 3	VPUSHD4()
#define VPUSHD4()	pushargi_d 4	VPUSHD5()
#define VPUSHD5()	pushargi_d 5	VPUSHD6()
#define VPUSHD6()	pushargi_d 6	VPUSHD7()
#define VPUSHD7()	pushargi_d 7	VPUSHD8()
#define VPUSHD8()	pushargi_d 8	VPUSHD9()
#define VPUSHD9()	pushargi_d 9	VPUSHD10()
#define VPUSHD10()	pushargi_d 10
#define IDPUSH1()	pushargi 1
#define IDPUSH2()	IDPUSH1()	pushargi_d 2
#define IDPUSH3()	IDPUSH2()	pushargi 3
#define IDPUSH4()	IDPUSH3()	pushargi_d 4
#define IDPUSH5()	IDPUSH4()	pushargi 5
#define IDPUSH6()	IDPUSH5()	pushargi_d 6
#define IDPUSH7()	IDPUSH6()	pushargi 7
#define IDPUSH8()	IDPUSH7()	pushargi_d 8
#define IDPUSH9()	IDPUSH8()	pushargi 9
#define IDVPUSH1()	pushargi 1	IDVPUSH2()
#define IDVPUSH2()	pushargi_d 2	IDVPUSH3()
#define IDVPUSH3()	pushargi 3	IDVPUSH4()
#define IDVPUSH4()	pushargi_d 4	IDVPUSH5()
#define IDVPUSH5()	pushargi 5	IDVPUSH6()
#define IDVPUSH6()	pushargi_d 6	IDVPUSH7()
#define IDVPUSH7()	pushargi 7	IDVPUSH8()
#define IDVPUSH8()	pushargi_d 8	IDVPUSH9()
#define IDVPUSH9()	pushargi 9	IDVPUSH10()
#define IDVPUSH10()	pushargi_d 10
#define DIPUSH1()	pushargi_d 1
#define DIPUSH2()	DIPUSH1()	pushargi 2
#define DIPUSH3()	DIPUSH2()	pushargi_d 3
#define DIPUSH4()	DIPUSH3()	pushargi 4
#define DIPUSH5()	DIPUSH4()	pushargi_d 5
#define DIPUSH6()	DIPUSH5()	pushargi 6
#define DIPUSH7()	DIPUSH6()	pushargi_d 7
#define DIPUSH8()	DIPUSH7()	pushargi 8
#define DIPUSH9()	DIPUSH8()	pushargi_d 9
#define DIVPUSH1()	pushargi_d 1	DIVPUSH2()
#define DIVPUSH2()	pushargi 2	DIVPUSH3()
#define DIVPUSH3()	pushargi_d 3	DIVPUSH4()
#define DIVPUSH4()	pushargi 4	DIVPUSH5()
#define DIVPUSH5()	pushargi_d 5	DIVPUSH6()
#define DIVPUSH6()	pushargi 6	DIVPUSH7()
#define DIVPUSH7()	pushargi_d 7	DIVPUSH8()
#define DIVPUSH8()	pushargi 8	DIVPUSH9()
#define DIVPUSH9()	pushargi_d 9	DIVPUSH10()
#define DIVPUSH10()	pushargi 10

main:
	prolog
	prepare
		ellipsis
		VPUSH1()
	finishi _iiiiiiiiii
	prepare
		PUSH1()
		ellipsis
		VPUSH2()
	finishi i_iiiiiiiii
	prepare
		PUSH2()
		ellipsis
		VPUSH3()
	finishi ii_iiiiiiii
	prepare
		PUSH3()
		ellipsis
		VPUSH4()
	finishi iii_iiiiiii
	prepare
		PUSH4()
		ellipsis
		VPUSH5()
	finishi iiii_iiiiii
	prepare
		PUSH5()
		ellipsis
		VPUSH6()
	finishi iiiii_iiiii
	prepare
		PUSH6()
		ellipsis
		VPUSH7()
	finishi iiiiii_iiii
	prepare
		PUSH7()
		ellipsis
		VPUSH8()
	finishi iiiiiii_iii
	prepare
		PUSH8()
		ellipsis
		VPUSH9()
	finishi iiiiiiii_ii
	prepare
		PUSH9()
		ellipsis
		VPUSH10()
	finishi iiiiiiiii_i
	prepare
		ellipsis
		VPUSHD1()
	finishi _dddddddddd
	prepare
		PUSHD1()
		ellipsis
		VPUSHD2()
	finishi d_ddddddddd
	prepare
		PUSHD2()
		ellipsis
		VPUSHD3()
	finishi dd_dddddddd
	prepare
		PUSHD3()
		ellipsis
		VPUSHD4()
	finishi ddd_ddddddd
	prepare
		PUSHD4()
		ellipsis
		VPUSHD5()
	finishi dddd_dddddd
	prepare
		PUSHD5()
		ellipsis
		VPUSHD6()
	finishi ddddd_ddddd
	prepare
		PUSHD6()
		ellipsis
		VPUSHD7()
	finishi dddddd_dddd
	prepare
		PUSHD7()
		ellipsis
		VPUSHD8()
	finishi ddddddd_ddd
	prepare
		PUSHD8()
		ellipsis
		VPUSHD9()
	finishi dddddddd_dd
	prepare
		PUSHD9()
		ellipsis
		VPUSHD10()
	finishi ddddddddd_d
	prepare
		ellipsis
		IDVPUSH1()
	finishi _ididididid
	prepare
		IDPUSH1()
		ellipsis
		IDVPUSH2()
	finishi i_didididid
	prepare
		IDPUSH2()
		ellipsis
		IDVPUSH3()
	finishi id_idididid
	prepare
		IDPUSH3()
		ellipsis
		IDVPUSH4()
	finishi idi_dididid
	prepare
		IDPUSH4()
		ellipsis
		IDVPUSH5()
	finishi idid_ididid
	prepare
		IDPUSH5()
		ellipsis
		IDVPUSH6()
	finishi ididi_didid
	prepare
		IDPUSH6()
		ellipsis
		IDVPUSH7()
	finishi ididid_idid
	prepare
		IDPUSH7()
		ellipsis
		IDVPUSH8()
	finishi idididi_did
	prepare
		IDPUSH8()
		ellipsis
		IDVPUSH9()
	finishi idididid_id
	prepare
		IDPUSH9()
		ellipsis
		IDVPUSH10()
	finishi ididididi_d
	prepare
		ellipsis
		DIVPUSH1()
	finishi _dididididi
	prepare
		DIPUSH1()
		ellipsis
		DIVPUSH2()
	finishi d_ididididi
	prepare
		DIPUSH2()
		ellipsis
		DIVPUSH3()
	finishi di_didididi
	prepare
		DIPUSH3()
		ellipsis
		DIVPUSH4()
	finishi did_idididi
	prepare
		DIPUSH4()
		ellipsis
		DIVPUSH5()
	finishi didi_dididi
	prepare
		DIPUSH5()
		ellipsis
		DIVPUSH6()
	finishi didid_ididi
	prepare
		DIPUSH6()
		ellipsis
		DIVPUSH7()
	finishi dididi_didi
	prepare
		DIPUSH7()
		ellipsis
		DIVPUSH8()
	finishi dididid_idi
	prepare
		DIPUSH8()
		ellipsis
		DIVPUSH9()
	finishi didididi_di
	prepare
		DIPUSH9()
		ellipsis
		DIVPUSH10()
	finishi didididid_i
	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
