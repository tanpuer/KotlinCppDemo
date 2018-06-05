package com.example.cw.kotlincppdemo

import android.graphics.Bitmap
import android.graphics.BitmapFactory
import android.graphics.Color
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import kotlinx.android.synthetic.main.activity_main.*

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val nativeHandler : Long = launchInterpreter()
        Log.d(TAG, nativeHandler.toString())
        val script = """
            var a = 1;
            var b = 100;
            a+b;
        """
        val result = evaluateJavaScript(script, nativeHandler)
        Log.d(TAG, result)

        //高斯模糊
        image.setImageResource(R.mipmap.ic_launcher)
        var bitmap = BitmapFactory.decodeResource(resources,R.mipmap.ic_launcher)
        gaussBlur(bitmap)
        image_blur.setImageBitmap(bitmap)
        image_blur.setBackgroundColor(Color.WHITE)
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun launchInterpreter() : Long
    external fun evaluateJavaScript(script: String, nativeHandler:Long): String
    external fun gaussBlur(bitmap: Bitmap)

    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
            System.loadLibrary("jsc")
        }
        const val TAG = "MainActivityDDDDD"

        @JvmStatic
        fun staticMethod(int: Int, float:Float, string:String) = int
    }


}
