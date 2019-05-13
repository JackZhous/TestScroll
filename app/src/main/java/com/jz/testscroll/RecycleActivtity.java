package com.jz.testscroll;

import android.os.Bundle;
import android.os.Debug;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.Log;

import com.jz.testscroll.thread.TaskSleep;
import com.jz.testscroll.thread.ThreadManager;

/**
 * @author jackzhous
 * @package com.jz.testscroll
 * @filename RecycleActivtity
 * date on 2019/5/6 7:14 PM
 * @describe TODO
 * @email jackzhouyu@foxmail.com
 **/
public class RecycleActivtity extends AppCompatActivity {


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        Debug.startMethodTracing("recycle.trace");
        Log.i("j_tag", "onCreate " + System.currentTimeMillis());
        super.onCreate(savedInstanceState);
        setContentView(R.layout.test_recycle);
        RecyclerView recycler = findViewById(R.id.recycle);
        recycler.setLayoutManager(new LinearLayoutManager(this));
        recycler.setAdapter(new Adapter());
        ThreadManager manager = new ThreadManager();
        for(int i = 0; i < 100; i++){
            TaskSleep sleep = new TaskSleep();
            manager.submitTask(sleep);
        }
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        Log.i("j_tag", hasFocus+ " " + System.currentTimeMillis());
        if(hasFocus){
            Debug.stopMethodTracing();
        }
        super.onWindowFocusChanged(hasFocus);
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.i("j_tag","onResume " + System.currentTimeMillis());
    }
}
