#include "visibility_system.h"

void VisibilitySystem::Update(Field& field, const PlayerRobot& player) const {
    int radius = player.GetVisibility();
    Position center = player.GetPosition();

    for (int y = 0; y < field.GetHeight(); ++y) {
        for (int x = 0; x < field.GetWidth(); ++x) {
            Position p{x, y};

            bool was_known = field.IsCellKnown(p);
            bool available = field.IsAvailableCell(p);

            if (was_known && !available) continue;

            bool should_be_known = (p.ManhattanDistance(center) <= radius);
            if (was_known != should_be_known)
                field.SetCellKnown(p, should_be_known);
        }
    }
}