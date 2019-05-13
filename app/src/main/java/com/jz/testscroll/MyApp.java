package com.jz.testscroll;

import android.app.Application;
import android.util.Log;

public class MyApp extends Application {

    @Override
    public void onCreate() {
        super.onCreate();
        
        Log.i("j_tag", "onCreate and pid" + android.os.Process.myPid())
    }
}
