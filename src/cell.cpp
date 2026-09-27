#include "cell.h"

Cell::Cell(bool is_available): is_available_(is_available), passability_(kPassabilityMin), is_known_(false) {}

void Cell::SetPassability(int passability) {
    if (passability < kPassabilityMin) {
        passability_ = kPassabilityMin;
    } else if (passability > kPassabilityMax) {
        passability_ = kPassabilityMax;
    } else {
        passability_ = passability;
    }
}