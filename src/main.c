#include "outside.h"
#include "mazemaker.h"
#include "solveur.h"
#include <stdio.h>

int main() {
  const maze_t maze = imperfect_one_way_maze(20, 10);
  printf("Maze made\n");
  has_exit_cascade(maze) ? printf("Maze has exit\n") : printf("Maze has no exit\n");
  return print_maze(maze, "maze") ? 0 : 1;
}