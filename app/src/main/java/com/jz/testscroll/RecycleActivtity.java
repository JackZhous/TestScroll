package com.jz.testscroll;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;

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
        super.onCreate(savedInstanceState);
        setContentView(R.layout.test_recycle);
        RecyclerView recycler = findViewById(R.id.recycle);
        recycler.setLayoutManager(new LinearLayoutManager(this));
        recycler.setAdapter(new Adapter());
    }
}
