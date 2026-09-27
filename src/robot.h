#pragma once
#include <stdexcept>
#include "position.h"

class Robot {
protected:
    int health_;
    int health_max_;
    int damage_;
    int heal_;
    int energy_;
    int energy_max_;
    int speed_;
    bool is_enemy_;
    Position position_;
public:
    Robot(int health_max, int damage, int heal, int energy_max, int speed, bool is_enemy, Position position);
    virtual ~Robot() = default;

    int GetHealth() const { return health_; }
    int GetMaxHealth() const { return health_max_; }
    int GetDamage() const { return damage_; }
    int GetHeal() const { return heal_; }
    int GetEnergy() const { return energy_; }
    int GetMaxEnergy() const { return energy_max_; }
    int GetSpeed() const { return speed_; }
    bool IsEnemy() const { return is_enemy_; }
    Position GetPosition() const { return position_; }
    bool IsAlive() const { return health_ > 0; }

    void SetHealth(int value);
    void SetMaxHealth(int value);
    void SetDamage(int value);
    void SetHeal(int value);
    void SetEnergy(int value);
    void SetMaxEnergy(int value);
    void SetSpeed(int value);
    void SetPosition(Position position);

    void ChangeHealth(int value);
    void ChangeEnergy(int value);

    void Interact(Robot& other);
};