#include <jni.h>
#include <string>
#include <JSContextRef.h>
#include <JavaScript.h>
#include <android/log.h>
#include <android/bitmap.h>
#include "util/GaussBlur.h"


extern "C" JNIEXPORT jlong JNICALL
Java_com_example_cw_kotlincppdemo_MainActivity_launchInterpreter(
        JNIEnv* env,
        jobject pThis
        ){
    JSGlobalContextRef contextRef = nullptr;
    JSClassRef *jsClassRef = new JSClassRef();
    contextRef = JSGlobalContextCreate(*jsClassRef);
    return reinterpret_cast<jlong > (contextRef);
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_cw_kotlincppdemo_MainActivity_evaluateJavaScript(
        JNIEnv* env,
        jobject pThis,
        jstring script,
        jlong nativeHandler
){
    JSClassRef *jsClassRef = new JSClassRef();
    JSGlobalContextRef contextRef = JSGlobalContextCreate(*jsClassRef);
    const char *argument = env->GetStringUTFChars(script, 0);
    JSStringRef stringRef = JSStringCreateWithUTF8CString(argument);
    JSValueRef value = JSEvaluateScript(contextRef,stringRef,0,0,0, nullptr);
    JSStringRef jsStringRef = JSValueToStringCopy(contextRef,value, nullptr);
    std::string val = "";
    val.resize(::JSStringGetMaximumUTF8CStringSize(jsStringRef), 0);
    val.resize(::JSStringGetUTF8CString(jsStringRef, &val[0], val.length()) -1);
    return env->NewStringUTF(val.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_cw_kotlincppdemo_MainActivity_cppCallKotlinMethod(
        JNIEnv* env,
        jobject pThis
        ){
    jclass clazz = env->FindClass("com/example/cw/kotlincppdemo/MainActivity");
    jmethodID methodID = env->GetStaticMethodID(clazz,"staticMethod","(IFLjava/lang/String;)I");
    if (methodID == NULL){
        __android_log_print(ANDROID_LOG_INFO,__FUNCTION__,"methodID == NULL");
    } else{
        jint value = env->CallStaticIntMethod(clazz,methodID,5);
        __android_log_print(ANDROID_LOG_INFO,__FUNCTION__,"value: %d",value);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_cw_kotlincppdemo_MainActivity_gaussBlur(
        JNIEnv* env,
        jobject pThis,
        jobject bmp
        ){
    AndroidBitmapInfo info = {0};//初始化bitmapInfo结构体
    int *data = nullptr;//初始化Bitmap图像数据指针
    AndroidBitmap_getInfo(env,bmp,&info);
    AndroidBitmap_lockPixels(env,bmp,(void **)&data);
    GaussBlur *gaussBlur = new GaussBlur();
    gaussBlur->blurBitmap(data, info.width, info.height, 10);
    AndroidBitmap_unlockPixels(env,bmp);
    delete gaussBlur;
}



