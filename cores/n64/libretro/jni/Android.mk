LOCAL_PATH := $(call my-dir)

ROOT_DIR     := $(LOCAL_PATH)/../..
LIBRETRO_DIR := $(ROOT_DIR)/libretro
INCFLAGS     := -I$(ROOT_DIR)/GLideN64/src/GLideNHQ/inc

# Reset flags that the common makefile doesn't properly handle
platform     := android
COREFLAGS    :=
COREASMFLAGS :=
CORELDLIBS   :=
DYNAFLAGS    :=
GLES         :=
GLFLAGS      :=
HAVE_NEON    :=
SOURCES_C    :=
SOURCES_CXX  :=
SOURCES_ASM  :=
SOURCES_NASM :=
ANDROID      := 1
AWK          ?= awk
STRINGS      ?= strings
TR           ?= tr
HAVE_PARALLEL_RSP ?= 0
HAVE_THR_AL       ?= 0
LLE               ?= 0
LOCAL_SHORT_COMMANDS := true
WITH_DYNAREC :=

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
  WITH_DYNAREC := arm
  HAVE_NEON := 1
  STRINGS := arm-linux-androideabi-$(STRINGS)
  LLE = 1
  HAVE_PARALLEL_RSP = 1
  HAVE_THR_AL = 1
else ifeq ($(TARGET_ARCH_ABI),arm64-v8a)
  WITH_DYNAREC := aarch64
  STRINGS := aarch64-linux-android-$(STRINGS)
  LLE = 1
  HAVE_PARALLEL_RSP = 1
  HAVE_THR_AL = 1
else ifeq ($(TARGET_ARCH_ABI),x86)
  # X86 dynarec isn't position independent, so it fails to build on newer ndks.
  # No warn shared textrel allows it to build, but still won't allow it to run on api 23+.
  WITH_DYNAREC := x86
  STRINGS := i686-linux-android-$(STRINGS)
  COREASMFLAGS := -f elf -d ELF_TYPE
  CORELDLIBS := -Wl,-no-warn-shared-textrel
else ifeq ($(TARGET_ARCH_ABI),x86_64)
  WITH_DYNAREC := x86_64
  STRINGS := x86_64-linux-android-$(STRINGS)
endif

ifeq ($(GLES3),1)
  GLLIB := -lGLESv3
else
  GLES  := 1
  GLLIB := -lGLESv2
endif

include $(ROOT_DIR)/Makefile.common

COREFLAGS += -D__LIBRETRO__ -DOS_ANDROID -DUSE_FILE32API -DM64P_PLUGIN_API -DM64P_CORE_PROTOTYPES -D_ENDUSER_RELEASE -DSINC_LOWER_QUALITY -DMUPENPLUSAPI -DTXFILTER_LIB -D__VEC4_OPT $(INCFLAGS) $(GLFLAGS) $(DYNAFLAGS) -DANDROID -DEGL_EGLEXT_PROTOTYPES -DHAVE_POSIX_MEMALIGN=1

ifeq ($(LLE), 1)
   COREFLAGS += -DHAVE_LLE
endif

GIT_VERSION := " $(shell git rev-parse --short HEAD || echo unknown)"
ifneq ($(GIT_VERSION)," unknown")
  COREFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
endif

include $(CLEAR_VARS)

include $(CLEAR_VARS)
LOCAL_MODULE           := retro
LOCAL_SRC_FILES        := $(SOURCES_CXX) $(SOURCES_C) $(SOURCES_ASM) $(SOURCES_NASM)
LOCAL_ASMFLAGS         := $(COREASMFLAGS)
LOCAL_CPPFLAGS         := -std=gnu++11 $(CXXFLAGS) $(COREFLAGS)
LOCAL_CFLAGS           := $(CFLAGS) $(COREFLAGS)
LOCAL_LDFLAGS          := -Wl,-version-script=$(LIBRETRO_DIR)/link.T
LOCAL_LDLIBS           := -llog -lEGL $(GLLIB) $(CORELDLIBS)
LOCAL_STATIC_LIBRARIES := 
LOCAL_CPP_FEATURES     := exceptions
LOCAL_ARM_MODE         := arm
LOCAL_ARM_NEON         := true
LOCAL_CONLYFLAGS       := -std=gnu11

include $(BUILD_SHARED_LIBRARY)
