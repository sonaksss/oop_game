#include "position.h"

Position::Position(int x, int y) : x_(x), y_(y) {}
Position::Position() : x_(0), y_(0) {}

int Position::X() const { return x_; }
int Position::Y() const { return y_; }

void Position::SetPosition(int x, int y) {
    x_ = x;
    y_ = y;
}

void Position::SetPosition(Position other) {
    x_ = other.X();
    y_ = other.Y();
}

int Position::ManhattanDistance(Position other) const {
    return std::abs(x_ - other.X()) + std::abs(y_ - other.Y());
}

Position Position::Up() const {
    return Position(x_, y_ - 1);
}

Position Position::Down() const {
    return Position(x_, y_ + 1);
}

Position Position::Left() const {
    return Position(x_ - 1, y_);
}

Position Position::Right() const {
    return Position(x_ + 1, y_);
}

bool Position::operator==(const Position& other) const {
    return x_ == other.X() && y_ == other.Y();
}

bool Position::operator!=(const Position& other) const {
    return !(*this == other);
}