// Based on https://github.com/chromium/chromium/blob/master/base/android/android_hardware_buffer_compat.h

#ifndef ANDROID_ANDROID_HARDWARE_BUFFER_COMPAT_H_
#define ANDROID_ANDROID_HARDWARE_BUFFER_COMPAT_H_

#include <android/hardware_buffer.h>
#include <android/sensor.h>
#include "dcheck.h"

extern "C" {
using PFAHardwareBuffer_allocate = int (*)(const AHardwareBuffer_Desc *desc,
											AHardwareBuffer **outBuffer) ;
using PFAHardwareBuffer_acquire = void (*)(AHardwareBuffer *buffer) ;
using PFAHardwareBuffer_describe = void (*)(const AHardwareBuffer *buffer,
											AHardwareBuffer_Desc *outDesc) ;
using PFAHardwareBuffer_lock = int (*)(AHardwareBuffer *buffer,
										uint64_t usage,
										int32_t fence,
										const ARect *rect,
										void **outVirtualAddress) ;
using PFAHardwareBuffer_recvHandleFromUnixSocket =
int (*)(int socketFd, AHardwareBuffer **outBuffer) ;
using PFAHardwareBuffer_release = void (*)(AHardwareBuffer *buffer) ;
using PFAHardwareBuffer_sendHandleToUnixSocket =
int (*)(const AHardwareBuffer *buffer, int socketFd) ;
using PFAHardwareBuffer_unlock = int (*)(AHardwareBuffer *buffer,
										 int32_t *fence) ;
}

namespace opengl {

// This class provides runtime support for working with AHardwareBuffer objects
// on Android O systems without requiring building for the Android O NDK level.
// Don't call GetInstance() unless IsSupportAvailable() returns true.
class AndroidHardwareBufferCompat {
public:

	AndroidHardwareBufferCompat (const AndroidHardwareBufferCompat&) = delete;
	AndroidHardwareBufferCompat& operator= (const AndroidHardwareBufferCompat&) = delete;

	static bool IsSupportAvailable();
	static AndroidHardwareBufferCompat& GetInstance();

	int Allocate(const AHardwareBuffer_Desc* desc, AHardwareBuffer** outBuffer);
	void Acquire(AHardwareBuffer* buffer);
	void Describe(const AHardwareBuffer* buffer, AHardwareBuffer_Desc* outDesc);
	int Lock(AHardwareBuffer* buffer,
		uint64_t usage,
		int32_t fence,
		const ARect* rect,
		void** out_virtual_address);
	int RecvHandleFromUnixSocket(int socketFd, AHardwareBuffer** outBuffer);
	void Release(AHardwareBuffer* buffer);
	int SendHandleToUnixSocket(const AHardwareBuffer* buffer, int socketFd);
	int Unlock(AHardwareBuffer* buffer, int32_t* fence);

private:

	AndroidHardwareBufferCompat();
	~AndroidHardwareBufferCompat() = default;

	PFAHardwareBuffer_allocate allocate_;
	PFAHardwareBuffer_acquire acquire_;
	PFAHardwareBuffer_describe describe_;
	PFAHardwareBuffer_lock lock_;
	PFAHardwareBuffer_recvHandleFromUnixSocket recv_handle_;
	PFAHardwareBuffer_release release_;
	PFAHardwareBuffer_sendHandleToUnixSocket send_handle_;
	PFAHardwareBuffer_unlock unlock_;

};

}  // namespace opengl

#endif  // ANDROID_ANDROID_HARDWARE_BUFFER_COMPAT_H_