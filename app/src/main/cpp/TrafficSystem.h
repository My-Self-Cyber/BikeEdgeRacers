#ifndef TRAFFIC_SYSTEM_H
#define TRAFFIC_SYSTEM_H

#include "VectorMath.h"
#include <vector>

struct TrafficVehicle {
    Vec3 position;
    float speedKmh;
    bool isOncoming;
    bool hasNearMissed;
    int vehicleType; // 0: Sedan, 1: SUV, 2: Truck
};

class TrafficSystem {
public:
    std::vector<TrafficVehicle> vehicles;
    bool isTwoWayMode;

    TrafficSystem();
    void reset(bool twoWay);
    void update(float deltaTime, float playerZ, float playerSpeedMps);
    void spawnVehicle(float minZ);

    bool checkCollision(const Vec3& playerPos, float& outDistance);
    bool checkNearMiss(const Vec3& playerPos, float playerSpeedKmh, bool& outIsOncoming);
};

#endif // TRAFFIC_SYSTEM_H
