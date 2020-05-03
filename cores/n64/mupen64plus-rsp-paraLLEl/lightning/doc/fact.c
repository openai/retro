#include <stdio.h>
#include <lightning.h>

static jit_state_t *_jit;

typedef long (*pwfw_t)(long);		/* Pointer to Long Function of Long */

int main(int argc, char *argv[])
{
    pwfw_t	 factorial;
    long	 arg;
    jit_node_t	*ac;			/* Accumulator */
    jit_node_t	*in;			/* Argument */
    jit_node_t	*call;
    jit_node_t	*fact;
    jit_node_t	*jump;
    jit_node_t	*fact_entry;
    jit_node_t	*fact_out;

    init_jit(argv[0]);
    _jit = jit_new_state();

    /* declare a forward label */
    fact = jit_forward();

    jit_prolog();			/* Entry point of the factorial function */
    in = jit_arg();			/* Receive an integer argument */
    jit_getarg(JIT_R0, in);		/* Move argument to RO */
    jit_prepare();
    jit_pushargi(1);			/* This is the accumulator */
    jit_pushargr(JIT_R0);		/* This is the argument */
    call = jit_finishi(NULL);		/* Call the tail call optimized function */
    jit_patch_at(call, fact);		/* Patch call to forward defined function */
    /* the above could have been written as:
     *		jit_patch_at(jit_finishi(NULL), fact);
     */
    jit_retval(JIT_R0);			/* Fetch the result */
    jit_retr(JIT_R0);			/* Return it */
    jit_epilog();			/* Epilog *before* label before prolog */

    /* define the forward label */
    jit_link(fact);			/* Entry point of the helper function */
    jit_prolog();
    jit_frame(16);			/* Reserve 16 bytes in the stack */
    fact_entry = jit_label();		/* This is the tail call entry point */
    ac = jit_arg();			/* The accumulator is the first argument */
    in = jit_arg();			/* The factorial argument */
    jit_getarg(JIT_R0, ac);		/* Move the accumulator to R0 */
    jit_getarg(JIT_R1, in);		/* Move the argument to R1 */
    fact_out = jit_blei(JIT_R1, 1);	/* Done if argument is one or less */
    jit_mulr(JIT_R0, JIT_R0, JIT_R1);	/* accumulator *= argument */
    jit_putargr(JIT_R0, ac);		/* Update the accumulator */
    jit_subi(JIT_R1, JIT_R1, 1);	/* argument -= 1 */
    jit_putargr(JIT_R1, in);		/* Update the argument */
    jump = jit_jmpi();
    jit_patch_at(jump, fact_entry);	/* Tail Call Optimize it! */
    jit_patch(fact_out);
    jit_retr(JIT_R0);			/* Return the accumulator */

    factorial = jit_emit();
    /* no need to query information about resolved addresses */
    jit_clear_state();

    if (argc == 2)
	arg = atoi(argv[1]);
    else
	arg = 5;

    /* call the generated code */
    printf("factorial(%ld) = %ld\n", arg, factorial(arg));
    /* release all memory associated with the _jit identifier */
    jit_destroy_state();
    finish_jit();
    return 0;
}
