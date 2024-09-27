#include "outside.h"
#include "mazemaker.h"
#include "solveur.h"

int main() {
    const maze_t test = column_maze(6, 6);
    minigame1(test);
    free_maze(test);

    return 0;
}