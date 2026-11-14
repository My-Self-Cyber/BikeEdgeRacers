#ifndef MOTORCYCLE_H
#define MOTORCYCLE_H

#include "VectorMath.h"

class Motorcycle {
public:
    Vec3 position;
    float currentSpeedKmh;
    float maxSpeedKmh;
    float accelerationRate;
    float brakeRate;
    float handlingRate;
    float currentLean;
    int currentGear;
    float currentRpm;

    bool isThrottleActive;
    bool isBrakeActive;
    float steeringInput; // -1.0 (Left) to +1.0 (Right)

    Motorcycle();
    void update(float deltaTime);
    void reset();
    int calculateGear() const;
    float getSpeedMps() const { return currentSpeedKmh / 3.6f; }
};

#endif // MOTORCYCLE_H
