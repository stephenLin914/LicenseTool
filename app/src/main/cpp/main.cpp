#include <linux/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <android/log.h>
#include <jni.h>
#include <fstream>
#include <locale>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include "ScopedUtfChars.h"

#define TAG "LicenseTool"
#define LOG(level, tag, args...) \
  __android_log_print((level), (tag), args)
#define LOGV(tag, args...) LOG(ANDROID_LOG_VERBOSE, tag, args)
#define LOGD(tag, args...) LOG(ANDROID_LOG_DEBUG, tag, args)
#define LOGI(tag, args...) LOG(ANDROID_LOG_INFO, tag, args)
#define LOGE(tag, args...) LOG(ANDROID_LOG_ERROR, tag, args)
#define LOGF(tag, args...) LOG(ANDROID_LOG_FATAL, tag, args)
#define LOGW(tag, args...) LOG(ANDROID_LOG_WARN, tag, args)
typedef	unsigned short	    uint16;
typedef	unsigned long	    uint32;
typedef	unsigned char	    uint8;
#define DIR "/dev/vendor_storage"
#define VENDOR_SN_ID		1
#define VENDOR_WIFI_MAC_ID	2
#define VENDOR_LAN_MAC_ID	3
#define VENDOR_BLUETOOTH_ID	4
#define VENDOR_CAST_LICENSE_ID	16

#define VENDOR_REQ_TAG		0x56524551
#define VENDOR_READ_IO		_IOW('v', 0x01, unsigned int)
#define VENDOR_WRITE_IO	_IOW('v', 0x02, unsigned int)

#define VENDOR_CAST_LICENSE_DATA_LEN 1024

struct rk_vendor_req {
    uint32 tag;
    uint16 id;
    uint16 len;
    uint8 data[VENDOR_CAST_LICENSE_DATA_LEN];
};

jstring vendorStorageReadLicense(JNIEnv* env) {
    int ret;
    uint16 len;
    struct rk_vendor_req req;
    int sys_fd = open(DIR, O_RDONLY, 0);
    if (sys_fd < 0) {
        LOGE(TAG, "vendor_storage open fail %s\n", strerror(errno));
        return NULL;
    }

    req.tag = VENDOR_REQ_TAG;
    req.id = VENDOR_CAST_LICENSE_ID;
    req.len = VENDOR_CAST_LICENSE_DATA_LEN;
    memset(req.data, 0, VENDOR_CAST_LICENSE_DATA_LEN);
    ret = ioctl(sys_fd, VENDOR_READ_IO, &req);
    close(sys_fd);
    if (ret) {
        LOGE(TAG, "vendor read error! ret = %d\n", ret);
        return NULL;
    }
    len = req.len;
    if (len <= 0) {
        LOGE(TAG, "illegal length! len = %d\n", len);
        return NULL;
    }
    return env->NewStringUTF((char *)req.data);
}

jint vendorStorageWriteLicense(JNIEnv* env, jobject thiz, jstring licenseStr) {
    ScopedUtfChars license(env, licenseStr);
    int ret ;
    struct rk_vendor_req req;
    int sys_fd = open(DIR, O_RDONLY, 0);
    if(sys_fd < 0){
        LOGE(TAG, "vendor_storage open fail %s\n", strerror(errno));
        return -1;
    }
    memset(&req, 0, sizeof(req));
    req.tag = VENDOR_REQ_TAG;
    req.id = VENDOR_CAST_LICENSE_ID;
    req.len = license.size();
    memcpy(req.data, license.c_str(), license.size());
    ret = ioctl(sys_fd, VENDOR_WRITE_IO, &req);
    close(sys_fd);
    if(ret){
        LOGE(TAG, "error in saving license to IDB.\n");
        return -1;
    }
    return 0;
}

static int
RegisterNativeMethods(JNIEnv *env, const char *className, JNINativeMethod *methods, int methodNum) {
    jclass clazz = (env)->FindClass(className);
    if (clazz == NULL) {
        LOGE(TAG, "RegisterNativeMethods fail. clazz == NULL");
        return JNI_FALSE;
    }
    if ((env)->RegisterNatives(clazz, methods, methodNum) < 0) {
        LOGE(TAG, "RegisterNativeMethods fail");
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static JNINativeMethod gJni_Methods_table[] = {
    {"vendorStorageWriteLicense", "(Ljava/lang/String;)I", (void*)vendorStorageWriteLicense},
    {"vendorStorageReadLicense", "()Ljava/lang/String;", (void*)vendorStorageReadLicense},
};

jint JNI_OnLoad(JavaVM* vm, void* reserved){
    JNIEnv* env = NULL;
    jint result = -1;

    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return result;
    }

    int res = RegisterNativeMethods(env, "com/seeya/cast/util/LicenseTool",
              gJni_Methods_table, sizeof(gJni_Methods_table) /
                                  sizeof(gJni_Methods_table[0]));

    if (res < 0) {
        LOGF(TAG, "Unable to register native methods.");
    }

    return JNI_VERSION_1_4;
}
