#include <lightning.h>
#include <stdarg.h>
#include <stdio.h>

#define W		jit_word_t
#define D		jit_float64_t
#define VASTART(A)							\
    va_list		ap;						\
    va_start(ap, A)
#define VARG2()		a2 = va_arg(ap, jit_word_t);	VARG3()
#define VARG3()		a3 = va_arg(ap, jit_word_t);	VARG4()
#define VARG4()		a4 = va_arg(ap, jit_word_t);	VARG5()
#define VARG5()		a5 = va_arg(ap, jit_word_t);	VARG6()
#define VARG6()		a6 = va_arg(ap, jit_word_t);	VARG7()
#define VARG7()		a7 = va_arg(ap, jit_word_t);	VARG8()
#define VARG8()		a8 = va_arg(ap, jit_word_t);	VARG9()
#define VARG9()		a9 = va_arg(ap, jit_word_t);	VARG10()
#define VARG10()	a10 = va_arg(ap, jit_word_t);	va_end(ap)

#define VARGD2()	a2 = va_arg(ap, jit_float64_t);	VARGD3()
#define VARGD3()	a3 = va_arg(ap, jit_float64_t);	VARGD4()
#define VARGD4()	a4 = va_arg(ap, jit_float64_t);	VARGD5()
#define VARGD5()	a5 = va_arg(ap, jit_float64_t);	VARGD6()
#define VARGD6()	a6 = va_arg(ap, jit_float64_t);	VARGD7()
#define VARGD7()	a7 = va_arg(ap, jit_float64_t);	VARGD8()
#define VARGD8()	a8 = va_arg(ap, jit_float64_t);	VARGD9()
#define VARGD9()	a9 = va_arg(ap, jit_float64_t);	VARGD10()
#define VARGD10()	a10 = va_arg(ap, jit_float64_t);va_end(ap)

#define IDVARG2()	a2 = va_arg(ap, jit_float64_t);	IDVARG3()
#define IDVARG3()	a3 = va_arg(ap, jit_word_t);	IDVARG4()
#define IDVARG4()	a4 = va_arg(ap, jit_float64_t);	IDVARG5()
#define IDVARG5()	a5 = va_arg(ap, jit_word_t);	IDVARG6()
#define IDVARG6()	a6 = va_arg(ap, jit_float64_t);	IDVARG7()
#define IDVARG7()	a7 = va_arg(ap, jit_word_t);	IDVARG8()
#define IDVARG8()	a8 = va_arg(ap, jit_float64_t);	IDVARG9()
#define IDVARG9()	a9 = va_arg(ap, jit_word_t);	IDVARG10()
#define IDVARG10()	a10 = va_arg(ap, jit_float64_t);va_end(ap)

#define DIVARG2()	a2 = va_arg(ap, jit_word_t);	DIVARG3()
#define DIVARG3()	a3 = va_arg(ap, jit_float64_t);	DIVARG4()
#define DIVARG4()	a4 = va_arg(ap, jit_word_t);	DIVARG5()
#define DIVARG5()	a5 = va_arg(ap, jit_float64_t);	DIVARG6()
#define DIVARG6()	a6 = va_arg(ap, jit_word_t);	DIVARG7()
#define DIVARG7()	a7 = va_arg(ap, jit_float64_t);	DIVARG8()
#define DIVARG8()	a8 = va_arg(ap, jit_word_t);	DIVARG9()
#define DIVARG9()	a9 = va_arg(ap, jit_float64_t);	DIVARG10()
#define DIVARG10()	a10 = va_arg(ap, jit_word_t);	va_end(ap)

#define CHECK()								\
    do {								\
	if (a1 != 1 || a2 != 2 || a3 != 3 || a4 != 4 || a5 != 5 ||	\
	    a6 != 6 || a7 != 7 || a8 != 8 || a9 != 9 || a10 != 10)	\
	    abort();							\
    } while (0)


void i_iiiiiiiii(W a1, ...)
{
    W		a2, a3, a4, a5, a6, a7, a8, a9, a10;
    VASTART(a1);
    VARG2();
    CHECK();
}

void ii_iiiiiiii(W a1, W a2, ...)
{
    W		a3, a4, a5, a6, a7, a8, a9, a10;
    VASTART(a2);
    VARG3();
    CHECK();
}

void iii_iiiiiii(W a1, W a2, W a3, ...)
{
    W		a4, a5, a6, a7, a8, a9, a10;
    VASTART(a3);
    VARG4();
    CHECK();
}

void iiii_iiiiii(W a1, W a2, W a3, W a4, ...)
{
    W		a5, a6, a7, a8, a9, a10;
    VASTART(a4);
    VARG5();
    CHECK();
}

void iiiii_iiiii(W a1, W a2, W a3, W a4, W a5, ...)
{
    W		a6, a7, a8, a9, a10;
    VASTART(a5);
    VARG6();
    CHECK();
}

void iiiiii_iiii(W a1, W a2, W a3, W a4, W a5, W a6, ...)
{
    W		a7, a8, a9, a10;
    VASTART(a6);
    VARG7();
    CHECK();
}

void iiiiiii_iii(W a1, W a2, W a3, W a4, W a5, W a6, W a7, ...)
{
    W		a8, a9, a10;
    VASTART(a7);
    VARG8();
    CHECK();
}

void iiiiiiii_ii(W a1, W a2, W a3, W a4, W a5, W a6, W a7, W a8, ...)
{
    W		a9, a10;
    VASTART(a8);
    VARG9();
    CHECK();
}

void iiiiiiiii_i(W a1, W a2, W a3, W a4, W a5, W a6, W a7, W a8, W a9, ...)
{
    W		a10;
    VASTART(a9);
    VARG10();
    CHECK();
}

void d_ddddddddd(D a1, ...)
{
    D		a2, a3, a4, a5, a6, a7, a8, a9, a10;
    VASTART(a1);
    VARGD2();
    CHECK();
}

void dd_dddddddd(D a1, D a2, ...)
{
    D		a3, a4, a5, a6, a7, a8, a9, a10;
    VASTART(a2);
    VARGD3();
    CHECK();
}

void ddd_ddddddd(D a1, D a2, D a3, ...)
{
    D		a4, a5, a6, a7, a8, a9, a10;
    VASTART(a3);
    VARGD4();
    CHECK();
}

void dddd_dddddd(D a1, D a2, D a3, D a4, ...)
{
    D		a5, a6, a7, a8, a9, a10;
    VASTART(a4);
    VARGD5();
    CHECK();
}

void ddddd_ddddd(D a1, D a2, D a3, D a4, D a5, ...)
{
    D		a6, a7, a8, a9, a10;
    VASTART(a5);
    VARGD6();
    CHECK();
}

void dddddd_dddd(D a1, D a2, D a3, D a4, D a5, D a6, ...)
{
    D		a7, a8, a9, a10;
    VASTART(a6);
    VARGD7();
    CHECK();
}

void ddddddd_ddd(D a1, D a2, D a3, D a4, D a5, D a6, D a7, ...)
{
    D		a8, a9, a10;
    VASTART(a7);
    VARGD8();
    CHECK();
}

void dddddddd_dd(D a1, D a2, D a3, D a4, D a5, D a6, D a7, D a8, ...)
{
    D		a9, a10;
    VASTART(a8);
    VARGD9();
    CHECK();
}

void ddddddddd_d(D a1, D a2, D a3, D a4, D a5, D a6, D a7, D a8, D a9, ...)
{
    D		a10;
    VASTART(a9);
    VARGD10();
    CHECK();
}

void i_didididid(W a1, ...)
{
    W		a3, a5, a7, a9;
    D		a2, a4, a6, a8, a10;
    VASTART(a1);
    IDVARG2();
    CHECK();
}

void id_idididid(W a1, D a2, ...)
{
    W		a3, a5, a7, a9;
    D		a4, a6, a8, a10;
    VASTART(a2);
    IDVARG3();
    CHECK();
}

void idi_dididid(W a1, D a2, W a3, ...)
{
    W		a5, a7, a9;
    D		a4, a6, a8, a10;
    VASTART(a3);
    IDVARG4();
    CHECK();
}

void idid_ididid(W a1, D a2, W a3, D a4, ...)
{
    W		a5, a7, a9;
    D		a6, a8, a10;
    VASTART(a4);
    IDVARG5();
    CHECK();
}

void ididi_didid(W a1, D a2, W a3, D a4, W a5, ...)
{
    W		a7, a9;
    D		a6, a8, a10;
    VASTART(a5);
    IDVARG6();
    CHECK();
}

void ididid_idid(W a1, D a2, W a3, D a4, W a5, D a6, ...)
{
    W		a7, a9;
    D		a8, a10;
    VASTART(a6);
    IDVARG7();
    CHECK();
}

void idididi_did(W a1, D a2, W a3, D a4, W a5, D a6, W a7, ...)
{
    W		a9;
    D		a8, a10;
    VASTART(a7);
    IDVARG8();
    CHECK();
}

void idididid_id(W a1, D a2, W a3, D a4, W a5, D a6, W a7, D a8, ...)
{
    W		a9;
    D		a10;
    VASTART(a8);
    IDVARG9();
    CHECK();
}

void ididididi_d(W a1, D a2, W a3, D a4, W a5, D a6, W a7, D a8, W a9, ...)
{
    D		a10;
    VASTART(a9);
    IDVARG10();
    CHECK();
}

void d_ididididi(D a1, ...)
{
    W		a2, a4, a6, a8, a10;
    D		a3, a5, a7, a9;
    VASTART(a1);
    DIVARG2();
    CHECK();
}

void di_didididi(D a1, W a2, ...)
{
    W		a4, a6, a8, a10;
    D		a3, a5, a7, a9;
    VASTART(a2);
    DIVARG3();
    CHECK();
}

void did_idididi(D a1, W a2, D a3, ...)
{
    W		a4, a6, a8, a10;
    D		a5, a7, a9;
    VASTART(a3);
    DIVARG4();
    CHECK();
}

void didi_dididi(D a1, W a2, D a3, W a4, ...)
{
    W		a6, a8, a10;
    D		a5, a7, a9;
    VASTART(a4);
    DIVARG5();
    CHECK();
}

void didid_ididi(D a1, W a2, D a3, W a4, D a5, ...)
{
    W		a6, a8, a10;
    D		a7, a9;
    VASTART(a5);
    DIVARG6();
    CHECK();
}

void dididi_didi(D a1, W a2, D a3, W a4, D a5, W a6, ...)
{
    W		a8, a10;
    D		a7, a9;
    VASTART(a6);
    DIVARG7();
    CHECK();
}

void dididid_idi(D a1, W a2, D a3, W a4, D a5, W a6, D a7, ...)
{
    W		a8, a10;
    D		a9;
    VASTART(a7);
    DIVARG8();
    CHECK();
}

void didididi_di(D a1, W a2, D a3, W a4, D a5, W a6, D a7, W a8, ...)
{
    W		a10;
    D		a9;
    VASTART(a8);
    DIVARG9();
    CHECK();
}

void didididid_i(D a1, W a2, D a3, W a4, D a5, W a6, D a7, W a8, D a9, ...)
{
    W		a10;
    VASTART(a9);
    DIVARG10();
    CHECK();
}

void va_i_iiiiiiiii(W a1, va_list ap)
{
    W		a2, a3, a4, a5, a6, a7, a8, a9, a10;
    VARG2();
    CHECK();
}

void va_ii_iiiiiiii(W a1, W a2, va_list ap)
{
    W		a3, a4, a5, a6, a7, a8, a9, a10;
    VARG3();
    CHECK();
}

void va_iii_iiiiiii(W a1, W a2, W a3, va_list ap)
{
    W		a4, a5, a6, a7, a8, a9, a10;
    VARG4();
    CHECK();
}

void va_iiii_iiiiii(W a1, W a2, W a3, W a4, va_list ap)
{
    W		a5, a6, a7, a8, a9, a10;
    VARG5();
    CHECK();
}


void va_d_ddddddddd(D a1, va_list ap)
{
    D		a2, a3, a4, a5, a6, a7, a8, a9, a10;
    VARGD2();
    CHECK();
}

void va_dd_dddddddd(D a1, D a2, va_list ap)
{
    D		a3, a4, a5, a6, a7, a8, a9, a10;
    VARGD3();
    CHECK();
}

void va_ddd_ddddddd(D a1, D a2, D a3, va_list ap)
{
    D		a4, a5, a6, a7, a8, a9, a10;
    VARGD4();
    CHECK();
}

void va_dddd_dddddd(D a1, D a2, D a3, D a4, va_list ap)
{
    D		a5, a6, a7, a8, a9, a10;
    VARGD5();
    CHECK();
}

void va_i_didididid(W a1, va_list ap)
{
    W		a3, a5, a7, a9;
    D		a2, a4, a6, a8, a10;
    IDVARG2();
    CHECK();
}

void va_id_idididid(W a1, D a2, va_list ap)
{
    W		a3, a5, a7, a9;
    D		a4, a6, a8, a10;
    IDVARG3();
    CHECK();
}

void va_idi_dididid(W a1, D a2, W a3, va_list ap)
{
    W		a5, a7, a9;
    D		a4, a6, a8, a10;
    IDVARG4();
    CHECK();
}

void va_idid_ididid(W a1, D a2, W a3, D a4, va_list ap)
{
    W		a5, a7, a9;
    D		a6, a8, a10;
    IDVARG5();
    CHECK();
}

void va_d_ididididi(D a1, va_list ap)
{
    W		a2, a4, a6, a8, a10;
    D		a3, a5, a7, a9;
    DIVARG2();
    CHECK();
}

void va_di_didididi(D a1, W a2, va_list ap)
{
    W		a4, a6, a8, a10;
    D		a3, a5, a7, a9;
    DIVARG3();
    CHECK();
}

void va_did_idididi(D a1, W a2, D a3, va_list ap)
{
    W		a4, a6, a8, a10;
    D		a5, a7, a9;
    DIVARG4();
    CHECK();
}

void va_didi_dididi(D a1, W a2, D a3, W a4, va_list ap)
{
    W		a6, a8, a10;
    D		a5, a7, a9;
    DIVARG5();
    CHECK();
}

#define PUSH1()					jit_pushargi(1)
#define PUSH2()		PUSH1();		jit_pushargi(2)
#define PUSH3()		PUSH2();		jit_pushargi(3)
#define PUSH4()		PUSH3();		jit_pushargi(4)
#define PUSH5()		PUSH4();		jit_pushargi(5)
#define PUSH6()		PUSH5();		jit_pushargi(6)
#define PUSH7()		PUSH6();		jit_pushargi(7)
#define PUSH8()		PUSH7();		jit_pushargi(8)
#define PUSH9()		PUSH8();		jit_pushargi(9)
#define VPUSH2()	jit_pushargi(2);	VPUSH3()
#define VPUSH3()	jit_pushargi(3);	VPUSH4()
#define VPUSH4()	jit_pushargi(4);	VPUSH5()
#define VPUSH5()	jit_pushargi(5);	VPUSH6()
#define VPUSH6()	jit_pushargi(6);	VPUSH7()
#define VPUSH7()	jit_pushargi(7);	VPUSH8()
#define VPUSH8()	jit_pushargi(8);	VPUSH9()
#define VPUSH9()	jit_pushargi(9);	VPUSH10()
#define VPUSH10()	jit_pushargi(10);
#define PUSHD1()				jit_pushargi_d(1)
#define PUSHD2()	PUSHD1();		jit_pushargi_d(2)
#define PUSHD3()	PUSHD2();		jit_pushargi_d(3)
#define PUSHD4()	PUSHD3();		jit_pushargi_d(4)
#define PUSHD5()	PUSHD4();		jit_pushargi_d(5)
#define PUSHD6()	PUSHD5();		jit_pushargi_d(6)
#define PUSHD7()	PUSHD6();		jit_pushargi_d(7)
#define PUSHD8()	PUSHD7();		jit_pushargi_d(8)
#define PUSHD9()	PUSHD8();		jit_pushargi_d(9)
#define VPUSHD2()	jit_pushargi_d(2);	VPUSHD3()
#define VPUSHD3()	jit_pushargi_d(3);	VPUSHD4()
#define VPUSHD4()	jit_pushargi_d(4);	VPUSHD5()
#define VPUSHD5()	jit_pushargi_d(5);	VPUSHD6()
#define VPUSHD6()	jit_pushargi_d(6);	VPUSHD7()
#define VPUSHD7()	jit_pushargi_d(7);	VPUSHD8()
#define VPUSHD8()	jit_pushargi_d(8);	VPUSHD9()
#define VPUSHD9()	jit_pushargi_d(9);	VPUSHD10()
#define VPUSHD10()	jit_pushargi_d(10);
#define IDPUSH1()				jit_pushargi(1)
#define IDPUSH2()	IDPUSH1();		jit_pushargi_d(2)
#define IDPUSH3()	IDPUSH2();		jit_pushargi(3)
#define IDPUSH4()	IDPUSH3();		jit_pushargi_d(4)
#define IDPUSH5()	IDPUSH4();		jit_pushargi(5)
#define IDPUSH6()	IDPUSH5();		jit_pushargi_d(6)
#define IDPUSH7()	IDPUSH6();		jit_pushargi(7)
#define IDPUSH8()	IDPUSH7();		jit_pushargi_d(8)
#define IDPUSH9()	IDPUSH8();		jit_pushargi(9)
#define IDVPUSH2()	jit_pushargi_d(2);	IDVPUSH3()
#define IDVPUSH3()	jit_pushargi(3);	IDVPUSH4()
#define IDVPUSH4()	jit_pushargi_d(4);	IDVPUSH5()
#define IDVPUSH5()	jit_pushargi(5);	IDVPUSH6()
#define IDVPUSH6()	jit_pushargi_d(6);	IDVPUSH7()
#define IDVPUSH7()	jit_pushargi(7);	IDVPUSH8()
#define IDVPUSH8()	jit_pushargi_d(8);	IDVPUSH9()
#define IDVPUSH9()	jit_pushargi(9);	IDVPUSH10()
#define IDVPUSH10()	jit_pushargi_d(10);
#define DIPUSH1()				jit_pushargi_d(1)
#define DIPUSH2()	DIPUSH1();		jit_pushargi(2)
#define DIPUSH3()	DIPUSH2();		jit_pushargi_d(3)
#define DIPUSH4()	DIPUSH3();		jit_pushargi(4)
#define DIPUSH5()	DIPUSH4();		jit_pushargi_d(5)
#define DIPUSH6()	DIPUSH5();		jit_pushargi(6)
#define DIPUSH7()	DIPUSH6();		jit_pushargi_d(7)
#define DIPUSH8()	DIPUSH7();		jit_pushargi(8)
#define DIPUSH9()	DIPUSH8();		jit_pushargi_d(9)
#define DIVPUSH2()	jit_pushargi(2);	DIVPUSH3()
#define DIVPUSH3()	jit_pushargi_d(3);	DIVPUSH4()
#define DIVPUSH4()	jit_pushargi(4);	DIVPUSH5()
#define DIVPUSH5()	jit_pushargi_d(5);	DIVPUSH6()
#define DIVPUSH6()	jit_pushargi(6);	DIVPUSH7()
#define DIVPUSH7()	jit_pushargi_d(7);	DIVPUSH8()
#define DIVPUSH8()	jit_pushargi(8);	DIVPUSH9()
#define DIVPUSH9()	jit_pushargi_d(9);	DIVPUSH10()
#define DIVPUSH10()	jit_pushargi(10);

jit_state_t	 *_jit;

int main(int argc, char *argv[])
{
    void		(*function)(void);
    jit_node_t		*jmpi_main;
    jit_node_t		*a1, *a2, *node;
    jit_node_t		*jva_i_iiiiiiiii, *jva_ii_iiiiiiii;
    jit_node_t		*jva_d_ddddddddd, *jva_dd_dddddddd;
    jit_node_t		*jva_i_didididid, *jva_id_idididid;
    jit_node_t		*jva_d_ididididi, *jva_di_didididi;
    jit_node_t		*jva_iii_iiiiiii, *jva_iiii_iiiiii;
    jit_node_t		*jva_ddd_ddddddd, *jva_dddd_dddddd;
    jit_node_t		*jva_idi_dididid, *jva_idid_ididid;
    jit_node_t		*jva_did_idididi, *jva_didi_dididi;

    init_jit(argv[0]);
    _jit = jit_new_state();

    jmpi_main = jit_jmpi();

    /* Define simple functions to validate a jit_va_list_t
     * is a valid va_list; these do not fetch arguments from
     * the va_list. */
    jva_i_iiiiiiiii = jit_label();
    jit_name("va_i_iiiiiiiii");
    jit_prolog();
    a1 = jit_arg();
    jit_getarg(JIT_V1, a1);
    jit_ellipsis();
    jit_va_start(JIT_V0);
    jit_prepare();
    jit_pushargr(JIT_V1);
    jit_va_push(JIT_V0);
    jit_finishi(va_i_iiiiiiiii);
    jit_va_end(JIT_V0);
    jit_ret();
    jit_epilog();
    jva_ii_iiiiiiii = jit_label();
    jit_name("va_ii_iiiiiiii");
    jit_prolog();
    a1 = jit_arg();
    a2 = jit_arg();
    jit_getarg(JIT_V1, a1);
    jit_getarg(JIT_V2, a2);
    jit_ellipsis();
    jit_va_start(JIT_V0);
    jit_prepare();
    jit_pushargr(JIT_V1);
    jit_pushargr(JIT_V2);
    jit_va_push(JIT_V0);
    jit_finishi(va_ii_iiiiiiii);
    jit_va_end(JIT_V0);
    jit_ret();
    jit_epilog();
    jva_d_ddddddddd = jit_label();
    jit_name("va_d_ddddddddd");
    jit_prolog();
    a1 = jit_arg_d();
    jit_getarg_d(JIT_F3, a1);
    jit_ellipsis();
    jit_va_start(JIT_V0);
    jit_prepare();
    jit_pushargr_d(JIT_F3);
    jit_va_push(JIT_V0);
    jit_finishi(va_d_ddddddddd);
    jit_va_end(JIT_V0);
    jit_ret();
    jit_epilog();
    jva_dd_dddddddd = jit_label();
    jit_name("va_dd_dddddddd");
    jit_prolog();
    a1 = jit_arg_d();
    a2 = jit_arg_d();
    jit_getarg_d(JIT_F3, a1);
    jit_getarg_d(JIT_F4, a2);
    jit_ellipsis();
    jit_va_start(JIT_V0);
    jit_prepare();
    jit_pushargr_d(JIT_F3);
    jit_pushargr_d(JIT_F4);
    jit_va_push(JIT_V0);
    jit_finishi(va_dd_dddddddd);
    jit_va_end(JIT_V0);
    jit_ret();
    jit_epilog();
    jva_i_didididid = jit_label();
    jit_name("va_i_didididid");
    jit_prolog();
    a1 = jit_arg();
    jit_getarg(JIT_V1, a1);
    jit_ellipsis();
    jit_va_start(JIT_V0);
    jit_prepare();
    jit_pushargr(JIT_V1);
    jit_va_push(JIT_V0);
    jit_finishi(va_i_didididid);
    jit_va_end(JIT_V0);
    jit_ret();
    jit_epilog();
    jva_id_idididid = jit_label();
    jit_name("va_id_idididid");
    jit_prolog();
    a1 = jit_arg();
    a2 = jit_arg_d();
    jit_getarg(JIT_V1, a1);
    jit_getarg_d(JIT_F3, a2);
    jit_ellipsis();
    jit_va_start(JIT_V0);
    jit_prepare();
    jit_pushargr(JIT_V1);
    jit_pushargr_d(JIT_F3);
    jit_va_push(JIT_V0);
    jit_finishi(va_id_idididid);
    jit_va_end(JIT_V0);
    jit_ret();
    jit_epilog();
    jva_d_ididididi = jit_label();
    jit_name("va_d_ididididi");
    jit_prolog();
    a1 = jit_arg_d();
    jit_getarg_d(JIT_F3, a1);
    jit_ellipsis();
    jit_va_start(JIT_V0);
    jit_prepare();
    jit_pushargr_d(JIT_F3);
    jit_va_push(JIT_V0);
    jit_finishi(va_d_ididididi);
    jit_va_end(JIT_V0);
    jit_ret();
    jit_epilog();
    jva_di_didididi = jit_label();
    jit_name("va_di_didididi");
    jit_prolog();
    a1 = jit_arg_d();
    a2 = jit_arg();
    jit_getarg_d(JIT_F3, a1);
    jit_getarg(JIT_V1, a2);
    jit_ellipsis();
    jit_va_start(JIT_V0);
    jit_prepare();
    jit_pushargr_d(JIT_F3);
    jit_pushargr(JIT_V1);
    jit_va_push(JIT_V0);
    jit_finishi(va_di_didididi);
    jit_va_end(JIT_V0);
    jit_ret();
    jit_epilog();

    /* Define complex functions to validate a jit_va_list_t
     * is a valid va_list; these do fetch arguments from
     * the va_list, to ensure it does the correct operations
     * fetching arguments, and pass a valid va_list to the
     * C function. */
    jva_iii_iiiiiii = jit_label();
    jit_name("va_iii_iiiiiii");
    jit_prolog();
    a1 = jit_arg();
    jit_getarg(JIT_V1, a1);
    node = jit_beqi(JIT_V1, 1);
    jit_calli(abort);
    jit_patch(node);
    jit_ellipsis();
    jit_va_start(JIT_V0);
    jit_va_arg(JIT_V1, JIT_V0);
    jit_va_arg(JIT_V2, JIT_V0);
    jit_prepare();
    jit_pushargi(1);
    jit_pushargr(JIT_V1);
    jit_pushargr(JIT_V2);
    jit_va_push(JIT_V0);
    jit_finishi(va_iii_iiiiiii);
    jit_va_end(JIT_V0);
    jit_ret();
    jit_epilog();
    jva_iiii_iiiiii = jit_label();
    jit_name("va_iiii_iiiiii");
    jit_prolog();
    a1 = jit_arg();
    a2 = jit_arg();
    jit_getarg(JIT_V1, a1);
    node = jit_beqi(JIT_V1, 1);
    jit_calli(abort);
    jit_patch(node);
    jit_getarg(JIT_V1, a2);
    node = jit_beqi(JIT_V1, 2);
    jit_calli(abort);
    jit_patch(node);
    jit_ellipsis();
    jit_va_start(JIT_V0);
    jit_va_arg(JIT_V1, JIT_V0);
    jit_va_arg(JIT_V2, JIT_V0);
    jit_prepare();
    jit_pushargi(1);
    jit_pushargi(2);
    jit_pushargr(JIT_V1);
    jit_pushargr(JIT_V2);
    jit_va_push(JIT_V0);
    jit_finishi(va_iiii_iiiiii);
    jit_va_end(JIT_V0);
    jit_ret();
    jit_epilog();
    jva_ddd_ddddddd = jit_label();
    jit_name("va_ddd_ddddddd");
    jit_prolog();
    a1 = jit_arg_d();
    jit_getarg_d(JIT_F3, a1);
    node = jit_beqi_d(JIT_F3, 1);
    jit_calli(abort);
    jit_patch(node);
    jit_ellipsis();
    jit_va_start(JIT_V0);
    jit_va_arg_d(JIT_F3, JIT_V0);
    jit_va_arg_d(JIT_F4, JIT_V0);
    jit_prepare();
    jit_pushargi_d(1);
    jit_pushargr_d(JIT_F3);
    jit_pushargr_d(JIT_F4);
    jit_va_push(JIT_V0);
    jit_finishi(va_ddd_ddddddd);
    jit_va_end(JIT_V0);
    jit_ret();
    jit_epilog();
    jva_dddd_dddddd = jit_label();
    jit_name("va_dddd_dddddd");
    jit_prolog();
    a1 = jit_arg_d();
    a2 = jit_arg_d();
    jit_getarg_d(JIT_F3, a1);
    node = jit_beqi_d(JIT_F3, 1);
    jit_calli(abort);
    jit_patch(node);
    jit_getarg_d(JIT_F3, a2);
    node = jit_beqi_d(JIT_F3, 2);
    jit_calli(abort);
    jit_patch(node);
    jit_ellipsis();
    jit_va_start(JIT_V0);
    jit_va_arg_d(JIT_F3, JIT_V0);
    jit_va_arg_d(JIT_F4, JIT_V0);
    jit_prepare();
    jit_pushargi_d(1);
    jit_pushargi_d(2);
    jit_pushargr_d(JIT_F3);
    jit_pushargr_d(JIT_F4);
    jit_va_push(JIT_V0);
    jit_finishi(va_dddd_dddddd);
    jit_va_end(JIT_V0);
    jit_ret();
    jit_epilog();
    jva_idi_dididid = jit_label();
    jit_name("va_idi_dididid");
    jit_prolog();
    a1 = jit_arg();
    jit_getarg(JIT_V1, a1);
    node = jit_beqi(JIT_V1, 1);
    jit_calli(abort);
    jit_patch(node);
    jit_ellipsis();
    jit_va_start(JIT_V0);
    jit_va_arg_d(JIT_F3, JIT_V0);
    jit_va_arg(JIT_V1, JIT_V0);
    jit_prepare();
    jit_pushargi(1);
    jit_pushargr_d(JIT_F3);
    jit_pushargr(JIT_V1);
    jit_va_push(JIT_V0);
    jit_finishi(va_idi_dididid);
    jit_va_end(JIT_V0);
    jit_ret();
    jit_epilog();
    jva_idid_ididid = jit_label();
    jit_name("va_idid_ididid");
    jit_prolog();
    a1 = jit_arg();
    a2 = jit_arg_d();
    jit_getarg(JIT_V1, a1);
    node = jit_beqi(JIT_V1, 1);
    jit_calli(abort);
    jit_patch(node);
    jit_getarg_d(JIT_F3, a2);
    node = jit_beqi_d(JIT_F3, 2);
    jit_calli(abort);
    jit_patch(node);
    jit_ellipsis();
    jit_va_start(JIT_V0);
    jit_va_arg(JIT_V1, JIT_V0);
    jit_va_arg_d(JIT_F3, JIT_V0);
    jit_prepare();
    jit_pushargi(1);
    jit_pushargi_d(2);
    jit_pushargr(JIT_V1);
    jit_pushargr_d(JIT_F3);
    jit_va_push(JIT_V0);
    jit_finishi(va_idid_ididid);
    jit_va_end(JIT_V0);
    jit_ret();
    jit_epilog();
    jva_did_idididi = jit_label();
    jit_name("va_did_idididi");
    jit_prolog();
    a1 = jit_arg_d();
    jit_getarg_d(JIT_F3, a1);
    node = jit_beqi_d(JIT_F3, 1);
    jit_calli(abort);
    jit_patch(node);
    jit_ellipsis();
    jit_va_start(JIT_V0);
    jit_va_arg(JIT_V1, JIT_V0);
    jit_va_arg_d(JIT_F3, JIT_V0);
    jit_prepare();
    jit_pushargi_d(1);
    jit_pushargr(JIT_V1);
    jit_pushargr_d(JIT_F3);
    jit_va_push(JIT_V0);
    jit_finishi(va_did_idididi);
    jit_va_end(JIT_V0);
    jit_ret();
    jit_epilog();
    jva_didi_dididi = jit_label();
    jit_name("va_didi_dididi");
    jit_prolog();
    a1 = jit_arg_d();
    a2 = jit_arg();
    jit_getarg_d(JIT_F3, a1);
    node = jit_beqi_d(JIT_F3, 1);
    jit_calli(abort);
    jit_patch(node);
    jit_getarg(JIT_V1, a2);
    node = jit_beqi(JIT_V1, 2);
    jit_calli(abort);
    jit_patch(node);
    jit_ellipsis();
    jit_va_start(JIT_V0);
    jit_va_arg_d(JIT_F3, JIT_V0);
    jit_va_arg(JIT_V1, JIT_V0);
    jit_prepare();
    jit_pushargi_d(1);
    jit_pushargi(2);
    jit_pushargr_d(JIT_F3);
    jit_pushargr(JIT_V1);
    jit_va_push(JIT_V0);
    jit_finishi(va_didi_dididi);
    jit_va_end(JIT_V0);
    jit_ret();
    jit_epilog();

    jit_patch(jmpi_main);
    jit_name("main");
    jit_prolog();

    /* Check that lightning properly calls vararg functions */
    jit_prepare();
    PUSH1();
    jit_ellipsis();
    VPUSH2();
    jit_finishi(i_iiiiiiiii);
    jit_prepare();
    PUSH2();
    jit_ellipsis();
    VPUSH3();
    jit_finishi(ii_iiiiiiii);
    jit_prepare();
    PUSH3();
    jit_ellipsis();
    VPUSH4();
    jit_finishi(iii_iiiiiii);
    jit_prepare();
    PUSH4();
    jit_ellipsis();
    VPUSH5();
    jit_finishi(iiii_iiiiii);
    jit_prepare();
    PUSH5();
    jit_ellipsis();
    VPUSH6();
    jit_finishi(iiiii_iiiii);
    jit_prepare();
    PUSH6();
    jit_ellipsis();
    VPUSH7();
    jit_finishi(iiiiii_iiii);
    jit_prepare();
    PUSH7();
    jit_ellipsis();
    VPUSH8();
    jit_finishi(iiiiiii_iii);
    jit_prepare();
    PUSH8();
    jit_ellipsis();
    VPUSH9();
    jit_finishi(iiiiiiii_ii);
    jit_prepare();
    PUSH9();
    jit_ellipsis();
    VPUSH10();
    jit_finishi(iiiiiiiii_i);
    jit_prepare();
    PUSHD1();
    jit_ellipsis();
    VPUSHD2();
    jit_finishi(d_ddddddddd);
    jit_prepare();
    PUSHD2();
    jit_ellipsis();
    VPUSHD3();
    jit_finishi(dd_dddddddd);
    jit_prepare();
    PUSHD3();
    jit_ellipsis();
    VPUSHD4();
    jit_finishi(ddd_ddddddd);
    jit_prepare();
    PUSHD4();
    jit_ellipsis();
    VPUSHD5();
    jit_finishi(dddd_dddddd);
    jit_prepare();
    PUSHD5();
    jit_ellipsis();
    VPUSHD6();
    jit_finishi(ddddd_ddddd);
    jit_prepare();
    PUSHD6();
    jit_ellipsis();
    VPUSHD7();
    jit_finishi(dddddd_dddd);
    jit_prepare();
    PUSHD7();
    jit_ellipsis();
    VPUSHD8();
    jit_finishi(ddddddd_ddd);
    jit_prepare();
    PUSHD8();
    jit_ellipsis();
    VPUSHD9();
    jit_finishi(dddddddd_dd);
    jit_prepare();
    PUSHD9();
    jit_ellipsis();
    VPUSHD10();
    jit_finishi(ddddddddd_d);
    jit_prepare();
    IDPUSH1();
    jit_ellipsis();
    IDVPUSH2();
    jit_finishi(i_didididid);
    jit_prepare();
    IDPUSH2();
    jit_ellipsis();
    IDVPUSH3();
    jit_finishi(id_idididid);
    jit_prepare();
    IDPUSH3();
    jit_ellipsis();
    IDVPUSH4();
    jit_finishi(idi_dididid);
    jit_prepare();
    IDPUSH4();
    jit_ellipsis();
    IDVPUSH5();
    jit_finishi(idid_ididid);
    jit_prepare();
    IDPUSH5();
    jit_ellipsis();
    IDVPUSH6();
    jit_finishi(ididi_didid);
    jit_prepare();
    IDPUSH6();
    jit_ellipsis();
    IDVPUSH7();
    jit_finishi(ididid_idid);
    jit_prepare();
    IDPUSH7();
    jit_ellipsis();
    IDVPUSH8();
    jit_finishi(idididi_did);
    jit_prepare();
    IDPUSH8();
    jit_ellipsis();
    IDVPUSH9();
    jit_finishi(idididid_id);
    jit_prepare();
    IDPUSH9();
    jit_ellipsis();
    IDVPUSH10();
    jit_finishi(ididididi_d);
    jit_prepare();
    DIPUSH1();
    jit_ellipsis();
    DIVPUSH2();
    jit_finishi(d_ididididi);
    jit_prepare();
    DIPUSH2();
    jit_ellipsis();
    DIVPUSH3();
    jit_finishi(di_didididi);
    jit_prepare();
    DIPUSH3();
    jit_ellipsis();
    DIVPUSH4();
    jit_finishi(did_idididi);
    jit_prepare();
    DIPUSH4();
    jit_ellipsis();
    DIVPUSH5();
    jit_finishi(didi_dididi);
    jit_prepare();
    DIPUSH5();
    jit_ellipsis();
    DIVPUSH6();
    jit_finishi(didid_ididi);
    jit_prepare();
    DIPUSH6();
    jit_ellipsis();
    DIVPUSH7();
    jit_finishi(dididi_didi);
    jit_prepare();
    DIPUSH7();
    jit_ellipsis();
    DIVPUSH8();
    jit_finishi(dididid_idi);
    jit_prepare();
    DIPUSH8();
    jit_ellipsis();
    DIVPUSH9();
    jit_finishi(didididi_di);
    jit_prepare();
    DIPUSH9();
    jit_ellipsis();
    DIVPUSH10();
    jit_finishi(didididid_i);

    /* Check that unmodified jit_va_list_t is a valid va_list */
    jit_prepare();
    PUSH1();
    jit_ellipsis();
    VPUSH2();
    jit_patch_at(jit_finishi(NULL), jva_i_iiiiiiiii);
    jit_prepare();
    PUSH2();
    jit_ellipsis();
    VPUSH3();
    jit_patch_at(jit_finishi(NULL), jva_ii_iiiiiiii);
    jit_prepare();
    PUSHD1();
    jit_ellipsis();
    VPUSHD2();
    jit_patch_at(jit_finishi(NULL), jva_d_ddddddddd);
    jit_prepare();
    PUSHD2();
    jit_ellipsis();
    VPUSHD3();
    jit_patch_at(jit_finishi(NULL), jva_dd_dddddddd);
    jit_prepare();
    IDPUSH1();
    jit_ellipsis();
    IDVPUSH2();
    jit_patch_at(jit_finishi(NULL), jva_i_didididid);
    jit_prepare();
    IDPUSH2();
    jit_ellipsis();
    IDVPUSH3();
    jit_patch_at(jit_finishi(NULL), jva_id_idididid);
    jit_prepare();
    DIPUSH1();
    jit_ellipsis();
    DIVPUSH2();
    jit_patch_at(jit_finishi(NULL), jva_d_ididididi);
    jit_prepare();
    DIPUSH2();
    jit_ellipsis();
    DIVPUSH3();
    jit_patch_at(jit_finishi(NULL), jva_di_didididi);

    /* Check that modified jit_va_list_t is a valid va_list */
    jit_prepare();
    PUSH1();
    jit_ellipsis();
    VPUSH2();
    jit_patch_at(jit_finishi(NULL), jva_iii_iiiiiii);
    jit_prepare();
    PUSH2();
    jit_ellipsis();
    VPUSH3();
    jit_patch_at(jit_finishi(NULL), jva_iiii_iiiiii);
    jit_prepare();
    PUSHD1();
    jit_ellipsis();
    VPUSHD2();
    jit_patch_at(jit_finishi(NULL), jva_ddd_ddddddd);
    jit_prepare();
    PUSHD2();
    jit_ellipsis();
    VPUSHD3();
    jit_patch_at(jit_finishi(NULL), jva_dddd_dddddd);
    jit_prepare();
    IDPUSH1();
    jit_ellipsis();
    IDVPUSH2();
    jit_patch_at(jit_finishi(NULL), jva_idi_dididid);
    jit_prepare();
    IDPUSH2();
    jit_ellipsis();
    IDVPUSH3();
    jit_patch_at(jit_finishi(NULL), jva_idid_ididid);
    jit_prepare();
    DIPUSH1();
    jit_ellipsis();
    DIVPUSH2();
    jit_patch_at(jit_finishi(NULL), jva_did_idididi);
    jit_prepare();
    DIPUSH2();
    jit_ellipsis();
    DIVPUSH3();
    jit_patch_at(jit_finishi(NULL), jva_didi_dididi);

    jit_ret();
    jit_epilog();


    function = jit_emit();
    jit_clear_state();
    //jit_disassemble();
    (*function)();
    jit_destroy_state();

    finish_jit();

    printf("ok\n");
    return 0;
}
