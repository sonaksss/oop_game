#include "renderer.h"
#include <iostream>

void Renderer::Draw(const Field& field,
                    const PlayerRobot& player,
                    const std::vector<EnemyRobot>& enemies,
                    const std::vector<RobotsFactory>& factories) const {
    for (int y = 0; y < field.GetHeight(); ++y) {
        for (int x = 0; x < field.GetWidth(); ++x) {
            Position p{x, y};

            char symbol;
            if (field.IsCellKnown(p)) {
                if (const Robot* r = FindRobotAt(p, player, enemies)) {
                    symbol = r->IsEnemy() ? 'E' : 'P';
                } else if (FindFactoryAt(p, factories)) {
                    symbol = 'F';
                } else if (!field.IsAvailableCell(p)) {
                    symbol = '#';
                } else {
                    symbol = '.';
                }
            } else {
                symbol = '?';
            }

            std::cout << symbol << ' ';
        }
        std::cout << '\n';
    }
    std::cout << "HP: " << player.GetHealth() << '/'
              << player.GetMaxHealth()
              << "  EN: " << player.GetEnergy() << '/'
              << player.GetMaxEnergy()
              << "  DMG: " << player.GetDamage()
              << "  HEAL: " << player.GetHeal()
              << "  EXP: " << player.GetExperience() << '/'
              << player.GetExperienceUp()
              << "  RANK: " << player.GetRank()
              << "  VIS: " << player.GetVisibility() << '\n';
    std::cout << '\n';
}

const Robot* Renderer::FindRobotAt(Position pos,
                                   const PlayerRobot& player,
                                   const std::vector<EnemyRobot>& enemies) const {
    if (player.IsAlive() && player.GetPosition() == pos)
        return &player;
    for (const EnemyRobot& e : enemies)
        if (e.IsAlive() && e.GetPosition() == pos)
            return &e;
    return nullptr;
}

const RobotsFactory* Renderer::FindFactoryAt(
        Position pos,
        const std::vector<RobotsFactory>& factories) const {
    for (const RobotsFactory& f : factories) {
        if (!f.IsAlive()) continue;
        for (const Position& c : f.GetOccupiedCells())
            if (c == pos) return &f;
    }
    return nullptr;
}