#include "outside.h"
#include "mazemaker.h"
#include "solveur.h"
#include <stdio.h>

int main() {
  const maze_t maze = rperfect_maze(95, 50);
  wall_up(maze, 94, 49);
  wall_left(maze, 94, 49);
  unwall_up(maze, 20, 20);
  unwall_left(maze, 20, 20);
  unwall_down(maze, 20, 20);
  show_is_perfect_cascade(maze);
  free_maze(maze);
  return 0;
}