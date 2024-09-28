#include "outside.h"
#include "mazemaker.h"
#include "solveur.h"

int main() {
    const maze_t test = line_maze(2, 3);
    minigame1(test);
    free_maze(test);

    return 0;
}