#pragma once
#include "robot.h"

class PlayerRobot : public Robot {
private:
    static constexpr int kHealthMaxUp = 50;
    static constexpr int kDamageUp = 10;
    static constexpr int kHealUp = 5;
    static constexpr int kEnergyMaxUp = 5;
    static constexpr int kExperienceUpGrowth = 50;
    static constexpr int kExperienceUpInitial = 100;
    static constexpr int kSpeedUp = 1;
    static constexpr int kVisibilityUp = 1;

    int experience_;
    int experience_up_;
    int rank_;
    int visibility_;

    void LevelUp();

public:
    PlayerRobot(int health_max, int damage, int heal, int energy_max,
                int speed, Position position, int visibility);
    ~PlayerRobot() override = default;

    int GetExperience() const { return experience_; }
    int GetExperienceUp() const { return experience_up_; }
    int GetRank() const { return rank_; }
    int GetVisibility() const { return visibility_; }

    void SetExperience(int value);
    void SetExperienceUp(int value);
    void SetRank(int value);
    void SetVisibility(int value);

    void AddExperience(int value);
};