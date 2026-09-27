#include "level_builder.h"
#include <algorithm>
#include <stdexcept>

LevelBuilder::LevelBuilder(int width, int height): width_(width), height_(height) {}

Position LevelBuilder::PlayerStart() const {
    return {0, 0};
}

Position LevelBuilder::FactoryStart() const {
    return {(width_ - 2) / 2, (height_ - 2) / 2};
}

int LevelBuilder::ObstacleCount() const {
    int count = static_cast<int>(width_ * height_ * kObstacleRatio);
    return std::max(2, count);
}

bool LevelBuilder::IsFactoryReachable(const Field& field, const RobotsFactory& factory) const {
    for (const Position& p : factory.GetOccupiedCells())
        if (!field.IsAvailableCell(p)) return false;
    return true;
}

Level LevelBuilder::Build() const {
    for (int attempt = 0; attempt < kMaxAttempts; ++attempt) {
        Field field(width_, height_);

        Position player_pos = PlayerStart();
        Position factory_pos = FactoryStart();
        RobotsFactory factory(factory_pos, kFactoryPeriod);

        std::vector<Position> forbidden;
        forbidden.push_back(player_pos);
        for (const Position& p : factory.GetOccupiedCells())
            forbidden.push_back(p);

        field.GenerateObstacles(ObstacleCount(), forbidden);
        field.GeneratePassability(forbidden);
        field.RemoveUnreachableCells(player_pos);

        if (!IsFactoryReachable(field, factory))
            continue;

        field.OccupyArea(factory.GetTopLeft(), factory.GetAreaWidth(), factory.GetAreaHeight());

        PlayerRobot player(kPlayerHealth, kPlayerDamage, kPlayerHeal, kPlayerEnergy, kPlayerSpeed, player_pos, kPlayerVisibility);

        std::vector<EnemyRobot> enemies;
        std::vector<RobotsFactory> factories;
        factories.push_back(factory);

        return Level(std::move(field), std::move(player), std::move(enemies), std::move(factories));
    }

    throw std::runtime_error("LevelBuilder: cannot generate valid level");
}