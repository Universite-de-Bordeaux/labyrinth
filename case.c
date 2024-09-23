#include "case.h"
#include <stdio.h>
#include <SDL2/SDL.h>

// Create a maze of width x height cells
maze_t create_basic_maze(const int width, const int height) {
    cell** start = malloc(sizeof(cell *) * height);
    for (int i = 0; i < height; i++) {
        start[i] = malloc(sizeof(cell) * width);
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++)
        {
            if(i == 0)
            {
                start[i][j].wall_up = true;
            }
            else
            {
                start[i][j].wall_up = false;
            }
            if(i == height - 1)
            {
                start[i][j].wall_down = true;
            }
            else
            {
                start[i][j].wall_down = false;
            }
            if(j == 0)
            {
                start[i][j].wall_left = true;
            }
            else
            {
                start[i][j].wall_left = false;
            }
            if(j == width - 1)
            {
                start[i][j].wall_right = true;
            }
            else
            {
                start[i][j].wall_right = false;
            }
        }
    }
    maze_t maze = {width, height, start};
    return maze;
}

void free_maze(maze_t maze) {
    for (int i = 0; i < maze.height; i++) {
        free(maze.cells[i]);
    }
    free(maze.cells);
}

void wall_up(maze_t maze, int x, int y)
{
    if(y > 0)
    {
        maze.cells[y][x].wall_up = true;
        maze.cells[y - 1][x].wall_down = true;
    }
    else
      {
        maze.cells[y][x].wall_up = true;
      }
}

void wall_down(maze_t maze, int x, int y)
{
    if(y < maze.height - 1)
    {
        maze.cells[y][x].wall_down = true;
        maze.cells[y + 1][x].wall_up = true;
    }
    else
    {
        maze.cells[y][x].wall_down = true;
    }
}
void wall_left(maze_t maze, int x, int y)
{
    if(x > 0)
    {
        maze.cells[y][x].wall_left = true;
        maze.cells[y][x - 1].wall_right = true;
    }
    else
    {
        maze.cells[y][x].wall_left = true;
    }
}
void wall_right(maze_t maze, int x, int y)
{
    if(x < maze.width - 1)
    {
        maze.cells[y][x].wall_right = true;
        maze.cells[y][x + 1].wall_left = true;
    }
    else
    {
        maze.cells[y][x].wall_right = true;
    }
}

int print_maze(maze_t maze)
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) //initilisation de la SDL avec l'image et les events (comprends des malloc)
    {
        const char *error = SDL_GetError();
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", error);
        SDL_Quit();
        return -1;
    }
    SDL_Window *fenetre = SDL_CreateWindow("maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, maze.width * 20, maze.height * 20, SDL_WINDOW_SHOWN); //creation d'une fenetre
    SDL_Renderer *renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED); //creation d'un renderer
    if(renderer == NULL)
    {
        renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_SOFTWARE);
        if(renderer == NULL)
        {
            const char *error = SDL_GetError();
            fprintf(stderr, "Erreur de creation du renderer : %s\n", error);
            SDL_DestroyRenderer(renderer);
            SDL_DestroyWindow(fenetre);
            SDL_Quit();
            return -1;
        }
    }
    SDL_Delay(100); //pause de 1 secondes
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //on définit la couleur de fond en blanc

    for(int x = 0; x < maze.width; x++)
    {
        for(int y = 0; y < maze.height; y++)
        {
            if(maze.cells[y][x].wall_up)
            {
                SDL_RenderDrawLine(renderer, x * 20, y * 20, (x * 20) + 19, y * 20);
            }
            if(maze.cells[y][x].wall_down)
            {
                SDL_RenderDrawLine(renderer, x * 20, (y * 20) + 19, (x * 20) + 19, (y * 20) + 19);
            }
            if(maze.cells[y][x].wall_left)
            {
                SDL_RenderDrawLine(renderer, x * 20, y * 20, x * 20, (y * 20) + 19);
            }
            if(maze.cells[y][x].wall_right)
            {
                SDL_RenderDrawLine(renderer, (x * 20) + 19, y * 20, (x * 20) + 19, (y * 20) + 19);
            }
        }
    }

    SDL_RenderPresent(renderer); //on met à jour l'affichage
    SDL_Delay(1); //pause de 0.001 secondes
    SDL_SetRenderDrawColor(renderer, 0, 255, 100, 255); //on définit la couleur en vert
    SDL_RenderDrawLine(renderer, 0, 0, 0, 20); //l'entrée en vert
    SDL_RenderDrawLine(renderer, 0, 0, 20, 0); //l'entrée en vert

    SDL_RenderPresent(renderer); //on met à jour l'affichage
    SDL_Delay(1); //pause de 0.001 secondes
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); //on définit la couleur en bleu
    SDL_RenderDrawLine(renderer, (maze.width * 20) - 1, (maze.height * 20) - 20, (maze.width * 20) - 1, (maze.height * 20)); //la sortie en bleu
    SDL_RenderDrawLine(renderer, (maze.width * 20) - 20, (maze.height * 20) - 1, (maze.width * 20), (maze.height * 20) - 1); //la sortie en bleu

    SDL_RenderPresent(renderer); //on met à jour l'affichage
    SDL_Delay(2500); //pause de 2.5 secondes

    SDL_DestroyRenderer(renderer); //destruction du renderer (desallocation de la memoire)
    SDL_DestroyWindow(fenetre); //destruction de la fenetre (desallocation de la memoire)
    SDL_Quit(); //desalocation de la memoire
    return 0;
}
