package com.example.licenseapply;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";

    public static final String PRO_GUARD_SALT = "";//加密参数，任意字符串
    public static final int PRO_GUARD_MODE = 0;//是否加密 0为不加密；1为加密

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        String key = "1111111";
        int ret = LicenseTool.vendorStorageWriteLicense(key);
        Log.d(TAG, "apply result: " + LicenseTool.vendorStorageReadLicense());
    }

}