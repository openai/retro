#include "imageops.h"

#ifdef __SSSE3__
#include <emmintrin.h>
#include <tmmintrin.h>
#endif
#include <stdexcept>
#include <cstring>

using namespace Retro;
using namespace std;

static void image565To888(const uint16_t* in, uint8_t* out, size_t w, size_t h, size_t stride);
static void imageX888To888(const uint32_t* in, uint8_t* out, size_t w, size_t h, size_t stride);

#ifdef __SSSE3__
const static __m128i maskR16 = _mm_set1_epi16(0xF800);
const static __m128i maskG16 = _mm_set1_epi16(0x07E0);
const static __m128i maskB16 = _mm_set1_epi16(0x001F);

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
