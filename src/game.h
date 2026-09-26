#pragma once
#include <vector>
#include "field.h"
#include "player_robot.h"
#include "enemy_robot.h"
#include "factory.h"
#include "input.h"
#include "renderer.h"

class Game {
private:
    static constexpr int kExperiencePerKill = 50;

    Field field_;
    PlayerRobot player_;
    std::vector<EnemyRobot> enemies_;
    std::vector<RobotsFactory> factories_;
    Input input_;
    Renderer renderer_;

    bool is_running_ = true;

    bool AreEnemiesAlive() const;
    bool AreAllFactoriesDead() const;
    bool IsVictory() const;

    Robot* FindRobotAt(Position pos, const Robot* excluded);
    RobotsFactory* FindFactoryAt(Position pos);

    bool TryAttackFactory(Robot& robot, Position target);
    void HandleInteraction(Robot& robot, Robot& other);

    bool TryMove(Robot& robot, Position delta);
    bool TryMoveByPath(Robot& robot, Position delta);

    void ProcessPlayerTurn();
    void ProcessEnemiesTurns();
    void TickFactories();
    void RestoreEnergy();
    void UpdateVisibility();
    void RemoveDeadEnemies();

public:
    Game(Field field,
         PlayerRobot player,
         std::vector<EnemyRobot> enemies,
         std::vector<RobotsFactory> factories,
         Input input,
         Renderer renderer);

    void Run();
};