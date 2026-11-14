#include "GameEngine.h"

GameEngine::GameEngine() {
    init();
}

void GameEngine::init() {
    currentState = GameState::MAIN_MENU;
    totalScore = 0;
    coinsCollected = 0;
    distanceTraveledMeters = 0.0f;
    nearMisses = 0;
    isPaused = false;
    playerBike.reset();
}

void GameEngine::startRace(bool twoWayMode) {
    currentState = GameState::GAMEPLAY;
    totalScore = 0;
    coinsCollected = 0;
    distanceTraveledMeters = 0.0f;
    nearMisses = 0;
    isPaused = false;
    playerBike.reset();
    traffic.reset(twoWayMode);
}

void GameEngine::update(float deltaTime) {
    if (currentState != GameState::GAMEPLAY || isPaused) return;

    // 1. Update motorcycle physics
    playerBike.update(deltaTime);

    float speedMps = playerBike.getSpeedMps();
    distanceTraveledMeters += speedMps * deltaTime;
    totalScore += static_cast<int>((playerBike.currentSpeedKmh / 10.0f) * deltaTime * 10.0f);
    coinsCollected += static_cast<int>((playerBike.currentSpeedKmh / 100.0f) * deltaTime * 2.0f);

    // 2. Update traffic vehicles
    traffic.update(deltaTime, playerBike.position.z, speedMps);

    // 3. Check near misses
    bool isOncoming = false;
    if (traffic.checkNearMiss(playerBike.position, playerBike.currentSpeedKmh, isOncoming)) {
        nearMisses++;
        totalScore += isOncoming ? 1000 : 500;
        coinsCollected += isOncoming ? 50 : 25;
    }

    // 4. Check crash collision
    float collisionDist = 0.0f;
    if (traffic.checkCollision(playerBike.position, collisionDist)) {
        currentState = GameState::GAME_OVER;
    }
}
