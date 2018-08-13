//
// Created by ShangPingping on 2018/5/3.
//

#ifndef LIVE555_MY_RTSP_CLIENT_H
#define LIVE555_MY_RTSP_CLIENT_H

#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_TAG "MyRTSPClient"
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#define RTSP_CLIENT_VERBOSITY_LEVEL 1
#define REQUEST_STREAMING_OVER_TCP false

JNIEXPORT jint JNICALL
Java_com_example_cynthiaty_live555_rtsp_RtspClient_start(JNIEnv *, jclass, jstring);
JNIEXPORT void JNICALL
Java_com_example_cynthiaty_live555_rtsp_RtspClient_stop(JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif

class MyRTSPClient {
public:
    static int start(const char* path);
    static void openURL(UsageEnvironment& env, char const* progName, char const* rtspURL);
    static void continueAfterDESCRIBE(RTSPClient* rtspClient, int resultCode, char* resultString);
    static void continueAfterSETUP(RTSPClient* rtspClient, int resultCode, char* resultString);
    static void continueAfterPLAY(RTSPClient* rtspClient, int resultCode, char* resultString);

    static void setupNextSubsession(RTSPClient* rtspClient);
    static void subsessionAfterPlaying(void* clientData);
    static void subsessionByeHandler(void* clientData);
    static void streamTimerHandler(void* clientData);
};

#endif // LIVE555_MY_RTSP_CLIENT_H
