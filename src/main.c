#include "mazemaker.h"
#include "solveur.h"
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

    //technique de la flemme
    #define EVALUATE_TIME(générateur, name, x, y) do { \
        clock_t start = clock(); \
        for(int i = 1; i < x; i++) \
        { \
            for(int j = 1; j < y; j++) \
            { \
                const maze_t maze = générateur(i, j); \
                free_maze(maze); \
            } \
        } \
        clock_t end = clock(); \
        printf("Temps de génération par %s : \t%f secondes\n", name, (double)(end - start) / CLOCKS_PER_SEC); \
    } while(0)
    EVALUATE_TIME(create_basic_maze, "create_basic_maze", 100, 100);
    EVALUATE_TIME(create_wall_maze, "create_wall_maze", 100, 100);
    EVALUATE_TIME(line_maze, "line_maze", 100, 100);
    EVALUATE_TIME(column_maze, "column_maze", 100, 100);
    EVALUATE_TIME(imperfect_one_way_maze, "imperfect_one_way_maze", 100, 100);
    EVALUATE_TIME(perfect_one_way_maze, "perfect_one_way_maze", 100, 100);
    EVALUATE_TIME(hunt_kill_maze, "hunt_kill_maze", 100, 100);
    EVALUATE_TIME(by_path_maze, "by_path_maze", 100, 100);
    return 0;
}