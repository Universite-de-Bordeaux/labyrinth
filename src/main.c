#include "outside.h"
#include "mazemaker.h"
#include "solveur.h"
#include <stdio.h>

int main() {
  const maze_t maze = hunt_kill_maze(50, 50);
  is_exit_cascade(maze) ? printf("Exit found\n") : printf("Exit not found\n");
  return print_maze(maze, "maze") ? 0 : 1;
}