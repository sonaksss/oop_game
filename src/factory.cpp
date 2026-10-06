#include "factory.h"
#include <stdexcept>

RobotsFactory::RobotsFactory(Position top_left, int period, int enemy_health, 
                              int enemy_damage, int enemy_heal, int enemy_energy, int enemy_speed): 
                              top_left_(top_left), period_(period), enemy_health_(enemy_health), 
                              enemy_damage_(enemy_damage), enemy_heal_(enemy_heal), 
                              enemy_energy_(enemy_energy), enemy_speed_(enemy_speed) {
    if (period <= 0)
        throw std::invalid_argument("RobotsFactory: period must be positive");
    if (enemy_health <= 0)
        throw std::invalid_argument("RobotsFactory: enemy health must be positive");
    if (enemy_damage < 0 || enemy_heal < 0 ||
        enemy_energy < 0 || enemy_speed < 0)
        throw std::invalid_argument("RobotsFactory: enemy stats cannot be negative");
}

bool RobotsFactory::Occupies(Position pos) const {
    return pos.X() >= top_left_.X() && pos.X() < top_left_.X() + kAreaWidth &&
           pos.Y() >= top_left_.Y() && pos.Y() < top_left_.Y() + kAreaHeight;
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
    return EnemyRobot(enemy_health_, enemy_damage_, enemy_heal_,
                      enemy_energy_, enemy_speed_, pos);
}