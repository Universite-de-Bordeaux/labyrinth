#include "outside.h"

int main() {
    const int width = 25;
    const int height = 25;
    maze_t test = create_basic_maze(width, height);
    if(print_maze(test) == -1)
    {
        return 1;
    }
    free_maze(test);
    test = maze_from_file("testy");
    if(print_maze(test) == -1)
    {
        return 1;
    }
    free_maze(test);
    return 0;
}