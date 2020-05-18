package com.example.andfixdemo;

import android.content.Context;
import android.text.TextUtils;
import android.util.Log;
import android.view.TextureView;

import java.io.File;
import java.io.IOException;
import java.lang.reflect.Method;
import java.util.Enumeration;

import dalvik.system.DexFile;


public class DexManager {
    private Context mContext;
    private static final String TAG = "DexManager";
    private static final DexManager ourInstance = new DexManager();
    static {
        System.loadLibrary("native-lib");
    }

    public static DexManager getInstance() {
        return ourInstance;
    }

    private DexManager() {
    }

    public void setContext(Context context) {
        mContext = context;
    }


    public void loadDexFile(File file) {
        try {
            DexFile dexFile = DexFile.loadDex(file.getAbsolutePath(), new File(mContext.getCacheDir(), "opt").getAbsolutePath(), Context.MODE_PRIVATE);
            Enumeration<String> entries = dexFile.entries();
            while (entries.hasMoreElements()) {
                String className = entries.nextElement();
                // 不能用Class.forName, 因为他只能读取到app中class
                Class fixClass = dexFile.loadClass(className, mContext.getClassLoader());
                if (fixClass != null) {
                    fixClass(fixClass);
                }

            }

        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void fixClass(Class fixClass) {
        Method[] declaredMethods = fixClass.getDeclaredMethods();
        MethodFix methodFix;
        String className;
        String methodName;
        Method bugMethod;
        Class bugClass;
        for (Method declaredMethod : declaredMethods) {
            methodFix = declaredMethod.getAnnotation(MethodFix.class);
            if (methodFix != null) {
                className = methodFix.className();
                methodName = methodFix.methodName();
                if (!TextUtils.isEmpty(className) && !TextUtils.isEmpty(methodName)) {
                    try {
                        bugClass = Class.forName(className);
                        bugMethod = bugClass.getDeclaredMethod(methodName, declaredMethod.getParameterTypes());
                        replace(bugMethod,declaredMethod);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }

                }
            }
        }
    }

    private native void replace(Method bugMethod, Method fixMethod);
}
