#include <lightning.h>
#include <stdio.h>
#include <assert.h>

int
main(int argc, char *argv[])
{
    jit_state_t		*_jit;

    /* Same JIT_XY are not constants */
    init_jit(argv[0]);

    _jit = jit_new_state();
    assert(JIT_R0 == jit_r(0));
    (void)jit_callee_save_p(JIT_R0);
    assert(JIT_R1 == jit_r(1));
    (void)jit_callee_save_p(JIT_R1);
    assert(JIT_R2 == jit_r(2));
    (void)jit_callee_save_p(JIT_R2);
#if defined(JIT_R3)
    assert(JIT_R3 == jit_r(3));
    (void)jit_callee_save_p(JIT_R3);
#  if defined(JIT_R4)
    assert(JIT_R4 == jit_r(4));
    (void)jit_callee_save_p(JIT_R4);
#    if defined(JIT_R5)
    assert(JIT_R5 == jit_r(5));
    (void)jit_callee_save_p(JIT_R5);
#      if defined(JIT_R6)
    assert(JIT_R6 == jit_r(6));
    (void)jit_callee_save_p(JIT_R6);
#        if defined(JIT_R7)
    assert(JIT_R7 == jit_r(7));
    (void)jit_callee_save_p(JIT_R7);
#        endif
#      endif
#    endif
#  endif
#endif
    assert(JIT_V0 == jit_v(0));
    assert(jit_callee_save_p(JIT_V0));
    assert(JIT_V1 == jit_v(1));
    assert(jit_callee_save_p(JIT_V1));
    assert(JIT_V2 == jit_v(2));
    assert(jit_callee_save_p(JIT_V2));
#if defined(JIT_V3)
    assert(JIT_V3 == jit_v(3));
    assert(jit_callee_save_p(JIT_V3));
#  if defined(JIT_V4)
    assert(JIT_V4 == jit_v(4));
    assert(jit_callee_save_p(JIT_V4));
#    if defined(JIT_V5)
    assert(JIT_V5 == jit_v(5));
    assert(jit_callee_save_p(JIT_V5));
#      if defined(JIT_V6)
    assert(JIT_V6 == jit_v(6));
    assert(jit_callee_save_p(JIT_V6));
#        if defined(JIT_V7)
    assert(JIT_V7 == jit_v(7));
    assert(jit_callee_save_p(JIT_V7));
#          if defined(JIT_V8)
    assert(JIT_V8 == jit_v(8));
    assert(jit_callee_save_p(JIT_V8));
#            if defined(JIT_V9)
    assert(JIT_V9 == jit_v(9));
    assert(jit_callee_save_p(JIT_V9));
#              if defined(JIT_V10)
    assert(JIT_V10 == jit_v(10));
    assert(jit_callee_save_p(JIT_V10));
#                if defined(JIT_V11)
    assert(JIT_V11 == jit_v(11));
    assert(jit_callee_save_p(JIT_V11));
#                  if defined(JIT_V12)
    assert(JIT_V12 == jit_v(12));
    assert(jit_callee_save_p(JIT_V12));
#                    if defined(JIT_V13)
    assert(JIT_V13 == jit_v(13));
    assert(jit_callee_save_p(JIT_V13));
#                      if defined(JIT_V14)
    assert(JIT_V14 == jit_v(14));
    assert(jit_callee_save_p(JIT_V14));
#                        if defined(JIT_V15)
    assert(JIT_V15 == jit_v(15));
    assert(jit_callee_save_p(JIT_V15));
#                          if defined(JIT_V16)
    assert(JIT_V16 == jit_v(16));
    assert(jit_callee_save_p(JIT_V16));
#                          endif
#                        endif
#                      endif
#                    endif
#                  endif
#                endif
#              endif
#            endif
#          endif
#        endif
#      endif
#    endif
#  endif
#endif
    assert(JIT_F0 == jit_f(0));
    (void)jit_callee_save_p(JIT_F0);
    assert(JIT_F1 == jit_f(1));
    (void)jit_callee_save_p(JIT_F1);
    assert(JIT_F2 == jit_f(2));
    (void)jit_callee_save_p(JIT_F2);
    assert(JIT_F3 == jit_f(3));
    (void)jit_callee_save_p(JIT_F3);
    assert(JIT_F4 == jit_f(4));
    (void)jit_callee_save_p(JIT_F4);
    assert(JIT_F5 == jit_f(5));
    (void)jit_callee_save_p(JIT_F5);
#if defined(JIT_F6)
    assert(JIT_F6 == jit_f(6));
    (void)jit_callee_save_p(JIT_F6);
#  if defined(JIT_F7)
    assert(JIT_F7 == jit_f(7));
    (void)jit_callee_save_p(JIT_F7);
#    if defined(JIT_F8)
    assert(JIT_F8 == jit_f(8));
    (void)jit_callee_save_p(JIT_F8);
#      if defined(JIT_F9)
    assert(JIT_F9 == jit_f(9));
    (void)jit_callee_save_p(JIT_F9);
#      endif
#    endif
#  endif
#endif

    jit_clear_state();
    jit_destroy_state();
    finish_jit();

    return (0);
}
