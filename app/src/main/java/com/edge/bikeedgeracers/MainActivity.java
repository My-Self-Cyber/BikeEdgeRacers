package com.edge.bikeedgeracers;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.view.WindowInsets;
import android.view.WindowInsetsController;
import android.view.WindowManager;
import android.webkit.JavascriptInterface;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;

public class MainActivity extends Activity {
    static {
        System.loadLibrary("bike_edge_racers");
    }

    // Native C++ Game Engine JNI bindings
    public native void nativeInitEngine();
    public native void nativeStartRace(boolean twoWay);
    public native void nativeUpdate(float deltaTime);
    public native void nativeSetInput(boolean throttle, boolean brake, float steer);
    public native float nativeGetSpeed();
    public native int nativeGetScore();
    public native float nativeGetDistance();
    public native int nativeGetCoins();
    public native int nativeGetGear();
    public native boolean nativeIsGameOver();

    private WebView webView;

    @SuppressLint("SetJavaScriptEnabled")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Initialize Native C++ Game Engine
        nativeInitEngine();

        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        hideSystemUI();

        webView = new WebView(this);
        WebSettings settings = webView.getSettings();
        settings.setJavaScriptEnabled(true);
        settings.setDomStorageEnabled(true);
        settings.setAllowFileAccess(true);
        settings.setMediaPlaybackRequiresUserGesture(false);
        settings.setUseWideViewPort(true);
        settings.setLoadWithOverviewMode(true);

        // Bind C++ Native bridge to JavaScript
        webView.addJavascriptInterface(new NativeGameBridge(), "NativeCppEngine");

        webView.setWebViewClient(new WebViewClient());
        webView.loadUrl("file:///android_asset/index.html");

        setContentView(webView);
    }

    public class NativeGameBridge {
        @JavascriptInterface
        public void startRace(boolean twoWay) {
            nativeStartRace(twoWay);
        }

        @JavascriptInterface
        public void setInput(boolean throttle, boolean brake, float steer) {
            nativeSetInput(throttle, brake, steer);
        }

        @JavascriptInterface
        public float getSpeed() {
            return nativeGetSpeed();
        }

        @JavascriptInterface
        public int getScore() {
            return nativeGetScore();
        }

        @JavascriptInterface
        public int getGear() {
            return nativeGetGear();
        }

        @JavascriptInterface
        public boolean isGameOver() {
            return nativeIsGameOver();
        }
    }

    private void hideSystemUI() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            WindowInsetsController controller = getWindow().getInsetsController();
            if (controller != null) {
                controller.hide(WindowInsets.Type.statusBars() | WindowInsets.Type.navigationBars());
                controller.setSystemBarsBehavior(WindowInsetsController.BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE);
            }
        } else {
            getWindow().getDecorView().setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
                | View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_STABLE
            );
        }
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            hideSystemUI();
        }
    }
}
