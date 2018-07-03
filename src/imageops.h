#pragma once

#include <cstdint>
#include <cstddef>

namespace Retro {

class Image {
public:
	enum class Format {
		RGB565,
		RGB888,
		RGBX888,
		G8
	};

	Image() {}
	Image(Format, const void* in, size_t w, size_t h, size_t stride);
	Image(Format, void* in, size_t w, size_t h, size_t stride);
	Image(const Image&) = default;

	void copyTo(Image* other);
	void halveTo(Image* other);
	void halveToInterlace(Image* other, const Image* old);
	void quarterTo(Image* other);
	void quarterToInterlace(Image* other, const Image* old);
	void divideTo(int divisor, Image* other);
	void divideToInterlace(int divisor, Image* other, const Image* old);

private:
	void copyDirectlyTo(Image* other);

	const void* m_constBuffer = nullptr;
	void* m_buffer = nullptr;
	size_t m_w;
	size_t m_h;
	size_t m_stride;
	Format m_format;
};
}
