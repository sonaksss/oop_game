#pragma once
#include <cstdlib>

class Position {
private:
    int x_;
    int y_;

public:
    Position(int x, int y);
    Position();

    int X() const;
    int Y() const;
    int ManhattanDistance(Position other) const;

    bool operator==(const Position& other) const;
    bool operator!=(const Position& other) const;
};