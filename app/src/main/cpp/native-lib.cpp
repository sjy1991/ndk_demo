#include <jni.h>
#include <string>

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_bsdiffdemo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


extern "C"{
    extern int bspatch_main(int argc, char *argv[]);//通过extern可以声明&实现
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_bsdiffdemo_MainActivity_doPatchNative(JNIEnv *env, jobject thiz, jstring old_apk,
                                                       jstring new_apk, jstring patch_) {
    const char *oldApk = env->GetStringUTFChars(old_apk, 0);
    const char *newApk = env->GetStringUTFChars(new_apk, 0);
    const char *patch = env->GetStringUTFChars(patch_, 0);
    char *argv[4] = {
            "bspetch",
            const_cast<char *>(oldApk),
            const_cast<char *>(newApk),
            const_cast<char *>(patch)
    };
    bspatch_main(4, argv);

    env->ReleaseStringUTFChars(old_apk, oldApk);
    env->ReleaseStringUTFChars(new_apk, newApk);
    env->ReleaseStringUTFChars(patch_, patch);

}