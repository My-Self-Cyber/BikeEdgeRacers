#include "GameEngine.h"

GameEngine::GameEngine() {
    init();
}

void GameEngine::init() {
    currentState = GameState::MAIN_MENU;
    currentTheme = ThemeArea::CYBER_MOUNTAINS;
    totalScore = 0;
    coinsCollected = 0;
    distanceTraveledMeters = 0.0f;
    nearMisses = 0;
    isPaused = false;
    playerBike.reset();
}

void GameEngine::startRace(bool twoWayMode, int themeId) {
    currentState = GameState::GAMEPLAY;
    currentTheme = static_cast<ThemeArea>(themeId);
    totalScore = 0;
    coinsCollected = 0;
    distanceTraveledMeters = 0.0f;
    nearMisses = 0;
    isPaused = false;
    playerBike.reset();

    // Theme-specific handling & physics calibration
    switch (currentTheme) {
        case ThemeArea::CYBER_MOUNTAINS:
            playerBike.maxSpeedKmh += 15.0f; // High-tech overdrive
            playerBike.handlingRate = 10.5f;
            break;
        case ThemeArea::MUD_WAYS:
            playerBike.maxSpeedKmh -= 10.0f; // Mud drag
            playerBike.handlingRate = 8.0f;  // Heavy offroad steering
            break;
        case ThemeArea::GOLDEN_DESERT:
            playerBike.accelerationRate += 5.0f; // Hot asphalt traction
            playerBike.handlingRate = 9.5f;
            break;
        case ThemeArea::FROZEN_EMPIRE:
            playerBike.handlingRate = 11.5f; // Ice drift responsiveness
            playerBike.brakeRate -= 15.0f;  // Slick ice braking distance
            break;
    }

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
