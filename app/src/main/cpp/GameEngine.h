#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "Motorcycle.h"
#include "TrafficSystem.h"

enum class GameState {
    MAIN_MENU,
    GARAGE,
    MODE_SELECT,
    GAMEPLAY,
    GAME_OVER
};

class GameEngine {
public:
    GameState currentState;
    Motorcycle playerBike;
    TrafficSystem traffic;

    int totalScore;
    int coinsCollected;
    float distanceTraveledMeters;
    int nearMisses;
    bool isPaused;

    GameEngine();
    void init();
    void startRace(bool twoWayMode);
    void update(float deltaTime);

    void setThrottle(bool active) { playerBike.isThrottleActive = active; }
    void setBrake(bool active) { playerBike.isBrakeActive = active; }
    void setSteering(float input) { playerBike.steeringInput = input; }

    float getSpeedKmh() const { return playerBike.currentSpeedKmh; }
    int getGear() const { return playerBike.currentGear; }
    int getRpm() const { return static_cast<int>(playerBike.currentRpm); }
};

#endif // GAME_ENGINE_H
