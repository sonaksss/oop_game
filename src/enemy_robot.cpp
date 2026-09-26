#include "enemy_robot.h"

EnemyRobot::EnemyRobot(int max_health, int damage, int heal, int max_energy, int speed, Position position): 
                       Robot(max_health, damage, heal, max_energy, speed, true, position) {}