#ifndef CASE_H
#define CASE_H
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

struct cell {
    bool wall_up;
    bool wall_down;
    bool wall_left;
    bool wall_right;
};

typedef struct cell cell;

cell **create_maze(int width, int height);
void print_maze(cell **maze, const int width, const int height);
#endif //CASE_H
