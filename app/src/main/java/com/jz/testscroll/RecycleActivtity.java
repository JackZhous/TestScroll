package com.jz.testscroll;

import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Bundle;
import android.os.Debug;
import android.os.IBinder;
import android.os.RemoteException;
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

    private IremoteService stub;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.test_recycle);
        RecyclerView recycler = findViewById(R.id.recycle);
        recycler.setLayoutManager(new LinearLayoutManager(this));
        recycler.setAdapter(new Adapter());
        Intent intent = new Intent();
        intent.setAction("com.jz.service");
        intent.setPackage(getPackageName());
        bindService(intent, connection, BIND_AUTO_CREATE);
    }


    private ServiceConnection connection = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            stub = IremoteService.Stub.asInterface(service);
            if(stub == null){
                Log.i("j_tag", "stud null");
            }else {
                try {
                    if(stub.connectActivity(1, "connection")){
                        Log.i("j_tag", "add 1 +2" + stub.add(1,2));
                    }
                } catch (RemoteException e) {
                    e.printStackTrace();
                }
            }
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            stub = null;
        }
    };

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
//        Log.i("j_tag", hasFocus+ " " + System.currentTimeMillis());
//        if(hasFocus){
//            Debug.stopMethodTracing();
//        }
        super.onWindowFocusChanged(hasFocus);
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.i("j_tag","onResume " + System.currentTimeMillis());
    }
}
