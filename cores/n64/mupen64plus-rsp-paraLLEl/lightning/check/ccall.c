#include <lightning.h>
#include <stdio.h>

#define _w0			0
#define _w1			1
#define _w2			(_w1-2)
#define _w3			(_w2-3)
#define _w4			(_w3-4)
#define _w5			(_w4-5)
#define _w6			(_w5-6)
#define _w7			(_w6-7)
#define _w8			(_w7-8)
#define _w9			(_w8-9)
#define _w10			(_w9-10)
#define _w11			(_w10-11)
#define _w12			(_w11-12)
#define _w13			(_w12-13)
#define _w14			(_w13-14)
#define _w15			(_w14-15)
#define _c0			_w0
#define _c1			_w1
#define _c2			_w2
#define _c3			_w3
#define _c4			_w4
#define _c5			_w5
#define _c6			_w6
#define _c7			_w7
#define _c8			_w8
#define _c9			_w9
#define _c10			_w10
#define _c11			_w11
#define _c12			_w12
#define _c13			_w13
#define _c14			_w14
#define _c15			_w15
#define _uc0			(_w0&0xff)
#define _uc1			(_w1&0xff)
#define _uc2			(_w2&0xff)
#define _uc3			(_w3&0xff)
#define _uc4			(_w4&0xff)
#define _uc5			(_w5&0xff)
#define _uc6			(_w6&0xff)
#define _uc7			(_w7&0xff)
#define _uc8			(_w8&0xff)
#define _uc9			(_w9&0xff)
#define _uc10			(_w10&0xff)
#define _uc11			(_w11&0xff)
#define _uc12			(_w12&0xff)
#define _uc13			(_w13&0xff)
#define _uc14			(_w14&0xff)
#define _uc15			(_w15&0xff)
#define _s0			_w0
#define _s1			_w1
#define _s2			_w2
#define _s3			_w3
#define _s4			_w4
#define _s5			_w5
#define _s6			_w6
#define _s7			_w7
#define _s8			_w8
#define _s9			_w9
#define _s10			_w10
#define _s11			_w11
#define _s12			_w12
#define _s13			_w13
#define _s14			_w14
#define _s15			_w15
#define _us0			(_w0&0xffff)
#define _us1			(_w1&0xffff)
#define _us2			(_w2&0xffff)
#define _us3			(_w3&0xffff)
#define _us4			(_w4&0xffff)
#define _us5			(_w5&0xffff)
#define _us6			(_w6&0xffff)
#define _us7			(_w7&0xffff)
#define _us8			(_w8&0xffff)
#define _us9			(_w9&0xffff)
#define _us10			(_w10&0xffff)
#define _us11			(_w11&0xffff)
#define _us12			(_w12&0xffff)
#define _us13			(_w13&0xffff)
#define _us14			(_w14&0xffff)
#define _us15			(_w15&0xffff)
#define _i0			_w0
#define _i1			_w1
#define _i2			_w2
#define _i3			_w3
#define _i4			_w4
#define _i5			_w5
#define _i6			_w6
#define _i7			_w7
#define _i8			_w8
#define _i9			_w9
#define _i10			_w10
#define _i11			_w11
#define _i12			_w12
#define _i13			_w13
#define _i14			_w14
#define _i15			_w15
#if __WORDSIZE == 64
#  define _ui0			(_w0&0xffffffff)
#  define _ui1			(_w1&0xffffffff)
#  define _ui2			(_w2&0xffffffff)
#  define _ui3			(_w3&0xffffffff)
#  define _ui4			(_w4&0xffffffff)
#  define _ui5			(_w5&0xffffffff)
#  define _ui6			(_w6&0xffffffff)
#  define _ui7			(_w7&0xffffffff)
#  define _ui8			(_w8&0xffffffff)
#  define _ui9			(_w9&0xffffffff)
#  define _ui10			(_w10&0xffffffff)
#  define _ui11			(_w11&0xffffffff)
#  define _ui12			(_w12&0xffffffff)
#  define _ui13			(_w13&0xffffffff)
#  define _ui14			(_w14&0xffffffff)
#  define _ui15			(_w15&0xffffffff)
#  define _l0			_w0
#  define _l1			_w1
#  define _l2			_w2
#  define _l3			_w3
#  define _l4			_w4
#  define _l5			_w5
#  define _l6			_w6
#  define _l7			_w7
#  define _l8			_w8
#  define _l9			_w9
#  define _l10			_w10
#  define _l11			_w11
#  define _l12			_w12
#  define _l13			_w13
#  define _l14			_w14
#  define _l15			_w15
#endif

/*
 * Types
 */
typedef signed char		_c;
typedef unsigned char		_uc;
typedef signed short		_s;
typedef unsigned short		_us;
typedef signed int		_i;
#if __WORDSIZE == 64
typedef unsigned int		_ui;
typedef jit_word_t		_l;
#endif
typedef float			_f;
typedef double			_d;

#define prt0(T)			T C##T##0(void);
#define prt1(T)			prt0(T)					\
				T C##T##1(T);
#define prt2(T)			prt1(T)					\
				T C##T##2(T,T);
#define prt3(T)			prt2(T)					\
				T C##T##3(T,T,T);
#define prt4(T)			prt3(T)					\
				T C##T##4(T,T,T,T);
#define prt5(T)			prt4(T)					\
				T C##T##5(T,T,T,T,T);
#define prt6(T)			prt5(T)					\
				T C##T##6(T,T,T,T,T,T);
#define prt7(T)			prt6(T)					\
				T C##T##7(T,T,T,T,T,T,T);
#define prt8(T)			prt7(T)					\
				T C##T##8(T,T,T,T,T,T,T,T);
#define prt9(T)			prt8(T)					\
				T C##T##9(T,T,T,T,T,T,T,T,T);
#define prt10(T)		prt9(T)					\
				T C##T##10(T,T,T,T,T,T,T,T,T,T);
#define prt11(T)		prt10(T)				\
				T C##T##11(T,T,T,T,T,T,T,T,T,T,T);
#define prt12(T)		prt11(T)				\
				T C##T##12(T,T,T,T,T,T,T,T,T,T,T,T);
#define prt13(T)		prt12(T)				\
				T C##T##13(T,T,T,T,T,T,T,T,T,T,T,T,T);
#define prt14(T)		prt13(T)				\
				T C##T##14(T,T,T,T,T,T,T,T,T,T,T,T,T,T);
#define prt15(T)		prt14(T)				\
				T C##T##15(T,T,T,T,T,T,T,T,T,T,T,T,T,T,T);
#define prt(T)			prt15(T)
prt(_c)
prt(_uc)
prt(_s)
prt(_us)
prt(_i)
#if __WORDSIZE == 64
prt(_ui)
prt(_l)
#endif
prt(_f)
prt(_d)
#undef prt
#undef prt15
#undef prt14
#undef prt13
#undef prt12
#undef prt11
#undef prt10
#undef prt9
#undef prt8
#undef prt7
#undef prt6
#undef prt5
#undef prt4
#undef prt3
#undef prt2
#undef prt1
#undef prt0

#define prtn(N,T)		T J##T##n(void);
#define prt0(T)			prtn(0,T)
#define prt1(T)			prt0(T)			prtn(1,T)
#define prt2(T)			prt1(T)			prtn(2,T)
#define prt3(T)			prt2(T)			prtn(3,T)
#define prt4(T)			prt3(T)			prtn(4,T)
#define prt5(T)			prt4(T)			prtn(5,T)
#define prt6(T)			prt5(T)			prtn(6,T)
#define prt7(T)			prt6(T)			prtn(7,T)
#define prt8(T)			prt7(T)			prtn(8,T)
#define prt9(T)			prt8(T)			prtn(9,T)
#define prt10(T)		prt9(T)			prtn(10,T)
#define prt11(T)		prt10(T)		prtn(11,T)
#define prt12(T)		prt11(T)		prtn(12,T)
#define prt13(T)		prt12(T)		prtn(13,T)
#define prt14(T)		prt13(T)		prtn(14,T)
#define prt15(T)		prt14(T)		prtn(15,T)
#define prt(T)			prt15(T)
prt(_c)
prt(_uc)
prt(_s)
prt(_us)
prt(_i)
#if __WORDSIZE == 64
prt(_ui)
prt(_l)
#endif
prt(_f)
prt(_d)
#undef prt
#undef prt15
#undef prt14
#undef prt13
#undef prt12
#undef prt11
#undef prt10
#undef prt9
#undef prt8
#undef prt7
#undef prt6
#undef prt5
#undef prt4
#undef prt3
#undef prt2
#undef prt1
#undef prt0
#undef prtn

/*
 * Initialization
 */

#define dat0(T)			T (*j##T##0)(void);			\
				jit_node_t *n##T##0;
#define dat1(T)			dat0(T)					\
				T (*j##T##1)(T);			\
				jit_node_t *n##T##1;
#define dat2(T)			dat1(T)					\
				T (*j##T##2)(T,T);			\
				jit_node_t *n##T##2;
#define dat3(T)			dat2(T)					\
				T (*j##T##3)(T,T,T);			\
				jit_node_t *n##T##3;
#define dat4(T)			dat3(T)					\
				T (*j##T##4)(T,T,T,T);			\
				jit_node_t *n##T##4;
#define dat5(T)			dat4(T)					\
				T (*j##T##5)(T,T,T,T,T);		\
				jit_node_t *n##T##5;
#define dat6(T)			dat5(T)					\
				T (*j##T##6)(T,T,T,T,T,T);		\
				jit_node_t *n##T##6;
#define dat7(T)			dat6(T)					\
				T (*j##T##7)(T,T,T,T,T,T,T);		\
				jit_node_t *n##T##7;
#define dat8(T)			dat7(T)					\
				T (*j##T##8)(T,T,T,T,T,T,T,T);		\
				jit_node_t *n##T##8;
#define dat9(T)			dat8(T)					\
				T (*j##T##9)(T,T,T,T,T,T,T,T,T);	\
				jit_node_t *n##T##9;
#define dat10(T)		dat9(T)					\
				T (*j##T##10)(T,T,T,T,T,T,T,T,T,T);	\
				jit_node_t *n##T##10;
#define dat11(T)		dat10(T)				\
				T (*j##T##11)(T,T,T,T,T,T,T,T,T,T,T);	\
				jit_node_t *n##T##11;
#define dat12(T)		dat11(T)				\
				T (*j##T##12)(T,T,T,T,T,T,T,T,T,T,T,T);	\
				jit_node_t *n##T##12;
#define dat13(T)		dat12(T)				\
				T (*j##T##13)(T,T,T,T,T,T,T,T,T,T,T,T,T);\
				jit_node_t *n##T##13;
#define dat14(T)		dat13(T)				\
				T (*j##T##14)(T,T,T,T,T,T,T,T,T,T,T,T,T,T);\
				jit_node_t *n##T##14;
#define dat15(T)		dat14(T)				\
				T (*j##T##15)(T,T,T,T,T,T,T,T,T,T,T,T,T,T,T);\
				jit_node_t *n##T##15;
#define dat(T)			dat15(T)
dat(_c)
dat(_uc)
dat(_s)
dat(_us)
dat(_i)
#if __WORDSIZE == 64
dat(_ui)
dat(_l)
#endif
dat(_f)
dat(_d)
#undef dat
#undef dat15
#undef dat14
#undef dat13
#undef dat12
#undef dat11
#undef dat10
#undef dat9
#undef dat8
#undef dat7
#undef dat6
#undef dat5
#undef dat4
#undef dat3
#undef dat2
#undef dat1
#undef dat0

/*
 * Implementation
 */
#define dcl0(T)								\
T C##T##0(void)								\
{									\
    return (0);								\
}
#define dcl1(T)								\
dcl0(T)									\
T C##T##1(T A)								\
{									\
    return (A);								\
}
#define dcl2(T)								\
dcl1(T)									\
T C##T##2(T A,T B)							\
{									\
    return (A-B);							\
}
#define dcl3(T)								\
dcl2(T)									\
T C##T##3(T A,T B,T C)							\
{									\
    return (A-B-C);							\
}
#define dcl4(T)								\
dcl3(T)									\
T C##T##4(T A,T B,T C,T D)						\
{									\
    return (A-B-C-D);							\
}
#define dcl5(T)								\
dcl4(T)									\
T C##T##5(T A,T B,T C,T D,T E)						\
{									\
    return (A-B-C-D-E);							\
}
#define dcl6(T)								\
dcl5(T)									\
T C##T##6(T A,T B,T C,T D,T E,T F)					\
{									\
    return (A-B-C-D-E-F);						\
}
#define dcl7(T)								\
dcl6(T)									\
T C##T##7(T A,T B,T C,T D,T E,T F,T G)					\
{									\
    return (A-B-C-D-E-F-G);						\
}
#define dcl8(T)								\
dcl7(T)									\
T C##T##8(T A,T B,T C,T D,T E,T F,T G,T H)				\
{									\
    return (A-B-C-D-E-F-G-H);						\
}
#define dcl9(T)								\
dcl8(T)									\
T C##T##9(T A,T B,T C,T D,T E,T F,T G,T H,T I)				\
{									\
    return (A-B-C-D-E-F-G-H-I);						\
}
#define dcl10(T)							\
dcl9(T)									\
T C##T##10(T A,T B,T C,T D,T E,T F,T G,T H,T I,T J)			\
{									\
    return (A-B-C-D-E-F-G-H-I-J);					\
}
#define dcl11(T)							\
dcl10(T)								\
T C##T##11(T A,T B,T C,T D,T E,T F,T G,T H,T I,T J,T K)			\
{									\
    return (A-B-C-D-E-F-G-H-I-J-K);					\
}
#define dcl12(T)							\
dcl11(T)								\
T C##T##12(T A,T B,T C,T D,T E,T F,T G,T H,T I,T J,T K,T L)		\
{									\
    return (A-B-C-D-E-F-G-H-I-J-K-L);					\
}
#define dcl13(T)							\
dcl12(T)								\
T C##T##13(T A,T B,T C,T D,T E,T F,T G,T H,T I,T J,T K,T L,T M)		\
{									\
    return (A-B-C-D-E-F-G-H-I-J-K-L-M);					\
}
#define dcl14(T)							\
dcl13(T)								\
T C##T##14(T A,T B,T C,T D,T E,T F,T G,T H,T I,T J,T K,T L,T M,T N)	\
{									\
    return (A-B-C-D-E-F-G-H-I-J-K-L-M-N);				\
}
#define dcl15(T)							\
dcl14(T)								\
T C##T##15(T A,T B,T C,T D,T E,T F,T G,T H,T I,T J,T K,T L,T M,T N,T O)	\
{									\
    return (A-B-C-D-E-F-G-H-I-J-K-L-M-N-O);				\
}
#define dcl(T)	dcl15(T)
dcl(_c)
dcl(_uc)
dcl(_s)
dcl(_us)
dcl(_i)
#if __WORDSIZE == 64
dcl(_ui)
dcl(_l)
#endif
dcl(_f)
dcl(_d)
#undef dcl
#undef dcl15
#undef dcl14
#undef dcl13
#undef dcl12
#undef dcl11
#undef dcl10
#undef dcl9
#undef dcl8
#undef dcl7
#undef dcl6
#undef dcl5
#undef dcl4
#undef dcl3
#undef dcl2
#undef dcl1
#undef dcl0

#define dcl0(T)								\
T CJ##T##0(void)							\
{									\
    return ((*j##T##0)());						\
}
#define dcl1(T)								\
dcl0(T)									\
T CJ##T##1(void)							\
{									\
    return ((*j##T##1)(1));						\
}
#define dcl2(T)								\
dcl1(T)									\
T CJ##T##2(void)							\
{									\
    return ((*j##T##2)(1,2));						\
}
#define dcl3(T)								\
dcl2(T)									\
T CJ##T##3(void)							\
{									\
    return ((*j##T##3)(1,2,3));						\
}
#define dcl4(T)								\
dcl3(T)									\
T CJ##T##4(void)							\
{									\
    return ((*j##T##4)(1,2,3,4));					\
}
#define dcl5(T)								\
dcl4(T)									\
T CJ##T##5(void)							\
{									\
    return ((*j##T##5)(1,2,3,4,5));					\
}
#define dcl6(T)								\
dcl5(T)									\
T CJ##T##6(void)							\
{									\
    return ((*j##T##6)(1,2,3,4,5,6));					\
}
#define dcl7(T)								\
dcl6(T)									\
T CJ##T##7(void)							\
{									\
    return ((*j##T##7)(1,2,3,4,5,6,7));					\
}
#define dcl8(T)								\
dcl7(T)									\
T CJ##T##8(void)							\
{									\
    return ((*j##T##8)(1,2,3,4,5,6,7,8));				\
}
#define dcl9(T)								\
dcl8(T)									\
T CJ##T##9(void)							\
{									\
    return ((*j##T##9)(1,2,3,4,5,6,7,8,9));				\
}
#define dcl10(T)							\
dcl9(T)									\
T CJ##T##10(void)							\
{									\
    return ((*j##T##10)(1,2,3,4,5,6,7,8,9,10));				\
}
#define dcl11(T)							\
dcl10(T)								\
T CJ##T##11(void)							\
{									\
    return ((*j##T##11)(1,2,3,4,5,6,7,8,9,10,11));			\
}
#define dcl12(T)							\
dcl11(T)								\
T CJ##T##12(void)							\
{									\
    return ((*j##T##12)(1,2,3,4,5,6,7,8,9,10,11,12));			\
}
#define dcl13(T)							\
dcl12(T)								\
T CJ##T##13(void)							\
{									\
    return ((*j##T##13)(1,2,3,4,5,6,7,8,9,10,11,12,13));		\
}
#define dcl14(T)							\
dcl13(T)								\
T CJ##T##14(void)							\
{									\
    return ((*j##T##14)(1,2,3,4,5,6,7,8,9,10,11,12,13,14));		\
}
#define dcl15(T)							\
dcl14(T)								\
T CJ##T##15(void)							\
{									\
    return ((*j##T##15)(1,2,3,4,5,6,7,8,9,10,11,12,13,14,15));		\
}
#define dcl(t)	dcl15(t)
dcl(_c)
dcl(_uc)
dcl(_s)
dcl(_us)
dcl(_i)
#if __WORDSIZE == 64
dcl(_ui)
dcl(_l)
#endif
dcl(_f)
dcl(_d)
#undef dcl
#undef dcl15
#undef dcl14
#undef dcl13
#undef dcl12
#undef dcl11
#undef dcl10
#undef dcl9
#undef dcl8
#undef dcl7
#undef dcl6
#undef dcl5
#undef dcl4
#undef dcl3
#undef dcl2
#undef dcl1
#undef dcl0

int
main(int argc, char *argv[])
{
    jit_state_t		 *_jit;
    jit_node_t		 *jmpi_main;
    void		(*function)(void);
    jit_node_t		 *a1,*a2,*a3,*a4,*a5,*a6,*a7,*a8,*a9;
    jit_node_t		 *a10,*a11,*a12,*a13,*a14,*a15;
    jit_node_t		 *jmp;

    init_jit(argv[0]);
    _jit = jit_new_state();

    jmpi_main = jit_jmpi();


#define arg0(T)			/**/
#define arg1(T)						a1 = jit_arg##T();
#define arg2(T)			arg1(T)			a2 = jit_arg##T();
#define arg3(T)			arg2(T)			a3 = jit_arg##T();
#define arg4(T)			arg3(T)			a4 = jit_arg##T();
#define arg5(T)			arg4(T)			a5 = jit_arg##T();
#define arg6(T)			arg5(T)			a6 = jit_arg##T();
#define arg7(T)			arg6(T)			a7 = jit_arg##T();
#define arg8(T)			arg7(T)			a8 = jit_arg##T();
#define arg9(T)			arg8(T)			a9 = jit_arg##T();
#define arg10(T)		arg9(T)			a10 = jit_arg##T();
#define arg11(T)		arg10(T)		a11 = jit_arg##T();
#define arg12(T)		arg11(T)		a12 = jit_arg##T();
#define arg13(T)		arg12(T)		a13 = jit_arg##T();
#define arg14(T)		arg13(T)		a14 = jit_arg##T();
#define arg15(T)		arg14(T)		a15 = jit_arg##T();

#define get0(B,T,R)		jit_movi##B(R##0,0);
#define get1(B,T,R)		jit_getarg##B(R##0,a##1);
#define get2(B,T,R)							\
	get1(B,T,R);							\
	jit_movr##B(R##1, R##0);					\
	jit_getarg##T(R##0, a##2);					\
	jit_subr##B(R##0, R##1, R##0);
#define get3(B,T,R)							\
	get2(B,T,R);							\
	jit_movr##B(R##1, R##0);					\
	jit_getarg##T(R##0, a##3);					\
	jit_subr##B(R##0, R##1, R##0);
#define get4(B,T,R)							\
	get3(B,T,R);							\
	jit_movr##B(R##1, R##0);					\
	jit_getarg##T(R##0, a##4);					\
	jit_subr##B(R##0, R##1, R##0);
#define get5(B,T,R)							\
	get4(B,T,R);							\
	jit_movr##B(R##1, R##0);					\
	jit_getarg##T(R##0, a##5);					\
	jit_subr##B(R##0, R##1, R##0);
#define get6(B,T,R)							\
	get5(B,T,R);							\
	jit_movr##B(R##1, R##0);					\
	jit_getarg##T(R##0, a##6);					\
	jit_subr##B(R##0, R##1, R##0);
#define get7(B,T,R)							\
	get6(B,T,R);							\
	jit_movr##B(R##1, R##0);					\
	jit_getarg##T(R##0, a##7);					\
	jit_subr##B(R##0, R##1, R##0);
#define get8(B,T,R)							\
	get7(B,T,R);							\
	jit_movr##B(R##1, R##0);					\
	jit_getarg##T(R##0, a##8);					\
	jit_subr##B(R##0, R##1, R##0);
#define get9(B,T,R)							\
	get8(B,T,R);							\
	jit_movr##B(R##1, R##0);					\
	jit_getarg##T(R##0, a##9);					\
	jit_subr##B(R##0, R##1, R##0);
#define get10(B,T,R)							\
	get9(B,T,R);							\
	jit_movr##B(R##1, R##0);					\
	jit_getarg##T(R##0, a##10);					\
	jit_subr##B(R##0, R##1, R##0);
#define get11(B,T,R)							\
	get10(B,T,R);							\
	jit_movr##B(R##1, R##0);					\
	jit_getarg##T(R##0, a##11);					\
	jit_subr##B(R##0, R##1, R##0);
#define get12(B,T,R)							\
	get11(B,T,R);							\
	jit_movr##B(R##1, R##0);					\
	jit_getarg##T(R##0, a##12);					\
	jit_subr##B(R##0, R##1, R##0);
#define get13(B,T,R)							\
	get12(B,T,R);							\
	jit_movr##B(R##1, R##0);					\
	jit_getarg##T(R##0, a##13);					\
	jit_subr##B(R##0, R##1, R##0);
#define get14(B,T,R)							\
	get13(B,T,R);							\
	jit_movr##B(R##1, R##0);					\
	jit_getarg##T(R##0, a##14);					\
	jit_subr##B(R##0, R##1, R##0);
#define get15(B,T,R)							\
	get14(B,T,R);							\
	jit_movr##B(R##1, R##0);					\
	jit_getarg##T(R##0, a##15);					\
	jit_subr##B(R##0, R##1, R##0);

#if __WORDSIZE == 32
#  define jit_extr_i(u, v)			/**/
#else
#  define jit_extr_l(u, v)			/**/
#endif

#define strfy(n)				#n
#define defi(T, N)							\
    n##T##N = jit_name(strfy(n##T##N));					\
    jit_note("ccall.c", __LINE__);					\
    jit_prolog();							\
    arg##N();								\
    get##N(,T,JIT_R)							\
    jit_extr##T(JIT_R0, JIT_R0);					\
    jit_retr(JIT_R0);							\
    jit_epilog();
#define deff(T, N)							\
    n##T##N = jit_name(strfy(n##T##N));					\
    jit_note("ccall.c", __LINE__);					\
    jit_prolog();							\
    arg##N(T);								\
    get##N(T,T,JIT_F);							\
    jit_retr##T(JIT_F0);						\
    jit_epilog();

#define  def0(X, T)				def##X(T, 0)
#define  def1(X, T)	def0(X, T)		def##X(T, 1)
#define  def2(X, T)	def1(X, T)		def##X(T, 2)
#define  def3(X, T)	def2(X, T)		def##X(T, 3)
#define  def4(X, T)	def3(X, T)		def##X(T, 4)
#define  def5(X, T)	def4(X, T)		def##X(T, 5)
#define  def6(X, T)	def5(X, T)		def##X(T, 6)
#define  def7(X, T)	def6(X, T)		def##X(T, 7)
#define  def8(X, T)	def7(X, T)		def##X(T, 8)
#define  def9(X, T)	def8(X, T)		def##X(T, 9)
#define def10(X, T)	def9(X, T)		def##X(T, 10)
#define def11(X, T)	def10(X, T)		def##X(T, 11)
#define def12(X, T)	def11(X, T)		def##X(T, 12)
#define def13(X, T)	def12(X, T)		def##X(T, 13)
#define def14(X, T)	def13(X, T)		def##X(T, 14)
#define def15(X, T)	def14(X, T)		def##X(T, 15)
#define def(T)		def15(i, T)
	def(_c)
	def(_uc)
	def(_s)
	def(_us)
	def(_i)
#if __WORDSIZE == 64
	def(_ui)
	def(_l)
#endif
#undef def
#define def(T)		def15(f, T)
	def(_f)
	def(_d)
#undef def

    jit_patch(jmpi_main);
    jit_name("main");
    jit_note("ccall.c", __LINE__);
    jit_prolog();

#define  push0(T)	/**/
#define  push1(T)				jit_pushargi##T(1);
#define  push2(T)	push1(T)		jit_pushargi##T(2);
#define  push3(T)	push2(T)		jit_pushargi##T(3);
#define  push4(T)	push3(T)		jit_pushargi##T(4);
#define  push5(T)	push4(T)		jit_pushargi##T(5);
#define  push6(T)	push5(T)		jit_pushargi##T(6);
#define  push7(T)	push6(T)		jit_pushargi##T(7);
#define  push8(T)	push7(T)		jit_pushargi##T(8);
#define  push9(T)	push8(T)		jit_pushargi##T(9);
#define  push10(T)	push9(T)		jit_pushargi##T(10);
#define  push11(T)	push10(T)		jit_pushargi##T(11);
#define  push12(T)	push11(T)		jit_pushargi##T(12);
#define  push13(T)	push12(T)		jit_pushargi##T(13);
#define  push14(T)	push13(T)		jit_pushargi##T(14);
#define  push15(T)	push14(T)		jit_pushargi##T(15);

#define calin(T,N)							\
	jit_prepare();							\
		push##N()						\
	jit_finishi(C##T##N);						\
	jit_retval##T(JIT_R0);						\
	jmp = jit_beqi(JIT_R0, T##N);					\
	jit_calli(abort);						\
	jit_patch(jmp);
#define calfn(T,N)							\
	jit_prepare();							\
		push##N(T)						\
	jit_finishi(C##T##N);						\
	jit_retval##T(JIT_F0);						\
	jmp = jit_beqi##T(JIT_F0, _w##N);				\
	jit_calli(abort);						\
	jit_patch(jmp);
#define  calx0(X,T)				cal##X##n(T,0)
#define  calx1(X,T)	calx0(X,T)		cal##X##n(T,1)
#define  calx2(X,T)	calx1(X,T)		cal##X##n(T,2)
#define  calx3(X,T)	calx2(X,T)		cal##X##n(T,3)
#define  calx4(X,T)	calx3(X,T)		cal##X##n(T,4)
#define  calx5(X,T)	calx4(X,T)		cal##X##n(T,5)
#define  calx6(X,T)	calx5(X,T)		cal##X##n(T,6)
#define  calx7(X,T)	calx6(X,T)		cal##X##n(T,7)
#define  calx8(X,T)	calx7(X,T)		cal##X##n(T,8)
#define  calx9(X,T)	calx8(X,T)		cal##X##n(T,9)
#define calx10(X,T)	calx9(X,T)		cal##X##n(T,10)
#define calx11(X,T)	calx10(X,T)		cal##X##n(T,11)
#define calx12(X,T)	calx11(X,T)		cal##X##n(T,12)
#define calx13(X,T)	calx12(X,T)		cal##X##n(T,13)
#define calx14(X,T)	calx13(X,T)		cal##X##n(T,14)
#define calx15(X,T)	calx14(X,T)		cal##X##n(T,15)
#define cali(T)		calx15(i,T)
#define calf(T)		calx15(f,T)

    cali(_c)
    cali(_uc)
    cali(_s)
    cali(_us)
    cali(_i)
#if __WORDSIZE == 64
    cali(_ui)
    cali(_l)
#endif
    calf(_f)
    calf(_d)

#undef calin
#undef calfn
#define calin(T,N)							\
	jit_prepare();							\
		push##N()						\
	jit_finishi(CJ##T##N);						\
	jit_retval##T(JIT_R0);						\
	jmp = jit_beqi(JIT_R0, T##N);					\
	jit_calli(abort);						\
	jit_patch(jmp);
#define calfn(T,N)							\
	jit_prepare();							\
		push##N(T)						\
	jit_finishi(CJ##T##N);						\
	jit_retval##T(JIT_F0);						\
	jmp = jit_beqi##T(JIT_F0, _w##N);				\
	jit_calli(abort);						\
	jit_patch(jmp);
    cali(_c)
    cali(_uc)
    cali(_s)
    cali(_us)
    cali(_i)
#if __WORDSIZE == 64
    cali(_ui)
    cali(_l)
#endif
    calf(_f)
    calf(_d)

    jit_ret();

    function = jit_emit();

#define initn(T,N)	j##T##N = jit_address(n##T##N);
#define init0(T)			initn(T,0)
#define init1(T)	 init0(T)	initn(T,1)
#define init2(T)	 init1(T)	initn(T,2)
#define init3(T)	 init2(T)	initn(T,3)
#define init4(T)	 init3(T)	initn(T,4)
#define init5(T)	 init4(T)	initn(T,5)
#define init6(T)	 init5(T)	initn(T,6)
#define init7(T)	 init6(T)	initn(T,7)
#define init8(T)	 init7(T)	initn(T,8)
#define init9(T)	 init8(T)	initn(T,9)
#define init10(T)	 init9(T)	initn(T,10)
#define init11(T)	init10(T)	initn(T,11)
#define init12(T)	init11(T)	initn(T,12)
#define init13(T)	init12(T)	initn(T,13)
#define init14(T)	init13(T)	initn(T,14)
#define init15(T)	init14(T)	initn(T,15)
#define init(T)		init15(T)
    init(_c)
    init(_uc)
    init(_s)
    init(_us)
    init(_i)
#if __WORDSIZE == 64
    init(_ui)
    init(_l)
#endif
    init(_f)
    init(_d)

#if 0
    jit_print();
    jit_disassemble();
#endif

    jit_clear_state();
    (*function)();
    jit_destroy_state();

    finish_jit();

    printf("ok\n");

    return (0);
}
