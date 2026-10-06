#pragma once
#include "field.h"
#include "player_robot.h"

class VisibilitySystem {
public:
    void Update(Field& field, const PlayerRobot& player) const;
};