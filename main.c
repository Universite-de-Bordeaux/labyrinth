#include "case.h"

int main() {
    cell **test = create_maze(5, 5);
    print_maze(test, 5, 5);
    return 0;
}