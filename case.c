#include "case.h"

// Create a maze of width x height cells
cell **create_maze(const int width, const int height) {
    cell **maze = malloc(sizeof(cell *) * height);
    for (int i = 0; i < height; i++) {
        maze[i] = malloc(sizeof(cell) * width);
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
        {
            if(i == 0)
            {
                maze[i][j].wall_up = true;
            }
            else
            {
                maze[i][j].wall_up = false;
            }
            if(i == height - 1)
            {
                maze[i][j].wall_down = true;
            }
            else
            {
                maze[i][j].wall_down = false;
            }
            if(j == 0)
            {
                maze[i][j].wall_left = true;
            }
            else
            {
                maze[i][j].wall_left = false;
            }
            if(j == width - 1)
            {
                maze[i][j].wall_right = true;
            }
            else
            {
                maze[i][j].wall_right = false;
            }
        }
    }
    return maze;
}

void print_maze(cell **maze, const int width, const int height)
{
    for(unsigned int i = 0; i < height; i++)
    {
        for(unsigned int j = 0; j < width; j++)
        {
            if(maze[i][j].wall_up)
            {
                printf(" _");
            }
            else
            {
                printf("  ");
            }
        }
        printf("\n");
        for(unsigned int j = 0; j < width; j++)
        {
            if(maze[i][j].wall_left)
            {
                printf("|");
            }
            else
            {
                printf(" ");
            }
            printf(" ");
            if(maze[i][j].wall_right)
            {
                printf("|");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
}