package com.example.licenseapply;

import android.util.Log;

public class LicenseTool {
    private static final String TAG = "LicenseTool";

    static {
        try {
            System.loadLibrary("license_tool");
        } catch (UnsatisfiedLinkError ule) {
            Log.e(TAG, "Error: failed to load license_persisted");
        }
    }
    public static native int vendorStorageWriteLicense(String license);
    public static native String vendorStorageReadLicense();
}
