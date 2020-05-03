// Based on https://github.com/chromium/chromium/blob/master/base/android/android_hardware_buffer_compat.h
#pragma once

#include <android/hardware_buffer.h>
#include <EGL/egl.h>
#include <Graphics/OpenGLContext/GraphicBuffer/PrivateApi/GraphicBuffer.h>

namespace opengl {

// This class provides runtime support for working with AHardwareBuffer objects
// on Android O systems without requiring building for the Android O NDK level.
// Don't call GetInstance() unless IsSupportAvailable() returns true.
class GraphicBufferWrapper {
public:

	GraphicBufferWrapper (const GraphicBufferWrapper&) = delete;
	GraphicBufferWrapper& operator= (const GraphicBufferWrapper&) = delete;

	GraphicBufferWrapper();
	~GraphicBufferWrapper();

	static bool isSupportAvailable();

    static bool isPublicSupportAvailable();

	bool allocate(const AHardwareBuffer_Desc* desc);
	int lock(uint64_t usage, void** out_virtual_address);
	void release();
	void unlock();
	EGLClientBuffer getClientBuffer();

private:

	static int getApiLevel();

	bool m_private;
	GraphicBuffer* m_privateGraphicBuffer;
	AHardwareBuffer* m_publicGraphicBuffer;

};

}  // namespace opengl
