#pragma once
#include "position.h"

class Input {
private:
    bool time_out_ = false;

public:
    Position ReadMove();
    bool WantsQuit() const { return time_out_; }
};