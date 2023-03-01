package com.example.licenseapply;

import androidx.appcompat.app.AppCompatActivity;

import android.graphics.Color;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;

import com.bjnet.licensev3.apply.ApplyLicenseRetInfo;
import com.bjnet.licensev3.apply.BJLicenseMoudle;

import java.net.NetworkInterface;
import java.net.SocketException;

public class MainActivity extends AppCompatActivity {
    private static final String TAG = "MainActivity";

    public static final String PRO_GUARD_SALT = "";//加密参数，任意字符串
    public static final int PRO_GUARD_MODE = 0;//是否加密 0为不加密；1为加密

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        String key = "";
        boolean ret = applyLicense(key);
        Log.d(TAG, "apply result: " + ret);
    }

    private boolean applyLicense(String key) {
        BJLicenseMoudle moudle = BJLicenseMoudle.getInstance();
        ApplyLicenseRetInfo info = moudle.applyLicense(key, getMac(), PRO_GUARD_MODE, PRO_GUARD_SALT);
        if (info == null) {
            Log.e(TAG, "apply failed! RetInfo = null");
            return false;
        }
        if (info.getRetcode() == 0 && !TextUtils.isEmpty(info.getLicenseKey())) {
            int ret = LicenseTool.vendorStorageWriteLicense(info.getLicenseKey());
            if (ret == 0) {
                return true;
            }
        }
        return false;
    }

    private String getMac(){
        StringBuilder mac = new StringBuilder();
        try {
            NetworkInterface networkInterface = NetworkInterface.getByName("wlan0");
            byte[] macbyte= networkInterface.getHardwareAddress();
            if(macbyte!=null){
                for(int i=0;i<macbyte.length;i++) {
                    byte b=macbyte[i];
                    mac.append(String.format("%02X", b));
                    if(i!=macbyte.length-1) mac.append(":");
                }
            }
        } catch (SocketException e) {
            e.printStackTrace();
        }

        return mac.toString();
    }
}