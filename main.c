#include "case.h"

int main() {
    const int width = 25;
    const int height = 25;
    cell **test = create_maze(width, height);
    print_maze(test, width, height);
    return 0;
}