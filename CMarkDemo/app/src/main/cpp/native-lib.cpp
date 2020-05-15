#include <jni.h>
#include <string>
#include "inc/fmod.hpp"
#include <android/log.h>

using namespace FMOD;

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_cmarkdemo_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    System *system;
    System_Create(&system);
    unsigned version;
    system->getVersion(&version);

    std::string hello = "Hello from C++";
    __android_log_print(ANDROID_LOG_ERROR,"Test", "FMOD Version: %08x", version);
    return env->NewStringUTF(hello.c_str());
}
