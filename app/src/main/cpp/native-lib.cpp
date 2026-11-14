#include <jni.h>
#include <android/log.h>
#include "GameEngine.h"

#define LOG_TAG "BikeEdgeRacersNative"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

static GameEngine* g_engine = nullptr;

extern "C" {

JNIEXPORT void JNICALL
Java_com_edge_bikeedgeracers_MainActivity_nativeInitEngine(JNIEnv* env, jobject thiz) {
    if (g_engine == nullptr) {
        g_engine = new GameEngine();
        LOGI("Native C++ Game Engine initialized successfully.");
    }
}

JNIEXPORT void JNICALL
Java_com_edge_bikeedgeracers_MainActivity_nativeStartRace(JNIEnv* env, jobject thiz, jboolean twoWay) {
    if (g_engine) {
        g_engine->startRace(twoWay);
        LOGI("Native C++ Race started in mode: %s", twoWay ? "TWO-WAY ONCOMING" : "ONE-WAY");
    }
}

JNIEXPORT void JNICALL
Java_com_edge_bikeedgeracers_MainActivity_nativeUpdate(JNIEnv* env, jobject thiz, jfloat deltaTime) {
    if (g_engine) {
        g_engine->update(deltaTime);
    }
}

JNIEXPORT void JNICALL
Java_com_edge_bikeedgeracers_MainActivity_nativeSetInput(JNIEnv* env, jobject thiz, jboolean throttle, jboolean brake, jfloat steer) {
    if (g_engine) {
        g_engine->setThrottle(throttle);
        g_engine->setBrake(brake);
        g_engine->setSteering(steer);
    }
}

JNIEXPORT jfloat JNICALL
Java_com_edge_bikeedgeracers_MainActivity_nativeGetSpeed(JNIEnv* env, jobject thiz) {
    return g_engine ? g_engine->getSpeedKmh() : 0.0f;
}

JNIEXPORT jint JNICALL
Java_com_edge_bikeedgeracers_MainActivity_nativeGetScore(JNIEnv* env, jobject thiz) {
    return g_engine ? g_engine->totalScore : 0;
}

JNIEXPORT jfloat JNICALL
Java_com_edge_bikeedgeracers_MainActivity_nativeGetDistance(JNIEnv* env, jobject thiz) {
    return g_engine ? g_engine->distanceTraveledMeters : 0.0f;
}

JNIEXPORT jint JNICALL
Java_com_edge_bikeedgeracers_MainActivity_nativeGetCoins(JNIEnv* env, jobject thiz) {
    return g_engine ? g_engine->coinsCollected : 0;
}

JNIEXPORT jint JNICALL
Java_com_edge_bikeedgeracers_MainActivity_nativeGetGear(JNIEnv* env, jobject thiz) {
    return g_engine ? g_engine->getGear() : 1;
}

JNIEXPORT jboolean JNICALL
Java_com_edge_bikeedgeracers_MainActivity_nativeIsGameOver(JNIEnv* env, jobject thiz) {
    return g_engine ? (g_engine->currentState == GameState::GAME_OVER) : false;
}

} // extern "C"
