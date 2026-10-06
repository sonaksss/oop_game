#include "movement_system.h"

Robot* MovementSystem::FindRobotAt(Position pos, Robot& self,
                                   PlayerRobot& player,
                                   std::vector<EnemyRobot>& enemies) const {
    if (&player != &self && player.IsAlive() && player.GetPosition() == pos)
        return &player;

    for (EnemyRobot& e : enemies) {
        if (&e != &self && e.IsAlive() && e.GetPosition() == pos)
            return &e;
    }
    return nullptr;
}

RobotsFactory* MovementSystem::FindFactoryAt(
        Position pos, std::vector<RobotsFactory>& factories) const {
    for (RobotsFactory& f : factories) {
        if (!f.IsAlive()) continue;
        if (f.Occupies(pos)) return &f;
    }
    return nullptr;
}

bool MovementSystem::TryInteractWithFactory(
        Robot& robot, Position target,
        std::vector<RobotsFactory>& factories) const {
    RobotsFactory* factory = FindFactoryAt(target, factories);
    if (!factory) return false;

    if (!robot.IsEnemy())
        factory->TakeDamage(robot.GetDamage());

    return true;
}

void MovementSystem::HandleInteraction(Robot& robot, Robot& other,
                                       PlayerRobot& player) const {
    bool was_alive = other.IsAlive();
    bool was_enemy = (robot.IsEnemy() != other.IsEnemy());

    robot.Interact(other);

    if (was_alive && was_enemy && !other.IsAlive()) {
        if (!robot.IsEnemy())
            player.AddExperience(kExperiencePerKill);
    }
}

bool MovementSystem::TryMoveByPath(Robot& robot, Position delta,
                                   Field& field,
                                   PlayerRobot& player,
                                   std::vector<EnemyRobot>& enemies,
                                   std::vector<RobotsFactory>& factories) const {
    Position step{0, 0};
    if (delta.X() > 0) step = {1, 0};
    else if (delta.X() < 0) step = {-1, 0};
    else if (delta.Y() > 0) step = {0, 1};
    else if (delta.Y() < 0) step = {0, -1};
    else return false;

    int remaining = robot.GetSpeed();
    bool moved = false;

    while (remaining > 0) {
        Position current = robot.GetPosition();
        Position target{current.X() + step.X(), current.Y() + step.Y()};

        if (TryInteractWithFactory(robot, target, factories))
            return true;

        if (!field.IsAvailableCell(target))
            break;

        if (Robot* other = FindRobotAt(target, robot, player, enemies)) {
            HandleInteraction(robot, *other, player);
            return true;
        }

        int passability = field.GetCellPassability(target);
        if (passability > remaining)
            break;

        robot.SetPosition(target);
        remaining -= passability;
        moved = true;
    }

    return moved;
}