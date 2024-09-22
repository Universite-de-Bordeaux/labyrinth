#ifndef CASE_H
#define CASE_H

#include <stdbool.h>

struct cell {
    bool wall_up;
    bool wall_down;
    bool wall_left;
    bool wall_right;
};

typedef struct cell cell;
cell **create_basic_maze(int width, int height);
int print_maze(cell **maze, const int width, const int height);
#endif //CASE_H
