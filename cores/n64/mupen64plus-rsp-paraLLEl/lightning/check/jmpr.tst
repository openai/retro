/*
This is a very simple check to a condition that on lightning 2.0.5
could cause an assertion on some backends, due to correcting a problem
with temporaries that could not be saved/reloaded due to being used only
in the hardware instruction, or being considered live for too long on the
lightning instruction, and that could not be reloaded after the jump target
(or after false/true target on conditional branches).

If this code in lib/lightning.c:_jit_update():

		    for (regno = 0; regno < _jitc->reglen; regno++) {
			spec = jit_class(_rvs[regno].spec);
			if (jit_regset_tstbit(mask, regno) &&
			    (spec & (jit_class_gpr|jit_class_fpr)) &&
			    !(spec & jit_class_sav))
			    jit_regset_clrbit(mask, regno);
		    }

were removed, this test case, on x86_64 would fail like this:

lt-lightning: lightning.c:305: _jit_get_reg: Assertion `regspec & 0x02000000' failed.
Aborted (core dumped)
 */

.data	32
ret:
#if __WORDSIZE == 32
.i	0
#else
.l	0
#endif
ok:
.c	"ok"

.code
	prolog
	jmpi start

add_v1_v2:
	addr %v1 %v1 %v2
	ldi %r0 ret
	jmpr %r0

start:
	movi %v1 1
	movi %v2 2
	movi %r0 ret_add_v1_v2
	sti ret %r0
	movi %v0 add_v1_v2
	jmpr %v0
	movi_d %f0 3
	beqi_d pass_movi_f0 %f0 3
	calli @abort
pass_movi_f0:
	beqi pass_check_v2 %v2 2
	calli @abort
pass_check_v2:
ret_add_v1_v2:
	beqi pass_add_v1_v2 %v1 3
	calli @abort
pass_add_v1_v2:
	prepare
		pushargi ok
	finishi @puts
	ret
	epilog
