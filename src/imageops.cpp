#include "imageops.h"

#ifdef __SSSE3__
#include <emmintrin.h>
#include <tmmintrin.h>
#endif
#include <stdexcept>
#include <cstring>

using namespace Retro;
using namespace std;

static void imageHalve565ToGray(const uint16_t* in, uint8_t* out, size_t w, size_t h, size_t stride);
static void imageHalve565ToGrayInterlace(const uint16_t* in, const uint16_t* oldin, uint16_t* out, size_t w, size_t h, size_t stride);
static void imageQuarter565ToGray(const uint16_t* in, uint8_t* out, size_t w, size_t h, size_t stride);
static void imageQuarter565ToGrayInterlace(const uint16_t* in, const uint16_t* oldin, uint16_t* out, size_t w, size_t h, size_t stride);
static void image565To888(const uint16_t* in, uint8_t* out, size_t w, size_t h, size_t stride);
static void imageHalveX888ToGray(const uint32_t* in, uint8_t* out, size_t w, size_t h, size_t stride);
static void imageHalveX888ToGrayInterlace(const uint32_t* in, const uint16_t* oldin, uint16_t* out, size_t w, size_t h, size_t stride);
static void imageQuarterX888ToGray(const uint32_t* in, uint8_t* out, size_t w, size_t h, size_t stride);
static void imageQuarterX888ToGrayInterlace(const uint32_t* in, const uint16_t* oldin, uint16_t* out, size_t w, size_t h, size_t stride);
static void imageX888To888(const uint32_t* in, uint8_t* out, size_t w, size_t h, size_t stride);

#ifdef __SSSE3__
const static __m128i maskR16 = _mm_set1_epi16(0xF800);
const static __m128i maskG16 = _mm_set1_epi16(0x07E0);
const static __m128i maskB16 = _mm_set1_epi16(0x001F);
const static __m128i maskR32 = _mm_set_epi8(0x80, 0x80, 0x80, 0x0E, 0x80, 0x80, 0x80, 0x0A, 0x80, 0x80, 0x80, 0x06, 0x80, 0x80, 0x80, 0x02);
const static __m128i maskG32 = _mm_set_epi8(0x80, 0x80, 0x80, 0x0D, 0x80, 0x80, 0x80, 0x09, 0x80, 0x80, 0x80, 0x05, 0x80, 0x80, 0x80, 0x01);
const static __m128i maskB32 = _mm_set_epi8(0x80, 0x80, 0x80, 0x0C, 0x80, 0x80, 0x80, 0x08, 0x80, 0x80, 0x80, 0x04, 0x80, 0x80, 0x80, 0x00);

static inline __m128i _convert565ToGray(__m128i pix) {
	/* Mask out channels */
	__m128i r = _mm_and_si128(pix, maskR16);
	__m128i g = _mm_and_si128(pix, maskG16);
	__m128i b = _mm_and_si128(pix, maskB16);
	/* Normalize channels */
	r = _mm_srli_epi16(r, 10);
	g = _mm_srli_epi16(g, 5);
	b = _mm_slli_epi16(b, 1);
	/* Combine channels */
	r = _mm_add_epi16(r, g);
	r = _mm_add_epi16(r, b);
	return r;
}
#endif

static inline uint8_t _convert565ToGray(uint16_t a, uint16_t b) {
	uint32_t ab = a | (b << 16);
	uint32_t r0 = ab & 0xF800F800;
	uint32_t g0 = ab & 0x07E007E0;
	uint32_t b0 = ab & 0x001F001F;
	r0 >>= 10;
	g0 >>= 5;
	b0 <<= 1;
	ab = r0 + g0 + b0;
	ab += ab >> 16;
	return ab / 2;
}

#ifdef __SSSE3__
static inline __m128i _convertX888ToGray(__m128i pix) {
	/* Mask out channels */
	__m128i r = _mm_shuffle_epi8(pix, maskR32);
	__m128i g = _mm_shuffle_epi8(pix, maskG32);
	__m128i b = _mm_shuffle_epi8(pix, maskB32);
	/* Combine channels */
	r = _mm_add_epi32(r, g);
	r = _mm_add_epi32(r, b);
	r = _mm_srli_epi16(r, 2);
	return r;
}
#endif

static inline uint8_t _convertX888ToGray(uint32_t a, uint32_t b) {
	uint32_t r0 = a & 0x00FF0000;
	uint32_t g0 = a & 0x0000FF00;
	uint32_t b0 = a & 0x000000FF;
	uint32_t r1 = b & 0x00FF0000;
	uint32_t g1 = b & 0x0000FF00;
	uint32_t b1 = b & 0x000000FF;
	r0 += r1;
	g0 += g1;
	b0 += b1;
	r0 >>= 16;
	g0 >>= 8;
	r0 = r0 + g0 + b0;
	return r0 / 8;
}

#ifdef __SSSE3__
static inline __m128i _halveW16(__m128i a, __m128i b) {
	/* Swizzle ABCDEFGH IJKLMNOP to ACEGIKMO BDFHJLNP */
	__m128i tmp0;
	tmp0 = _mm_shufflelo_epi16(a, 0xD8); /* ABCDEFGH -> ACBDEFGH */
	tmp0 = _mm_shufflehi_epi16(tmp0, 0xD8); /* ACBDEFGH -> ACBDEGFH */
	tmp0 = _mm_shuffle_epi32(tmp0, 0xD8); /* ACBDEGFH -> ACEGBDFH */
	__m128i tmp1;
	tmp1 = _mm_shufflelo_epi16(b, 0xD8); /* IJKLMNOP -> IKJLMNOP */
	tmp1 = _mm_shufflehi_epi16(tmp1, 0xD8); /* IKJLMNOP -> IKJLMONP */
	tmp1 = _mm_shuffle_epi32(tmp1, 0xD8); /* IKJLMONP -> IKMOJLNP */
	__m128i tmp2 = _mm_unpacklo_epi64(tmp0, tmp1); /* ACEGBDFH IKMOJLNP -> ACEGIKMO */
	__m128i tmp3 = _mm_unpackhi_epi64(tmp0, tmp1); /* ACEGBDFH IKMOJLNP -> BDFHJLNP */
	/* Halve width */
	return _mm_avg_epu16(tmp2, tmp3);
}

static inline __m128i _halveWNeighbor16(__m128i a, __m128i b) {
	/* Swizzle ABCDEFGH IJKLMNOP to ACEGIKMO */
	__m128i tmp0;
	tmp0 = _mm_shufflelo_epi16(a, 0xD8); /* ABCDEFGH -> ACBDEFGH */
	tmp0 = _mm_shufflehi_epi16(tmp0, 0xD8); /* ACBDEFGH -> ACBDEGFH */
	tmp0 = _mm_shuffle_epi32(tmp0, 0xD8); /* ACBDEGFH -> ACEGBDFH */
	__m128i tmp1;
	tmp1 = _mm_shufflelo_epi16(b, 0xD8); /* IJKLMNOP -> IKJLMNOP */
	tmp1 = _mm_shufflehi_epi16(tmp1, 0xD8); /* IKJLMNOP -> IKJLMONP */
	tmp1 = _mm_shuffle_epi32(tmp1, 0xD8); /* IKJLMONP -> IKMOJLNP */
	return _mm_unpacklo_epi64(tmp0, tmp1); /* ACEGBDFH IKMOJLNP -> ACEGIKMO */
}

static inline __m128i _halveW32(__m128i a, __m128i b) {
	/* Swizzle ABCD EFGH to ACEG EFGH */
	__m128i tmp0 = _mm_shuffle_epi32(a, 0xD8); /* ABCD -> ACBD */
	__m128i tmp1 = _mm_shuffle_epi32(b, 0xD8); /* EFGH -> EGFH */
	__m128i tmp2 = _mm_unpacklo_epi64(tmp0, tmp1); /* ACBD EGFH -> ACEG */
	__m128i tmp3 = _mm_unpackhi_epi64(tmp0, tmp1); /* ACBD EFGH -> BDFH */
	/* Halve width */
	return _mm_avg_epu16(tmp2, tmp3);
}

static inline __m128i _halveWNeighbor32(__m128i a, __m128i b) {
	/* Swizzle ABCD EFGH to ACEG */
	__m128i tmp0 = _mm_shuffle_epi32(a, 0xD8); /* ABCD -> ACBD */
	__m128i tmp1 = _mm_shuffle_epi32(b, 0xD8); /* EFGH -> EGFH */
	return _mm_unpacklo_epi64(tmp0, tmp1); /* ACBD EGFH -> ACEG */
}
#endif

void imageHalve565ToGray(const uint16_t* in, uint8_t* out, size_t w, size_t h, size_t stride) {
	for (size_t y = 0; y + 1 < h; y += 2) {
		size_t x = 0;
#ifdef __SSSE3__
		for (; x + 15 < w; x += 16) {
			__m128i gray0;
			__m128i gray1;

			gray0 = _convert565ToGray(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x])));
			gray1 = _convert565ToGray(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 8])));
			__m128i out0 = _halveW16(gray0, gray1);

			gray0 = _convert565ToGray(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + stride / 2])));
			gray1 = _convert565ToGray(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 8 + stride / 2])));
			__m128i out1 = _halveW16(gray0, gray1);

			// Halve height
			out0 = _mm_avg_epu16(out0, out1);
			out0 = _mm_packus_epi16(out0, _mm_undefined_si128());
			_mm_storel_epi64(reinterpret_cast<__m128i*>(out), out0);
			out += 8;
		}
#endif
		for (; x < w; x += 2) {
			unsigned gray0 = _convert565ToGray(in[x], in[x + 1]);
			unsigned gray1 = _convert565ToGray(in[x + stride / 2], in[x + stride / 2 + 1]);
			*out = (gray0 + gray1) / 2;
			++out;
		}
		in += stride;
	}
}

void imageHalve565ToGrayInterlace(const uint16_t* in, const uint16_t* oldin, uint16_t* out, size_t w, size_t h, size_t stride) {
	for (size_t y = 0; y + 1 < h; y += 2) {
		size_t x = 0;
#ifdef __SSSE3__
		for (; x + 15 < w; x += 16) {
			__m128i gray0;
			__m128i gray1;

			gray0 = _convert565ToGray(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x])));
			gray1 = _convert565ToGray(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 8])));
			__m128i out0 = _halveW16(gray0, gray1);

			gray0 = _convert565ToGray(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + stride / 2])));
			gray1 = _convert565ToGray(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 8 + stride / 2])));
			__m128i out1 = _halveW16(gray0, gray1);

			// Halve height
			out0 = _mm_avg_epu16(out0, out1);

			// Interlace with old data
			out1 = _mm_load_si128(reinterpret_cast<const __m128i*>(oldin));
			out1 = _mm_slli_epi16(out1, 8);
			out0 = _mm_add_epi8(out0, out1);
			_mm_store_si128(reinterpret_cast<__m128i*>(out), out0);
			oldin += 8;
			out += 8;
		}
#endif
		for (; x < w; x += 2) {
			unsigned gray0 = _convert565ToGray(in[x], in[x + 1]);
			unsigned gray1 = _convert565ToGray(in[x + stride / 2], in[x + stride / 2 + 1]);
			gray0 = (gray0 + gray1) / 2;
			gray0 |= *oldin << 8;
			*out = gray0;
			++oldin;
			++out;
		}
		in += stride;
	}
}

void imageQuarter565ToGray(const uint16_t* in, uint8_t* out, size_t w, size_t h, size_t stride) {
	for (size_t y = 0; y + 3 < h; y += 4) {
		size_t x = 0;
#ifdef __SSSE3__
		for (; x + 31 < w; x += 32) {
			__m128i gray0;
			__m128i gray1;

			gray0 = _halveWNeighbor16(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x])), _mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 8])));
			gray1 = _halveWNeighbor16(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 16])), _mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 24])));
			gray0 = _convert565ToGray(gray0);
			gray1 = _convert565ToGray(gray1);
			__m128i out0 = _halveW16(gray0, gray1);

			gray0 = _halveWNeighbor16(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + stride])), _mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 8 + stride])));
			gray1 = _halveWNeighbor16(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 16 + stride])), _mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 24 + stride])));
			gray0 = _convert565ToGray(gray0);
			gray1 = _convert565ToGray(gray1);
			__m128i out1 = _halveW16(gray0, gray1);

			// Halve height
			out0 = _mm_avg_epu16(out0, out1);
			out0 = _mm_packus_epi16(out0, _mm_undefined_si128());
			_mm_storel_epi64(reinterpret_cast<__m128i*>(out), out0);
			out += 8;
		}
#endif
		for (; x + 3 < w; x += 4) {
			unsigned gray0 = _convert565ToGray(in[x], in[x + 2]);
			unsigned gray1 = _convert565ToGray(in[x + stride], in[x + stride + 2]);
			*out = (gray0 + gray1) / 2;
			++out;
		}
		in += stride * 2;
	}
}

void imageQuarter565ToGrayInterlace(const uint16_t* in, const uint16_t* oldin, uint16_t* out, size_t w, size_t h, size_t stride) {
	for (size_t y = 0; y + 3 < h; y += 4) {
		size_t x = 0;
#ifdef __SSSE3__
		for (; x + 31 < w; x += 32) {
			__m128i gray0;
			__m128i gray1;

			gray0 = _halveWNeighbor16(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x])), _mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 8])));
			gray1 = _halveWNeighbor16(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 16])), _mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 24])));
			gray0 = _convert565ToGray(gray0);
			gray1 = _convert565ToGray(gray1);
			__m128i out0 = _halveW16(gray0, gray1);

			gray0 = _halveWNeighbor16(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + stride])), _mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 8 + stride])));
			gray1 = _halveWNeighbor16(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 16 + stride])), _mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 24 + stride])));
			gray0 = _convert565ToGray(gray0);
			gray1 = _convert565ToGray(gray1);
			__m128i out1 = _halveW16(gray0, gray1);

			// Halve height
			out0 = _mm_avg_epu16(out0, out1);

			// Interlace with old data
			out1 = _mm_loadu_si128(reinterpret_cast<const __m128i*>(oldin));
			out1 = _mm_slli_epi16(out1, 8);
			out0 = _mm_add_epi8(out0, out1);
			_mm_storeu_si128(reinterpret_cast<__m128i*>(out), out0);
			oldin += 8;
			out += 8;
		}
#endif
		for (; x + 3 < w; x += 4) {
			unsigned gray0 = _convert565ToGray(in[x], in[x + 2]);
			unsigned gray1 = _convert565ToGray(in[x + stride], in[x + stride + 2]);
			gray0 = (gray0 + gray1) / 2;
			gray0 |= *oldin << 8;
			*out = gray0;
			++oldin;
			++out;
		}
		in += stride * 2;
	}
}

#ifdef __SSSE3__
static inline void _convert565To888(const __m128i* in, __m128i* out) {
	/* 00 R0 00 R1 00 R2 00 R3 00 R4 00 R5 00 R6 00 R7 -> R0 00 00 R1 00 00 R2 00 00 R3 00 00 R4 00 00 R5 */
	const static __m128i rblend00 = _mm_set_epi8(0x0A, 0x80, 0x80, 0x08, 0x80, 0x80, 0x06, 0x80, 0x80, 0x04, 0x80, 0x80, 0x02, 0x80, 0x80, 0x00);

	/* 00 R0 00 R1 00 R2 00 R3 00 R4 00 R5 00 R6 00 R7 -> 00 00 R6 00 00 R7 00 00 00 00 00 00 00 00 00 00 */
	const static __m128i rblend10 = _mm_set_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0E, 0x80, 0x80, 0x0C, 0x80, 0x80);

	/* 00 R8 00 R9 00 RA 00 RB 00 RC 00 RD 00 RE 00 RF -> 00 00 00 00 00 00 00 00 R8 00 00 R9 00 00 RA 00 */
	const static __m128i rblend11 = _mm_set_epi8(0x80, 0x04, 0x80, 0x80, 0x02, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80);

	/* 00 R8 00 R9 00 RA 00 RB 00 RC 00 RD 00 RE 00 RF -> 00 RB 00 00 RC 00 00 RD 00 00 RE 00 00 RF 00 00 */
	const static __m128i rblend21 = _mm_set_epi8(0x80, 0x80, 0x0E, 0x80, 0x80, 0x0C, 0x80, 0x80, 0x0A, 0x80, 0x80, 0x08, 0x80, 0x80, 0x06, 0x80);

	/* 00 G0 00 G1 00 G2 00 G3 00 G4 00 G5 00 G6 00 G7 -> 00 G0 00 00 G1 00 00 G2 00 00 G3 00 00 G4 00 00 */
	const static __m128i gblend00 = _mm_set_epi8(0x80, 0x80, 0x08, 0x80, 0x80, 0x06, 0x80, 0x80, 0x04, 0x80, 0x80, 0x02, 0x80, 0x80, 0x00, 0x80);

	/* 00 G0 00 G1 00 G2 00 G3 00 G4 00 G5 00 G6 00 G7 -> G5 00 00 G6 00 00 G7 00 00 00 00 00 00 00 00 00 */
	const static __m128i gblend10 = _mm_set_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0E, 0x80, 0x80, 0x0C, 0x80, 0x80, 0x0A);

	/* 00 G8 00 G9 00 GA 00 GB 00 GC 00 GD 00 GE 00 GF -> 00 00 00 00 00 00 00 00 00 G8 00 00 G9 00 00 GA */
	const static __m128i gblend11 = _mm_set_epi8(0x04, 0x80, 0x80, 0x02, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80);

	/* 00 G8 00 G9 00 GA 00 GB 00 GC 00 GD 00 GE 00 GF -> 00 00 GB 00 00 GC 00 00 GD 00 00 GE 00 00 GF 00 */
	const static __m128i gblend21 = _mm_set_epi8(0x80, 0x0E, 0x80, 0x80, 0x0C, 0x80, 0x80, 0x0A, 0x80, 0x80, 0x08, 0x80, 0x80, 0x06, 0x80, 0x80);

	/* 00 B0 00 B1 00 B2 00 B3 00 B4 00 B5 00 B6 00 B7 -> 00 00 B0 00 00 B1 00 00 B2 00 00 B3 00 00 B4 00 */
	const static __m128i bblend00 = _mm_set_epi8(0x80, 0x08, 0x80, 0x80, 0x06, 0x80, 0x80, 0x04, 0x80, 0x80, 0x02, 0x80, 0x80, 0x00, 0x80, 0x80);

	/* 00 B0 00 B1 00 B2 00 B3 00 B4 00 B5 00 B6 00 B7 -> 00 B5 00 00 B6 00 00 B7 00 00 00 00 00 00 00 00 */
	const static __m128i bblend10 = _mm_set_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0E, 0x80, 0x80, 0x0C, 0x80, 0x80, 0x0A, 0x80);

	/* 00 B8 00 B9 00 BA 00 BB 00 BC 00 BD 00 BE 00 BF -> 00 00 00 00 00 00 00 00 00 00 B8 00 00 B9 00 00 */
	const static __m128i bblend11 = _mm_set_epi8(0x80, 0x80, 0x02, 0x80, 0x80, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80);

	/* 00 B8 00 B9 00 BA 00 BB 00 BC 00 BD 00 BE 00 BF -> BA 00 00 BB 00 00 BC 00 00 BD 00 00 BE 00 00 BF */
	const static __m128i bblend21 = _mm_set_epi8(0x0E, 0x80, 0x80, 0x0C, 0x80, 0x80, 0x0A, 0x80, 0x80, 0x08, 0x80, 0x80, 0x06, 0x80, 0x80, 0x04);

	__m128i pix0 = _mm_load_si128(&in[0]);
	__m128i pix1 = _mm_load_si128(&in[1]);

	// Mask out channels
	__m128i r0 = _mm_and_si128(pix0, maskR16);
	__m128i g0 = _mm_and_si128(pix0, maskG16);
	__m128i b0 = _mm_and_si128(pix0, maskB16);
	__m128i r1 = _mm_and_si128(pix1, maskR16);
	__m128i g1 = _mm_and_si128(pix1, maskG16);
	__m128i b1 = _mm_and_si128(pix1, maskB16);

	// Normalize channels to 16-bit per channel
	r0 = _mm_srli_epi16(r0, 8);
	g0 = _mm_srli_epi16(g0, 3);
	b0 = _mm_slli_epi16(b0, 3);
	r1 = _mm_srli_epi16(r1, 8);
	g1 = _mm_srli_epi16(g1, 3);
	b1 = _mm_slli_epi16(b1, 3);

	// Halve channel width and mix to discrete bytes
	__m128i out0 = _mm_shuffle_epi8(r0, rblend00);
	out0 = _mm_or_si128(out0, _mm_shuffle_epi8(g0, gblend00));
	out0 = _mm_or_si128(out0, _mm_shuffle_epi8(b0, bblend00));

	__m128i out1 = _mm_shuffle_epi8(r0, rblend10);
	out1 = _mm_or_si128(out1, _mm_shuffle_epi8(g0, gblend10));
	out1 = _mm_or_si128(out1, _mm_shuffle_epi8(b0, bblend10));
	out1 = _mm_or_si128(out1, _mm_shuffle_epi8(r1, rblend11));
	out1 = _mm_or_si128(out1, _mm_shuffle_epi8(g1, gblend11));
	out1 = _mm_or_si128(out1, _mm_shuffle_epi8(b1, bblend11));

	__m128i out2 = _mm_shuffle_epi8(r1, rblend21);
	out2 = _mm_or_si128(out2, _mm_shuffle_epi8(g1, gblend21));
	out2 = _mm_or_si128(out2, _mm_shuffle_epi8(b1, bblend21));

	_mm_store_si128(&out[0], out0);
	_mm_store_si128(&out[1], out1);
	_mm_store_si128(&out[2], out2);
}
#endif

void image565To888(const uint16_t* in, uint8_t* out, size_t w, size_t h, size_t stride) {
	for (size_t y = 0; y < h; ++y) {
		size_t x = 0;
#ifdef __SSSE3__
		for (; x < w; x += 16) {
			_convert565To888(reinterpret_cast<const __m128i*>(&in[x]), reinterpret_cast<__m128i*>(out));
			out += 16 * 3;
		}
#endif
		for (; x < w; ++x) {
			uint16_t rgb = in[x];
			out[0] = (rgb & 0xF800) >> 8;
			out[1] = (rgb & 0x07E0) >> 3;
			out[2] = (rgb & 0x001F) << 3;
			out += 3;
		}
		in += stride / 2;
	}
}

void imageHalveX888ToGray(const uint32_t* in, uint8_t* out, size_t w, size_t h, size_t stride) {
	for (size_t y = 0; y + 1 < h; y += 2) {
		size_t x = 0;
#ifdef __SSSE3__
		for (; x + 7 < w; x += 8) {
			__m128i gray0;
			__m128i gray1;

			gray0 = _convertX888ToGray(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x])));
			gray1 = _convertX888ToGray(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 4])));
			__m128i out0 = _halveW32(gray0, gray1);

			gray0 = _convertX888ToGray(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + stride / 2])));
			gray1 = _convertX888ToGray(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 4 + stride / 2])));
			__m128i out1 = _halveW32(gray0, gray1);

			// Halve height
			out0 = _mm_avg_epu16(out0, out1);
			out0 = _mm_packus_epi16(out0, _mm_undefined_si128());
			out0 = _mm_packus_epi16(out0, _mm_undefined_si128());
			unsigned outx = _mm_cvtsi128_si32(out0);
			*reinterpret_cast<uint32_t*>(out) = outx;
			out += 4;
		}
#endif
		for (; x + 1 < w; x += 2) {
			*out = _convertX888ToGray(in[x], in[x + 1]);
			++out;
		}
		in += stride / 2;
	}
}

void imageHalveX888ToGrayInterlace(const uint32_t* in, const uint16_t* oldin, uint16_t* out, size_t w, size_t h, size_t stride) {
	for (size_t y = 0; y + 1 < h; y += 2) {
		size_t x = 0;
#ifdef __SSSE3__
		for (; x + 7 < w; x += 8) {
			__m128i gray0;
			__m128i gray1;

			gray0 = _convertX888ToGray(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x])));
			gray1 = _convertX888ToGray(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 4])));
			__m128i out0 = _halveW32(gray0, gray1);

			gray0 = _convertX888ToGray(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + stride / 2])));
			gray1 = _convertX888ToGray(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 4 + stride / 2])));
			__m128i out1 = _halveW32(gray0, gray1);

			// Halve height
			out0 = _mm_avg_epu16(out0, out1);
			out0 = _mm_packus_epi16(out0, _mm_undefined_si128());

			// Interlace with old data
			out1 = _mm_loadl_epi64(reinterpret_cast<const __m128i*>(oldin));
			out1 = _mm_slli_epi16(out1, 8);
			out0 = _mm_add_epi8(out0, out1);
			_mm_storel_epi64(reinterpret_cast<__m128i*>(out), out0);
			oldin += 4;
			out += 4;
		}
#endif
		for (; x + 1 < w; x += 2) {
			unsigned gray0 = _convertX888ToGray(in[x], in[x + 1]);
			gray0 |= *oldin << 8;
			*out = gray0;
			++oldin;
			++out;
		}
		in += stride / 2;
	}
}

void imageQuarterX888ToGray(const uint32_t* in, uint8_t* out, size_t w, size_t h, size_t stride) {
	for (size_t y = 0; y + 3 < h; y += 4) {
		size_t x = 0;
#ifdef __SSSE3__
		for (; x + 15 < w; x += 16) {
			__m128i gray0;
			__m128i gray1;

			gray0 = _halveWNeighbor32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x])), _mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 4])));
			gray1 = _halveWNeighbor32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 8])), _mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 12])));
			gray0 = _convertX888ToGray(gray0);
			gray1 = _convertX888ToGray(gray1);
			__m128i out0 = _halveW32(gray0, gray1);

			gray0 = _halveWNeighbor32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + stride / 2])), _mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 4 + stride / 2])));
			gray1 = _halveWNeighbor32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 8 + stride / 2])), _mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 12 + stride / 2])));
			gray0 = _convertX888ToGray(gray0);
			gray1 = _convertX888ToGray(gray1);
			__m128i out1 = _halveW32(gray0, gray1);

			// Halve height
			out0 = _mm_avg_epu16(out0, out1);
			out0 = _mm_packus_epi16(out0, _mm_undefined_si128());
			out0 = _mm_packus_epi16(out0, _mm_undefined_si128());
			unsigned outx = _mm_cvtsi128_si32(out0);
			*reinterpret_cast<uint32_t*>(out) = outx;
			out += 4;
		}
#endif
		for (; x + 3 < w; x += 4) {
			unsigned gray0 = _convertX888ToGray(in[x], in[x + 2]);
			unsigned gray1 = _convertX888ToGray(in[x + stride / 2], in[x + stride / 2 + 2]);
			*out = (gray0 + gray1) / 2;
			++out;
		}
		in += stride;
	}
}

void imageQuarterX888ToGrayInterlace(const uint32_t* in, const uint16_t* oldin, uint16_t* out, size_t w, size_t h, size_t stride) {
	for (size_t y = 0; y + 3 < h; y += 4) {
		size_t x = 0;
#ifdef __SSSE3__
		for (; x + 15 < w; x += 16) {
			__m128i gray0;
			__m128i gray1;

			gray0 = _halveWNeighbor32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x])), _mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 4])));
			gray1 = _halveWNeighbor32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 8])), _mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 12])));
			gray0 = _convertX888ToGray(gray0);
			gray1 = _convertX888ToGray(gray1);
			__m128i out0 = _halveW32(gray0, gray1);

			gray0 = _halveWNeighbor32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + stride / 2])), _mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 4 + stride / 2])));
			gray1 = _halveWNeighbor32(_mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 8 + stride / 2])), _mm_loadu_si128(reinterpret_cast<const __m128i*>(&in[x + 12 + stride / 2])));
			gray0 = _convertX888ToGray(gray0);
			gray1 = _convertX888ToGray(gray1);
			__m128i out1 = _halveW32(gray0, gray1);

			// Halve height
			out0 = _mm_avg_epu16(out0, out1);
			out0 = _mm_packus_epi16(out0, _mm_undefined_si128());

			// Interlace with old data
			out1 = _mm_loadl_epi64(reinterpret_cast<const __m128i*>(oldin));
			out1 = _mm_slli_epi16(out1, 8);
			out0 = _mm_add_epi8(out0, out1);
			_mm_storel_epi64(reinterpret_cast<__m128i*>(out), out0);
			oldin += 4;
			out += 4;
		}
#endif
		for (; x + 3 < w; x += 4) {
			unsigned gray0 = _convertX888ToGray(in[x], in[x + 2]);
			unsigned gray1 = _convertX888ToGray(in[x + stride / 2], in[x + stride / 2 + 2]);
			gray0 = (gray0 + gray1) / 2;
			gray0 |= *oldin << 8;
			*out = gray0;
			++oldin;
			++out;
		}
		in += stride;
	}
}

void imageX888To888(const uint32_t* in, uint8_t* out, size_t w, size_t h, size_t stride) {
	for (size_t y = 0; y < h; ++y) {
		size_t x = 0;
#ifdef __SSSE3__
		for (; x + 15 < w; x += 16) {
			/* B0 G0 R0 X0 B1 G1 R1 X1 B2 G2 R2 X2 B3 G3 R3 X3 -> R0 G0 B0 R1 G1 B1 R2 G2 B2 R3 G3 B3 00 00 00 00 */
			const static __m128i blend00 = _mm_set_epi8(0x80, 0x80, 0x80, 0x80, 0x0C, 0x0D, 0x0E, 0x08, 0x09, 0x0A, 0x04, 0x05, 0x06, 0x00, 0x01, 0x02);

			/* B4 G4 R4 X4 B5 G5 R5 X5 B6 G6 R6 X6 B7 G7 R7 X7 -> 00 00 00 00 00 00 00 00 00 00 00 00 R4 G4 B4 R5 */
			const static __m128i blend01 = _mm_set_epi8(0x06, 0x00, 0x01, 0x02, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80);

			/* B4 G4 R4 X4 B5 G5 R5 X5 B6 G6 R6 X6 B7 G7 R7 X7 -> G5 B5 R6 G6 B6 R7 G7 B7 00 00 00 00 00 00 00 00 */
			const static __m128i blend11 = _mm_set_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0C, 0x0D, 0x0E, 0x08, 0x09, 0x0A, 0x04, 0x05);

			/* B8 G8 R8 X8 B9 G9 R9 X9 BA GA RA XA BB GB RB XB -> 00 00 00 00 00 00 00 00 R8 G8 B8 R9 G9 B9 RA GA */
			const static __m128i blend12 = _mm_set_epi8(0x09, 0x0A, 0x04, 0x05, 0x06, 0x00, 0x01, 0x02, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80);

			/* B8 G8 R8 X8 B9 G9 R9 X9 BA GA RA XA BB GB RB XB -> BA RB GB BB 00 00 00 00 00 00 00 00 00 00 00 00 */
			const static __m128i blend22 = _mm_set_epi8(0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x0C, 0x0D, 0x0E, 0x08);

			/* BC GC RC XC BD GD RD XD BE GE RE XE BF GF RF XF -> 00 00 00 00 RC GC BC RD GD BD RE GE BE RF GF DF */
			const static __m128i blend23 = _mm_set_epi8(0x0C, 0x0D, 0x0E, 0x08, 0x09, 0x0A, 0x04, 0x05, 0x06, 0x00, 0x01, 0x02, 0x80, 0x80, 0x80, 0x80);

			__m128i pix0 = _mm_load_si128(reinterpret_cast<const __m128i*>(&in[x]));
			__m128i pix1 = _mm_load_si128(reinterpret_cast<const __m128i*>(&in[x + 4]));
			__m128i pix2 = _mm_load_si128(reinterpret_cast<const __m128i*>(&in[x + 8]));
			__m128i pix3 = _mm_load_si128(reinterpret_cast<const __m128i*>(&in[x + 12]));

			__m128i out0 = _mm_shuffle_epi8(pix0, blend00);
			out0 = _mm_or_si128(out0, _mm_shuffle_epi8(pix1, blend01));

			__m128i out1 = _mm_shuffle_epi8(pix1, blend11);
			out1 = _mm_or_si128(out1, _mm_shuffle_epi8(pix2, blend12));

			__m128i out2 = _mm_shuffle_epi8(pix2, blend22);
			out2 = _mm_or_si128(out2, _mm_shuffle_epi8(pix3, blend23));

			_mm_storeu_si128(reinterpret_cast<__m128i*>(&out[0]), out0);
			_mm_storeu_si128(reinterpret_cast<__m128i*>(&out[16]), out1);
			_mm_storeu_si128(reinterpret_cast<__m128i*>(&out[32]), out2);
			out += 48;
		}
#endif
		for (; x < w; ++x) {
			uint32_t xrgb = in[x];
			out[0] = xrgb;
			out[1] = xrgb >> 8;
			out[2] = xrgb >> 16;
			out += 3;
		}
		in += stride / 4;
	}
}

Image::Image(Format format, const void* in, size_t w, size_t h, size_t stride)
	: m_constBuffer(in)
	, m_w(w)
	, m_h(h)
	, m_stride(stride)
	, m_format(format) {
}

Image::Image(Format format, void* in, size_t w, size_t h, size_t stride)
	: m_constBuffer(in)
	, m_buffer(in)
	, m_w(w)
	, m_h(h)
	, m_stride(stride)
	, m_format(format) {
}

void Image::copyTo(Image* other) {
	if (m_w != other->m_w || m_h != other->m_h) {
		throw invalid_argument("Image dimensions don't match");
	}
	switch (m_format) {
	case Image::Format::RGB565:
		switch (other->m_format) {
		case Image::Format::RGB565:
			copyDirectlyTo(other);
			break;
		case Image::Format::RGB888:
			image565To888(static_cast<const uint16_t*>(m_constBuffer), static_cast<uint8_t*>(other->m_buffer), m_w, m_h, m_stride);
			break;
		default:
			throw logic_error("unimplemented conversion");
		}
		break;
	case Image::Format::RGB888:
		switch (other->m_format) {
		case Image::Format::RGB888:
			copyDirectlyTo(other);
		default:
			throw logic_error("unimplemented conversion");
		}
		break;
	case Image::Format::RGBX888:
		switch (other->m_format) {
		case Image::Format::RGBX888:
			copyDirectlyTo(other);
			break;
		case Image::Format::RGB888:
			imageX888To888(static_cast<const uint32_t*>(m_constBuffer), static_cast<uint8_t*>(other->m_buffer), m_w, m_h, m_stride);
			break;
		default:
			throw logic_error("unimplemented conversion");
		}
		break;
	case Image::Format::G8:
		switch (other->m_format) {
		case Image::Format::G8:
			copyDirectlyTo(other);
		default:
			throw logic_error("unimplemented conversion");
		}
		break;
	}
}

void Image::halveTo(Image* other) {
	if (m_w / 2 != other->m_w || m_h / 2 != other->m_h) {
		throw invalid_argument("Image dimensions don't match");
	}
	switch (m_format) {
	case Image::Format::RGB565:
		switch (other->m_format) {
		case Image::Format::G8:
			imageHalve565ToGray(static_cast<const uint16_t*>(m_constBuffer), static_cast<uint8_t*>(other->m_buffer), m_w, m_h, m_stride);
			break;
		default:
			throw logic_error("unimplemented conversion");
		}
		break;
	case Image::Format::RGB888:
		throw logic_error("unimplemented conversion");
	case Image::Format::RGBX888:
		switch (other->m_format) {
		case Image::Format::G8:
			imageHalveX888ToGray(static_cast<const uint32_t*>(m_constBuffer), static_cast<uint8_t*>(other->m_buffer), m_w, m_h, m_stride);
			break;
		default:
			throw logic_error("unimplemented conversion");
		}
		break;
	case Image::Format::G8:
		throw logic_error("unimplemented conversion");
	}
}

void Image::halveToInterlace(Image* other, const Image* old) {
	if (m_w / 2 != other->m_w / 2 || m_h / 2 != other->m_h) {
		throw invalid_argument("Image dimensions don't match");
	}
	if (old->m_w != other->m_w || old->m_h != other->m_h) {
		throw invalid_argument("Image dimensions don't match");
	}
	switch (m_format) {
	case Image::Format::RGB565:
		switch (other->m_format) {
		case Image::Format::G8:
			imageHalve565ToGrayInterlace(static_cast<const uint16_t*>(m_constBuffer), static_cast<const uint16_t*>(old->m_constBuffer), static_cast<uint16_t*>(other->m_buffer), m_w, m_h, m_stride);
			break;
		default:
			throw logic_error("unimplemented conversion");
		}
		break;
	case Image::Format::RGB888:
		throw logic_error("unimplemented conversion");
	case Image::Format::RGBX888:
		switch (other->m_format) {
		case Image::Format::G8:
			imageHalveX888ToGrayInterlace(static_cast<const uint32_t*>(m_constBuffer), static_cast<const uint16_t*>(old->m_constBuffer), static_cast<uint16_t*>(other->m_buffer), m_w, m_h, m_stride);
			break;
		default:
			throw logic_error("unimplemented conversion");
		}
		break;
	case Image::Format::G8:
		throw logic_error("unimplemented conversion");
	}
}

void Image::quarterTo(Image* other) {
	if (m_w / 4 != other->m_w || m_h / 4 != other->m_h) {
		throw invalid_argument("Image dimensions don't match");
	}
	switch (m_format) {
	case Image::Format::RGB565:
		switch (other->m_format) {
		case Image::Format::G8:
			imageQuarter565ToGray(static_cast<const uint16_t*>(m_constBuffer), static_cast<uint8_t*>(other->m_buffer), m_w, m_h, m_stride);
			break;
		default:
			throw logic_error("unimplemented conversion");
		}
		break;
	case Image::Format::RGB888:
		throw logic_error("unimplemented conversion");
	case Image::Format::RGBX888:
		switch (other->m_format) {
		case Image::Format::G8:
			imageQuarterX888ToGray(static_cast<const uint32_t*>(m_constBuffer), static_cast<uint8_t*>(other->m_buffer), m_w, m_h, m_stride);
			break;
		default:
			throw logic_error("unimplemented conversion");
		}
		break;
	case Image::Format::G8:
		throw logic_error("unimplemented conversion");
	}
}

void Image::quarterToInterlace(Image* other, const Image* old) {
	if (m_w / 4 != other->m_w / 2 || m_h / 4 != other->m_h) {
		throw invalid_argument("Image dimensions don't match");
	}
	if (old->m_w != other->m_w || old->m_h != other->m_h) {
		throw invalid_argument("Image dimensions don't match");
	}
	if (old->m_format != other->m_format || old->m_format != other->m_format) {
		throw invalid_argument("Image formats don't match");
	}
	switch (m_format) {
	case Image::Format::RGB565:
		switch (other->m_format) {
		case Image::Format::G8:
			imageQuarter565ToGrayInterlace(static_cast<const uint16_t*>(m_constBuffer), static_cast<const uint16_t*>(old->m_constBuffer), static_cast<uint16_t*>(other->m_buffer), m_w, m_h, m_stride);
			break;
		default:
			throw logic_error("unimplemented conversion");
		}
		break;
	case Image::Format::RGB888:
		throw logic_error("unimplemented conversion");
	case Image::Format::RGBX888:
		switch (other->m_format) {
		case Image::Format::G8:
			imageQuarterX888ToGrayInterlace(static_cast<const uint32_t*>(m_constBuffer), static_cast<const uint16_t*>(old->m_constBuffer), static_cast<uint16_t*>(other->m_buffer), m_w, m_h, m_stride);
			break;
		default:
			throw logic_error("unimplemented conversion");
		}
		break;
	case Image::Format::G8:
		throw logic_error("unimplemented conversion");
	}
}

void Image::divideTo(int divisor, Image* other) {
	switch (divisor) {
	case 0:
		throw invalid_argument("Cannot divide by zero");
		break;
	case 1:
		copyTo(other);
		break;
	case 2:
		halveTo(other);
		break;
	case 4:
		quarterTo(other);
		break;
	default:
		throw logic_error("unimplemented conversion");
		break;
	}
}

void Image::divideToInterlace(int divisor, Image* other, const Image* old) {
	switch (divisor) {
	case 0:
		throw invalid_argument("Cannot divide by zero");
		break;
	case 2:
		halveToInterlace(other, old);
		break;
	case 4:
		quarterToInterlace(other, old);
		break;
	default:
		throw logic_error("unimplemented conversion");
		break;
	}
}

void Image::copyDirectlyTo(Image* other) {
	size_t depth = 1;
	switch (m_format) {
	case Image::Format::RGB565:
		depth = 2;
		break;
	case Image::Format::RGB888:
		depth = 3;
		break;
	case Image::Format::RGBX888:
		depth = 4;
		break;
	case Image::Format::G8:
		break;
	}
	if (m_stride == other->m_stride) {
		memcpy(other->m_buffer, m_constBuffer, m_stride * m_h);
	} else {
		const uint8_t* in = static_cast<const uint8_t*>(m_constBuffer);
		uint8_t* out = static_cast<uint8_t*>(other->m_buffer);
		for (size_t y = 0; y < m_h; ++y) {
			memcpy(&out[other->m_stride * y], &in[m_stride * y], depth * y * m_w);
		}
	}
}
