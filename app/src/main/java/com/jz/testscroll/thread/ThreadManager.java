package com.jz.testscroll.thread;

import java.util.Collection;
import java.util.Iterator;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingDeque;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

public class ThreadManager {

    private ThreadPoolExecutor executor;

    private BlockingQueue<Runnable> queue;
    public ThreadManager() {
        queue = new ArrayBlockingQueue<>(10, true);     //true是公平锁
        executor = new ThreadPoolExecutor(4, 100, 1000, TimeUnit.MICROSECONDS, queue);
    }


    public void submitTask(Runnable runnable){
        executor.execute(runnable);
    }

    public void onRelease(){
        executor.shutdown();
        executor = null;
        queue = null;
    }
}
