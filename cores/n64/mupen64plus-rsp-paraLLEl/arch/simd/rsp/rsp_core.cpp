//
// arch/x86_64/rsp/rsp.c
//
// Declarations for host RSP functions.
//
// This file is subject to the terms and conditions defined in
// 'LICENSE', which is part of this source code package.
//

#include "../../../state.hpp"
#include "rsp_common.h"
#include <string.h>

#ifdef __SSSE3__
//
// This table is used to "shuffle" the RSP vector after loading it.
//
alignas(64) const uint16_t shuffle_keys[16][8] = {
	/* -- */ { 0x0100, 0x0302, 0x0504, 0x0706, 0x0908, 0x0B0A, 0x0D0C, 0x0F0E },
	/* -- */ { 0x0100, 0x0302, 0x0504, 0x0706, 0x0908, 0x0B0A, 0x0D0C, 0x0F0E },

	/* 0q */ { 0x0100, 0x0100, 0x0504, 0x0504, 0x0908, 0x0908, 0x0D0C, 0x0D0C },
	/* 1q */ { 0x0302, 0x0302, 0x0706, 0x0706, 0x0B0A, 0x0B0A, 0x0F0E, 0x0F0E },

	/* 0h */ { 0x0100, 0x0100, 0x0100, 0x0100, 0x0908, 0x0908, 0x0908, 0x0908 },
	/* 1h */ { 0x0302, 0x0302, 0x0302, 0x0302, 0x0B0A, 0x0B0A, 0x0B0A, 0x0B0A },
	/* 2h */ { 0x0504, 0x0504, 0x0504, 0x0504, 0x0D0C, 0x0D0C, 0x0D0C, 0x0D0C },
	/* 3h */ { 0x0706, 0x0706, 0x0706, 0x0706, 0x0F0E, 0x0F0E, 0x0F0E, 0x0F0E },

	/* 0w */ { 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100 },
	/* 1w */ { 0x0302, 0x0302, 0x0302, 0x0302, 0x0302, 0x0302, 0x0302, 0x0302 },
	/* 2w */ { 0x0504, 0x0504, 0x0504, 0x0504, 0x0504, 0x0504, 0x0504, 0x0504 },
	/* 3w */ { 0x0706, 0x0706, 0x0706, 0x0706, 0x0706, 0x0706, 0x0706, 0x0706 },
	/* 4w */ { 0x0908, 0x0908, 0x0908, 0x0908, 0x0908, 0x0908, 0x0908, 0x0908 },
	/* 5w */ { 0x0B0A, 0x0B0A, 0x0B0A, 0x0B0A, 0x0B0A, 0x0B0A, 0x0B0A, 0x0B0A },
	/* 6w */ { 0x0D0C, 0x0D0C, 0x0D0C, 0x0D0C, 0x0D0C, 0x0D0C, 0x0D0C, 0x0D0C },
	/* 7w */ { 0x0F0E, 0x0F0E, 0x0F0E, 0x0F0E, 0x0F0E, 0x0F0E, 0x0F0E, 0x0F0E },
};
#endif

//
// These tables are used to shift data loaded from DMEM.
// In addition to shifting, they also take into account that
// DMEM uses big-endian byte ordering, whereas vectors are
// 2-byte little-endian.
//

// Shift left LUT; shifts in zeros from the right, one byte at a time.
alignas(64) static const uint16_t sll_b2l_keys[16][8] = {
	{ 0x0001, 0x0203, 0x0405, 0x0607, 0x0809, 0x0A0B, 0x0C0D, 0x0E0F },
	{ 0x8000, 0x0102, 0x0304, 0x0506, 0x0708, 0x090A, 0x0B0C, 0x0D0E },
	{ 0x8080, 0x0001, 0x0203, 0x0405, 0x0607, 0x0809, 0x0A0B, 0x0C0D },
	{ 0x8080, 0x8000, 0x0102, 0x0304, 0x0506, 0x0708, 0x090A, 0x0B0C },

	{ 0x8080, 0x8080, 0x0001, 0x0203, 0x0405, 0x0607, 0x0809, 0x0A0B },
	{ 0x8080, 0x8080, 0x8000, 0x0102, 0x0304, 0x0506, 0x0708, 0x090A },
	{ 0x8080, 0x8080, 0x8080, 0x0001, 0x0203, 0x0405, 0x0607, 0x0809 },
	{ 0x8080, 0x8080, 0x8080, 0x8000, 0x0102, 0x0304, 0x0506, 0x0708 },

	{ 0x8080, 0x8080, 0x8080, 0x8080, 0x0001, 0x0203, 0x0405, 0x0607 },
	{ 0x8080, 0x8080, 0x8080, 0x8080, 0x8000, 0x0102, 0x0304, 0x0506 },
	{ 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x0001, 0x0203, 0x0405 },
	{ 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8000, 0x0102, 0x0304 },

	{ 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x0001, 0x0203 },
	{ 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8000, 0x0102 },
	{ 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x0001 },
	{ 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8000 },
};

// Shift left LUT; shirts low order to high order, inserting 0x00s.
alignas(64) static const uint16_t sll_l2b_keys[16][8] = {
	{ 0x0001, 0x0203, 0x0405, 0x0607, 0x0809, 0x0A0B, 0x0C0D, 0x0E0F },
	{ 0x0180, 0x0300, 0x0502, 0x0704, 0x0906, 0x0B08, 0x0D0A, 0x0E0C },
	{ 0x8080, 0x0001, 0x0203, 0x0405, 0x0607, 0x0809, 0x0A0B, 0x0C0D },
	{ 0x8080, 0x0180, 0x0300, 0x0502, 0x0704, 0x0906, 0x0B08, 0x0D0A },

	{ 0x8080, 0x8080, 0x0001, 0x0203, 0x0405, 0x0607, 0x0809, 0x0A0B },
	{ 0x8080, 0x8080, 0x0180, 0x0300, 0x0502, 0x0704, 0x0906, 0x0B08 },
	{ 0x8080, 0x8080, 0x8080, 0x0001, 0x0203, 0x0405, 0x0607, 0x0809 },
	{ 0x8080, 0x8080, 0x8080, 0x0180, 0x0300, 0x0502, 0x0704, 0x0906 },

	{ 0x8080, 0x8080, 0x8080, 0x8080, 0x0001, 0x0203, 0x0405, 0x0607 },
	{ 0x8080, 0x8080, 0x8080, 0x8080, 0x0180, 0x0300, 0x0502, 0x0704 },
	{ 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x0001, 0x0203, 0x0405 },
	{ 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x0180, 0x0300, 0x0502 },

	{ 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x0001, 0x0203 },
	{ 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x0180, 0x0300 },
	{ 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x0001 },
	{ 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x0180 },
};

// Shift right LUT; shifts in zeros from the left, one byte at a time.
alignas(64) static const uint16_t srl_b2l_keys[16][8] = {
	{ 0x0001, 0x0203, 0x0405, 0x0607, 0x0809, 0x0A0B, 0x0C0D, 0x0E0F },
	{ 0x0102, 0x0304, 0x0506, 0x0708, 0x090A, 0x0B0C, 0x0D0E, 0x0F80 },
	{ 0x0203, 0x0405, 0x0607, 0x0809, 0x0A0B, 0x0C0D, 0x0E0F, 0x8080 },
	{ 0x0304, 0x0506, 0x0708, 0x090A, 0x0B0C, 0x0D0E, 0x0F80, 0x8080 },

	{ 0x0405, 0x0607, 0x0809, 0x0A0B, 0x0C0D, 0x0E0F, 0x8080, 0x8080 },
	{ 0x0506, 0x0708, 0x090A, 0x0B0C, 0x0D0E, 0x0F80, 0x8080, 0x8080 },
	{ 0x0607, 0x0809, 0x0A0B, 0x0C0D, 0x0E0F, 0x8080, 0x8080, 0x8080 },
	{ 0x0708, 0x090A, 0x0B0C, 0x0D0E, 0x0F80, 0x8080, 0x8080, 0x8080 },

	{ 0x0809, 0x0A0B, 0x0C0D, 0x0E0F, 0x8080, 0x8080, 0x8080, 0x8080 },
	{ 0x090A, 0x0B0C, 0x0D0E, 0x0F80, 0x8080, 0x8080, 0x8080, 0x8080 },
	{ 0x0A0B, 0x0C0D, 0x0E0F, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080 },
	{ 0x0B0C, 0x0D0E, 0x0F80, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080 },

	{ 0x0C0D, 0x0E0F, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080 },
	{ 0x0D0E, 0x0F80, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080 },
	{ 0x0E0F, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080 },
	{ 0x0F80, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080 },
};

alignas(64) static const uint16_t ror_b2l_keys[16][8] = {
	{ 0x0001, 0x0203, 0x0405, 0x0607, 0x0809, 0x0A0B, 0x0C0D, 0x0E0F },
	{ 0x0102, 0x0304, 0x0506, 0x0708, 0x090A, 0x0B0C, 0x0D0E, 0x0F00 },
	{ 0x0203, 0x0405, 0x0607, 0x0809, 0x0A0B, 0x0C0D, 0x0E0F, 0x0001 },
	{ 0x0304, 0x0506, 0x0708, 0x090A, 0x0B0C, 0x0D0E, 0x0F00, 0x0102 },

	{ 0x0405, 0x0607, 0x0809, 0x0A0B, 0x0C0D, 0x0E0F, 0x0001, 0x0203 },
	{ 0x0506, 0x0708, 0x090A, 0x0B0C, 0x0D0E, 0x0F00, 0x0102, 0x0304 },
	{ 0x0607, 0x0809, 0x0A0B, 0x0C0D, 0x0E0F, 0x0001, 0x0203, 0x0405 },
	{ 0x0708, 0x090A, 0x0B0C, 0x0D0E, 0x0F00, 0x0102, 0x0304, 0x0506 },

	{ 0x0809, 0x0A0B, 0x0C0D, 0x0E0F, 0x0001, 0x0203, 0x0405, 0x0607 },
	{ 0x090A, 0x0B0C, 0x0D0E, 0x0F00, 0x0102, 0x0304, 0x0506, 0x0708 },
	{ 0x0A0B, 0x0C0D, 0x0E0F, 0x0001, 0x0203, 0x0405, 0x0607, 0x0809 },
	{ 0x0B0C, 0x0D0E, 0x0F00, 0x0102, 0x0304, 0x0506, 0x0708, 0x090A },

	{ 0x0C0D, 0x0E0F, 0x0001, 0x0203, 0x0405, 0x0607, 0x0809, 0x0A0B },
	{ 0x0D0E, 0x0F00, 0x0102, 0x0304, 0x0506, 0x0708, 0x090A, 0x0B0C },
	{ 0x0E0F, 0x0001, 0x0203, 0x0405, 0x0607, 0x0809, 0x0A0B, 0x0C0D },
	{ 0x0F00, 0x0102, 0x0304, 0x0506, 0x0708, 0x090A, 0x0B0C, 0x0D0E },
};

// Rotate left LUT; rotates high order bytes back to low order.
alignas(64) static const uint16_t rol_l2b_keys[16][8] = {
	{ 0x0001, 0x0203, 0x0405, 0x0607, 0x0809, 0x0A0B, 0x0C0D, 0x0E0F },
	{ 0x010E, 0x0300, 0x0502, 0x0704, 0x0906, 0x0B08, 0x0D0A, 0x0F0C },
	{ 0x0E0F, 0x0001, 0x0203, 0x0405, 0x0607, 0x0809, 0x0A0B, 0x0C0D },
	{ 0x0F0C, 0x010E, 0x0300, 0x0502, 0x0704, 0x0906, 0x0B08, 0x0D0A },

	{ 0x0C0D, 0x0E0F, 0x0001, 0x0203, 0x0405, 0x0607, 0x0809, 0x0A0B },
	{ 0x0D0A, 0x0F0C, 0x010E, 0x0300, 0x0502, 0x0704, 0x0906, 0x0B08 },
	{ 0x0A0B, 0x0C0D, 0x0E0F, 0x0001, 0x0203, 0x0405, 0x0607, 0x0809 },
	{ 0x0B08, 0x0D0A, 0x0F0C, 0x010E, 0x0300, 0x0502, 0x0704, 0x0906 },

	{ 0x0809, 0x0A0B, 0x0C0D, 0x0E0F, 0x0001, 0x0203, 0x0405, 0x0607 },
	{ 0x0906, 0x0B08, 0x0D0A, 0x0F0C, 0x010E, 0x0300, 0x0502, 0x0704 },
	{ 0x0607, 0x0809, 0x0A0B, 0x0C0D, 0x0E0F, 0x0001, 0x0203, 0x0405 },
	{ 0x0704, 0x0906, 0x0B08, 0x0D0A, 0x0F0C, 0x010E, 0x0300, 0x0502 },

	{ 0x0405, 0x0607, 0x0809, 0x0A0B, 0x0C0D, 0x0E0F, 0x0001, 0x0203 },
	{ 0x0502, 0x0704, 0x0906, 0x0B08, 0x0D0A, 0x0F0C, 0x010E, 0x0300 },
	{ 0x0203, 0x0405, 0x0607, 0x0809, 0x0A0B, 0x0C0D, 0x0E0F, 0x0001 },
	{ 0x0300, 0x0502, 0x0704, 0x0906, 0x0B08, 0x0D0A, 0x0F0C, 0x010E },
};

// Rotate right LUT; rotates high order bytes back to low order.
alignas(64) static const uint16_t ror_l2b_keys[16][8] = {
	{ 0x0001, 0x0203, 0x0405, 0x0607, 0x0809, 0x0A0B, 0x0C0D, 0x0E0F },
	{ 0x0300, 0x0502, 0x0704, 0x0906, 0x0B08, 0x0D0A, 0x0F0C, 0x010E },
	{ 0x0203, 0x0405, 0x0607, 0x0809, 0x0A0B, 0x0C0D, 0x0E0F, 0x0001 },
	{ 0x0502, 0x0704, 0x0906, 0x0B08, 0x0D0A, 0x0F0C, 0x010E, 0x0300 },

	{ 0x0405, 0x0607, 0x0809, 0x0A0B, 0x0C0D, 0x0E0F, 0x0001, 0x0203 },
	{ 0x0704, 0x0906, 0x0B08, 0x0D0A, 0x0F0C, 0x010E, 0x0300, 0x0502 },
	{ 0x0607, 0x0809, 0x0A0B, 0x0C0D, 0x0E0F, 0x0001, 0x0203, 0x0405 },
	{ 0x0906, 0x0B08, 0x0D0A, 0x0F0C, 0x010E, 0x0300, 0x0502, 0x0704 },

	{ 0x0809, 0x0A0B, 0x0C0D, 0x0E0F, 0x0001, 0x0203, 0x0405, 0x0607 },
	{ 0x0B08, 0x0D0A, 0x0F0C, 0x010E, 0x0300, 0x0502, 0x0704, 0x0906 },
	{ 0x0A0B, 0x0C0D, 0x0E0F, 0x0001, 0x0203, 0x0405, 0x0607, 0x0809 },
	{ 0x0D0A, 0x0F0C, 0x010E, 0x0300, 0x0502, 0x0704, 0x0906, 0x0B08 },

	{ 0x0C0D, 0x0E0F, 0x0001, 0x0203, 0x0405, 0x0607, 0x0809, 0x0A0B },
	{ 0x0F0C, 0x010E, 0x0300, 0x0502, 0x0704, 0x0906, 0x0B08, 0x0D0A },
	{ 0x0E0F, 0x0001, 0x0203, 0x0405, 0x0607, 0x0809, 0x0A0B, 0x0C0D },
	{ 0x010E, 0x0300, 0x0502, 0x0704, 0x0906, 0x0B08, 0x0D0A, 0x0F0C },
};

#ifndef __SSSE3__
static inline __m128i sse2_pshufb_loop8(__m128i v, const uint8_t *keys)
{
	alignas(16) uint8_t temp[(0x80 | 128) + 1];
	unsigned j;

	_mm_store_si128((__m128i *)temp, v);
	temp[0x80] = 0;

#if 0
  for (j = 0; j < 16; j++)
    temp[j + 16] = temp[keys[j]];
#else
	for (j = 0; j < 16; j += 4)
	{
		temp[j + 16] = temp[keys[j + 0]];
		temp[j + 17] = temp[keys[j + 1]];
		temp[j + 18] = temp[keys[j + 2]];
		temp[j + 19] = temp[keys[j + 3]];
	}
#endif

	return _mm_load_si128(((__m128i *)temp) + 1);
}
static inline __m128i sse2_pshufb(__m128i v, const uint16_t *keys)
{
	union {
		const uint16_t *k16;
		const uint8_t *k8;
	} x;
	x.k16 = keys;
	return sse2_pshufb_loop8(v, x.k8);
}
#endif

// Uses a LUT to populate flag registers.
void rsp_set_flags(uint16_t *flags, uint16_t rt)
{
	unsigned i;

	static const uint16_t array[16][4] = {
		{ 0x0000, 0x0000, 0x0000, 0x0000 }, { 0xFFFF, 0x0000, 0x0000, 0x0000 }, { 0x0000, 0xFFFF, 0x0000, 0x0000 },
		{ 0xFFFF, 0xFFFF, 0x0000, 0x0000 }, { 0x0000, 0x0000, 0xFFFF, 0x0000 }, { 0xFFFF, 0x0000, 0xFFFF, 0x0000 },
		{ 0x0000, 0xFFFF, 0xFFFF, 0x0000 }, { 0xFFFF, 0xFFFF, 0xFFFF, 0x0000 }, { 0x0000, 0x0000, 0x0000, 0xFFFF },
		{ 0xFFFF, 0x0000, 0x0000, 0xFFFF }, { 0x0000, 0xFFFF, 0x0000, 0xFFFF }, { 0xFFFF, 0xFFFF, 0x0000, 0xFFFF },
		{ 0x0000, 0x0000, 0xFFFF, 0xFFFF }, { 0xFFFF, 0x0000, 0xFFFF, 0xFFFF }, { 0x0000, 0xFFFF, 0xFFFF, 0xFFFF },
		{ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF },
	};

	for (i = 0; i < 2; i++, rt >>= 4)
		memcpy(flags + 8 + i * 4, array[rt & 0xF], sizeof(array[0]));

	for (i = 0; i < 2; i++, rt >>= 4)
		memcpy(flags + 0 + i * 4, array[rt & 0xF], sizeof(array[0]));
}

#ifndef __SSSE3__
__m128i rsp_vect_load_and_shuffle_operand(const uint16_t *src, unsigned element)
{
	__m128i v;

	switch (element)
	{
	case 0:
	case 1:
		v = _mm_load_si128((__m128i *)src);
		return v;

	// element => 0q
	case 2:
		v = _mm_load_si128((__m128i *)src);
		v = _mm_shufflelo_epi16(v, _MM_SHUFFLE(2, 2, 0, 0));
		v = _mm_shufflehi_epi16(v, _MM_SHUFFLE(2, 2, 0, 0));
		return v;

	// element => 1q
	case 3:
		v = _mm_load_si128((__m128i *)src);
		v = _mm_shufflelo_epi16(v, _MM_SHUFFLE(3, 3, 1, 1));
		v = _mm_shufflehi_epi16(v, _MM_SHUFFLE(3, 3, 1, 1));
		return v;

	// element => 0h ... 3h
	case 4:
	case 5:
	case 6:
	case 7:
		__asm__("" : "=x"(v)); /* Do not remove. */
		v = _mm_insert_epi16(v, src[element - 4], 0);
		v = _mm_insert_epi16(v, src[element - 0], 1);
		v = _mm_shufflelo_epi16(v, _MM_SHUFFLE(1, 1, 0, 0));
		v = _mm_shuffle_epi32(v, _MM_SHUFFLE(1, 1, 0, 0));
		return v;

	// element => 0w ... 7w
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
	case 14:
	case 15:
		__asm__("" : "=x"(v)); /* Do not remove. */
		v = _mm_insert_epi16(v, src[element - 8], 0);
		v = _mm_unpacklo_epi16(v, v);
		v = _mm_shuffle_epi32(v, _MM_SHUFFLE(0, 0, 0, 0));
		return v;
	}

#ifdef NDEBUG
	__builtin_unreachable();
#else
	__builtin_trap();
#endif
}
#endif

//
// SSSE3+ accelerated loads for group I. Byteswap big-endian to 2-byte
// little-endian vector. Start at vector element offset, discarding any
// wraparound as necessary.
//
// TODO: Reverse-engineer what happens when loads to vector elements must
//       wraparound. Do we just discard the data, as below, or does the
//       data effectively get rotated around the edge of the vector?
//
void rsp_vload_group1(RSP::CPUState *rsp, uint32_t addr, unsigned element, uint16_t *regp, rsp_vect_t reg,
                      rsp_vect_t dqm)
{
	__m128i data;

	unsigned offset = addr & 0x7;
	unsigned ror = offset - element;

	// Always load in 8-byte chunks to emulate wraparound.
	if (offset)
	{
		uint32_t aligned_addr_lo = addr & ~0x7;
		uint32_t aligned_addr_hi = (aligned_addr_lo + 8) & 0xFFF;
		__m128i temp;

		data = _mm_loadl_epi64((__m128i *)(rsp->dmem + aligned_addr_lo));
		temp = _mm_loadl_epi64((__m128i *)(rsp->dmem + aligned_addr_hi));
		data = _mm_unpacklo_epi64(data, temp);
	}

	else
		data = _mm_loadl_epi64((__m128i *)(rsp->dmem + addr));

		// Shift the DQM up to the point where we mux in the data.
#ifndef __SSSE3__
	dqm = sse2_pshufb(dqm, sll_b2l_keys[element]);
#else
	__m128i ekey = _mm_load_si128((__m128i *)(sll_b2l_keys[element]));
	dqm = _mm_shuffle_epi8(dqm, ekey);
#endif

	// Align the data to the DQM so we can mask it in.
#ifndef __SSSE3__
	data = sse2_pshufb(data, ror_b2l_keys[ror & 0xF]);
#else
	ekey = _mm_load_si128((__m128i *)(ror_b2l_keys[ror & 0xF]));
	data = _mm_shuffle_epi8(data, ekey);
#endif

	// Mask and mux in the data.
#ifdef __SSE4_1__
	reg = _mm_blendv_epi8(reg, data, dqm);
#else
	data = _mm_and_si128(dqm, data);
	reg = _mm_andnot_si128(dqm, reg);
	reg = _mm_or_si128(data, reg);
#endif

	_mm_store_si128((__m128i *)regp, reg);
}

//
// SSSE3+ accelerated loads for group II.
//
// TODO: Reverse-engineer what happens when loads to vector elements must
//       wraparound. Do we just discard the data, as below, or does the
//       data effectively get rotated around the edge of the vector?
//
// TODO: Reverse-engineer what happens when element != 0.
//
void rsp_vload_group2(RSP::CPUState *rsp, uint32_t addr, unsigned element, uint16_t *regp, rsp_vect_t reg,
                      rsp_vect_t dqm)
{
	unsigned offset = addr & 0x7;
	__m128i data, zero;

	// Always load in 8-byte chunks to emulate wraparound.
	if (offset)
	{
		uint32_t aligned_addr_lo = addr & ~0x7;
		uint32_t aligned_addr_hi = (aligned_addr_lo + 8) & 0xFFF;
		uint64_t datalow, datahigh;

		memcpy(&datalow, rsp->dmem + aligned_addr_lo, sizeof(datalow));
		memcpy(&datahigh, rsp->dmem + aligned_addr_hi, sizeof(datahigh));

		// TODO: Get rid of GNU extensions.
		datalow = __builtin_bswap64(datalow);
		datahigh = __builtin_bswap64(datahigh);
		datahigh >>= ((8 - offset) << 3);
		datalow <<= (offset << 3);
		datalow = datahigh | datalow;
		datalow = __builtin_bswap64(datalow);

		data = _mm_loadl_epi64((__m128i *)&datalow);
	}

	else
		data = _mm_loadl_epi64((__m128i *)(rsp->dmem + addr));

	// "Unpack" the data.
	zero = _mm_setzero_si128();
	data = _mm_unpacklo_epi8(zero, data);

#if 0
  if (rsp->pipeline.exdf_latch.request.type != RSP_MEM_REQUEST_PACK)
#endif
	data = _mm_srli_epi16(data, 1);

	_mm_store_si128((__m128i *)regp, data);
}

//
// SSSE3+ accelerated loads for group IV. Byteswap big-endian to 2-byte
// little-endian vector. Stop loading at quadword boundaries.
//
// TODO: Reverse-engineer what happens when loads from vector elements
//       must wraparound (i.e., the address offset is small, starting
//       element is large).
//
void rsp_vload_group4(RSP::CPUState *rsp, uint32_t addr, unsigned element, uint16_t *regp, rsp_vect_t reg,
                      rsp_vect_t dqm)
{
	uint32_t aligned_addr = addr & 0xFF0;
	unsigned offset = addr & 0xF;
	unsigned ror;

	__m128i data = _mm_load_si128((__m128i *)(rsp->dmem + aligned_addr));

	// TODO: Use of element is almost certainly wrong...
	ror = 16 - element + offset;

#if 0
  if (rsp->pipeline.exdf_latch.request.type != RSP_MEM_REQUEST_QUAD)
#endif
	dqm = _mm_cmpeq_epi8(_mm_setzero_si128(), dqm);

#ifndef __SSSE3__
	data = sse2_pshufb(data, ror_b2l_keys[ror & 0xF]);
	dqm = sse2_pshufb(dqm, ror_b2l_keys[ror & 0xF]);
#else
	__m128i dkey = _mm_load_si128((__m128i *)(ror_b2l_keys[ror & 0xF]));
	data = _mm_shuffle_epi8(data, dkey);
	dqm = _mm_shuffle_epi8(dqm, dkey);
#endif

	// Mask and mux in the data.
#ifdef __SSE4_1__
	data = _mm_blendv_epi8(reg, data, dqm);
#else
	data = _mm_and_si128(dqm, data);
	reg = _mm_andnot_si128(dqm, reg);
	data = _mm_or_si128(data, reg);
#endif

	_mm_store_si128((__m128i *)regp, data);
}

//
// SSE3+ accelerated stores for group I. Byteswap 2-byte little-endian
// vector back to big-endian. Start at vector element offset, wrapping
// around the edge of the vector as necessary.
//
// TODO: Reverse-engineer what happens when stores from vector elements
//       must wraparound. Do we just stop storing the data, or do we
//       continue storing from the front of the vector, as below?
//
void rsp_vstore_group1(RSP::CPUState *rsp, uint32_t addr, unsigned element, uint16_t *regp, rsp_vect_t reg,
                       rsp_vect_t dqm)
{
	unsigned offset = addr & 0x7;
	unsigned ror = element - offset;
	__m128i data;

	// Shift the DQM up to the point where we mux in the data.
#ifndef __SSSE3__
	dqm = sse2_pshufb(dqm, sll_l2b_keys[offset]);
#else
	__m128i ekey = _mm_load_si128((__m128i *)(sll_l2b_keys[offset]));
	dqm = _mm_shuffle_epi8(dqm, ekey);
#endif

	// Rotate the reg to align with the DQM.
#ifndef __SSSE3__
	reg = sse2_pshufb(reg, ror_l2b_keys[ror & 0xF]);
#else
	ekey = _mm_load_si128((__m128i *)(ror_l2b_keys[ror & 0xF]));
	reg = _mm_shuffle_epi8(reg, ekey);
#endif

	// Always load in 8-byte chunks to emulate wraparound.
	if (offset)
	{
		uint32_t aligned_addr_lo = addr & ~0x7;
		uint32_t aligned_addr_hi = (aligned_addr_lo + 8) & 0xFFF;
		__m128i temp;

		data = _mm_loadl_epi64((__m128i *)(rsp->dmem + aligned_addr_lo));
		temp = _mm_loadl_epi64((__m128i *)(rsp->dmem + aligned_addr_hi));
		data = _mm_unpacklo_epi64(data, temp);

		// Mask and mux in the data.
#ifdef __SSE4_1__
		data = _mm_blendv_epi8(data, reg, dqm);
#else
		data = _mm_andnot_si128(dqm, data);
		reg = _mm_and_si128(dqm, reg);
		data = _mm_or_si128(data, reg);
#endif

		_mm_storel_epi64((__m128i *)(rsp->dmem + aligned_addr_lo), data);

		data = _mm_srli_si128(data, 8);
		_mm_storel_epi64((__m128i *)(rsp->dmem + aligned_addr_hi), data);
	}

	else
	{
		data = _mm_loadl_epi64((__m128i *)(rsp->dmem + addr));

		// Mask and mux in the data.
#ifdef __SSE4_1__
		data = _mm_blendv_epi8(data, reg, dqm);
#else
		data = _mm_andnot_si128(dqm, data);
		reg = _mm_and_si128(dqm, reg);
		data = _mm_or_si128(data, reg);
#endif

		_mm_storel_epi64((__m128i *)(rsp->dmem + addr), data);
	}
}

//
// SSE3+ accelerated stores for group II. Byteswap 2-byte little-endian
// vector back to big-endian. Start at vector element offset, wrapping
// around the edge of the vector as necessary.
//
// TODO: Reverse-engineer what happens when stores from vector elements
//       must wraparound. Do we just stop storing the data, or do we
//       continue storing from the front of the vector, as below?
//
// TODO: Reverse-engineer what happens when element != 0.
//
void rsp_vstore_group2(RSP::CPUState *rsp, uint32_t addr, unsigned element, uint16_t *regp, rsp_vect_t reg,
                       rsp_vect_t dqm)
{

	// "Pack" the data.
#if 0
  if (rsp->pipeline.exdf_latch.request.type != RSP_MEM_REQUEST_PACK)
#endif
	reg = _mm_slli_epi16(reg, 1);

	reg = _mm_srai_epi16(reg, 8);
	reg = _mm_packs_epi16(reg, reg);

	// TODO: Always store in 8-byte chunks to emulate wraparound.
	_mm_storel_epi64((__m128i *)(rsp->dmem + addr), reg);
}

//
// SSE3+ accelerated stores for group IV. Byteswap 2-byte little-endian
// vector back to big-endian. Stop storing at quadword boundaries.
//
void rsp_vstore_group4(RSP::CPUState *rsp, uint32_t addr, unsigned element, uint16_t *regp, rsp_vect_t reg,
                       rsp_vect_t dqm)
{
	uint32_t aligned_addr = addr & 0xFF0;
	unsigned offset = addr & 0xF;
	unsigned rol = offset;

	__m128i data = _mm_load_si128((__m128i *)(rsp->dmem + aligned_addr));

#if 0
  if (rsp->pipeline.exdf_latch.request.type == RSP_MEM_REQUEST_QUAD)
#else
	if (0)
#endif
	rol -= element;

	// TODO: How is this adjusted for SRV when e != 0?
	else dqm = _mm_cmpeq_epi8(_mm_setzero_si128(), dqm);

#ifndef __SSSE3__
	reg = sse2_pshufb(reg, rol_l2b_keys[rol & 0xF]);
#else
	__m128i ekey = _mm_load_si128((__m128i *)(rol_l2b_keys[rol & 0xF]));
	reg = _mm_shuffle_epi8(reg, ekey);
#endif

	// Mask and mux out the data, write.
#ifdef __SSE4_1__
	data = _mm_blendv_epi8(data, reg, dqm);
#else
	reg = _mm_and_si128(dqm, reg);
	data = _mm_andnot_si128(dqm, data);
	data = _mm_or_si128(data, reg);
#endif

	_mm_store_si128((__m128i *)(rsp->dmem + aligned_addr), data);
}
