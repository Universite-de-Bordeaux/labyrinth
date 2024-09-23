#include "case.h"
#include "outside.h"

int main() {
    const int width = 25;
    const int height = 25;
    cell **test = create_basic_maze(width, height);
    wall_up(test, 10, 10, width, height);
    wall_down(test, 10, 10, width, height);
    wall_right(test, 10, 10, width, height);
    if(print_maze(test, width, height) == -1)
    {
        return 1;
    }
    return 0;
}