#pragma once

class Cell {
private:
    static constexpr int kPassabilityMin = 1;
    static constexpr int kPassabilityMax = 3;

    bool is_available_;
    int passability_;
    bool is_known_;

public:
    explicit Cell(bool is_available);

    bool IsAvailable() const { return is_available_; }
    void SetAvailable(bool is_available) { is_available_ = is_available; }

    int GetPassability() const { return passability_; }
    void SetPassability(int passability);

    bool IsKnown() const { return is_known_; }
    void SetKnown(bool is_known) { is_known_ = is_known; }
};