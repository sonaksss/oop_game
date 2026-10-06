#pragma once
#include <vector>
#include "field.h"
#include "player_robot.h"
#include "enemy_robot.h"
#include "factory.h"
#include "input.h"
#include "renderer.h"
#include "level.h"
#include "movement_system.h"
#include "spawn_system.h"
#include "visibility_system.h"

class Game {
private:
    Field field_;
    PlayerRobot player_;
    std::vector<EnemyRobot> enemies_;
    std::vector<RobotsFactory> factories_;
    Input input_;
    Renderer renderer_;

    MovementSystem movement_;
    SpawnSystem spawner_;
    VisibilitySystem visibility_;

    bool is_running_ = true;

    bool AreEnemiesAlive() const;
    bool IsVictory() const;

    bool ProcessPlayerTurn();
    void ProcessEnemiesTurns();
    void EndOfTurn();

public:
    Game(Level level, Input input, Renderer renderer);

    void Run();
};