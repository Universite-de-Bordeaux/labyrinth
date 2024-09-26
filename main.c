#include "outside.h"

int main() {
    const maze_t test = maze_from_file("test");
    if(print_maze(test) == -1)
    {
        free_maze(test);
        return 1;
    }
    free_maze(test);
    return 0;
}