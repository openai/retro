#if __WORDSIZE == 32
#  define SIZE	4
#else
#  define SIZE	8
#endif
.data	8192
fmt:
.c	"xfibs(%d) = %d\n"
/* Simulate a language specific stack */
.align	SIZE
top:
/* Top, or base of the stack */
.size	SIZE
stk:
.size	8160

.code
	jmpi main

/* Usually a trampoline is created before the code that uses it, but
 * for testing purposes it is not required.
 * In this test case, it would mean "main" would be converted in a
 * different jit_state_t to native code, before xfibs was know.
 */
	name xfibs
xfibs:
	/* return address is in %r0 */
	/* argument and return value in %v0 */
	prolog
	tramp 64
	blti_u out %v0 2
	subi %v1 %v0 1		/* V1 = N-1 */
	subi %v2 %v0 2		/* V1 = N-2 */

	/* save return address */
	ldi %r1 top
	stxi $(SIZE * 0) %r1 %r0
	/* save operands */
	stxi $(SIZE * 1) %r1 %v0
	stxi $(SIZE * 2) %r1 %v1
	stxi $(SIZE * 3) %r1 %v2
	/* adjust "language" stack */
	addi %r1 %r1 $(SIZE * 4)
	sti top %r1

	/* return address */
	movi %r0 ret1
	/* argument */
	movr %v0 %v1
	/* indirect goto */
	jmpi xfibs
ret1:
	movr %v1 %v0		/* V1 = rfibs(N-1) */
	/* save V1 */
	ldi %r1 top
	stxi $(-SIZE * 2) %r1 %v1

	/* reload V2 */
	ldxi %v2 %r1 $(-SIZE * 1)

	/* return address */
	movi %r0 ret2
	/* argument */
	movr %v0 %v2
	/* indirect goto */
	jmpi xfibs
ret2:
	movr %v2 %v0		/* V2 = rfibs(N-2) */

	/* reload return address */
	ldi %r1 top
	subi %r1 %r1 $(SIZE * 4)
	ldxi %r0 %r1 $(SIZE * 0)
	/* reload operands */
	ldxi %v0 %r1 $(SIZE * 1)
	ldxi %v1 %r1 $(SIZE * 2)
	/* V2 already loaded */
	/* update "language" stack */
	sti top %r1

	addi %v1 %v1 1
	addr %v0 %v1 %v2
	jmpr %r0
out:
	movi %v0 1
	jmpr %r0
	epilog

	name main
main:
	prolog
	frame 64

	/* Initialize language stack */
	movi %r0 stk
	sti top %r0

	/* return address */
	movi %r0 done
	/* argument */
	movi %v0 32
	jmpi xfibs
done:
	prepare
		pushargi fmt
		ellipsis
		pushargi 32
		pushargr %v0
	finishi @printf
	ret
	epilog
