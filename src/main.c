#include "mazemaker.h"
#include "test.h"
#include <time.h>
#include <stdio.h>

int main() {
    //EVALUATE_MAZEMAKERN(perfect_one_way_maze, "perfect_one_way_maze");        //théoriquement parfait
    //EVALUATE_MAZEMAKERN(imperfect_one_way_maze, "imperfect_one_way_maze");    //théoriquement solvable
    //EVALUATE_MAZEMAKERN(hunt_kill_maze, "hunt_kill_maze");                    //théoriquement parfait
    //EVALUATE_MAZEMAKERN(by_path_maze, "by_path_maze");                        //théoriquement parfait
    //EVALUATE_MAZEMAKERN(line_maze, "line_maze");                              //théoriquement parfait
    //EVALUATE_MAZEMAKERN(column_maze, "column_maze");                          //théoriquement parfait
    //EVALUATE_MAZEMAKERN(create_basic_maze, "create_basic_maze");              //théoriquement solvable
    //EVALUATE_MAZEMAKERN(create_wall_maze, "create_wall_maze");                //théoriquement insolvable

    EVALUATE_TIME(create_basic_maze, "create_basic_maze", 100, 100);            //très rapide
    EVALUATE_TIME(create_wall_maze, "create_wall_maze", 100, 100);              //très rapide
    EVALUATE_TIME(line_maze, "line_maze", 100, 100);                            //très rapide
    EVALUATE_TIME(column_maze, "column_maze", 100, 100);                        //très rapide
    EVALUATE_TIME(imperfect_one_way_maze, "imperfect_one_way_maze", 100, 100);  //plutot rapide
    EVALUATE_TIME(perfect_one_way_maze, "perfect_one_way_maze", 100, 100);      //convenable
    EVALUATE_TIME(hunt_kill_maze, "hunt_kill_maze", 100, 100);                  //WTF c'est SUPER LENT !!!
    EVALUATE_TIME(by_path_maze, "by_path_maze", 100, 100);                      //un peu lent
    return 0;
}