#include "outside.h"
#include "mazemaker.h"
#include "solveur.h"

int main() {
  maze_t maze = hunt_kill_maze(10, 10);
  minigame3(maze);
  return 0;
}