//
// Created by vins on 20/09/24.
//

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
#endif //CASE_H
