#include "outside.h"
#include "mazemaker.h"
#include "solveur.h"

int main() {
    const maze_t test = maze_from_file("test");
    minigame1(test);
    free_maze(test);

    return 0;
}