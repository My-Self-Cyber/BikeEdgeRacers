#include "TrafficSystem.h"
#include <cstdlib>
#include <cmath>

static const float LANES[4] = { -6.75f, -2.25f, 2.25f, 6.75f };

TrafficSystem::TrafficSystem() : isTwoWayMode(false) {
    reset(false);
}

void TrafficSystem::reset(bool twoWay) {
    isTwoWayMode = twoWay;
    vehicles.clear();
    for (int i = 0; i < 8; ++i) {
        spawnVehicle(40.0f + i * 45.0f);
    }
}

void TrafficSystem::spawnVehicle(float minZ) {
    TrafficVehicle v;
    int laneIdx = std::rand() % 4;
    v.isOncoming = (isTwoWayMode && laneIdx < 2);
    v.position = Vec3(LANES[laneIdx], 0.65f, minZ + (std::rand() % 50));
    v.speedKmh = 65.0f + (std::rand() % 35);
    v.hasNearMissed = false;
    v.vehicleType = std::rand() % 3;
    vehicles.push_back(v);
}

void TrafficSystem::update(float deltaTime, float playerZ, float playerSpeedMps) {
    for (size_t i = 0; i < vehicles.size(); ++i) {
        float carSpeedMps = vehicles[i].speedKmh / 3.6f;
        float relativeSpeed = vehicles[i].isOncoming ? (-carSpeedMps - playerSpeedMps) : (carSpeedMps - playerSpeedMps);
        vehicles[i].position.z += relativeSpeed * deltaTime;

        // Despawn behind player or far ahead
        if (vehicles[i].position.z < -40.0f || vehicles[i].position.z > 500.0f) {
            vehicles.erase(vehicles.begin() + i);
            --i;
            spawnVehicle(200.0f);
        }
    }
}

bool TrafficSystem::checkCollision(const Vec3& playerPos, float& outDistance) {
    for (const auto& v : vehicles) {
        float dx = std::fabs(v.position.x - playerPos.x);
        float dz = std::fabs(v.position.z - playerPos.z);
        if (dz < 2.2f && dx < 1.05f) {
            outDistance = std::sqrt(dx * dx + dz * dz);
            return true;
        }
    }
    return false;
}

bool TrafficSystem::checkNearMiss(const Vec3& playerPos, float playerSpeedKmh, bool& outIsOncoming) {
    if (playerSpeedKmh < 95.0f) return false;

    for (auto& v : vehicles) {
        if (v.hasNearMissed) continue;
        float dx = std::fabs(v.position.x - playerPos.x);
        float dz = std::fabs(v.position.z - playerPos.z);

        if (dz < 2.4f && dx < 2.3f && dx > 1.05f) {
            v.hasNearMissed = true;
            outIsOncoming = v.isOncoming;
            return true;
        }
    }
    return false;
}
