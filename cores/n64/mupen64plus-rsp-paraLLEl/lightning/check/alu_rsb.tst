#include "alu.inc"

.code
	prolog

#define RSB(N, I0, I1, V)	ALU(N, , rsb, I0, I1, V)

	RSB(0,	1,	    0x7fffffff,	0x7ffffffe)
	RSB(2,	1,	    0x80000000,	0x7fffffff)
	RSB(3,	0x7fffffff, 0x80000000,	1)
	RSB(4,	0xffffffff, 0xffffffff,	0)
	RSB(5,	0x7fffffff, 0xffffffff, 0x80000000)
	RSB(6,	0, 0x7fffffff,		0x7fffffff)
#if __WORDSIZE == 32
	RSB(7,	0x7fffffff,	     1, 0x80000002)
	RSB(8,	0x80000000,	     1, 0x80000001)
	RSB(9,  0x80000000, 0x7fffffff, 0xffffffff)
	RSB(10, 0xffffffff, 0x7fffffff, 0x80000000)
	RSB(11,	0x7fffffff,	     0,	0x80000001)
#else
	RSB(7,	        0x7fffffff,		    1,	0xffffffff80000002)
	RSB(8,  0xffffffff80000000,		    1,		0x80000001)
	RSB(9,  0xffffffff80000000,	   0x7fffffff,		0xffffffff)
	RSB(10, 0xffffffffffffffff, 0xffffffff7fffffff, 0xffffffff80000000)
	RSB(11,	        0x7fffffff,		     0, 0xffffffff80000001)
	RSB(12,	1,		    0x7fffffffffffffff, 0x7ffffffffffffffe)
	RSB(13,	0x7fffffffffffffff,		     1, 0x8000000000000002)
	RSB(14,			 1, 0x8000000000000000, 0x7fffffffffffffff)
	RSB(15,	0x8000000000000000,		     1, 0x8000000000000001)
	RSB(16, 0x8000000000000000, 0x7fffffffffffffff, 0xffffffffffffffff)
	RSB(17, 0x7fffffffffffffff, 0x8000000000000000, 1)
	RSB(18, 0xffffffffffffffff, 0x7fffffffffffffff, 0x8000000000000000)
	RSB(19, 0x7fffffffffffffff, 0xffffffffffffffff, 0x8000000000000000)
	RSB(20, 0xffffffffffffffff, 0xffffffffffffffff, 0)
#endif

#undef RSB
#define RSB(N, T, I0, I1, V)	FOP(N, T, rsb, I0, I1, V)
	RSB(0, _f,	    0.5,	-0.5,	-1.0)
	RSB(1, _f,	    0.75,	 0.25,	-0.5)
	RSB(0, _d,	    0.5,	-0.5,	-1.0)
	RSB(1, _d,	    0.75,	 0.25,	-0.5)

	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
