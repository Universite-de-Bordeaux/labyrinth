#include "mazemaker.h"
#include "test.h"

int main() {
    EVALUATE_MAZEMAKERN(perfect_one_way_maze, "perfect_one_way_maze");
    EVALUATE_MAZEMAKERN(imperfect_one_way_maze, "imperfect_one_way_maze");
    EVALUATE_MAZEMAKER(line_maze);
    return 0;
}