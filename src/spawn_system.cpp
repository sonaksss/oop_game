#include "spawn_system.h"
#include <algorithm>

const std::vector<Position>& SpawnSystem::NeighbourOffsets() {
    static const std::vector<Position> kOffsets = {
        {-1,-1}, {0,-1}, {1,-1},
        {-1, 0},          {1, 0},
        {-1, 1}, {0, 1}, {1, 1}
    };
    return kOffsets;
}

bool SpawnSystem::IsCellFree(Position pos,
                             const std::vector<EnemyRobot>& enemies,
                             const PlayerRobot& player) const {
    if (player.IsAlive() && player.GetPosition() == pos)
        return false;
    for (const EnemyRobot& e : enemies)
        if (e.IsAlive() && e.GetPosition() == pos)
            return false;
    return true;
}

std::vector<Position> SpawnSystem::CollectSpawnCandidates(
        const RobotsFactory& factory,
        const Field& field,
        const PlayerRobot& player,
        const std::vector<EnemyRobot>& enemies) const {
    std::vector<Position> candidates;
    const Position top_left = factory.GetTopLeft();

    for (const Position& offset : NeighbourOffsets()) {
        for (int dy = 0; dy < factory.GetAreaHeight(); ++dy) {
            for (int dx = 0; dx < factory.GetAreaWidth(); ++dx) {
                Position candidate{
                    top_left.X() + dx + offset.X(),
                    top_left.Y() + dy + offset.Y()
                };

                if (!field.IsAvailableCell(candidate)) continue;
                if (!IsCellFree(candidate, enemies, player)) continue;

                candidates.push_back(candidate);
            }
        }
    }
    return candidates;
}

void SpawnSystem::SpawnInitial(RobotsFactory& factory,
                               Field& field,
                               const PlayerRobot& player,
                               std::vector<EnemyRobot>& enemies) const {
    const std::vector<Position> candidates =
        CollectSpawnCandidates(factory, field, player, enemies);

    const int to_spawn =
        std::min<int>(kInitialEnemiesPerFactory,
                      static_cast<int>(candidates.size()));

    for (int i = 0; i < to_spawn; ++i)
        enemies.push_back(factory.Spawn(candidates[i]));
}

void SpawnSystem::Tick(RobotsFactory& factory,
                       Field& field,
                       const PlayerRobot& player,
                       std::vector<EnemyRobot>& enemies) const {
    if (!factory.Tick()) return;

    const std::vector<Position> candidates =
        CollectSpawnCandidates(factory, field, player, enemies);

    if (!candidates.empty())
        enemies.push_back(factory.Spawn(candidates.front()));
}