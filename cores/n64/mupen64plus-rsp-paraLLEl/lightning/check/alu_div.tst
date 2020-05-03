#include "alu.inc"

.code
	prolog

#define DIV(N, I0, I1, V)	ALU(N, , div, I0, I1, V)
#define UDIV(N, I0, I1, V)	ALU(N, _u, div, I0, I1, V)

	DIV(0,	0x7fffffff, 1,		0x7fffffff)
	DIV(1,	1,	    0x7fffffff, 0)
	DIV(2,	0x80000000, 1,		0x80000000)
	DIV(3,	1,	    0x80000000, 0)
	DIV(4,	0x7fffffff, 2,		0x3fffffff)
	DIV(5,	2,	    0x7fffffff,	0)
	DIV(6,	2,	    0x80000000, 0)
	DIV(7,	0x7fffffff, 0x80000000, 0)
	DIV(8,	0,	    0x7fffffff,	0)
	DIV(9,	0xffffffff, 0xffffffff, 1)
	UDIV(0,	0x7fffffff, 1,		0x7fffffff)
	UDIV(1,	1,	    0x7fffffff, 0)
	UDIV(2,	0x80000000, 1,		0x80000000)
	UDIV(3,	1,	    0x80000000, 0)
	UDIV(4,	0x7fffffff, 2,		0x3fffffff)
	UDIV(5,	2,	    0x7fffffff,	0)
	UDIV(6,	0x80000000, 2,		0x40000000)
	UDIV(7,	2,	    0x80000000, 0)
	UDIV(8,	0x7fffffff, 0x80000000, 0)
	UDIV(9,	0x80000000, 0x7fffffff, 1)
	UDIV(10,0,	    0x7fffffff,	0)
	UDIV(11,0x7fffffff, 0xffffffff, 0)
	UDIV(12,0xffffffff, 0x7fffffff, 2)
	UDIV(13,0xffffffff, 0xffffffff, 1)
#if __WORDSIZE == 32
	DIV(10,	0x80000000, 2,		0xc0000000)
	DIV(11,	0x80000000, 0x7fffffff, 0xffffffff)
	DIV(12,	0x7fffffff, 0xffffffff, 0x80000001)
	DIV(13,	0xffffffff, 0x7fffffff, 0)
#else
	DIV(10,	0x80000000,	    2,			0x40000000)
	DIV(11,	0x80000000,	    0x7fffffff,		1)
	DIV(12,	0x7fffffff,	    0xffffffff,		0)
	DIV(13,	0xffffffff,	    0x7fffffff,		2)
	DIV(14,	0x7fffffffffffffff, 1,			0x7fffffffffffffff)
	DIV(15,	1,		    0x7fffffffffffffff,	0)
	DIV(16,	0x8000000000000000, 1,			0x8000000000000000)
	DIV(17,	1,		    0x8000000000000000,	0)
	DIV(18,	0x7fffffffffffffff, 2,			0x3fffffffffffffff)
	DIV(19,	2,		    0x7fffffffffffffff,	0)
	DIV(20,	0x8000000000000000, 2,			0xc000000000000000)
	DIV(21,	2,		    0x8000000000000000,	0)
	DIV(22,	0x7fffffffffffffff, 0x8000000000000000,	0)
	DIV(23,	0x8000000000000000, 0x7fffffffffffffff,	0xffffffffffffffff)
	DIV(24,	0x7fffffffffffffff, 0xffffffffffffffff,	0x8000000000000001)
	DIV(25,	0xffffffffffffffff, 0x7fffffffffffffff,	0)
	DIV(26,	0xffffffffffffffff, 0xffffffffffffffff,	1)
	UDIV(14,0x7fffffffffffffff, 1,			0x7fffffffffffffff)
	UDIV(15,1,		    0x7fffffffffffffff,	0)
	UDIV(16,0x8000000000000000, 1,			0x8000000000000000)
	UDIV(17,1,		    0x8000000000000000,	0)
	UDIV(18,0x7fffffffffffffff, 2,			0x3fffffffffffffff)
	UDIV(19,2,		    0x7fffffffffffffff,	0)
	UDIV(20,0x8000000000000000, 2,			0x4000000000000000)
	UDIV(21,2,		    0x8000000000000000,	0)
	UDIV(22,0x7fffffffffffffff, 0x8000000000000000,	0)
	UDIV(23,0x8000000000000000, 0x7fffffffffffffff,	1)
	UDIV(24,0x7fffffffffffffff, 0xffffffffffffffff,	0)
	UDIV(25,0xffffffffffffffff, 0x7fffffffffffffff,	2)
	UDIV(26,0xffffffffffffffff, 0xffffffffffffffff,	1)
#endif

#undef DIV
#define DIV(N, T, I0, I1, V)	FOP(N, T, div, I0, I1, V)
	DIV(0, _f,	-0.5,	    0.5,	-1.0)
	DIV(1, _f,	 1.25,	    0.5,	 2.5)
	DIV(0, _d,	-0.5,	    0.5,	-1.0)
	DIV(1, _d,	 1.25,	    0.5,	 2.5)

	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
