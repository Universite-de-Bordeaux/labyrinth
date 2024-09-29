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

//Crée un labyrinthe semi-parfait de taille width x height
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
//seuls le chemins de sortie possèdent des embranchements
maze_t semiperfect_one_way_maze(const int width, const int height)
{
    const maze_t maze = create_wall_maze(width, height);
    const bool_tab visited = create_booltab(width, height);
    const time_t t = time(NULL);
    srand(t);
    int x = 0;
    int y = 0;
    while(!(x == width - 1 && y == height - 1))
    {
        if(rand() % 2 == 0 || y == height - 1)
        {
            const int next_x = (rand() % (width - x)) + x;
            for (int i = x; i < next_x; i++)
            {
                unwall_right(maze, i, y);
                visited.tab[i][y] = true;
            }
            x = next_x;
        }
        else
        {
            const int next_y = (rand() % (height - y)) + y;
            for (int i = y; i < next_y; i++)
            {
                unwall_down(maze, x, i);
                visited.tab[x][i] = true;
            }
            y = next_y;
        }
    }
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            if(!visited.tab[i][j])
            {
                int direction = rand() % 4;
                if(direction == 0 && i > 0)
                {
                    unwall_left(maze, i, j);
                }
                else if(direction == 1 && i < width - 1)
                {
                    unwall_right(maze, i, j);
                }
                else if(direction == 2 && j > 0)
                {
                    unwall_up(maze, i, j);
                }
                else if(direction == 3 && j < height - 1)
                {
                    unwall_down(maze, i, j);
                }
            }
        }
    }
    return maze;
}

//Crée un labyrinthe parfait de taille width x height
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
//seuls le chemins de sortie possèdent des embranchements
maze_t perfect_one_way_maze(const int width, const int height)
{
    const maze_t maze = create_wall_maze(width, height);
    const bool_tab visited = create_booltab(width, height);
    const time_t t = time(NULL);
    srand(t);
    int x = 0;
    int y = 0;
    while(!(x == width - 1 && y == height - 1))
    {
        if(rand() % 2 == 0 || y == height - 1)
        {
            const int next_x = (rand() % (width - x)) + x;
            for (int i = x; i < next_x; i++)
            {
                unwall_right(maze, i, y);
                visited.tab[i][y] = true;
            }
            x = next_x;
        }
        else
        {
            const int next_y = (rand() % (height - y)) + y;
            for (int i = y; i < next_y; i++)
            {
                unwall_down(maze, x, i);
                visited.tab[x][i] = true;
            }
            y = next_y;
        }
    }
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            if(!visited.tab[i][j])
            {
                int direction = rand() % 4;
                if(direction == 0 && i > 0)
                {
                    unwall_left(maze, i, j);
                }
                else if(direction == 1 && i < width - 1)
                {
                    unwall_right(maze, i, j);
                }
                else if(direction == 2 && j > 0)
                {
                    unwall_up(maze, i, j);
                }
                else if(direction == 3 && j < height - 1)
                {
                    unwall_down(maze, i, j);
                }
            }
        }
    }
    return maze;
}