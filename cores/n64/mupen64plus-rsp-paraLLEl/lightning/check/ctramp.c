#include <lightning.h>
#include <stdio.h>

jit_state_t		*_jit;
long			 top;
long			 stk[1024];

int
main(int argc, char *argv[])
{
    void		 *address;
    void		(*call)(void*);
    jit_state_t		 *frame_jit, *tramp_jit;
    jit_node_t		 *arg, *done, *xfibs, *out, *ret1, *ret2;

    init_jit(argv[0]);
    _jit = frame_jit = jit_new_state();
    jit_name("main");
    jit_prolog();
    jit_frame(64);

    arg = jit_arg();
    jit_getarg(JIT_R1, arg);

    /* Initialize language stack */
    jit_movi(JIT_R0, (jit_word_t)stk);
    jit_sti(&top, JIT_R0);

    /* return address */
    done = jit_movi(JIT_R0, 0);
    /* argument */
    jit_movi(JIT_V0, 32);
    /* jump to code */
    jit_jmpr(JIT_R1);
    jit_patch(done);

    jit_prepare();
    jit_pushargi((jit_word_t)"xfibs(%d) = %d\n");
    jit_ellipsis();
    jit_pushargi(32);
    jit_pushargr(JIT_V0);
    jit_finishi(printf);
    jit_ret();
    jit_epilog();
    call = jit_emit();
    jit_clear_state();

#define SIZE				sizeof(jit_word_t)
    _jit = tramp_jit = jit_new_state();
    jit_name("xfibs");
    xfibs = jit_label();
    jit_prolog();
    jit_tramp(64);
    out = jit_blti(JIT_V0, 2);
    jit_subi(JIT_V1, JIT_V0, 1);	/* V1 = N-1 */
    jit_subi(JIT_V2, JIT_V0, 2);	/* V1 = N-2 */

    /* save return address */
    jit_ldi(JIT_R1, &top);
    jit_stxi(SIZE * 0, JIT_R1, JIT_R0);
    /* save operands */
    jit_stxi(SIZE * 1, JIT_R1, JIT_V0);
    jit_stxi(SIZE * 2, JIT_R1, JIT_V1);
    jit_stxi(SIZE * 3, JIT_R1, JIT_V2);
    /* adjust "language" stack */
    jit_addi(JIT_R1, JIT_R1, SIZE * 4);
    jit_sti(&top, JIT_R1);

    /* return address */
    ret1 = jit_movi(JIT_R0, 0);
    /* argument */
    jit_movr(JIT_V0, JIT_V1);
    /* indirect goto */
    jit_patch_at(jit_jmpi(), xfibs);
    jit_patch(ret1);
    jit_movr(JIT_V1, JIT_V0);		/* V1 = rfibs(N-1) */
    /* save V1 */
    jit_ldi(JIT_R1, &top);
    jit_stxi(-SIZE * 2, JIT_R1, JIT_V1);

    /* reload V2 */
    jit_ldxi(JIT_V2, JIT_R1, -SIZE * 1);

    /* return address */
    ret2 = jit_movi(JIT_R0, 0);
    /* argument */
    jit_movr(JIT_V0, JIT_V2);
    /* indirect goto */
    jit_patch_at(jit_jmpi(), xfibs);
    jit_patch(ret2);
    jit_movr(JIT_V2, JIT_V0);		/* V2 = rfibs(N-2) */

    /* reload return address */
    jit_ldi(JIT_R1, &top);
    jit_subi(JIT_R1, JIT_R1, SIZE * 4);
    jit_ldxi(JIT_R0, JIT_R1, SIZE * 0);
    /* reload operands */
    jit_ldxi(JIT_V0, JIT_R1, SIZE * 1);
    jit_ldxi(JIT_V1, JIT_R1, SIZE * 2);
    /* V2 already loaded */
    /* update "language" stack */
    jit_sti(&top, JIT_R1);

    jit_addi(JIT_V1, JIT_V1, 1);
    jit_addr(JIT_V0, JIT_V1, JIT_V2);
    jit_jmpr(JIT_R0);

    jit_patch(out);
    jit_movi(JIT_V0, 1);
    jit_jmpr(JIT_R0);
    jit_epilog();

    address = jit_emit();
    jit_clear_state();

    (*call)(address);

    jit_destroy_state();

    _jit = frame_jit;
    jit_destroy_state();
    return 0;
}
