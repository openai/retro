/*
 * Simple test of using an alternate buffer for the code.
 */

#include <lightning.h>
#include <stdio.h>
#include <assert.h>
#include <sys/mman.h>
#if defined(__sgi)
#  include <fcntl.h>
#endif

#ifndef MAP_ANON
#  define MAP_ANON			MAP_ANONYMOUS
#  ifndef MAP_ANONYMOUS
#    define MAP_ANONYMOUS		0
#  endif
#endif

#if !defined(__sgi)
#define  mmap_fd			-1
#endif

jit_uint8_t		 *data;
jit_state_t		 *_jit;
jit_word_t		  data_length;
jit_word_t		  note_length;
#if defined(__sgi)
int			  mmap_fd;
#endif
void			(*function)(void);

void
gencode(jit_word_t flags)
{
    jit_word_t		offset;
    jit_word_t		length;

    _jit = jit_new_state();

    jit_name("main");
    jit_prolog();
    jit_prepare();
    jit_pushargi((jit_word_t)"%f\n");
    jit_ellipsis();
    jit_pushargi_d(1.5);
    jit_finishi(printf);
    jit_note("nodata.c", __LINE__);

    /* call to jit_realize() is only required when using an alternate
     * code buffer. Note that not using mmap'ed memory may not work
     * on several ports and/or operating system versions */
    jit_realize();

    if (jit_get_data(&data_length, &note_length) != NULL)
	abort();

    length = 0;
    if (!(flags & JIT_DISABLE_DATA))
	length += data_length;
    if (!(flags & JIT_DISABLE_NOTE))
	length += note_length;

    /* check that a too small buffer fails */
    if (flags)
	jit_set_data(length ? data : NULL, length, flags);

    /* and calling again with enough space works */
    offset = (length + 7) & -8;
    function = jit_emit();
    if (function == NULL)
	abort();

    jit_clear_state();
    (*function)();
    jit_destroy_state();
}

int
main(int argc, char *argv[])
{
#if defined(__sgi)
    mmap_fd = open("/dev/zero", O_RDWR);
#endif

    data = mmap(NULL, 4096,
		PROT_READ | PROT_WRITE,
		MAP_PRIVATE | MAP_ANON, mmap_fd, 0);
    assert(data != MAP_FAILED);
#if defined(__sgi)
    close(mmap_fd);
#endif

    init_jit(argv[0]);

    gencode(0);
    gencode(JIT_DISABLE_DATA);
    gencode(JIT_DISABLE_NOTE);
    gencode(JIT_DISABLE_DATA | JIT_DISABLE_NOTE);

    finish_jit();

    munmap(data, 4096);

    return (0);
}
