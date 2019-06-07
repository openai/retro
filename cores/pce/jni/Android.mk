LOCAL_PATH := $(call my-dir)
DEBUG = 0
FRONTEND_SUPPORTS_RGB565 = 1
FAST = 1

include $(CLEAR_VARS)

GIT_VERSION := " $(shell git rev-parse --short HEAD || echo unknown)"
ifneq ($(GIT_VERSION)," unknown")
	LOCAL_CXXFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
endif

ifeq ($(TARGET_ARCH),arm)
ANDROID_FLAGS := -DANDROID_ARM
LOCAL_ARM_MODE := arm
endif

ifeq ($(TARGET_ARCH),x86)
ANDROID_FLAGS := -DANDROID_X86
endif

ifeq ($(TARGET_ARCH),mips)
ANDROID_FLAGS := -DANDROID_MIPS -D__mips__ -D__MIPSEL__
endif

LOCAL_CXXFLAGS += $(ANDROID_FLAGS)
LOCAL_CFLAGS   += $(ANDROID_FLAGS)

CORE_DIR       := ..
LOCAL_MODULE    := libretro

# If you have a system with 1GB RAM or more - cache the whole 
# CD for CD-based systems in order to prevent file access delays/hiccups
CACHE_CD = 0

PTHREAD_FLAGS = -pthread
NEED_CD = 1
NEED_BPP = 16
WANT_NEW_API = 1
NEED_BLIP = 1
NEED_STEREO_SOUND = 1
NEED_THREADING = 1
NEED_TREMOR = 1
NEED_CRC32 = 1
CORE_DEFINE := -DWANT_PCE_FAST_EMU

TARGET_NAME := mednafen_pce_fast_libretro

include ../Makefile.common

LOCAL_SRC_FILES += $(SOURCES_CXX) $(SOURCES_C)
EXTRA_GCC_FLAGS := -funroll-loops

ifeq ($(DEBUG),0)
   FLAGS += -O3 $(EXTRA_GCC_FLAGS)
else
   FLAGS += -O0 -g
endif

LDFLAGS += $(fpic) $(SHARED)
FLAGS += $(fpic) $(NEW_GCC_FLAGS) $(INCFLAGS)

FLAGS += $(ENDIANNESS_DEFINES) -DSIZEOF_DOUBLE=8 $(WARNINGS) -DPACKAGE=\"mednafen\" -DMEDNAFEN_VERSION_NUMERIC=926 -DPSS_STYLE=1 -DMPC_FIXED_POINT $(CORE_DEFINE) -DSTDC_HEADERS -D__STDC_LIMIT_MACROS -D__LIBRETRO__ -DNDEBUG -D_LOW_ACCURACY_ $(SOUND_DEFINE) -DINLINE="inline"

LOCAL_CFLAGS =  $(FLAGS) 
LOCAL_CXXFLAGS += $(FLAGS) -fexceptions

include $(BUILD_SHARED_LIBRARY)
