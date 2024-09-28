#include "outside.h"
#include "mazemaker.h"
#include "solveur.h"

int main() {
    const maze_t test = box_maze(10, 10);
if(print_maze(test, "test") != 0)
    {
        free_maze(test);
        return -1;
    }
    free_maze(test);

    return 0;
}