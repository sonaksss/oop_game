#pragma once
#include <vector>
#include "field.h"
#include "player_robot.h"
#include "enemy_robot.h"
#include "factory.h"

class Level {
private:
    Field field_;
    PlayerRobot player_;
    std::vector<EnemyRobot> enemies_;
    std::vector<RobotsFactory> factories_;

public:
    Level(Field field, PlayerRobot player, std::vector<EnemyRobot> enemies, std::vector<RobotsFactory> factories);

    Field&& TakeField();
    PlayerRobot&& TakePlayer();
    std::vector<EnemyRobot>&& TakeEnemies();
    std::vector<RobotsFactory>&& TakeFactories();

    const Field& GetField() const;
    const PlayerRobot& GetPlayer() const;
    const std::vector<EnemyRobot>& GetEnemies() const;
    const std::vector<RobotsFactory>& GetFactories() const;
};