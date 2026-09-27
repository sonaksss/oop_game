#include "factory.h"
#include <stdexcept>

RobotsFactory::RobotsFactory(Position top_left, int period)
    : top_left_(top_left), period_(period) {
    if (period <= 0)
        throw std::invalid_argument("RobotsFactory: period must be positive");
}

std::vector<Position> RobotsFactory::GetOccupiedCells() const {
    std::vector<Position> cells;
    cells.reserve(kAreaWidth * kAreaHeight);
    for (int dy = 0; dy < kAreaHeight; ++dy)
        for (int dx = 0; dx < kAreaWidth; ++dx)
            cells.push_back({top_left_.X() + dx, top_left_.Y() + dy});
    return cells;
}

void RobotsFactory::SetHealth(int value) {
    if (value < 0) value = 0;
    if (value > kMaxHealth) value = kMaxHealth;
    health_ = value;
}

void RobotsFactory::TakeDamage(int amount) {
    if (amount < 0)
        throw std::invalid_argument("RobotsFactory::TakeDamage: negative");
    SetHealth(health_ - amount);
}

bool RobotsFactory::Tick() {
    if (!IsAlive()) return false;

    ++counter_;
    if (counter_ >= period_) {
        counter_ = 0;
        return true;
    }
    return false;
}

EnemyRobot RobotsFactory::Spawn(Position pos) const {
    return EnemyRobot(30, 3, 0, 10, 3, pos);
}