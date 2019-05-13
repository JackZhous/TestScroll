package com.jz.testscroll.thread;

import android.util.Log;

public class TaskSleep implements Runnable {



    @Override
    public void run() {
        try {
            Log.i("j_tag", "我是线程"+ Thread.currentThread().getId() + "正在执行");
            Thread.sleep(5000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        Log.i("j_tag", "线程"+Thread.currentThread().getId() + "结束");
    }
}
