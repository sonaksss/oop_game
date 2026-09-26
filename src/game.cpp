#include "game.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>

Game::Game(Field field, PlayerRobot player,
           std::vector<EnemyRobot> enemies,
           std::vector<RobotsFactory> factories,
           Input input, Renderer renderer)
    : field_(std::move(field)),
      player_(std::move(player)),
      enemies_(std::move(enemies)),
      factories_(std::move(factories)),
      input_(input),
      renderer_(renderer) {}

bool Game::AreEnemiesAlive() const {
    for (const EnemyRobot& e : enemies_)
        if (e.IsAlive()) return true;
    return false;
}

bool Game::AreAllFactoriesDead() const {
    for (const RobotsFactory& f : factories_)
        if (f.IsAlive()) return false;
    return true;
}

bool Game::IsVictory() const {
    return !AreEnemiesAlive() && AreAllFactoriesDead();
}

Robot* Game::FindRobotAt(Position pos, const Robot* excluded) {
    if (&player_ != excluded && player_.IsAlive() &&
        player_.GetPosition() == pos)
        return &player_;

    for (EnemyRobot& e : enemies_) {
        if (&e != excluded && e.IsAlive() && e.GetPosition() == pos)
            return &e;
    }
    return nullptr;
}

RobotsFactory* Game::FindFactoryAt(Position pos) {
    for (RobotsFactory& f : factories_) {
        if (!f.IsAlive()) continue;
        for (const Position& c : f.GetOccupiedCells())
            if (c == pos) return &f;
    }
    return nullptr;
}

bool Game::TryAttackFactory(Robot& robot, Position target) {
    RobotsFactory* factory = FindFactoryAt(target);
    if (!factory) return false;

    if (robot.IsEnemy()) return true;

    factory->TakeDamage(robot.GetDamage());
    return true;
}

void Game::HandleInteraction(Robot& robot, Robot& other) {
    bool was_alive = other.IsAlive();
    bool was_enemy = (robot.IsEnemy() != other.IsEnemy());

    robot.Interact(other);

    if (was_alive && was_enemy && !other.IsAlive()) {
        if (!robot.IsEnemy())
            player_.AddExperience(kExperiencePerKill);
    }
}

bool Game::TryMove(Robot& robot, Position delta) {
    Position target{robot.GetPosition().X() + delta.X(),
                    robot.GetPosition().Y() + delta.Y()};

    if (TryAttackFactory(robot, target))
        return true;

    if (!field_.IsAvailableCell(target))
        return false;

    if (Robot* other = FindRobotAt(target, &robot)) {
        HandleInteraction(robot, *other);
        return true;
    }

    robot.SetPosition(target);
    return true;
}

bool Game::TryMoveByPath(Robot& robot, Position delta) {
    Position step{0, 0};
    if (delta.X() > 0) step = {1, 0};
    else if (delta.X() < 0) step = {-1, 0};
    else if (delta.Y() > 0) step = {0, 1};
    else if (delta.Y() < 0) step = {0, -1};
    else return false;

    int remaining = robot.GetSpeed();
    bool moved = false;

    while (remaining > 0) {
        Position current = robot.GetPosition();
        Position target{current.X() + step.X(), current.Y() + step.Y()};

        if (TryAttackFactory(robot, target))
            break;

        if (!field_.IsAvailableCell(target))
            break;

        if (Robot* other = FindRobotAt(target, &robot)) {
            HandleInteraction(robot, *other);
            break;
        }

        int passability = field_.GetCellPassability(target);
        if (passability > remaining)
            break;

        robot.SetPosition(target);
        remaining -= passability;
        moved = true;
    }

    return moved;
}

void Game::ProcessPlayerTurn() {
    Position delta = input_.ReadMove();
    if (input_.WantsQuit()) {
        is_running_ = false;
        return;
    }
    if (delta.X() == 0 && delta.Y() == 0)
        return;

    TryMoveByPath(player_, delta);
}

void Game::ProcessEnemiesTurns() {
    const Position kDirs[4] = {{1,0}, {-1,0}, {0,1}, {0,-1}};

    for (EnemyRobot& e : enemies_) {
        if (!e.IsAlive()) continue;
        Position dir = kDirs[std::rand() % 4];
        TryMove(e, dir);
    }
}

void Game::TickFactories() {
    const Position kNeighbours[8] = {
        {-1,-1}, {0,-1}, {1,-1},
        {-1, 0},          {1, 0},
        {-1, 1}, {0, 1}, {1, 1}
    };

    for (RobotsFactory& f : factories_) {
        if (!f.Tick()) continue;

        Position top_left = f.GetTopLeft();
        bool spawned = false;

        for (const Position& d : kNeighbours) {
            for (int dy = 0; dy < f.GetAreaHeight() && !spawned; ++dy) {
                for (int dx = 0; dx < f.GetAreaWidth() && !spawned; ++dx) {
                    Position candidate{top_left.X() + dx + d.X(),
                                       top_left.Y() + dy + d.Y()};

                    if (!field_.IsAvailableCell(candidate)) continue;
                    if (FindRobotAt(candidate, nullptr)) continue;

                    enemies_.push_back(
                        EnemyRobot(30, 3, 0, 10, 3, candidate));
                    spawned = true;
                }
            }
            if (spawned) break;
        }
    }
}

void Game::RestoreEnergy() {
    player_.ChangeEnergy(1);
    for (EnemyRobot& e : enemies_)
        if (e.IsAlive())
            e.ChangeEnergy(1);
}

void Game::UpdateVisibility() {
    int radius = player_.GetVisibility();
    Position center = player_.GetPosition();

    for (int y = 0; y < field_.GetHeight(); ++y) {
        for (int x = 0; x < field_.GetWidth(); ++x) {
            Position p{x, y};
            if (field_.IsCellKnown(p) && !field_.IsAvailableCell(p))
                continue;
            field_.SetCellKnown(p, false);
        }
    }

    for (int y = 0; y < field_.GetHeight(); ++y) {
        for (int x = 0; x < field_.GetWidth(); ++x) {
            Position p{x, y};
            if (p.ManhattanDistance(center) <= radius)
                field_.SetCellKnown(p, true);
        }
    }
}

void Game::RemoveDeadEnemies() {
    enemies_.erase(
        std::remove_if(enemies_.begin(), enemies_.end(),
                       [](const EnemyRobot& e) { return !e.IsAlive(); }),
        enemies_.end());
}

void Game::Run() {
    UpdateVisibility();

    while (is_running_ && player_.IsAlive() && !IsVictory()) {
        renderer_.Draw(field_, player_, enemies_, factories_);

        ProcessPlayerTurn();
        if (!is_running_) break;

        ProcessEnemiesTurns();
        RemoveDeadEnemies();
        TickFactories();
        RestoreEnergy();
        UpdateVisibility();
    }

    if (!player_.IsAlive()) {
        std::cout << "Поражение.\n";
    } else if (IsVictory()) {
        std::cout << "Победа!\n";
    } else {
        std::cout << "Выход из игры.\n";
    }
}