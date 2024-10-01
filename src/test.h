#ifndef TEST_H
#define TEST_H
#include "case.h"

bool walk_possible(maze_t maze, int x, int y,int width, int height ,bool_tab visited);
bool all_visited(int width, int height, bool_tab visited);
char rand_dir(int x, int y, int width, int height, bool_tab visited, int size, int dir_x[size], int dir_y[size], char dir[size]);
char rand_dir_hunt(int x, int y, int width, int height, bool_tab visited, int size, int dir_x[size], int dir_y[size], char dir[size]);
bool finding_hunt(int width, int height, bool_tab visited, int *px, int *py);
maze_t hunt_kill_maze(const int width, const int height);


#endif //TEST_H
