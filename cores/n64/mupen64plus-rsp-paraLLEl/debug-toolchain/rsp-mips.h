#ifndef __RSP_MIPS_H
#define __RSP_MIPS_H

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef u32 size_t;

void rsp_break(void);
void rsp_debug_break(u32 a, u32 b, u32 c, u32 d);

#endif
