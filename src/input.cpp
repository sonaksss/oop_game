#include "input.h"
#include <iostream>

Position Input::ReadMove() {
    std::cout << "Ход (z/s/q/d, t — выход):\n";
    char ch;

    if (!(std::cin >> ch)) {
        time_out_ = true;
        return {0, 0};
    }

    switch (ch) {
        case 'z': return { 0, -1};
        case 's': return { 0,  1};
        case 'q': return {-1,  0};
        case 'd': return { 1,  0};
        case 't': time_out_ = true; return {0, 0};
        default:  return {0, 0};
    }
}