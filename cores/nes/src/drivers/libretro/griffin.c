#ifdef WANT_GRIFFIN

#include "drivers/libretro/libretro.c"

#ifndef STATIC_LINKING
#include "drivers/libretro/libretro-common/streams/memory_stream.c"
#endif

#include "cart.c"
#include "cheat.c"
#include "crc32.c"

#ifdef DEBUG
#include "debug.c"
#endif
#include "fceu-endian.c"
#include "fceu-memory.c"
#include "misc.c"
#include "fceu.c"
#include "fds.c"
#include "file.c"
#include "filter.c"
#include "general.c"
#include "input.c"
#include "md5.c"
#include "nsf.c"
#include "palette.c"
#include "ppu.c"
#include "sound.c"
#include "state.c"
#include "video.c"
#include "vsuni.c"

//#include "x6502.c"
//#include "ines.c"
//#include "unif.c"

#endif
