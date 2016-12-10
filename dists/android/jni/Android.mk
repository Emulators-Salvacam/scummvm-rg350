LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

GIT_VERSION := " $(shell git rev-parse --short HEAD || echo unknown)"
ifneq ($(GIT_VERSION)," unknown")
	LOCAL_CXXFLAGS += -DGIT_VERSION=\"$(GIT_VERSION)\"
endif

APP_ABI := $(ABI)
LOCAL_MODULE := scummvm
LOCAL_SRC_FILES := ../libscummvm.so

include $(PREBUILT_SHARED_LIBRARY)
