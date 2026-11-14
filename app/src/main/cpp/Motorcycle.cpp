#include "Motorcycle.h"
#include <algorithm>

Motorcycle::Motorcycle() {
    reset();
}

void Motorcycle::reset() {
    position = Vec3(0.0f, 1.45f, 0.0f);
    currentSpeedKmh = 0.0f;
    maxSpeedKmh = 220.0f;
    accelerationRate = 35.0f;
    brakeRate = 60.0f;
    handlingRate = 9.5f;
    currentLean = 0.0f;
    currentGear = 1;
    currentRpm = 1000.0f;

    isThrottleActive = false;
    isBrakeActive = false;
    steeringInput = 0.0f;
}

void Motorcycle::update(float deltaTime) {
    // 1. Acceleration & Braking Dynamics
    if (isThrottleActive) {
        currentSpeedKmh += accelerationRate * deltaTime;
        if (currentSpeedKmh > maxSpeedKmh) currentSpeedKmh = maxSpeedKmh;
    } else if (isBrakeActive) {
        currentSpeedKmh -= brakeRate * deltaTime;
        if (currentSpeedKmh < 0.0f) currentSpeedKmh = 0.0f;
    } else {
        // Natural highway aerodynamic drag
        currentSpeedKmh -= 12.0f * deltaTime;
        if (currentSpeedKmh < 0.0f) currentSpeedKmh = 0.0f;
    }

    // 2. Steering & Lean angle interpolation
    float targetLean = -steeringInput * 0.35f; // Radian tilt
    currentLean += (targetLean - currentLean) * (handlingRate * deltaTime);

    // 3. Horizontal position along road (bounded to 4 highway lanes)
    position.x += steeringInput * (handlingRate * 1.15f) * deltaTime;
    const float roadLimit = 7.8f;
    if (position.x < -roadLimit) position.x = -roadLimit;
    if (position.x > roadLimit) position.x = roadLimit;

    // 4. Gear & RPM
    currentGear = calculateGear();
    float speedRatio = currentSpeedKmh / maxSpeedKmh;
    currentRpm = 1200.0f + speedRatio * 9800.0f;
}

int Motorcycle::calculateGear() const {
    if (currentSpeedKmh > 180.0f) return 6;
    if (currentSpeedKmh > 140.0f) return 5;
    if (currentSpeedKmh > 100.0f) return 4;
    if (currentSpeedKmh > 60.0f) return 3;
    if (currentSpeedKmh > 20.0f) return 2;
    return 1;
}
