package com.example.loadgif;

import android.graphics.Bitmap;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.widget.ImageView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    ImageView iv_img;
    private GifNativeDecoder mGifNativeDecoder;
    private Bitmap mBitmap;
    private File mFile;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        iv_img = findViewById(R.id.image);
        mFile = new File(Environment.getExternalStorageDirectory(),"demo.gif");
    }


    Handler handler = new Handler(){
        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);
            handler.sendEmptyMessageDelayed(1,GifNativeDecoder.upFrame(mBitmap,mGifNativeDecoder.getGifPointer()));
            iv_img.setImageBitmap(mBitmap);
        }
    };




}
