#ifndef CASE_H
#define CASE_H

#include <stdbool.h>

struct cell {
    bool wall_up;
    bool wall_down;
    bool wall_left;
    bool wall_right;
};

struct maze {
    int width;
    int height;
    struct cell **cells;
};

typedef struct cell cell;
typedef struct maze maze_t;

maze_t create_basic_maze(int width, int height);
void free_maze(maze_t maze);
int print_maze(maze_t maze);
void wall_up(maze_t maze, int x, int y);
void wall_down(maze_t maze, int x, int y);
void wall_left(maze_t maze, int x, int y);
void wall_right(maze_t maze, int x, int y);
void unwall_up(maze_t maze, int x, int y);
void unwall_down(maze_t maze, int x, int y);
void unwall_left(maze_t maze, int x, int y);
void unwall_right(maze_t maze, int x, int y);
#endif //CASE_H
