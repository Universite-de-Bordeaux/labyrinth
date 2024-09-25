#ifndef OUTSIDE_H
#define OUTSIDE_H
#include "case.h"

maze_t maze_from_file(const char *filename);
void maze_to_file(maze_t maze, const char *filename);

#endif //OUTSIDE_H
