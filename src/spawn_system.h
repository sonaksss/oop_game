#pragma once
#include <vector>
#include "field.h"
#include "factory.h"
#include "enemy_robot.h"
#include "player_robot.h"

class SpawnSystem {
private:
    static constexpr int kInitialEnemiesPerFactory = 3;

    static const std::vector<Position>& NeighbourOffsets();

    std::vector<Position> CollectSpawnCandidates(
        const RobotsFactory& factory,
        const Field& field,
        const PlayerRobot& player,
        const std::vector<EnemyRobot>& enemies) const;

    bool IsCellFree(Position pos,
                    const std::vector<EnemyRobot>& enemies,
                    const PlayerRobot& player) const;

public:
    void SpawnInitial(RobotsFactory& factory,
                      Field& field,
                      const PlayerRobot& player,
                      std::vector<EnemyRobot>& enemies) const;

    void Tick(RobotsFactory& factory,
              Field& field,
              const PlayerRobot& player,
              std::vector<EnemyRobot>& enemies) const;
};