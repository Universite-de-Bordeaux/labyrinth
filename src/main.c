#include "mazemaker.h"
#include "test.h"
#include <time.h>
#include <stdio.h>

#include "outside.h"
#include "solveur.h"

int main() {
    //EVALUATE_MAZEMAKERN(perfect_one_way_maze, "perfect_one_way_maze");        //théoriquement parfait
    //EVALUATE_MAZEMAKERN(imperfect_one_way_maze, "imperfect_one_way_maze");    //théoriquement solvable
    //EVALUATE_MAZEMAKERN(hunt_kill_maze, "hunt_kill_maze");                    //théoriquement parfait
    //EVALUATE_MAZEMAKERN(by_path_maze, "by_path_maze");                        //théoriquement parfait
    //EVALUATE_MAZEMAKERN(line_maze, "line_maze");                              //théoriquement parfait
    //EVALUATE_MAZEMAKERN(column_maze, "column_maze");                          //théoriquement parfait
    //EVALUATE_MAZEMAKERN(create_basic_maze, "create_basic_maze");              //théoriquement solvable
    //EVALUATE_MAZEMAKERN(create_wall_maze, "create_wall_maze");                //théoriquement insolvable

    //EVALUATE_TIME(create_basic_maze, "create_basic_maze", 100, 100);          //très rapide
    //EVALUATE_TIME(create_wall_maze, "create_wall_maze", 100, 100);            //très rapide
    //EVALUATE_TIME(line_maze, "line_maze", 100, 100);                          //très rapide
    //EVALUATE_TIME(column_maze, "column_maze", 100, 100);                      //très rapide
    //EVALUATE_TIME(imperfect_one_way_maze, "imperfect_one_way_maze", 100, 100);//plutot rapide
    //EVALUATE_TIME(perfect_one_way_maze, "perfect_one_way_maze", 100, 100);    //convenable
    //EVALUATE_TIME(hunt_kill_maze, "hunt_kill_maze", 100, 100);                //WTF c'est SUPER LENT !!!
    //EVALUATE_TIME(by_path_maze, "by_path_maze", 100, 100);                    //un peu lent

    //const maze_t maze = column_maze(20, 20);
    //for(int i = 0; i < 10; i++)
    //{
    //    unwall_down(maze, 19, i);
    //}
    //show_shortest_exit_right_hand(maze);
    //free_maze(maze);
    for(int k = 1; k < 100; k++)
    {
        for(int l = 1; l < 100; l++){
            const maze_t maze = r_maze(k, l);
            maze_to_file(maze, "maze");
            const maze_t maze2 = maze_from_file("maze");
            for(int i = 0; i < k; i++)
            {
                for(int j = 0; j < l; j++)
                {
                    if(has_wall_up(maze, i, j) != has_wall_up(maze2, i, j))
                    {
                        fprintf(stderr, "Erreur de copie de mur en haut en %d %d\n", i, j);
                        return 0;
                    }
                    if(has_wall_down(maze, i, j) != has_wall_down(maze2, i, j))
                    {
                        fprintf(stderr, "Erreur de copie de mur en bas en %d %d\n", i, j);
                        return 0;
                    }
                    if(has_wall_left(maze, i, j) != has_wall_left(maze2, i, j))
                    {
                        fprintf(stderr, "Erreur de copie de mur à gauche en %d %d\n", i, j);
                        return 0;
                    }
                    if(has_wall_right(maze, i, j) != has_wall_right(maze2, i, j))
                    {
                        fprintf(stderr, "Erreur de copie de mur à droite en %d %d\n", i, j);
                        return 0;
                    }
                }
            }
        }
    }
    printf("Looks good\n");    return 0;
}
