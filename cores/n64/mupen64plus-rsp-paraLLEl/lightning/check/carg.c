#include <lightning.h>
#include <stdio.h>

/*   Simple test for arguments handling, that also shows how to use
 * arguments to store values.
 *   Register arguments, if available, are very fast, but are also
 * very volatile on some ports, because some ports will do C calls
 * to implement division, remainder, sometimes multiplication, or
 * some float operations.
 *   Arguments in registers should be fetched in the prolog of the
 * function, and if they must be saved, they should be saved in
 * the prolog.
 *   The predicate macro "jit_arg_register_p(arg)" allows knowing if
 * an argument lives in a register, where it is known for being a very
 * fast to read/write temporary storage.
 */

#define W		jit_word_t
#define F		jit_float32_t
#define D		jit_float64_t

jit_state_t		 *_jit;

void
cw(W a1, W  a2, W  a3, W  a4, W  a5, W  a6, W  a7, W  a8,
   W a9, W a10, W a11, W a12, W a13, W a14, W a15, W a16)
{
    if ( a1 !=  1 ||  a2 !=  2 ||  a3 !=  3 ||  a4 !=  4 ||
	 a5 !=  5 ||  a6 !=  6 ||  a7 !=  7 ||  a8 !=  8 ||
	 a9 !=  9 || a10 != 10 || a11 != 11 || a12 != 12 ||
	a13 != 13 || a14 != 14 || a15 != 15 || a16 != 16)
	abort();
}

void
cf(F a1, F  a2, F  a3, F  a4, F  a5, F  a6, F  a7, F  a8,
   F a9, F a10, F a11, F a12, F a13, F a14, F a15, F a16)
{
    if ( a1 !=  1 ||  a2 !=  2 ||  a3 !=  3 ||  a4 !=  4 ||
	 a5 !=  5 ||  a6 !=  6 ||  a7 !=  7 ||  a8 !=  8 ||
	 a9 !=  9 || a10 != 10 || a11 != 11 || a12 != 12 ||
	a13 != 13 || a14 != 14 || a15 != 15 || a16 != 16)
	abort();
}

void
cd(D a1, D  a2, D  a3, D  a4, D  a5, D  a6, D  a7, D  a8,
   D a9, D a10, D a11, D a12, D a13, D a14, D a15, D a16)
{
    if ( a1 !=  1 ||  a2 !=  2 ||  a3 !=  3 ||  a4 !=  4 ||
	 a5 !=  5 ||  a6 !=  6 ||  a7 !=  7 ||  a8 !=  8 ||
	 a9 !=  9 || a10 != 10 || a11 != 11 || a12 != 12 ||
	a13 != 13 || a14 != 14 || a15 != 15 || a16 != 16)
	abort();
}

int
main(int argc, char *argv[])
{
    void		(*code)(void);
    jit_node_t		*jmp, *pass;
    jit_node_t		 *jw,  *jf,  *jd;
    jit_int32_t		  s1,   s2,   s3,   s4,   s5,   s6,   s7,   s8,
			  s9,  s10,  s11,  s12,  s13,  s14,  s15,  s16;
    jit_node_t		 *a1,  *a2,  *a3,  *a4,  *a5,  *a6,  *a7,  *a8,
			 *a9, *a10, *a11, *a12, *a13, *a14, *a15, *a16;

    init_jit(argv[0]);
    _jit = jit_new_state();

    /* jump to "main" label */
    jmp = jit_jmpi();

    /* Create jit function that
     * o Receives 16 word arguments
     * o Save in the stack any register argument. Also force register
     *   arguments to be clobbered to properly make the test
     * o Calls a C function that receives 16 word arguments, with
     *   values different from the ones received by this function
     * o Reload from stack any register argument
     * o Validated all arguments were not modified in the known
     *   cases it could have been clobbered
     */
    jw = jit_label();
    jit_name("jw");
    jit_note(__FILE__, __LINE__);
    jit_prolog();
    a1  = jit_arg();
    a2  = jit_arg();
    a3  = jit_arg();
    a4  = jit_arg();
    a5  = jit_arg();
    a6  = jit_arg();
    a7  = jit_arg();
    a8  = jit_arg();
    a9  = jit_arg();
    a10 = jit_arg();
    a11 = jit_arg();
    a12 = jit_arg();
    a13 = jit_arg();
    a14 = jit_arg();
    a15 = jit_arg();
    a16 = jit_arg();
#define SAVE_ARG(N)							\
    do {								\
	if (jit_arg_register_p(a##N)) {					\
	    s##N = jit_allocai(sizeof(W));				\
	    jit_getarg(JIT_R0, a##N);					\
	    jit_stxi(s##N, JIT_FP, JIT_R0);				\
	    jit_putargi(-1, a##N);					\
	}								\
    } while (0)
    SAVE_ARG(1);
    SAVE_ARG(2);
    SAVE_ARG(3);
    SAVE_ARG(4);
    SAVE_ARG(5);
    SAVE_ARG(6);
    SAVE_ARG(7);
    SAVE_ARG(8);
    SAVE_ARG(9);
    SAVE_ARG(10);
    SAVE_ARG(11);
    SAVE_ARG(12);
    SAVE_ARG(13);
    SAVE_ARG(14);
    SAVE_ARG(15);
    SAVE_ARG(16);
#undef SAVE_ARG
    jit_prepare();
    {
	jit_pushargi(1);
	jit_pushargi(2);
	jit_pushargi(3);
	jit_pushargi(4);
	jit_pushargi(5);
	jit_pushargi(6);
	jit_pushargi(7);
	jit_pushargi(8);
	jit_pushargi(9);
	jit_pushargi(10);
	jit_pushargi(11);
	jit_pushargi(12);
	jit_pushargi(13);
	jit_pushargi(14);
	jit_pushargi(15);
	jit_pushargi(16);
    }
    jit_finishi(cw);
#define LOAD_ARG(N)							\
    do {								\
	if (jit_arg_register_p(a##N)) {					\
	    jit_ldxi(JIT_R0, JIT_FP, s##N);				\
	    jit_putargr(JIT_R0, a##N);					\
	}								\
    } while (0)
    LOAD_ARG(1);
    LOAD_ARG(2);
    LOAD_ARG(3);
    LOAD_ARG(4);
    LOAD_ARG(5);
    LOAD_ARG(6);
    LOAD_ARG(7);
    LOAD_ARG(8);
    LOAD_ARG(9);
    LOAD_ARG(10);
    LOAD_ARG(11);
    LOAD_ARG(12);
    LOAD_ARG(13);
    LOAD_ARG(14);
    LOAD_ARG(15);
    LOAD_ARG(16);
#undef LOAD_ARG
    pass = jit_forward();
#define CHECK_ARG(N)							\
    do {								\
	jit_getarg(JIT_R0, a##N);					\
	jit_patch_at(jit_beqi(JIT_R0, 17 - N), pass);			\
    } while (0)
    CHECK_ARG(1);
    CHECK_ARG(2);
    CHECK_ARG(3);
    CHECK_ARG(4);
    CHECK_ARG(5);
    CHECK_ARG(6);
    CHECK_ARG(7);
    CHECK_ARG(8);
    CHECK_ARG(9);
    CHECK_ARG(10);
    CHECK_ARG(11);
    CHECK_ARG(12);
    CHECK_ARG(13);
    CHECK_ARG(14);
    CHECK_ARG(15);
    CHECK_ARG(16);
#undef CHECK_ARG
    jit_calli(abort);
    jit_link(pass);
    jit_ret();
    jit_epilog();

    /* Create jit function that
     * o Receives 16 float arguments
     * o Save in the stack any register argument. Also force register
     *   arguments to be clobbered to properly make the test
     * o Calls a C function that receives 16 float arguments, with
     *   values different from the ones received by this function
     * o Reload from stack any register argument
     * o Validated all arguments were not modified in the known
     *   cases it could have been clobbered
     */
    jf = jit_label();
    jit_name("jf");
    jit_note(__FILE__, __LINE__);
    jit_prolog();
    a1  = jit_arg_f();
    a2  = jit_arg_f();
    a3  = jit_arg_f();
    a4  = jit_arg_f();
    a5  = jit_arg_f();
    a6  = jit_arg_f();
    a7  = jit_arg_f();
    a8  = jit_arg_f();
    a9  = jit_arg_f();
    a10 = jit_arg_f();
    a11 = jit_arg_f();
    a12 = jit_arg_f();
    a13 = jit_arg_f();
    a14 = jit_arg_f();
    a15 = jit_arg_f();
    a16 = jit_arg_f();
#define SAVE_ARG(N)							\
    do {								\
	if (jit_arg_register_p(a##N)) {					\
	    s##N = jit_allocai(sizeof(F));				\
	    jit_getarg_f(JIT_F0, a##N);					\
	    jit_stxi_f(s##N, JIT_FP, JIT_F0);				\
	    jit_putargi_f(-1, a##N);					\
	}								\
    } while (0)
    SAVE_ARG(1);
    SAVE_ARG(2);
    SAVE_ARG(3);
    SAVE_ARG(4);
    SAVE_ARG(5);
    SAVE_ARG(6);
    SAVE_ARG(7);
    SAVE_ARG(8);
    SAVE_ARG(9);
    SAVE_ARG(10);
    SAVE_ARG(11);
    SAVE_ARG(12);
    SAVE_ARG(13);
    SAVE_ARG(14);
    SAVE_ARG(15);
    SAVE_ARG(16);
#undef SAVE_ARG
    jit_prepare();
    {
	jit_pushargi_f(1);
	jit_pushargi_f(2);
	jit_pushargi_f(3);
	jit_pushargi_f(4);
	jit_pushargi_f(5);
	jit_pushargi_f(6);
	jit_pushargi_f(7);
	jit_pushargi_f(8);
	jit_pushargi_f(9);
	jit_pushargi_f(10);
	jit_pushargi_f(11);
	jit_pushargi_f(12);
	jit_pushargi_f(13);
	jit_pushargi_f(14);
	jit_pushargi_f(15);
	jit_pushargi_f(16);
    }
    jit_finishi(cf);
#define LOAD_ARG(N)							\
    do {								\
	if (jit_arg_register_p(a##N)) {					\
	    jit_ldxi_f(JIT_F0, JIT_FP, s##N);				\
	    jit_putargr_f(JIT_F0, a##N);				\
	}								\
    } while (0)
    LOAD_ARG(1);
    LOAD_ARG(2);
    LOAD_ARG(3);
    LOAD_ARG(4);
    LOAD_ARG(5);
    LOAD_ARG(6);
    LOAD_ARG(7);
    LOAD_ARG(8);
    LOAD_ARG(9);
    LOAD_ARG(10);
    LOAD_ARG(11);
    LOAD_ARG(12);
    LOAD_ARG(13);
    LOAD_ARG(14);
    LOAD_ARG(15);
    LOAD_ARG(16);
#undef LOAD_ARG
    pass = jit_forward();
#define CHECK_ARG(N)							\
    do {								\
	jit_getarg_f(JIT_F0, a##N);					\
	jit_patch_at(jit_beqi_f(JIT_F0, 17 - N), pass);			\
    } while (0)
    CHECK_ARG(1);
    CHECK_ARG(2);
    CHECK_ARG(3);
    CHECK_ARG(4);
    CHECK_ARG(5);
    CHECK_ARG(6);
    CHECK_ARG(7);
    CHECK_ARG(8);
    CHECK_ARG(9);
    CHECK_ARG(10);
    CHECK_ARG(11);
    CHECK_ARG(12);
    CHECK_ARG(13);
    CHECK_ARG(14);
    CHECK_ARG(15);
    CHECK_ARG(16);
#undef CHECK_ARG
    jit_calli(abort);
    jit_link(pass);
    jit_ret();
    jit_epilog();

    /* Create jit function that
     * o Receives 16 double arguments
     * o Save in the stack any register argument. Also force register
     *   arguments to be clobbered to properly make the test
     * o Calls a C function that receives 16 double arguments, with
     *   values different from the ones received by this function
     * o Reload from stack any register argument
     * o Validated all arguments were not modified in the known
     *   cases it could have been clobbered
     */
    jd = jit_label();
    jit_name("jd");
    jit_note(__FILE__, __LINE__);
    jit_prolog();
    a1  = jit_arg_d();
    a2  = jit_arg_d();
    a3  = jit_arg_d();
    a4  = jit_arg_d();
    a5  = jit_arg_d();
    a6  = jit_arg_d();
    a7  = jit_arg_d();
    a8  = jit_arg_d();
    a9  = jit_arg_d();
    a10 = jit_arg_d();
    a11 = jit_arg_d();
    a12 = jit_arg_d();
    a13 = jit_arg_d();
    a14 = jit_arg_d();
    a15 = jit_arg_d();
    a16 = jit_arg_d();
#define SAVE_ARG(N)							\
    do {								\
	if (jit_arg_register_p(a##N)) {					\
	    s##N = jit_allocai(sizeof(D));				\
	    jit_getarg_d(JIT_F0, a##N);					\
	    jit_stxi_d(s##N, JIT_FP, JIT_F0);				\
	    jit_putargi_d(-1, a##N);					\
	}								\
    } while (0)
    SAVE_ARG(1);
    SAVE_ARG(2);
    SAVE_ARG(3);
    SAVE_ARG(4);
    SAVE_ARG(5);
    SAVE_ARG(6);
    SAVE_ARG(7);
    SAVE_ARG(8);
    SAVE_ARG(9);
    SAVE_ARG(10);
    SAVE_ARG(11);
    SAVE_ARG(12);
    SAVE_ARG(13);
    SAVE_ARG(14);
    SAVE_ARG(15);
    SAVE_ARG(16);
#undef SAVE_ARG
    jit_prepare();
    {
	jit_pushargi_d(1);
	jit_pushargi_d(2);
	jit_pushargi_d(3);
	jit_pushargi_d(4);
	jit_pushargi_d(5);
	jit_pushargi_d(6);
	jit_pushargi_d(7);
	jit_pushargi_d(8);
	jit_pushargi_d(9);
	jit_pushargi_d(10);
	jit_pushargi_d(11);
	jit_pushargi_d(12);
	jit_pushargi_d(13);
	jit_pushargi_d(14);
	jit_pushargi_d(15);
	jit_pushargi_d(16);
    }
    jit_finishi(cd);
#define LOAD_ARG(N)							\
    do {								\
	if (jit_arg_register_p(a##N)) {					\
	    jit_ldxi_d(JIT_F0, JIT_FP, s##N);				\
	    jit_putargr_d(JIT_F0, a##N);				\
	}								\
    } while (0)
    LOAD_ARG(1);
    LOAD_ARG(2);
    LOAD_ARG(3);
    LOAD_ARG(4);
    LOAD_ARG(5);
    LOAD_ARG(6);
    LOAD_ARG(7);
    LOAD_ARG(8);
    LOAD_ARG(9);
    LOAD_ARG(10);
    LOAD_ARG(11);
    LOAD_ARG(12);
    LOAD_ARG(13);
    LOAD_ARG(14);
    LOAD_ARG(15);
    LOAD_ARG(16);
#undef LOAD_ARG
    pass = jit_forward();
#define CHECK_ARG(N)							\
    do {								\
	jit_getarg_d(JIT_F0, a##N);					\
	jit_patch_at(jit_beqi_d(JIT_F0, 17 - N), pass);			\
    } while (0)
    CHECK_ARG(1);
    CHECK_ARG(2);
    CHECK_ARG(3);
    CHECK_ARG(4);
    CHECK_ARG(5);
    CHECK_ARG(6);
    CHECK_ARG(7);
    CHECK_ARG(8);
    CHECK_ARG(9);
    CHECK_ARG(10);
    CHECK_ARG(11);
    CHECK_ARG(12);
    CHECK_ARG(13);
    CHECK_ARG(14);
    CHECK_ARG(15);
    CHECK_ARG(16);
#undef CHECK_ARG
    jit_calli(abort);
    jit_link(pass);
    jit_ret();
    jit_epilog();

    /* Create a jit function that calls the 3 previous ones.
     * o First call the function that receives 16 word arguments
     * o Then call the function that receives 16 float arguments
     * o Finally call the function that receives 16 double arguments
     */
    jit_patch(jmp);
    jit_name("main");
    jit_note(__FILE__, __LINE__);
    jit_prolog();
    jit_prepare();
    {
	jit_pushargi(16);
	jit_pushargi(15);
	jit_pushargi(14);
	jit_pushargi(13);
	jit_pushargi(12);
	jit_pushargi(11);
	jit_pushargi(10);
	jit_pushargi(9);
	jit_pushargi(8);
	jit_pushargi(7);
	jit_pushargi(6);
	jit_pushargi(5);
	jit_pushargi(4);
	jit_pushargi(3);
	jit_pushargi(2);
	jit_pushargi(1);
    }
    jit_patch_at(jit_finishi(NULL), jw);
    jit_prepare();
    {
	jit_pushargi_f(16);
	jit_pushargi_f(15);
	jit_pushargi_f(14);
	jit_pushargi_f(13);
	jit_pushargi_f(12);
	jit_pushargi_f(11);
	jit_pushargi_f(10);
	jit_pushargi_f(9);
	jit_pushargi_f(8);
	jit_pushargi_f(7);
	jit_pushargi_f(6);
	jit_pushargi_f(5);
	jit_pushargi_f(4);
	jit_pushargi_f(3);
	jit_pushargi_f(2);
	jit_pushargi_f(1);
    }
    jit_patch_at(jit_finishi(NULL), jf);
    jit_prepare();
    {
	jit_pushargi_d(16);
	jit_pushargi_d(15);
	jit_pushargi_d(14);
	jit_pushargi_d(13);
	jit_pushargi_d(12);
	jit_pushargi_d(11);
	jit_pushargi_d(10);
	jit_pushargi_d(9);
	jit_pushargi_d(8);
	jit_pushargi_d(7);
	jit_pushargi_d(6);
	jit_pushargi_d(5);
	jit_pushargi_d(4);
	jit_pushargi_d(3);
	jit_pushargi_d(2);
	jit_pushargi_d(1);
    }
    jit_patch_at(jit_finishi(NULL), jd);
    jit_ret();
    jit_epilog();

    code = jit_emit();
    jit_clear_state();

    (*code)();

    jit_destroy_state();
    finish_jit();
    return (0);
}
