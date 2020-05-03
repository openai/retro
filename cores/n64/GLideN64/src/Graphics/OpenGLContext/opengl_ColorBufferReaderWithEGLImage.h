#pragma once
#ifdef EGL
#include <Graphics/ColorBufferReader.h>
#include <Graphics/OpenGLContext/GraphicBuffer/GraphicBufferWrapper.h>
#include "opengl_CachedFunctions.h"

#include <android/hardware_buffer.h>

namespace opengl {

class ColorBufferReaderWithEGLImage : public graphics::ColorBufferReader
{
public:
	ColorBufferReaderWithEGLImage(CachedTexture * _pTexture, CachedBindTexture * _bindTexture);
	~ColorBufferReaderWithEGLImage() override;

	const u8 * _readPixels(const ReadColorBufferParams& _params, u32& _heightOffset, u32& _stride) override;

	void cleanUp() override;

private:
	void _initBuffers();

	CachedBindTexture * m_bindTexture;
	GraphicBufferWrapper m_hardwareBuffer;
	EGLImageKHR m_image;
	uint64_t m_usage;
	bool m_bufferLocked;
};

}

#endif //EGL
