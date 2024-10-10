#include "outside.h"
#include "mazemaker.h"
#include "solveur.h"
#include <stdio.h>

int main() {
  const maze_t maze = rmaze(25, 25);
  show_has_exit_cascade(maze);
  free_maze(maze);
  return 0;
}