#include "field.h"
#include <cstdlib>
#include <queue>
#include <stdexcept>

Field::Field(int width, int height) : width_(width), height_(height) {
    if (width_ < kWidthMin || width_ > kWidthMax ||
        height_ < kHeightMin || height_ > kHeightMax)
        throw std::invalid_argument("Field: size out of range");

    grid_.resize(height_, std::vector<Cell>(width_, Cell(true)));
}

bool Field::IsCorrectCell(Position dot) const {
    return dot.X() >= 0 && dot.X() < width_ &&
           dot.Y() >= 0 && dot.Y() < height_;
}

bool Field::IsAvailableCell(Position dot) const {
    if (!IsCorrectCell(dot)) return false;
    return grid_[dot.Y()][dot.X()].IsAvailable();
}

void Field::SetCellAvailability(Position dot, bool is_available) {
    if (IsCorrectCell(dot))
        grid_[dot.Y()][dot.X()].SetAvailable(is_available);
}

bool Field::IsForbidden(Position p, const std::vector<Position>& forbidden) const {
    for (const Position& f : forbidden)
        if (f == p) return true;
    return false;
}

void Field::GenerateObstacles(int count, const std::vector<Position>& forbidden) {
    if (count < 0)
        throw std::invalid_argument("Field: negative obstacles count");

    int placed = 0;
    int attempts = 0;
    const int kMaxAttempts = width_ * height_ * 10;

    while (placed < count && attempts < kMaxAttempts) {
        ++attempts;
        Position p{std::rand() % width_, std::rand() % height_};

        if (IsForbidden(p, forbidden)) continue;
        if (!grid_[p.Y()][p.X()].IsAvailable()) continue;

        grid_[p.Y()][p.X()].SetAvailable(false);
        ++placed;
    }
}

int Field::GetCellPassability(Position dot) const {
    if (!IsCorrectCell(dot)) return 0;
    return grid_[dot.Y()][dot.X()].GetPassability();
}

void Field::SetCellPassability(Position dot, int passability) {
    if (IsCorrectCell(dot))
        grid_[dot.Y()][dot.X()].SetPassability(passability);
}

void Field::GeneratePassability(const std::vector<Position>& forbidden) {
    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            Position p{x, y};
            if (!grid_[y][x].IsAvailable()) continue;
            if (IsForbidden(p, forbidden)) continue;
            grid_[y][x].SetPassability(1 + std::rand() % 3);
        }
    }
}

bool Field::IsCellKnown(Position dot) const {
    if (!IsCorrectCell(dot)) return false;
    return grid_[dot.Y()][dot.X()].IsKnown();
}

void Field::SetCellKnown(Position dot, bool is_known) {
    if (IsCorrectCell(dot))
        grid_[dot.Y()][dot.X()].SetKnown(is_known);
}

bool Field::CanPlaceArea(Position top_left, int area_width, int area_height) const {
    for (int dy = 0; dy < area_height; ++dy) {
        for (int dx = 0; dx < area_width; ++dx) {
            Position p{top_left.X() + dx, top_left.Y() + dy};
            if (!IsCorrectCell(p)) return false;
            if (!grid_[p.Y()][p.X()].IsAvailable()) return false;
        }
    }
    return true;
}

void Field::OccupyArea(Position top_left, int area_width, int area_height) {
    if (!CanPlaceArea(top_left, area_width, area_height))
        throw std::invalid_argument("Field: cannot occupy area");
    for (int dy = 0; dy < area_height; ++dy)
        for (int dx = 0; dx < area_width; ++dx)
            SetCellAvailability({top_left.X() + dx, top_left.Y() + dy}, false);
}

void Field::RemoveUnreachableCells(Position start, const std::vector<Position>& keep) {
    if (!IsCorrectCell(start) || !IsAvailableCell(start)) return;

    std::vector<std::vector<bool>> visited(
        height_, std::vector<bool>(width_, false));

    std::queue<Position> q;
    q.push(start);
    visited[start.Y()][start.X()] = true;

    const Position kDirs[4] = {{1,0}, {-1,0}, {0,1}, {0,-1}};

    while (!q.empty()) {
        Position cur = q.front(); q.pop();
        for (const Position& d : kDirs) {
            Position next{cur.X() + d.X(), cur.Y() + d.Y()};
            if (!IsCorrectCell(next)) continue;
            if (!IsAvailableCell(next)) continue;
            if (visited[next.Y()][next.X()]) continue;
            visited[next.Y()][next.X()] = true;
            q.push(next);
        }
    }

    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            Position p{x, y};
            if (!IsAvailableCell(p)) continue;
            if (visited[y][x]) continue;
            if (IsForbidden(p, keep)) continue;
            grid_[y][x].SetAvailable(false);
        }
    }
}