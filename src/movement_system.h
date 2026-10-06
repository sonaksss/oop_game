#pragma once
#include <vector>
#include "field.h"
#include "robot.h"
#include "player_robot.h"
#include "enemy_robot.h"
#include "factory.h"

class MovementSystem {
private:
    static constexpr int kExperiencePerKill = 50;

    Robot* FindRobotAt(Position pos, Robot& self, PlayerRobot& player,
                       std::vector<EnemyRobot>& enemies) const;

    RobotsFactory* FindFactoryAt(Position pos, std::vector<RobotsFactory>& factories) const;

    bool TryInteractWithFactory(Robot& robot, Position target, 
                                std::vector<RobotsFactory>& factories) const;

    void HandleInteraction(Robot& robot, Robot& other, PlayerRobot& player) const;

public:
    bool TryMoveByPath(Robot& robot, Position delta, Field& field, PlayerRobot& player,
                       std::vector<EnemyRobot>& enemies, std::vector<RobotsFactory>& factories) const;
};