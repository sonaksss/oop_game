#include "level.h"

Level::Level(Field field, PlayerRobot player, std::vector<EnemyRobot> enemies, std::vector<RobotsFactory> factories): 
             field_(std::move(field)), player_(std::move(player)), enemies_(std::move(enemies)), factories_(std::move(factories)) {}

Field&& Level::TakeField() { return std::move(field_); }
PlayerRobot&& Level::TakePlayer() { return std::move(player_); }
std::vector<EnemyRobot>&& Level::TakeEnemies() { return std::move(enemies_); }
std::vector<RobotsFactory>&& Level::TakeFactories() { return std::move(factories_); }

const Field& Level::GetField() const { return field_; }
const PlayerRobot& Level::GetPlayer() const { return player_; }
const std::vector<EnemyRobot>& Level::GetEnemies() const { return enemies_; }
const std::vector<RobotsFactory>& Level::GetFactories() const { return factories_; }