#pragma once
#include <vector>
#include "level.h"

class LevelBuilder {
private:
    static constexpr double kObstacleRatio = 0.1;
    static constexpr int kMaxAttempts = 50;

    static constexpr int kPlayerHealth = 100;
    static constexpr int kPlayerDamage = 7;
    static constexpr int kPlayerHeal = 10;
    static constexpr int kPlayerEnergy = 50;
    static constexpr int kPlayerSpeed = 3;
    static constexpr int kPlayerVisibility = 5;

    static constexpr int kFactoryPeriod = 3;

    static constexpr int kEnemyHealth = 50;
    static constexpr int kEnemyDamage = 5;
    static constexpr int kEnemyHeal = 0;
    static constexpr int kEnemyEnergy = 10;
    static constexpr int kEnemySpeed = 3;

    int width_;
    int height_;

    Position PlayerStart() const;
    Position FactoryStart() const;
    int ObstacleCount() const;

    std::vector<Position> FactoryZone(const RobotsFactory& factory) const;

    bool HasPathToFactory(const Field& field, Position player_pos,
                          const RobotsFactory& factory) const;

public:
    LevelBuilder(int width, int height);

    Level Build() const;

    static LevelBuilder ReadFromConsole();
};