/******************************************************************************\
* Project:  Module Subsystem Interface to SP Interpreter Core                  *
* Authors:  Iconoclast                                                         *
* Release:  2016.03.23                                                         *
* License:  CC0 Public Domain Dedication                                       *
*                                                                              *
* To the extent possible under law, the author(s) have dedicated all copyright *
* and related and neighboring rights to this software to the public domain     *
* worldwide. This software is distributed without any warranty.                *
*                                                                              *
* You should have received a copy of the CC0 Public Domain Dedication along    *
* with this software.                                                          *
* If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.             *
\******************************************************************************/

#define LIBRETRO

#if defined(USE_SSE2NEON) && defined(__ARM_NEON__)
#include "sse2neon/SSE2NEON.h"
#define ARCH_MIN_SSE2
#endif

#include "vu/add.c"
#include "vu/divide.c"
#include "vu/logical.c"
#include "vu/multiply.c"
#include "vu/select.c"
#include "vu/vu.c"
#include "su.c"
#include "module.c"

unsigned char rsp_conf[32];
