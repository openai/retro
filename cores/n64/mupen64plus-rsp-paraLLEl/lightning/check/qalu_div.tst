#include "qalu.inc"

.code
	prolog
#define QDIV(N, I0, I1, LO, HI)		QALU(N, , qdiv, I0, I1, LO, HI)
#define UQDIV(N, I0, I1, LO, HI)	QALU(N, _u, qdiv, I0, I1, LO, HI)
	 QDIV(0, 10, 3, 3, 1)
	 QDIV(1, -33, 9, -3, -6)
	 QDIV(2, -41, -7, 5, -6)
	 QDIV(3, 65536, 4096, 16, 0)
	UQDIV(4, -1, -2, 1, 1)
	UQDIV(5, -2, -5, 1, 3)
	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
