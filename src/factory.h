#pragma once
#include <vector>
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

public:
    RobotsFactory(Position top_left, int period);

    Position GetTopLeft() const { return top_left_; }
    int GetAreaWidth() const { return kAreaWidth; }
    int GetAreaHeight() const { return kAreaHeight; }
    int GetPeriod() const { return period_; }
    int GetHealth() const { return health_; }
    int GetMaxHealth() const { return kMaxHealth; }

    bool IsAlive() const { return health_ > 0; }

    std::vector<Position> GetOccupiedCells() const;

    void TakeDamage(int amount);
    void SetHealth(int value);

    bool Tick();
};