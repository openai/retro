#include "alu.inc"

.code
	prolog

#define ADD(N, I0, I1, V)	ALU(N, , add, I0, I1, V)

	ADD(0,	0x7fffffff, 1,		0x80000000)
	ADD(1,	1,	    0x7fffffff, 0x80000000)
	ADD(2,	0x80000000, 1,		0x80000001)
	ADD(3,	1,	    0x80000000, 0x80000001)
	ADD(4,	0x7fffffff, 0x80000000, 0xffffffff)
	ADD(5,	0x80000000, 0x7fffffff, 0xffffffff)
	ADD(6,	0x7fffffff, 0,		0x7fffffff)
	ADD(7,	0,	    0x7fffffff,	0x7fffffff)
#if __WORDSIZE == 32
	ADD(8,	0x7fffffff, 0xffffffff, 0x7ffffffe)
	ADD(9,	0xffffffff, 0x7fffffff, 0x7ffffffe)
	ADD(10,	0xffffffff, 0xffffffff, 0xfffffffe)
#else
	ADD(8,	0x7fffffff,	    0xffffffff,		0x17ffffffe)
	ADD(9,	0xffffffff,	    0x7fffffff,		0x17ffffffe)
	ADD(10,	0xffffffff,	    0xffffffff,		0x1fffffffe)
	ADD(11,	0x7fffffffffffffff, 1,			0x8000000000000000)
	ADD(12,	1,		    0x7fffffffffffffff,	0x8000000000000000)
	ADD(13,	0x8000000000000000, 1,			0x8000000000000001)
	ADD(14,	1,		    0x8000000000000000,	0x8000000000000001)
	ADD(15,	0x7fffffffffffffff, 0x8000000000000000,	0xffffffffffffffff)
	ADD(16,	0x8000000000000000, 0x7fffffffffffffff,	0xffffffffffffffff)
	ADD(17,	0x7fffffffffffffff, 0xffffffffffffffff,	0x7ffffffffffffffe)
	ADD(18,	0x7fffffffffffffff, 0x7fffffffffffffff,	0xfffffffffffffffe)
	ADD(19,	0xffffffffffffffff, 0xffffffffffffffff,	0xfffffffffffffffe)
#endif

#undef ADD
#define ADD(N, T, I0, I1, V)	FOP(N, T, add, I0, I1, V)
	ADD(0, _f,	-0.5,	    0.5,	0.0)
	ADD(1, _f,	 0.25,	    0.75,	1.0)
	ADD(0, _d,	-0.5,	    0.5,	0.0)
	ADD(1, _d,	 0.25,	    0.75,	1.0)

	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
