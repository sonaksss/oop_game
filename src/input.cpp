#include "input.h"
#include <ncurses.h>

Position Input::ReadMove() {
    int ch = getch();

    switch (ch) {
        case 'z': case 'Z': case KEY_UP:    return { 0, -1};
        case 's': case 'S': case KEY_DOWN:  return { 0,  1};
        case 'q': case 'Q': case KEY_LEFT:  return {-1,  0};
        case 'd': case 'D': case KEY_RIGHT: return { 1,  0};
        case 't': case 'T':
            time_out_ = true;
            return {0, 0};
        default:
            return {0, 0};
    }
}