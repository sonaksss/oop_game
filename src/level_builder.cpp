#include "level_builder.h"
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <queue>

LevelBuilder::LevelBuilder(int width, int height): width_(width), height_(height) {}

Position LevelBuilder::PlayerStart() const {
    return {0, 0};
}

Position LevelBuilder::FactoryStart() const {
    return {width_ - 2, height_ - 2};
}

int LevelBuilder::ObstacleCount() const {
    int count = static_cast<int>(width_ * height_ * kObstacleRatio);
    return std::max(2, count);
}

LevelBuilder LevelBuilder::ReadFromConsole() {
    int width, height;
    std::cout << "Ширина (" << Field::MinWidth() << "-" << Field::MaxWidth() << "): ";
    std::cin >> width;
    std::cout << "Высота (" << Field::MinHeight() << "-" << Field::MaxHeight() << "): ";
    std::cin >> height;
    return LevelBuilder(width, height);
}

std::vector<Position> LevelBuilder::FactoryZone(const RobotsFactory& factory) const {
    std::vector<Position> zone;
    Position tl = factory.GetTopLeft();
    int aw = factory.GetAreaWidth();
    int ah = factory.GetAreaHeight();

    for (int dy = -1; dy <= ah; ++dy)
        for (int dx = -1; dx <= aw; ++dx)
            zone.push_back({tl.X() + dx, tl.Y() + dy});

    return zone;
}

bool LevelBuilder::HasPathToFactory(const Field& field, Position player_pos, 
                                    const RobotsFactory& factory) const {
    if (!field.IsCorrectCell(player_pos) || !field.IsAvailableCell(player_pos))
        return false;

    std::vector<std::vector<bool>> visited(
        field.GetHeight(), std::vector<bool>(field.GetWidth(), false));

    std::queue<Position> q;
    q.push(player_pos);
    visited[player_pos.Y()][player_pos.X()] = true;

    const Position kDirs[4] = {{1,0}, {-1,0}, {0,1}, {0,-1}};

    while (!q.empty()) {
        Position cur = q.front();
        q.pop();

        for (const Position& d : kDirs) {
            Position next{cur.X() + d.X(), cur.Y() + d.Y()};
            if (!field.IsCorrectCell(next)) continue;
            if (!field.IsAvailableCell(next)) continue;
            if (visited[next.Y()][next.X()]) continue;
            visited[next.Y()][next.X()] = true;
            q.push(next);
        }
    }

    Position tl = factory.GetTopLeft();
    int aw = factory.GetAreaWidth();
    int ah = factory.GetAreaHeight();

    for (int dy = -1; dy <= ah; ++dy) {
        for (int dx = -1; dx <= aw; ++dx) {
            if (dy >= 0 && dy < ah && dx >= 0 && dx < aw) continue;
            Position n{tl.X() + dx, tl.Y() + dy};
            if (!field.IsCorrectCell(n)) continue;
            if (visited[n.Y()][n.X()]) return true;
        }
    }
    return false;
}

Level LevelBuilder::Build() const {
    for (int attempt = 0; attempt < kMaxAttempts; ++attempt) {
        Field field(width_, height_);

        Position player_pos = PlayerStart();
        Position factory_pos = FactoryStart();

        RobotsFactory factory(factory_pos, kFactoryPeriod,
                              kEnemyHealth, kEnemyDamage, kEnemyHeal,
                              kEnemyEnergy, kEnemySpeed);

        std::vector<Position> zone = FactoryZone(factory);

        std::vector<Position> forbidden;
        forbidden.push_back(player_pos);
        for (const Position& p : zone)
            forbidden.push_back(p);

        field.GenerateObstacles(ObstacleCount(), forbidden);
        field.GeneratePassability(forbidden);

        field.OccupyArea(factory.GetTopLeft(),
                         factory.GetAreaWidth(),
                         factory.GetAreaHeight());

        field.RemoveUnreachableCells(player_pos, zone);

        if (!HasPathToFactory(field, player_pos, factory))
            continue;

        PlayerRobot player(kPlayerHealth, kPlayerDamage, kPlayerHeal,
                           kPlayerEnergy, kPlayerSpeed, player_pos,
                           kPlayerVisibility);

        std::vector<EnemyRobot> enemies;
        std::vector<RobotsFactory> factories;
        factories.push_back(factory);

        return Level(std::move(field), std::move(player),
                     std::move(enemies), std::move(factories));
    }

    throw std::runtime_error("LevelBuilder: cannot generate valid level");
}