package com.example.andfixdemo;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.TextView;

import java.io.File;

public class MainActivity extends AppCompatActivity {
    private Work mWork;

    // Used to load the 'native-lib' library on application startup.


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        DexManager.getInstance().setContext(this);
        mWork = new Work();
    }

    public void start(View view) {
        mWork.test();
    }

    public void fix(View view) {
        DexManager.getInstance().loadDexFile(new File(Environment.getExternalStorageDirectory(),"out.dex"));
    }
}
