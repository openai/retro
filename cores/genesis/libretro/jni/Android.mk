LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

GIT_VERSION ?= " $(shell git rev-parse --short HEAD || echo unknown)"
ifneq ($(GIT_VERSION)," unknown")
	LOCAL_CFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
endif

WANT_CRC32 := 1

CORE_DIR := ../..

TREMOR_SRC_DIR	:= $(CORE_DIR)/core/tremor
LIBRETRO_DIR	:= ..

LOCAL_MODULE    := retro

ifeq ($(TARGET_ARCH),arm)
LOCAL_CFLAGS += -DANDROID_ARM -D_ARM_ASSEM_
LOCAL_ARM_MODE := arm
endif

include $(LIBRETRO_DIR)/Makefile.common

LOCAL_SRC_FILES := $(SOURCES_C)

LOCAL_C_INCLUDES = $(foreach dir,$(GENPLUS_SRC_DIR),$(LOCAL_PATH)/$(dir)) \
			$(CORE_DIR)/libretro/libretro-common/include \
			$(LOCAL_PATH)/$(TREMOR_SRC_DIR) \
			$(LOCAL_PATH)/$(LIBRETRO_DIR)

LOCAL_CFLAGS += -ffast-math -O2 -funroll-loops -DINLINE="static inline" -DUSE_LIBTREMOR -DUSE_16BPP_RENDERING -DLSB_FIRST -DBYTE_ORDER=LITTLE_ENDIAN -D__LIBRETRO__ -DFRONTEND_SUPPORTS_RGB565 -DALIGN_LONG -DALIGN_WORD

include $(BUILD_SHARED_LIBRARY)
