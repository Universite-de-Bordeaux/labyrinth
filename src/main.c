#include "outside.h"
#include "mazemaker.h"
#include "solveur.h"
#include <stdio.h>

int main() {
  const maze_t maze = rmaze(0, 0);
  free_maze(maze);
  return 0;
}