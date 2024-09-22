#include "case.h"

int main() {
    const int width = 5;
    const int height = 5;
    cell **test = create_basic_maze(width, height);
    if(print_maze(test, width, height) == -1)
    {
        return 1;
    }
    return 0;
}