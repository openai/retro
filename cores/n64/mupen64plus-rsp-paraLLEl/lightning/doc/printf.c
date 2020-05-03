#include <stdio.h>
#include <lightning.h>

static jit_state_t *_jit;

typedef void (*pvfi)(int);    /* Pointer to Void Function of Int */

int main(int argc, char *argv[])
{
  pvfi          myFunction;             /* ptr to generated code */
  jit_node_t    *start, *end;           /* a couple of labels */
  jit_node_t    *in;                    /* to get the argument */

  init_jit(argv[0]);
  _jit = jit_new_state();

  start = jit_note(__FILE__, __LINE__);
  jit_prolog();
  in = jit_arg();
  jit_getarg(JIT_R1, in);
  jit_prepare();
  jit_pushargi((jit_word_t)"generated %d bytes\n");
  jit_ellipsis();
  jit_pushargr(JIT_R1);
  jit_finishi(printf);
  jit_ret();
  jit_epilog();
  end = jit_note(__FILE__, __LINE__);

  myFunction = jit_emit();

  /* call the generated code, passing its size as argument */
  myFunction((char*)jit_address(end) - (char*)jit_address(start));
  jit_clear_state();

  jit_disassemble();

  jit_destroy_state();
  finish_jit();
  return 0;
}
