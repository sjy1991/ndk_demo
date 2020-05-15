#include <jni.h>

int test(){
    return 123;
}
// 返回值 + Java + 包名 + 类名 + 方法名
jint Java_com_example_ndk_11_MainActivity_myTest(){
return test();
}