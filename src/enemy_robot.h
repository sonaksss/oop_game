#pragma once
#include "robot.h"

class EnemyRobot: public Robot {
public:
    EnemyRobot(int health_max, int damage, int heal, int energy_max, int speed, Position position);
    ~EnemyRobot() override = default;
};