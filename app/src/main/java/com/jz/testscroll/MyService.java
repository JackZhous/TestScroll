package com.jz.testscroll;

import android.app.Service;
import android.content.Intent;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.SparseArray;

import java.util.HashMap;
import java.util.Map;

public class MyService extends Service {

    private SparseArray<String> params;

    public MyService() {
        params = new SparseArray<>();
    }

    @Override
    public IBinder onBind(Intent intent) {
        return stub;
    }

    IremoteService.Stub stub = new IremoteService.Stub() {

        @Override
        public boolean connectActivity(int key, String vaalue) throws RemoteException {
            try {
                params.append(key, vaalue);
                return true;
            }catch (Exception e){
                return false;
            }

        }

        @Override
        public int add(int num1, int num2) throws RemoteException {
            return num1+num2;
        }
    };

}
