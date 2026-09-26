#pragma once
#include <vector>
#include "cell.h"
#include "position.h"

class Field {
private:
    static constexpr int kWidthMin = 5;
    static constexpr int kWidthMax = 50;
    static constexpr int kHeightMin = 5;
    static constexpr int kHeightMax = 50;

    int width_;
    int height_;
    std::vector<std::vector<Cell>> grid_;

    bool IsForbidden(Position p, const std::vector<Position>& forbidden) const;

public:
    explicit Field(int width, int height);

    int GetWidth() const { return width_; }
    int GetHeight() const { return height_; }

    bool IsCorrectCell(Position dot) const;
    bool IsAvailableCell(Position dot) const;
    void SetCellAvailability(Position dot, bool is_available);
    void GenerateObstacles(int count, const std::vector<Position>& forbidden);

    int GetCellPassability(Position dot) const;
    void SetCellPassability(Position dot, int passability);
    void GeneratePassability(const std::vector<Position>& forbidden);

    bool IsCellKnown(Position dot) const;
    void SetCellKnown(Position dot, bool is_known);

    void OccupyArea(Position top_left, int area_width, int area_height);
    bool CanPlaceArea(Position top_left, int area_width, int area_height) const;
};