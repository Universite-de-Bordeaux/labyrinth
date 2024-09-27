#include "outside.h"
#include "mazemaker.h"
#include "solveur.h"

int main() {
    maze_t test = maze_from_file("test");
    if(print_maze(test, "lecture") == -1)
    {
        free_maze(test);
        return 1;
    }
    test = line_maze(10, 10);
    minigame1(test);
    free_maze(test);

    return 0;
}