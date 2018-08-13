//
// Created by ShangPingping on 2018/6/21.
//

#include <jni.h>
#include <cstring>
#include <string>
#include <Boolean.hh>
#include <Media.hh>
#include <RTSPClient.hh>
#include <UsageEnvironment.hh>
#include "StreamClientState.h"
#include "SubRTSPClient.h"
#include "RTSPCommon.h"

JNIEnv *_env = NULL;
jclass _clazz = NULL;
jobject _obj = NULL;
jmethodID _methodID1 = NULL;
jmethodID _methodID2 = NULL;

RTSPClient* _rtspClient = NULL;
unsigned char runningFlag = 0;
char eventLoopWatchVariable = 0;
Boolean isRunning = false;
std::string printInfo = "";

/**
 * c/c++ string turn to jstring.
 * @param env
 * @param pat
 * @return
 */
jstring RTSPCommon::charToJstring(JNIEnv* env, const char* pat) {
    if (env == NULL) return NULL;

    jclass _strclass = (env)->FindClass("java/lang/String");
    jstring _encode = (env)->NewStringUTF("utf-8");
    jmethodID ctorID = env->GetMethodID(_strclass, "<init>", "([BLjava/lang/String;)V");
    jbyteArray bytes = env->NewByteArray(strlen(pat));
    env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*) pat);
    jstring jstr = (jstring) env->NewObject(_strclass, ctorID, bytes, _encode);

    env->DeleteLocalRef(bytes);
    env->DeleteLocalRef(_strclass);
    env->DeleteLocalRef(_encode);

    return jstr;
}

/**
 * info call back.
 * @param jstr
 */
void RTSPCommon::infoCallBack(jstring jstr) {
    if (!isRunning) return;

    if (_obj == NULL && _clazz != NULL) {
        _obj = _env->AllocObject(_clazz);
    }

    if (_obj != NULL && _methodID1 != NULL) {
        _env->CallVoidMethod(_obj, _methodID1, jstr);
        _env->DeleteLocalRef(jstr);
    }

    printInfo = "";
}

