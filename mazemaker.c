#include "mazemaker.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

//Crée un labyrinthe parfait de taille width x height
//tous les murs sont verticaux
maze_t line_maze(const int width, const int height)
{
    const time_t t = time(NULL);
    srand(t);
    const maze_t maze = create_basic_maze(width, height);
    for(int i = 0; i < width - 1; i++)
    {
        const int break_wall = rand() % height;
        for(int j = 0; j < height; j++)
        {
            if(j != break_wall)
            {
                wall_right(maze, i, j);
            }
        }
    }
    return maze;
}