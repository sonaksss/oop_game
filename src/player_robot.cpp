#include "player_robot.h"

PlayerRobot::PlayerRobot(int max_health, int damage, int heal, int max_energy,
                         int speed, Position position, int visibility)
    : Robot(max_health, damage, heal, max_energy, speed, false, position),
      experience_(0), experience_up_(kExperienceUpInitial),
      rank_(1), visibility_(visibility) {
    if (visibility < 0)
        throw std::invalid_argument("PlayerRobot: negative visibility");
}

void PlayerRobot::SetExperience(int value) {
    if (value < 0) value = 0;
    experience_ = value;
}

void PlayerRobot::SetExperienceUp(int value) {
    if (value <= 0)
        throw std::invalid_argument("PlayerRobot: experience up must be positive");
    experience_up_ = value;
}

void PlayerRobot::SetRank(int value) {
    if (value < 1) value = 1;
    rank_ = value;
}

void PlayerRobot::SetVisibility(int value) {
    if (value < 0)
        throw std::invalid_argument("PlayerRobot: visibility cannot be negative");
    visibility_ = value;
}

void PlayerRobot::AddExperience(int value) {
    if (value < 0)
        throw std::invalid_argument("PlayerRobot: negative experience");

    experience_ += value;
    while (experience_ >= experience_up_) {
        experience_ -= experience_up_;
        LevelUp();
    }
}

void PlayerRobot::LevelUp() {
    SetRank(rank_ + 1);
    SetMaxHealth(health_max_ + kHealthMaxUp);
    SetHealth(health_ + kHealthMaxUp);
    SetDamage(damage_ + kDamageUp);
    SetHeal(heal_ + kHealUp);
    SetMaxEnergy(energy_max_ + kEnergyMaxUp);
    SetEnergy(energy_ + kEnergyMaxUp);
    SetExperienceUp(experience_up_ + kExperienceUpGrowth);
    SetSpeed(speed_ + kSpeedUp);
    SetVisibility(visibility_ + kVisibilityUp);
}