//
// Created by ShangPingping on 2018/6/21.
//

#ifndef LIVE555_RTSP_COMMON_H
#define LIVE555_RTSP_COMMON_H

class RTSPCommon {
public:
    static jstring charToJstring(JNIEnv* env, const char* pat);
    static void infoCallBack(jstring jstr);
};

#endif // LIVE555_RTSP_COMMON_H
