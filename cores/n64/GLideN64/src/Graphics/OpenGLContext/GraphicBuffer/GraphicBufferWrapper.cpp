// Based on https://github.com/chromium/chromium/blob/master/base/android/android_hardware_buffer_compat.h

#include "GraphicBufferWrapper.h"
#include <Graphics/OpenGLContext/GraphicBuffer/PublicApi/android_hardware_buffer_compat.h>
#include "../GLFunctions.h"

#include <dlfcn.h>
#include <sys/system_properties.h>
#include <sstream>

namespace opengl {


GraphicBufferWrapper::GraphicBufferWrapper() {
	m_private = false;

	if (!isSupportAvailable()) {
		return;
	}

	if (getApiLevel() <= 23) {
		m_private = true;
		m_privateGraphicBuffer = new GraphicBuffer();
	}
}

GraphicBufferWrapper::~GraphicBufferWrapper() {
	if (m_private) {
		delete m_privateGraphicBuffer;
	}
}

bool GraphicBufferWrapper::isSupportAvailable() {

	int apiLevel = getApiLevel();
	return apiLevel >= 26 || apiLevel <= 23;
}

bool GraphicBufferWrapper::isPublicSupportAvailable() {

	int apiLevel = getApiLevel();
	return apiLevel >= 26;
}

bool GraphicBufferWrapper::allocate(const AHardwareBuffer_Desc *desc) {

	if (m_private) {
		return m_privateGraphicBuffer->reallocate(desc->width, desc->height, desc->format, desc->usage);
	} else {
		return AndroidHardwareBufferCompat::GetInstance().Allocate(desc, &m_publicGraphicBuffer) == 0;
	}
}


int GraphicBufferWrapper::lock(uint64_t usage, void **out_virtual_address) {

	int returnValue = 0;
	if (m_private) {
		returnValue = m_privateGraphicBuffer->lock(usage, out_virtual_address);
	} else {
		returnValue = AndroidHardwareBufferCompat::GetInstance().Lock(m_publicGraphicBuffer, usage, -1, nullptr, out_virtual_address);
	};

	return returnValue;
}


void GraphicBufferWrapper::release() {
	if (!m_private) {
		AndroidHardwareBufferCompat::GetInstance().Release(m_publicGraphicBuffer);
	}
}

void GraphicBufferWrapper::unlock() {

	if (m_private) {
		m_privateGraphicBuffer->unlock();
	} else {
		AndroidHardwareBufferCompat::GetInstance().Unlock(m_publicGraphicBuffer, nullptr);
	}

}


EGLClientBuffer GraphicBufferWrapper::getClientBuffer() {
	EGLClientBuffer clientBuffer = nullptr;
	if (m_private) {
		clientBuffer = (EGLClientBuffer)m_privateGraphicBuffer->getNativeBuffer();
	} else {
		clientBuffer = eglGetNativeClientBufferANDROID(m_publicGraphicBuffer);
	}

	return clientBuffer;
}


int GraphicBufferWrapper::getApiLevel()
{
	static bool apiLevelChecked = false;
	static int apiLevel = 0;

	if (!apiLevelChecked)
	{
		char *androidApiLevel = new char[PROP_VALUE_MAX];

		int valid = __system_property_get("ro.build.version.sdk", androidApiLevel);

		if (valid > 0) {
			std::stringstream convert(androidApiLevel);
			convert >> apiLevel;
		}
	}
	return apiLevel;
}

}
