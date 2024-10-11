#include "outside.h"
#include "mazemaker.h"
#include "solveur.h"
#include <stdio.h>

int main() {
  const maze_t maze = rmaze(80, 50);
  wall_up(maze, 79, 49);
  wall_left(maze, 79, 49);
  show_is_perfect_cascade(maze);
  free_maze(maze);
  return 0;
}