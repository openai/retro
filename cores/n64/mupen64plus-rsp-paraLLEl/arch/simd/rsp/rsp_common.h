//
// arch/x86_64/rsp/rsp.h
//
// Extern declarations for host RSP functions.
//
// This file is subject to the terms and conditions defined in
// 'LICENSE', which is part of this source code package.
//

#ifndef __arch_rsp_h__
#define __arch_rsp_h__

#ifdef __ARM_NEON
#define __SSSE3__
#include "SSE2NEON.h"
#elif defined(__SSE4_2__)
#include <nmmintrin.h>
#elif defined(__SSE4_1__)
#include <smmintrin.h>
#elif defined(__SSSE3__)
#include <tmmintrin.h>
#elif defined(__SSE3__)
#include <pmmintrin.h>
#else
#include <emmintrin.h>
#endif

#include <stdint.h>

typedef __m128i rsp_vect_t;

namespace RSP
{
struct CPUState;
}

// Loads and shuffles a 16x8 vector according to element.
#ifdef __SSSE3__
extern const uint16_t shuffle_keys[16][8];

static inline __m128i rsp_vect_load_and_shuffle_operand(const uint16_t *src, unsigned element)
{
	__m128i operand = _mm_load_si128((__m128i *)src);
	__m128i key = _mm_load_si128((__m128i *)shuffle_keys[element]);

	return _mm_shuffle_epi8(operand, key);
}
#else
__m128i rsp_vect_load_and_shuffle_operand(const uint16_t *src, unsigned element);
#endif

// Loads a vector without shuffling its elements.
static inline __m128i rsp_vect_load_unshuffled_operand(const uint16_t *src)
{
	return _mm_load_si128((__m128i *)src);
}

// Writes an operand back to memory.
static inline void rsp_vect_write_operand(uint16_t *dest, __m128i src)
{
	_mm_store_si128((__m128i *)dest, src);
}

static inline __m128i read_acc_lo(const uint16_t *acc)
{
	return rsp_vect_load_unshuffled_operand(acc + 16);
}
static inline __m128i read_acc_md(const uint16_t *acc)
{
	return rsp_vect_load_unshuffled_operand(acc + 8);
}
static inline __m128i read_acc_hi(const uint16_t *acc)
{
	return rsp_vect_load_unshuffled_operand(acc);
}
static inline __m128i read_vcc_lo(const uint16_t *vcc)
{
	return rsp_vect_load_unshuffled_operand(vcc + 8);
}
static inline __m128i read_vcc_hi(const uint16_t *vcc)
{
	return rsp_vect_load_unshuffled_operand(vcc);
}
static inline __m128i read_vco_lo(const uint16_t *vco)
{
	return rsp_vect_load_unshuffled_operand(vco + 8);
}
static inline __m128i read_vco_hi(const uint16_t *vco)
{
	return rsp_vect_load_unshuffled_operand(vco);
}
static inline __m128i read_vce(const uint16_t *vce)
{
	return rsp_vect_load_unshuffled_operand(vce + 8);
}
static inline void write_acc_lo(uint16_t *acc, __m128i acc_lo)
{
	rsp_vect_write_operand(acc + 16, acc_lo);
}
static inline void write_acc_md(uint16_t *acc, __m128i acc_md)
{
	rsp_vect_write_operand(acc + 8, acc_md);
}
static inline void write_acc_hi(uint16_t *acc, __m128i acc_hi)
{
	rsp_vect_write_operand(acc, acc_hi);
}
static inline void write_vcc_lo(uint16_t *vcc, __m128i vcc_lo)
{
	rsp_vect_write_operand(vcc + 8, vcc_lo);
}
static inline void write_vcc_hi(uint16_t *vcc, __m128i vcc_hi)
{
	rsp_vect_write_operand(vcc, vcc_hi);
}
static inline void write_vco_lo(uint16_t *vco, __m128i vco_lo)
{
	rsp_vect_write_operand(vco + 8, vco_lo);
}
static inline void write_vco_hi(uint16_t *vco, __m128i vco_hi)
{
	rsp_vect_write_operand(vco, vco_hi);
}
static inline void write_vce(uint16_t *vce, __m128i vce_r)
{
	rsp_vect_write_operand(vce + 8, vce_r);
}

// Returns scalar bitmasks for VCO/VCC/VCE.
static inline int16_t rsp_get_flags(const uint16_t *flags)
{
	return (int16_t)_mm_movemask_epi8(
	    _mm_packs_epi16(_mm_load_si128((__m128i *)(flags + 8)), _mm_load_si128((__m128i *)(flags + 0))));
}

void rsp_set_flags(uint16_t *flags, uint16_t rt);

// Zeroes out a vector register.
static inline __m128i rsp_vzero(void)
{
	return _mm_setzero_si128();
}

extern const uint16_t vdiv_mask_table[8][8];

#define HES(x) ((x) ^ 2)
#define BES(x) ((x) ^ 3)
#define MES(x) ((x) ^ 1)

#define READ_MEM_U8(mem, addr) (reinterpret_cast<const uint8_t *>(mem)[BES(addr)])
#define READ_MEM_U16(mem, addr) (reinterpret_cast<const uint16_t *>(mem)[HES(addr) >> 1])
#define READ_MEM_U32(mem, addr) (reinterpret_cast<const uint32_t *>(mem)[addr >> 2])

#define WRITE_MEM_U8(mem, addr, data) (reinterpret_cast<uint8_t *>(mem)[BES(addr)] = data)
#define WRITE_MEM_U16(mem, addr, data) (reinterpret_cast<uint16_t *>(mem)[HES(addr) >> 1] = data)
#define WRITE_MEM_U32(mem, addr, data) (reinterpret_cast<uint32_t *>(mem)[addr >> 2] = data)

#endif
