package com.example.bsdiffdemo;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.FileProvider;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import java.io.File;
import java.io.IOException;

public class MainActivity extends AppCompatActivity {

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        TextView tv = findViewById(R.id.sample_text);
        tv.setText(BuildConfig.VERSION_NAME);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            String[] perms = {Manifest.permission.WRITE_EXTERNAL_STORAGE,
                    Manifest.permission.REQUEST_INSTALL_PACKAGES};
            if (checkSelfPermission(
                    perms[0]) == PackageManager.PERMISSION_DENIED || checkSelfPermission(
                    perms[1]) == PackageManager.PERMISSION_DENIED) {
                requestPermissions(perms, 200);
            }
        }
    }



    public native void doPatchNative(String oldApk, String newApk, String patch);

    public void onDoWork(View view) {
        new AsyncTask<Void,Void, File>(){

            @Override
            protected File doInBackground(Void... voids) {
                String oldApk = getApplicationInfo().sourceDir;
                File newFile = new File(Environment.getExternalStorageDirectory(),"newApk.apk");
                if (!newFile.exists()) {
                    try {
                        newFile.createNewFile();
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }

                File patch = new File(Environment.getExternalStorageDirectory(),"patch.diff");
                // 合成差分包
                doPatchNative(oldApk,newFile.getAbsolutePath(),patch.getAbsolutePath());
                return newFile;
            }


            @Override
            protected void onPostExecute(File apkFile) {
                //安装
                if (!apkFile.exists()) {
                    return;
                }
                //安装
                Intent intent = new Intent(Intent.ACTION_VIEW);
                intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
                if (Build.VERSION.SDK_INT >= 24) { //Android 7.0及以上
                    // 参数2 清单文件中provider节点里面的authorities ; 参数3  共享的文件,即apk包的file类
                    Uri apkUri = FileProvider.getUriForFile(MainActivity.this,
                            getApplicationInfo().packageName + ".provider", apkFile);
                    //对目标应用临时授权该Uri所代表的文件
                    intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
                    intent.setDataAndType(apkUri, "application/vnd.android.package-archive");
                } else {
                    intent.setDataAndType(Uri.fromFile(apkFile),
                            "application/vnd.android.package-archive");
                }
                startActivity(intent);
            }
        }.execute();
    }
}
