#include "alu.inc"

.code
	prolog

#define LSH(N, I0, I1, V)	ALU(N, , lsh, I0, I1, V)

	LSH(0,	 0x7f,		 1,	0xfe)
	LSH(1,	 0x7fff,	 2,	0x1fffc)
	LSH(2,	 0x81,		16,	0x810000)
	LSH(3,	 0xff,		15,	0x7f8000)
	LSH(4,	 0x7fffffff,	 0,	0x7fffffff)
#if __WORDSIZE == 32
	LSH(5,	 0xffffffff,	8,	0xffffff00)
	LSH(6,	 0x7fffffff,	3,	0xfffffff8)
	LSH(7,	-0x7f,		31,	0x80000000)
	LSH(8,	-0x7fff,	30,	0x40000000)
	LSH(9,	-0x7fffffff,	29,	0x20000000)
	LSH(10,	 0x80000001,	28,	0x10000000)
	LSH(11,	 0x8001,	17,	0x20000)
	LSH(12,	 0x80000001,	18,	0x40000)
	LSH(13,	-0xffff,	24,	0x1000000)
#else
	LSH(5,	 0xffffffff,	 8,	0xffffffff00)
	LSH(6,	 0x7fffffff,	 3,	0x3fffffff8)
	LSH(7,	-0x7f,		31,	0xffffffc080000000)
	LSH(8,	-0x7fff,	30,	0xffffe00040000000)
	LSH(9,	-0x7fffffff,	29,	0xf000000020000000)
	LSH(10,	 0x80000001,	28,	0x800000010000000)
	LSH(11,	 0x8001,	17,	0x100020000)
	LSH(12,	 0x80000001,	18,	0x2000000040000)
	LSH(13,	-0xffff,	24,	0xffffff0001000000)
	LSH(14,	 0x7f,		33,	0xfe00000000)
	LSH(15,	 0x7ffff,	34,	0x1ffffc00000000)
	LSH(16,	 0x7fffffff,	35,	0xfffffff800000000)
	LSH(17,	-0x7f,		63,	0x8000000000000000)
	LSH(18,	-0x7fff,	62,	0x4000000000000000)
	LSH(19,	-0x7fffffff,	61,	0x2000000000000000)
	LSH(20,	 0x80000001,	60,	0x1000000000000000)
	LSH(21,	 0x81,		48,	0x81000000000000)
	LSH(22,	 0x8001,	49,	0x2000000000000)
	LSH(23,	 0x80000001,	40,	0x10000000000)
	LSH(24,	 0xff,		47,	0x7f800000000000)
	LSH(25,	 0xffff0001,	56,	0x100000000000000)
	LSH(26,	 0xffffffff,	40,	0xffffff0000000000)
	LSH(27,	 0x7fffffffff,	33,	0xfffffffe00000000)
	LSH(28,	-0x7fffffffff,	63,	0x8000000000000000)
	LSH(29,	 0x8000000001,	48,	0x1000000000000)
	LSH(30,	 0xffffffffff,	47,	0xffff800000000000)
#endif

	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
