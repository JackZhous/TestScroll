package com.jz.testscroll;

import android.content.Intent;
import android.os.Debug;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Debug.startMethodTracing("scroll.trace");
        Log.i("j_tag", "onCreate " + System.currentTimeMillis());
        super.onCreate(savedInstanceState);
        setContentView(R.layout.test_scroll);
    }


    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        Log.i("j_tag", hasFocus+ " " + System.currentTimeMillis());
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



    public void onClick(View view){
//        startActivity(new Intent(this, RecycleActivtity.class));
    }
}
