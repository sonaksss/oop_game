#pragma once
#include <vector>
#include "enemy_robot.h"
#include "position.h"

class RobotsFactory {
private:
    static constexpr int kAreaWidth = 2;
    static constexpr int kAreaHeight = 2;
    static constexpr int kMaxHealth = 100;

    Position top_left_;
    int period_;
    int counter_ = 0;
    int health_ = kMaxHealth;
    bool was_released_ = false;

    int enemy_health_;
    int enemy_damage_;
    int enemy_heal_;
    int enemy_energy_;
    int enemy_speed_;

public:
    RobotsFactory(Position top_left, int period,
                  int enemy_health, int enemy_damage, int enemy_heal,
                  int enemy_energy, int enemy_speed);

    Position GetTopLeft() const { return top_left_; }
    int GetAreaWidth() const { return kAreaWidth; }
    int GetAreaHeight() const { return kAreaHeight; }
    int GetPeriod() const { return period_; }
    int GetHealth() const { return health_; }
    int GetMaxHealth() const { return kMaxHealth; }

    bool IsAlive() const { return health_ > 0; }

    bool WasReleased() const { return was_released_; }
    void MarkReleased() { was_released_ = true; }

    bool Occupies(Position pos) const;

    std::vector<Position> GetOccupiedCells() const;

    void TakeDamage(int amount);
    void SetHealth(int value);

    bool Tick();
    EnemyRobot Spawn(Position pos) const;
};