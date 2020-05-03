#ifndef CLIB_DCHECK_H
#define CLIB_DCHECK_H

#include "check.h"

// check and bail if nope. however, this only works if `-D DCHECK_IS_ON`
// or equivalent is defined
#ifdef DCHECK_IS_ON
#  define DCHECK(...) CHECK(__VA_ARGS__);
#else
#  define DCHECK(expr) ((void) expr);
#endif

#endif
