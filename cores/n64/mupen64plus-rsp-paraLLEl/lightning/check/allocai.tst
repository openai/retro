.data	128
idfmt:
.c	"received %d\n"
failure_message:
.c	"numbers don't add up to zero\n"
report_message:
.c	"failed: got %i instead of %i\n"
succeeded_message:
.c	"succeeded\n"

.code
	jmpi main

/*
static int
identity (int arg)
{
  printf ("received %i\n", arg);
  return arg;
}
 */
	name identify
identify:
	prolog
	arg $i
	getarg %v0 $i
	prepare
		pushargi idfmt
		ellipsis
		pushargr %v0
	finishi @printf
	retr %v0
	epilog

	name identity_func
identity_func:
	prolog
	arg $i
	getarg %r1 $i

	/* Store the argument on the stack.  */
	allocai $(__WORDSIZE >> 3) $off
	stxi $off %fp %r1

	/* Store the negative of the argument on the stack.  */
	allocai $(__WORDSIZE >> 3) $neg
	negr %r2 %r1
	stxi $neg %fp %r2

	/* Invoke FUNC.  */
	prepare
		pushargr %r1
	finishi identify

	/* Ignore the result.  */

	/* Restore the negative and the argument from the stack.  */
	ldxi %r2 %fp $neg
	ldxi %v1 %fp $off

	/* Make sure they still add to zero.  */
	addr %r0 %v1 %r2
	bnei branch %r0 0

	/* Return it.  */
	retr %v1

	/* Display a failure message.  */
branch:
	prepare
		pushargi failure_message
		ellipsis
	finishi @printf

	/* Leave.  */
	retr %v1
	epilog

	name main
main:
	prolog
	prepare
		pushargi 7777
	finishi identity_func
	retval %r0
	beqi succeeded %r0 7777
	prepare
		pushargi report_message
		ellipsis
		pushargr %r0
		pushargi 7777
	finishi @printf
	reti 1
succeeded:
	prepare
		pushargi succeeded_message
		ellipsis
	finishi @printf
	reti 0
	epilog
