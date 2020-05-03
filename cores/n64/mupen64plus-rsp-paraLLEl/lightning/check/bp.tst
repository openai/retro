.data	32
fmt:
.c	"nfibs(%d) = %d\n"

.code
	jmpi main

	name rfibs
rfibs:
	prolog
	arg $in
	getarg %r0 $in		/* R0 = N */
	beqi out %r0 0
	movr %v0 %r0		/* V0 = R0 */
	movi %r0 1
	blei_u out %v0 2
	subi %v1 %v0 1		/* V1 = N-1 */
	subi %v2 %v0 2		/* V1 = N-2 */
	prepare
		pushargr %v1
	finishi rfibs
	retval %v1		/* V1 = rfibs(N-1) */
	prepare
		pushargr %v2
	finishi rfibs
	retval %r0		/* R0 = rfibs(N-2) */
	addr %r0 %r0 %v1
out:
	retr %r0
	epilog

	name main
main:
	prolog
	prepare
		pushargi 32
	finishi rfibs
	retval %v0
	prepare
		pushargi fmt
		ellipsis
		pushargi 32
		pushargr %v0
	finishi @printf
	ret
	epilog
