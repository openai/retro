.data	16
ok:
.c	"ok"

.code
	jmpi main

/*
 * very simple test on purpose because otherwise it would not trigger
 * the bug where the retr %r0 or retr_d %f0 would be omitted because
 * the argument was already the return register, but the register end
 * clobbered by another instruction, like the div*, and the wrong
 * value returned because the retr* was removed and this way, lost
 * information that the register was live at function exit.
 */

check_r0:
	prolog
	movi %r0 1
	movi %r2 10
	// on x86 this changes %rax on other arches could use %r0 as temporary
	divi %r1 %r2 3
	// %r0 must still be 1
	retr %r0
	epilog

check_f0:
	prolog
	movi_d %f0 0.5
	movi_d %f2 10
	divi_d %f1 %f2 3
	retr_d %f0
	epilog

main:
	prolog
	calli check_r0
	retval %r1
	beqi r0_ok %r1 1
	calli @abort
r0_ok:
	calli check_f0
	retval_d %f1
	beqi_d f0_ok %f1 0.5
	calli @abort
f0_ok:
	prepare
		pushargi ok
	finishi @puts
	ret
	epilog
