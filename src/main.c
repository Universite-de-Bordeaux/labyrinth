#include "mazemaker.h"
#include "solveur.h"
#include "test.h"

int main() {
    EVALUATE_MAZEMAKERN(perfect_one_way_maze, "perfect_one_way_maze");        //théoriquement parfait
    EVALUATE_MAZEMAKERN(imperfect_one_way_maze, "imperfect_one_way_maze");    //théoriquement solvable
    EVALUATE_MAZEMAKERN(hunt_kill_maze, "hunt_kill_maze");                    //théoriquement parfait
    EVALUATE_MAZEMAKERN(by_path_maze, "by_path_maze");                        //théoriquement parfait
    EVALUATE_MAZEMAKERN(line_maze, "line_maze");                              //théoriquement parfait
    EVALUATE_MAZEMAKERN(column_maze, "column_maze");                          //théoriquement parfait
    EVALUATE_MAZEMAKERN(create_basic_maze, "create_basic_maze");              //théoriquement solvable
    EVALUATE_MAZEMAKERN(create_wall_maze, "create_wall_maze");                //théoriquement insolvable
    return 0;
}