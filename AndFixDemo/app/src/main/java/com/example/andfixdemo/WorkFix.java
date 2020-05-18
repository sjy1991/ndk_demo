package com.example.andfixdemo;

public class WorkFix {
    @MethodFix(className = "com.example.andfixdemo.Work",methodName = "test")
    public void test(){
        String str = new String("test");
        String s = str.intern();
    }
}
