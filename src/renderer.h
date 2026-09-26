#pragma once
#include <vector>
#include "field.h"
#include "player_robot.h"
#include "enemy_robot.h"
#include "factory.h"
#include "position.h"

class Renderer {
private:
    const Robot* FindRobotAt(Position pos,
                             const PlayerRobot& player,
                             const std::vector<EnemyRobot>& enemies) const;

    const RobotsFactory* FindFactoryAt(
        Position pos,
        const std::vector<RobotsFactory>& factories) const;

public:
    void Draw(const Field& field,
              const PlayerRobot& player,
              const std::vector<EnemyRobot>& enemies,
              const std::vector<RobotsFactory>& factories) const;
};