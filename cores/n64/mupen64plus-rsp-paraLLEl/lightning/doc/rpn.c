#include <stdio.h>
#include <lightning.h>

typedef int (*pifi)(int);       /* Pointer to Int Function of Int */

static jit_state_t *_jit;

void stack_push(int reg, int *sp)
{
  jit_stxi_i (*sp, JIT_FP, reg);
  *sp += sizeof (int);
}

void stack_pop(int reg, int *sp)
{
  *sp -= sizeof (int);
  jit_ldxi_i (reg, JIT_FP, *sp);
}

jit_node_t *compile_rpn(char *expr)
{
  jit_node_t *in, *fn;
  int stack_base, stack_ptr;

  fn = jit_note(NULL, 0);
  jit_prolog();
  in = jit_arg();
  stack_ptr = stack_base = jit_allocai (32 * sizeof (int));

  jit_getarg_i(JIT_R2, in);

  while (*expr) {
    char buf[32];
    int n;
    if (sscanf(expr, "%[0-9]%n", buf, &n)) {
      expr += n - 1;
      stack_push(JIT_R0, &stack_ptr);
      jit_movi(JIT_R0, atoi(buf));
    } else if (*expr == 'x') {
      stack_push(JIT_R0, &stack_ptr);
      jit_movr(JIT_R0, JIT_R2);
    } else if (*expr == '+') {
      stack_pop(JIT_R1, &stack_ptr);
      jit_addr(JIT_R0, JIT_R1, JIT_R0);
    } else if (*expr == '-') {
      stack_pop(JIT_R1, &stack_ptr);
      jit_subr(JIT_R0, JIT_R1, JIT_R0);
    } else if (*expr == '*') {
      stack_pop(JIT_R1, &stack_ptr);
      jit_mulr(JIT_R0, JIT_R1, JIT_R0);
    } else if (*expr == '/') {
      stack_pop(JIT_R1, &stack_ptr);
      jit_divr(JIT_R0, JIT_R1, JIT_R0);
    } else {
      fprintf(stderr, "cannot compile: %s\n", expr);
      abort();
    }
    ++expr;
  }
  jit_retr(JIT_R0);
  jit_epilog();
  return fn;
}

int main(int argc, char *argv[])
{
  jit_node_t *nc, *nf;
  pifi c2f, f2c;
  int i;

  init_jit(argv[0]);
  _jit = jit_new_state();

  nc = compile_rpn("32x9*5/+");
  nf = compile_rpn("x32-5*9/");
  (void)jit_emit();
  c2f = (pifi)jit_address(nc);
  f2c = (pifi)jit_address(nf);
  jit_clear_state();

  printf("\nC:");
  for (i = 0; i <= 100; i += 10) printf("%3d ", i);
  printf("\nF:");
  for (i = 0; i <= 100; i += 10) printf("%3d ", c2f(i));
  printf("\n");

  printf("\nF:");
  for (i = 32; i <= 212; i += 18) printf("%3d ", i);
  printf("\nC:");
  for (i = 32; i <= 212; i += 18) printf("%3d ", f2c(i));
  printf("\n");

  jit_destroy_state();
  finish_jit();
  return 0;
}
