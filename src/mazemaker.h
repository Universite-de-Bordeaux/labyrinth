#ifndef MAZEMAKER_H
#define MAZEMAKER_H
#include "case.h"

maze_t line_maze(const int width, const int height);
maze_t column_maze(const int width, const int height);
maze_t imperfect_one_way_maze(const int width, const int height);
maze_t perfect_one_way_maze(const int width, const int height);
maze_t rmaze(const int width, const int height);
maze_t rperfect_maze(const int width, const int height);
maze_t rimperfect_maze(const int width, const int height);

#endif //MAZEMAKER_H
