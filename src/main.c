#include "outside.h"
#include "mazemaker.h"
#include "solveur.h"
#include <stdio.h>

int main() {
  const maze_t maze = rmaze(95, 50);
  wall_up(maze, 94, 49);
  wall_left(maze, 94, 49);
  show_has_exit_cascade(maze);
  free_maze(maze);
  return 0;
}