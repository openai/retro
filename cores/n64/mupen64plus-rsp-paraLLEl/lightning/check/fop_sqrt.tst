#include "alu.inc"

.code
	prolog
#define SQRT(N, T, I, V)	FUN(N, T, sqrt, I, V)
#define USQRT(N, T, I, V)	UFUN(N, T, sqrt, I, V)
	 SQRT(0, _f,	-0.0,		 0.0)
	 SQRT(1, _f,	 4.0,		 2.0)
	 SQRT(2, _f,	 2.25,		 1.5)
	 SQRT(3, _f,	$Inf,		$Inf)
	USQRT(0, _f,	$NaN,		$NaN)
	 SQRT(0, _d,	-0.0,		 0.0)
	 SQRT(1, _d,	 4.0,		 2.0)
	 SQRT(2, _d,	 2.25,		 1.5)
	 SQRT(3, _d,	$Inf,		$Inf)
	USQRT(0, _d,	$NaN,		$NaN)

	prepare
		pushargi ok
		ellipsis
	finishi @printf
	ret
	epilog
