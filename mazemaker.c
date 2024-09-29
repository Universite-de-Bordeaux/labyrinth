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

//Crée un labyrinthe parfait de taille width x height
//tous les murs sont horizontaux
maze_t column_maze(const int width, const int height)
{
    const time_t t = time(NULL);
    srand(t);
    const maze_t maze = create_basic_maze(width, height);
    for(int i = 0; i < height - 1; i++)
    {
        const int break_wall = rand() % width;
        for(int j = 0; j < width; j++)
        {
            if(j != break_wall)
            {
                wall_down(maze, j, i);
            }
        }
    }
    return maze;
}

//Crée un labyrinthe parfait de taille width x height
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
//seuls le chemins de sortie possèdent des embranchements
maze_t one_way_maze(const int width, const int height)
{
    maze_t maze = create_basic_maze(width, height);
    const time_t t = time(NULL);
    srand(t);
    const int exit = rand() % 2;
    if(exit == 0)
    {
        for(int i = 0; i < width - 1; i++)
        {
            wall_right(maze, i, 0);
        }
        for(int i = 0; i < height - 1; i++)
        {
            wall_down(maze, width - 1, i);
        }
        for(int i = 0; i < width - 1; i++)
        {
            wall_right(maze, i, height - 1);
        }
    }
    else
    {
        for(int i = 0; i < height - 1; i++)
        {
            wall_down(maze, 0, i);
        }
        for(int i = 0; i < width - 1; i++)
        {
            wall_right(maze, i, height - 1);
        }
        for(int i = 0; i < height - 1; i++)
        {
            wall_down(maze, width - 1, i);
        }
    }
    return maze;
}