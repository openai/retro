#include "alu.inc"

.code
	prolog

#define XOR(N, I0, I1, V)	ALU(N, , xor, I0, I1, V)

	XOR(0,	0x7fffffff,	    1,			0x7ffffffe)
	XOR(1,	1,	   	    0x7fffffff,		0x7ffffffe)
	XOR(2,	0x80000000,	    1,			0x80000001)
	XOR(3,	1,	   	    0x80000000,		0x80000001)
	XOR(4,	0x7fffffff,	    0x80000000,		0xffffffff)
	XOR(5,	0x80000000,	    0x7fffffff,		0xffffffff)
	XOR(6,	0x7fffffff,	    0xffffffff,		0x80000000)
	XOR(7,	0xffffffff,	    0x7fffffff,		0x80000000)
	XOR(9,	0xffffffff,	    0xffffffff,		0)
	XOR(10,	0x7fffffff,	    0,			0x7fffffff)
	XOR(11,	0,		    0x7fffffff,		0x7fffffff)
#if __WORDSIZE == 64
	XOR(12,	0x7fffffffffffffff, 1,			0x7ffffffffffffffe)
	XOR(13,	1,		    0x7fffffffffffffff,	0x7ffffffffffffffe)
	XOR(14,	0x8000000000000000, 1,			0x8000000000000001)
	XOR(15,	1,		    0x8000000000000000,	0x8000000000000001)
	XOR(16,	0x7fffffffffffffff, 0x8000000000000000,	0xffffffffffffffff)
	XOR(17,	0x8000000000000000, 0x7fffffffffffffff,	0xffffffffffffffff)
	XOR(18,	0x7fffffffffffffff, 0xffffffffffffffff,	0x8000000000000000)
	XOR(19,	0xffffffffffffffff, 0x7fffffffffffffff,	0x8000000000000000)
	XOR(20,	0xffffffffffffffff, 0xffffffffffffffff,	0)
#endif

	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
