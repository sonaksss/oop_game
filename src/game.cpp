#include "game.h"
#include <algorithm>
#include <cstdlib>

Game::Game(Level level, Input input, Renderer renderer)
    : field_(level.TakeField()),
      player_(level.TakePlayer()),
      enemies_(level.TakeEnemies()),
      factories_(level.TakeFactories()),
      input_(input),
      renderer_(renderer) {}

bool Game::AreEnemiesAlive() const {
    for (const EnemyRobot& e : enemies_)
        if (e.IsAlive()) return true;
    return false;
}

bool Game::IsVictory() const {
    return !AreEnemiesAlive();
}

bool Game::ProcessPlayerTurn() {
    Position delta = input_.ReadMove();

    if (input_.WantsQuit()) {
        is_running_ = false;
        return false;
    }
    if (delta.X() == 0 && delta.Y() == 0)
        return false;

    return movement_.TryMoveByPath(player_, delta,
                                   field_, player_,
                                   enemies_, factories_);
}

void Game::ProcessEnemiesTurns() {
    const Position kDirs[4] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
    for (EnemyRobot& e : enemies_) {
        if (!e.IsAlive()) continue;
        Position dir = kDirs[std::rand() % 4];
        movement_.TryMoveByPath(e, dir, field_, player_, enemies_, factories_);
    }
}

void Game::EndOfTurn() {
    player_.ChangeEnergy(1);
    for (EnemyRobot& e : enemies_)
        if (e.IsAlive())
            e.ChangeEnergy(1);

    if (AreEnemiesAlive()) {
        for (RobotsFactory& f : factories_)
            spawner_.Tick(f, field_, player_, enemies_);
    }
    for (RobotsFactory& f : factories_) {
        if (f.IsAlive() || f.WasReleased()) continue;
        for (const Position& p : f.GetOccupiedCells()) {
            field_.SetCellAvailability(p, true);
            field_.SetCellKnown(p, false);
        }
        f.MarkReleased();
    }

    enemies_.erase(
        std::remove_if(enemies_.begin(), enemies_.end(),
                       [](const EnemyRobot& e) { return !e.IsAlive(); }),
        enemies_.end());

    visibility_.Update(field_, player_);
}

void Game::Run() {
    input_.Reset();
    visibility_.Update(field_, player_);

    for (RobotsFactory& f : factories_)
        spawner_.SpawnInitial(f, field_, player_, enemies_);

    renderer_.Draw(field_, player_, enemies_, factories_);

    while (is_running_ && player_.IsAlive() && !IsVictory()) {
        if (!ProcessPlayerTurn()) {
            if (!is_running_) break;
            continue;
        }

        ProcessEnemiesTurns();
        EndOfTurn();

        renderer_.Draw(field_, player_, enemies_, factories_);
    }

    if (!player_.IsAlive())
        renderer_.DrawGameOver("Поражение.");
    else if (IsVictory())
        renderer_.DrawGameOver("Победа!");
    else
        renderer_.DrawGameOver("Выход из игры.");
}