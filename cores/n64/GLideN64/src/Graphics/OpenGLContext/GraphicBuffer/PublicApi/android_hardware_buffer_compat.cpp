// Based on https://github.com/chromium/chromium/blob/master/base/android/android_hardware_buffer_compat.h

#include "android_hardware_buffer_compat.h"

#include <dlfcn.h>
#include <sys/system_properties.h>
#include <sstream>

namespace opengl {


AndroidHardwareBufferCompat::AndroidHardwareBufferCompat() {
    DCHECK(IsSupportAvailable());

    // TODO(klausw): If the Chromium build requires __ANDROID_API__ >= 26 at some
    // point in the future, we could directly use the global functions instead of
    // dynamic loading. However, since this would be incompatible with pre-Oreo
    // devices, this is unlikely to happen in the foreseeable future, so just
    // unconditionally use dynamic loading.

    // cf. base/android/linker/modern_linker_jni.cc
    void *main_dl_handle = dlopen(nullptr, RTLD_NOW);

    *reinterpret_cast<void **>(&allocate_) =
            dlsym(main_dl_handle, "AHardwareBuffer_allocate");
    DCHECK(allocate_);

    *reinterpret_cast<void **>(&acquire_) =
            dlsym(main_dl_handle, "AHardwareBuffer_acquire");
    DCHECK(acquire_);

    *reinterpret_cast<void **>(&describe_) =
            dlsym(main_dl_handle, "AHardwareBuffer_describe");
    DCHECK(describe_);

    *reinterpret_cast<void **>(&lock_) =
            dlsym(main_dl_handle, "AHardwareBuffer_lock");
    DCHECK(lock_);

    *reinterpret_cast<void **>(&recv_handle_) =
            dlsym(main_dl_handle, "AHardwareBuffer_recvHandleFromUnixSocket");
    DCHECK(recv_handle_);

    *reinterpret_cast<void **>(&release_) =
            dlsym(main_dl_handle, "AHardwareBuffer_release");
    DCHECK(release_);

    *reinterpret_cast<void **>(&send_handle_) =
            dlsym(main_dl_handle, "AHardwareBuffer_sendHandleToUnixSocket");
    DCHECK(send_handle_);

    *reinterpret_cast<void **>(&unlock_) =
            dlsym(main_dl_handle, "AHardwareBuffer_unlock");
    DCHECK(unlock_);
}

bool AndroidHardwareBufferCompat::IsSupportAvailable() {

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

    return apiLevel >= 26;
}


AndroidHardwareBufferCompat &AndroidHardwareBufferCompat::GetInstance() {
    static AndroidHardwareBufferCompat compat;
    return compat;
}

int AndroidHardwareBufferCompat::Allocate(const AHardwareBuffer_Desc *desc,
                                           AHardwareBuffer **out_buffer) {
    DCHECK(IsSupportAvailable());
    return allocate_(desc, out_buffer);
}

void AndroidHardwareBufferCompat::Acquire(AHardwareBuffer *buffer) {
    DCHECK(IsSupportAvailable());
    acquire_(buffer);
}

void AndroidHardwareBufferCompat::Describe(const AHardwareBuffer *buffer,
                                           AHardwareBuffer_Desc *out_desc) {
    DCHECK(IsSupportAvailable());
    describe_(buffer, out_desc);
}

int AndroidHardwareBufferCompat::Lock(AHardwareBuffer *buffer,
                                      uint64_t usage,
                                      int32_t fence,
                                      const ARect *rect,
                                      void **out_virtual_address) {
    DCHECK(IsSupportAvailable());
    return lock_(buffer, usage, fence, rect, out_virtual_address);
}

int AndroidHardwareBufferCompat::RecvHandleFromUnixSocket(
        int socket_fd,
        AHardwareBuffer **out_buffer) {
    DCHECK(IsSupportAvailable());
    return recv_handle_(socket_fd, out_buffer);
}

void AndroidHardwareBufferCompat::Release(AHardwareBuffer *buffer) {
    DCHECK(IsSupportAvailable());
    release_(buffer);
}

int AndroidHardwareBufferCompat::SendHandleToUnixSocket(
        const AHardwareBuffer *buffer,
        int socket_fd) {
    DCHECK(IsSupportAvailable());
    return send_handle_(buffer, socket_fd);
}

int AndroidHardwareBufferCompat::Unlock(AHardwareBuffer *buffer,
                                        int32_t *fence) {
    DCHECK(IsSupportAvailable());
    return unlock_(buffer, fence);
}

}  // namespace base