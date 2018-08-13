#include <jni.h>

extern "C"
JNIEXPORT jstring

JNICALL
Java_com_example_cynthiaty_helloworld_MainActivity_stringFromJNI(JNIEnv* env, jobject thiz) {
    const char* hello = "Hello world from jni";
    return env->NewStringUTF(hello);
}
