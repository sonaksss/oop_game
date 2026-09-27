#pragma once
#include "level.h"

class LevelBuilder {
private:
    static constexpr double kObstacleRatio = 0.1;
    static constexpr int kMaxAttempts = 50;

    static constexpr int kPlayerHealth = 100;
    static constexpr int kPlayerDamage = 10;
    static constexpr int kPlayerHeal = 5;
    static constexpr int kPlayerEnergy = 50;
    static constexpr int kPlayerSpeed = 3;
    static constexpr int kPlayerVisibility = 5;
    static constexpr int kFactoryPeriod = 5;

    int width_;
    int height_;

    Position PlayerStart() const;
    Position FactoryStart() const;
    int ObstacleCount() const;

    bool IsFactoryReachable(const Field& field, const RobotsFactory& factory) const;

public:
    LevelBuilder(int width, int height);

    Level Build() const;
};