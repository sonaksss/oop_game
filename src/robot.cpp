#include "robot.h"

Robot::Robot(int max_health, int damage, int heal, int max_energy, int speed, bool is_enemy, Position position): 
             health_(max_health), health_max_(max_health), damage_(damage), heal_(heal),
             energy_(max_energy), energy_max_(max_energy), speed_(speed), is_enemy_(is_enemy), position_(position) {
    
    if (max_health <= 0 || damage < 0 || heal < 0 ||
        max_energy < 0 || speed < 0)
        throw std::invalid_argument("Robot: invalid stats");
}

void Robot::SetHealth(int value) {
    if (value < 0) value = 0;
    if (value > health_max_) value = health_max_;
    health_ = value;
}

void Robot::SetMaxHealth(int value) {
    if (value <= 0)
        throw std::invalid_argument("Robot: max health must be positive");
    health_max_ = value;
    if (health_ > health_max_) health_ = health_max_;
}

void Robot::SetDamage(int value) {
    if (value < 0)
        throw std::invalid_argument("Robot: damage cannot be negative");
    damage_ = value;
}

void Robot::SetHeal(int value) {
    if (value < 0)
        throw std::invalid_argument("Robot: heal cannot be negative");
    heal_ = value;
}

void Robot::SetEnergy(int value) {
    if (value < 0) value = 0;
    if (value > energy_max_) value = energy_max_;
    energy_ = value;
}

void Robot::SetMaxEnergy(int value) {
    if (value < 0)
        throw std::invalid_argument("Robot: max energy cannot be negative");
    energy_max_ = value;
    if (energy_ > energy_max_) energy_ = energy_max_;
}

void Robot::SetSpeed(int value) {
    if (value < 0)
        throw std::invalid_argument("Robot: speed cannot be negative");
    speed_ = value;
}

void Robot::SetPosition(Position position) {
    position_ = position;
}

void Robot::ChangeHealth(int value) {
    SetHealth(health_ + value);
}

void Robot::ChangeEnergy(int value) {
    SetEnergy(energy_ + value);
}

void Robot::Interact(Robot& other) {
    if (is_enemy_ != other.is_enemy_) {
        other.ChangeHealth(-damage_);
    } else {
        other.ChangeHealth(heal_);
    }
}