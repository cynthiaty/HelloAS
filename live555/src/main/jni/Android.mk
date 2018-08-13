LOCAL_PATH := $(call my-dir)

# Module: "live555" start.
include $(CLEAR_VARS)
LOCAL_MODULE := live555
LOCAL_SRC_FILES := live/libs/armeabi-v7a/liblive555.so
include $(PREBUILT_SHARED_LIBRARY)
# Module: "live555" stop.

# Module: "rtsplive555" start.
include $(CLEAR_VARS)

SRC = \
    MyRTSPClient.cpp \
    common/RTSPCommon.cpp \
    rtsp/DummySink.cpp \
    rtsp/SubRTSPClient.cpp \
    rtsp/StreamClientState.cpp \

INCLUDES = \
    $(LOCAL_PATH) \
    $(LOCAL_PATH)/live \
    $(LOCAL_PATH)/live/jni/BasicUsageEnvironment \
    $(LOCAL_PATH)/live/jni/BasicUsageEnvironment/include \
    $(LOCAL_PATH)/live/jni/UsageEnvironment \
    $(LOCAL_PATH)/live/jni/UsageEnvironment/include \
    $(LOCAL_PATH)/live/jni/groupsock \
    $(LOCAL_PATH)/live/jni/groupsock/include \
    $(LOCAL_PATH)/live/jni/liveMedia \
    $(LOCAL_PATH)/live/jni/liveMedia/include \
    $(LOCAL_PATH)/common \
    $(LOCAL_PATH)/rtsp \


EXPORT_INCLUDES = $(INCLUDES)

STATIC_LIBRARIES =

SHARED_LIBRARIES = live555

LOCAL_MODULE := rtsplive555
LOCAL_SRC_FILES := $(SRC)
LOCAL_C_INCLUDES := $(INCLUDES)
LOCAL_EXPORT_C_INCLUDES := $(EXPORT_INCLUDES)
LOCAL_STATIC_LIBRARIES := $(STATIC_LIBRARIES)
LOCAL_SHARED_LIBRARIES := $(SHARED_LIBRARIES)

LOCAL_CPPFLAGS := \
    -DNULL=0 -DSOCKLEN_T=socklen_t -DNO_SSTREAM -DBSD=1 -DNO_SSTREAM -fexceptions -DANDROID -DXLOCALE_NOT_USED

LOCAL_MODULE_TAGS := optional

LOCAL_LDLIBS := -llog

include $(BUILD_SHARED_LIBRARY)
# Module: "rtsplive555" stop.
