#include "outside.h"
#include "mazemaker.h"

int main() {
    maze_t test = maze_from_file("test");
    if(print_maze(test) == -1)
    {
        free_maze(test);
        return 1;
    }
    free_maze(test);
    test = line_maze(10, 10);
    if(print_maze(test) == -1)
    {
        free_maze(test);
        return 1;
    }
    return 0;
}