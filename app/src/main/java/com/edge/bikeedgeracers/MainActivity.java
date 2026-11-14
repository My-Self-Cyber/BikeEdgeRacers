package com.edge.bikeedgeracers;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.Window;
import android.view.WindowInsets;
import android.view.WindowInsetsController;
import android.view.WindowManager;
import android.webkit.ConsoleMessage;
import android.webkit.JavascriptInterface;
import android.webkit.WebChromeClient;
import android.webkit.WebSettings;
import android.webkit.WebView;
import android.webkit.WebViewClient;
import android.widget.FrameLayout;
import android.widget.TextView;

public class MainActivity extends Activity {
    private static final String TAG = "BikeEdgeRacers";
    private static boolean sNativeLoaded = false;

    static {
        try {
            System.loadLibrary("bike_edge_racers");
            sNativeLoaded = true;
            Log.i(TAG, "Native C++ library loaded successfully.");
        } catch (Throwable t) {
            Log.e(TAG, "Native library load fallback: " + t.getMessage());
            sNativeLoaded = false;
        }
    }

    // Native C++ Game Engine JNI bindings
    public native void nativeInitEngine();
    public native void nativeStartRace(boolean twoWay, int themeId);
    public native void nativeUpdate(float deltaTime);
    public native void nativeSetInput(boolean throttle, boolean brake, float steer);
    public native float nativeGetSpeed();
    public native int nativeGetScore();
    public native float nativeGetDistance();
    public native int nativeGetCoins();
    public native int nativeGetGear();
    public native int nativeGetTheme();
    public native boolean nativeIsGameOver();

    private WebView webView;

    @SuppressLint("SetJavaScriptEnabled")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        try {
            // Standard Fullscreen window setup
            requestWindowFeature(Window.FEATURE_NO_TITLE);
            getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
            getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

            if (sNativeLoaded) {
                try {
                    nativeInitEngine();
                } catch (Throwable t) {
                    Log.e(TAG, "Native engine init error: " + t.getMessage());
                }
            }

            // Create and configure WebView safely
            webView = new WebView(this);
            webView.setBackgroundColor(Color.BLACK);

            WebSettings settings = webView.getSettings();
            settings.setJavaScriptEnabled(true);
            settings.setDomStorageEnabled(true);
            settings.setDatabaseEnabled(true);
            settings.setAllowFileAccess(true);
            settings.setAllowContentAccess(true);
            settings.setAllowFileAccessFromFileURLs(true);
            settings.setAllowUniversalAccessFromFileURLs(true);
            settings.setMediaPlaybackRequiresUserGesture(false);
            settings.setUseWideViewPort(true);
            settings.setLoadWithOverviewMode(true);

            webView.addJavascriptInterface(new NativeGameBridge(), "NativeCppEngine");

            webView.setWebChromeClient(new WebChromeClient() {
                @Override
                public boolean onConsoleMessage(ConsoleMessage consoleMessage) {
                    Log.d("GameJS", consoleMessage.message());
                    return true;
                }
            });

            webView.setWebViewClient(new WebViewClient() {
                @Override
                public void onReceivedError(WebView view, int errorCode, String description, String failingUrl) {
                    Log.e(TAG, "WebView error: " + description);
                }
            });

            webView.loadUrl("file:///android_asset/index.html");
            setContentView(webView);

        } catch (Throwable e) {
            Log.e(TAG, "Fatal startup error: " + e.getMessage(), e);

            // Crash fallback screen: Displays readable message rather than closing
            FrameLayout layout = new FrameLayout(this);
            layout.setBackgroundColor(Color.BLACK);

            TextView tv = new TextView(this);
            tv.setTextColor(Color.WHITE);
            tv.setTextSize(16);
            tv.setPadding(40, 40, 40, 40);
            tv.setText("Starting Bike Edge Racers...\n" + e.getMessage());
            layout.addView(tv);

            setContentView(layout);
        }
    }

    public class NativeGameBridge {
        @JavascriptInterface
        public void startRace(boolean twoWay, int themeId) {
            try {
                if (sNativeLoaded) nativeStartRace(twoWay, themeId);
            } catch (Throwable ignored) {}
        }

        @JavascriptInterface
        public void setInput(boolean throttle, boolean brake, float steer) {
            try {
                if (sNativeLoaded) nativeSetInput(throttle, brake, steer);
            } catch (Throwable ignored) {}
        }

        @JavascriptInterface
        public float getSpeed() {
            try {
                if (sNativeLoaded) return nativeGetSpeed();
            } catch (Throwable ignored) {}
            return 0.0f;
        }

        @JavascriptInterface
        public int getScore() {
            try {
                if (sNativeLoaded) return nativeGetScore();
            } catch (Throwable ignored) {}
            return 0;
        }

        @JavascriptInterface
        public int getGear() {
            try {
                if (sNativeLoaded) return nativeGetGear();
            } catch (Throwable ignored) {}
            return 1;
        }

        @JavascriptInterface
        public boolean isGameOver() {
            try {
                if (sNativeLoaded) return nativeIsGameOver();
            } catch (Throwable ignored) {}
            return false;
        }
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            try {
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
            } catch (Throwable ignored) {}
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (webView != null) webView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (webView != null) webView.onResume();
    }
}
