#include <jni.h>
#include <string>
#include "art_5_1.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_example_andfixdemo_DexManager_replace(JNIEnv *env, jobject thiz, jobject bug_method,
                                               jobject fix_method) {
    art::mirror::ArtMethod *bugArtMethod = reinterpret_cast<art::mirror::ArtMethod *>(env->FromReflectedMethod(
            bug_method));
    art::mirror::ArtMethod *fixArtMethod = reinterpret_cast<art::mirror::ArtMethod *>(env->FromReflectedMethod(
            fix_method));

    // 进行属性替换
    reinterpret_cast<art::mirror::Class *>(fixArtMethod->declaring_class_)->class_loader_
            = reinterpret_cast<art::mirror::Class *>(bugArtMethod->declaring_class_)->class_loader_;
    reinterpret_cast<art::mirror::Class *>(bugArtMethod->declaring_class_)->status_
            = reinterpret_cast<art::mirror::Class *>(fixArtMethod->declaring_class_)->status_ - 1;
    reinterpret_cast<art::mirror::Class *>(fixArtMethod->declaring_class_)->super_class_
            = reinterpret_cast<art::mirror::Class *>(bugArtMethod->declaring_class_)->super_class_ = 0;
    reinterpret_cast<art::mirror::Class *>(fixArtMethod->declaring_class_)->clinit_thread_id_
            = reinterpret_cast<art::mirror::Class *>(bugArtMethod->declaring_class_)->clinit_thread_id_;

    bugArtMethod->declaring_class_ = fixArtMethod->declaring_class_;
    bugArtMethod->dex_cache_resolved_methods_ = fixArtMethod->dex_cache_resolved_methods_;
    bugArtMethod->access_flags_ = fixArtMethod->access_flags_;
    bugArtMethod->dex_cache_resolved_types_ = fixArtMethod->dex_cache_resolved_types_;
    bugArtMethod->dex_code_item_offset_ = fixArtMethod->dex_code_item_offset_;
    bugArtMethod->method_index_ = fixArtMethod->method_index_;
    bugArtMethod->dex_method_index_ = fixArtMethod->dex_method_index_;

    bugArtMethod->ptr_sized_fields_.entry_point_from_interpreter_ = fixArtMethod->ptr_sized_fields_.entry_point_from_interpreter_;
    bugArtMethod->ptr_sized_fields_.entry_point_from_jni_ = fixArtMethod->ptr_sized_fields_.entry_point_from_jni_;
    bugArtMethod->ptr_sized_fields_.entry_point_from_quick_compiled_code_ = fixArtMethod->ptr_sized_fields_.entry_point_from_quick_compiled_code_;


}