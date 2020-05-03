/**
 *
 * You can read more about these macros on chromium.org.
 * Go to the direct link - https://www.chromium.org/developers/coding-style#TOC-CHECK-DCHECK-and-NOTREACHED-
 *
 *     ... other source files should be here in this repo or folder or w/e
 *
 */

#ifndef CLIB_CHECK_H
#define CLIB_CHECK_H

// thank you brother tj & stephen (https://github.com/clibs/unlikely)
#if defined(__GNUC__) && (__GNUC__ > 2) && defined(__OPTIMIZE__)
#  define likely(expr) (__builtin_expect(!!(expr), 1))
#  define unlikely(expr) (__builtin_expect(!!(expr), 0))
#else
#  define likely(expr) (1 == !!(expr))
#  define unlikely(expr) (0 == !!(expr))
#endif

// overload if you want your own abort fn - w/e
#ifndef CHECK_ABORT_FUNCTION
// need that prototype def
# include <stdlib.h>
# define CHECK_ABORT_FUNCTION abort
#endif

// check and bail if nope
#ifndef CHECK
# define CHECK(expr) if (!likely(expr)) { CHECK_ABORT_FUNCTION(); }
#endif

#endif
