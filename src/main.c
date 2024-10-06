#include "outside.h"
#include "mazemaker.h"
#include "solveur.h"
#include <stdio.h>

int main() {
  const maze_t maze = rmaze(10, 10);
  cascade(maze) ? printf("solvable\n") : printf("insolvable\n");
  return 0;
}