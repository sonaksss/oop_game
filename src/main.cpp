#include <cstdlib>
#include <ctime>
#include <vector>
#include "field.h"
#include "player_robot.h"
#include "enemy_robot.h"
#include "factory.h"
#include "input.h"
#include "renderer.h"
#include "game.h"

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    constexpr int kWidth = 12;
    constexpr int kHeight = 12;

    Field field(kWidth, kHeight);

    RobotsFactory factory({9, 1}, 5);

    std::vector<Position> forbidden;
    forbidden.push_back({1, 1});
    forbidden.push_back({1, 9});
    forbidden.push_back({9, 9});
    for (const Position& p : factory.GetOccupiedCells())
        forbidden.push_back(p);

    field.GenerateObstacles(20, forbidden);
    field.GeneratePassability(forbidden);
    field.OccupyArea(factory.GetTopLeft(),
                     factory.GetAreaWidth(),
                     factory.GetAreaHeight());

    PlayerRobot player(100, 10, 5, 50, 3, {1, 1}, 5);

    std::vector<EnemyRobot> enemies;
    enemies.emplace_back(50, 5, 0, 20, 3, Position{1, 9});
    enemies.emplace_back(50, 5, 0, 20, 3, Position{9, 9});

    std::vector<RobotsFactory> factories;
    factories.push_back(factory);

    Input input;
    Renderer renderer;

    Game game(std::move(field), std::move(player),
              std::move(enemies), std::move(factories),
              input, renderer);
    game.Run();
    return 0;
}