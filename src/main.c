#include "outside.h"
#include "mazemaker.h"
#include "solveur.h"

int main() {
    maze_t maze = line_maze(10, 10);
    print_maze(maze, "test");
    free_maze(maze);
    return 0;
}