#include <jni.h>
#include <string>
#include "gif_lib.h"
#include <android/log.h>

#define  LOG_TAG   "gifcompare"
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


#define  argb(a, r, g, b) ( ((a) & 0xff) << 24 ) | ( ((b) & 0xff) << 16 ) | ( ((g) & 0xff) << 8 ) | ((r) & 0xff)

typedef struct GifInfo {
    int current_frame;
    int total_frame;
    int *delays;
};

extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_loadgif_GifNativeDecoder_loadGifNative(JNIEnv *env, jclass type, jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);
    int error;

    // GifFileType保存了Gif的结构数据（图像的宽高，时间等信息）
    GifFileType *gifFileType = DGifOpenFileName(path, &error);
    // Gif初始化
    int ref = DGifSlurp(gifFileType);
    if (ref != GIF_OK) {
        LOGE("DGifSlurp 失败：%d", ref);

    }

    GifInfo *gifInfo = (GifInfo *) (malloc(sizeof(GifInfo)));

    // 清空内存 以防有旧数据在里面
    memset(gifInfo, 0, sizeof(GifInfo));

    gifInfo->delays = (int *) (malloc(sizeof(int) * gifFileType->ImageCount));
    memset(gifInfo->delays, 0, sizeof(int) * gifFileType->ImageCount);

    // GifInfo信息填充
    ExtensionBlock *ext;
    for (int i = 0; i < gifFileType->ImageCount; i++) {
        // 拿每一帧图像
        SavedImage image = gifFileType->SavedImages[i];

        for (int j = 0; j < image.ExtensionBlockCount; ++j) {
            if (image.ExtensionBlocks[j].Function == GRAPHICS_EXT_FUNC_CODE) {//找到扩展块的开头
                ext = &image.ExtensionBlocks[j];
                break;
            }
        }

        if (ext) {
            // 拿图型控制扩展块里的延时时间 单位10毫秒
            // 小端模式
            // Bytes[0] 保留字段
            // Bytes[1] 低八位
            // Bytes[2] 高八位
            int frameDelay = (ext->Bytes[2] << 8 | ext->Bytes[1]) * 10;
            gifInfo->delays[i] = frameDelay;
        }
    }
    gifInfo->total_frame = gifFileType->ImageCount;
    gifFileType->UserData = gifInfo;//方便后面
    env->ReleaseStringUTFChars(path_, path);
     return (jlong) (gifFileType);
    //return 100;
}


void
drawFrame(GifFileType *gifFileType, GifInfo *gifInfo, AndroidBitmapInfo bitmapInfo, void *pixles) {
    // 当前帧
    SavedImage image = gifFileType->SavedImages[gifInfo->current_frame];
    GifImageDesc imageDesc = image.ImageDesc;
    int *px = static_cast<int *>(pixles);// 像素首地址
    ColorMapObject *colorMapObject = imageDesc.ColorMap;
    if (colorMapObject == NULL) {
        colorMapObject = gifFileType->SColorMap;
    }
    // imageInfo.Top y方向的偏移
    px = reinterpret_cast<int *>((char *) px + bitmapInfo.stride * imageDesc.Top);
    int pointPixel;//记录像素位置
    int *line;//每一行的首地址
    GifColorType gifColorType;
    GifByteType gifByteType;
    for (int y = imageDesc.Top; y < imageDesc.Top + imageDesc.Height; ++y) {
        line = px;
        for (int x = imageDesc.Left; x < imageDesc.Left + imageDesc.Width; ++x) {
            pointPixel = (y - imageDesc.Top) * imageDesc.Width + (x - imageDesc.Width);
            // 当前帧的像素数据 压缩 LZW算法
            gifByteType = image.RasterBits[pointPixel];
            // 给每个像素上色
            if (colorMapObject != NULL) {
                gifColorType = colorMapObject->Colors[gifByteType];
                line[x] = argb(255, gifColorType.Red, gifColorType.Green, gifColorType.Blue);
            }
        }
        px = (int *) ((char *) px + bitmapInfo.stride);
    }

}


extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_loadgif_GifNativeDecoder_getWidth(JNIEnv *env, jclass type, jlong pointer_) {
    GifFileType *gifFileType = (GifFileType *) (pointer_);
    return gifFileType->SWidth;
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_loadgif_GifNativeDecoder_getHeight(JNIEnv *env, jclass type, jlong pointer_) {
    GifFileType *gifFileType = (GifFileType *) (pointer_);
    return gifFileType->SHeight;
}


extern "C"
JNIEXPORT jlong JNICALL
Java_com_example_loadgif_GifNativeDecoder_upFrame(JNIEnv *env, jclass type, jobject bitmap,
                                                  jlong pointer_) {
    GifFileType *gifFileType = (GifFileType *) (pointer_);
    GifInfo *gifInfo = static_cast<GifInfo *>(gifFileType->UserData);
    AndroidBitmapInfo bitmapInfo;
    // 通过Bitmap获取AndroidBitmap
    AndroidBitmap_getInfo(env, bitmap, &bitmapInfo);
    void *pixels;// 像素组
    AndroidBitmap_lockPixels(env, bitmap, &pixels);
    drawFrame(gifFileType, gifInfo, bitmapInfo, pixels);
    gifInfo->current_frame += 1;
    if (gifInfo->current_frame >= gifInfo->total_frame) {
        gifInfo->current_frame = 0;
    }
    AndroidBitmap_unlockPixels(env, bitmap);
    return gifInfo->delays[gifInfo->current_frame];

};

