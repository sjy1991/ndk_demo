package com.example.loadgif;

import android.graphics.Bitmap;

public class GifNativeDecoder {

    static {
        System.loadLibrary("native-lib");
    }


    public long getGifPointer() {
        return gifPointer;
    }

    private long gifPointer;

    public GifNativeDecoder(long gifPointer) {
        this.gifPointer = gifPointer;
    }

    public static GifNativeDecoder load(String path) {
        long gifHander = loadGifNative(path);//拿到文件句柄
        return new GifNativeDecoder(gifHander);
    }

    public static native int getWidth(long pointer);

    public static native int getHeight(long pointer);

    private static native long loadGifNative(String path);

    public static native long upFrame(Bitmap bitmap, long pointer);

}
