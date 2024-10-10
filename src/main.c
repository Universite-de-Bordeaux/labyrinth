#include "outside.h"
#include "mazemaker.h"
#include "solveur.h"
#include <stdio.h>

int main() {
  maze_t maze = hunt_kill_maze(1, 100);
  r_minigame(maze);
  return 0;
}