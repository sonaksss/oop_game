#pragma once
#include <string>
#include <vector>
#include "field.h"
#include "player_robot.h"
#include "enemy_robot.h"
#include "factory.h"
#include "position.h"

class Renderer {
private:
    enum ColorPair { CP_UNKNOWN = 1,CP_FLOOR_1, CP_FLOOR_2, CP_FLOOR_3, CP_OBSTACLE,
                     CP_FACTORY, CP_PLAYER, CP_ENEMY, CP_HUD, CP_HUD_LABEL, CP_HINT };

    void InitColors();

    const PlayerRobot* FindPlayerAt(Position pos,
                                    const PlayerRobot& player) const;

    const EnemyRobot* FindEnemyAt(Position pos,
                                  const std::vector<EnemyRobot>& enemies) const;
    
    const RobotsFactory* FindFactoryAt(
        Position pos,
        const std::vector<RobotsFactory>& factories) const;

public:
    Renderer();
    ~Renderer();

    void Draw(const Field& field,
              const PlayerRobot& player,
              const std::vector<EnemyRobot>& enemies,
              const std::vector<RobotsFactory>& factories) const;

    void DrawGameOver(const std::string& message) const;
};