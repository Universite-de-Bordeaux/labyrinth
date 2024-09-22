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
    maze[width/2][height/2].wall_down = true;
    maze[width/2][height/2].wall_up = true;
    maze[width/2][height/2].wall_left = true;
    maze[width/2][height/2].wall_right = true;
    return maze;
}

int print_maze(cell **maze, const int width, const int height)
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) //initilisation de la SDL avec l'image et les events (comprends des malloc)
    {
        const char *error = SDL_GetError();
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", error);
        SDL_Quit();
        return -1;
    }
    SDL_Window *fenetre = SDL_CreateWindow("maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width * 20, height * 20, SDL_WINDOW_SHOWN); //creation d'une fenetre
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
    SDL_Delay(1000); //pause de 1 secondes
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //on définit la couleur de fond en blanc

    for(int x = 0; x < width; x++)
    {
        for(int y = 0; y < height; y++)
        {
            if(maze[y][x].wall_up)
            {
                SDL_RenderDrawLine(renderer, x * 20, y * 20, (x * 20) + 19, y * 20);
            }
            if(maze[y][x].wall_down)
            {
                SDL_RenderDrawLine(renderer, x * 20, (y * 20) + 19, (x * 20) + 19, (y * 20) + 19);
            }
            if(maze[y][x].wall_left)
            {
                SDL_RenderDrawLine(renderer, x * 20, y * 20, x * 20, (y * 20) + 19);
            }
            if(maze[y][x].wall_right)
            {
                SDL_RenderDrawLine(renderer, (x * 20) + 19, y * 20, (x * 20) + 19, (y * 20) + 19);
            }
        }
    }

    SDL_RenderPresent(renderer); //on met à jour l'affichage
    SDL_Delay(500); //pause de 0.5 secondes
    SDL_SetRenderDrawColor(renderer, 0, 255, 100, 255); //on définit la couleur en vert
    SDL_RenderDrawLine(renderer, 0, 0, 0, 20); //l'entrée en vert
    SDL_RenderDrawLine(renderer, 0, 0, 20, 0); //l'entrée en vert

    SDL_RenderPresent(renderer); //on met à jour l'affichage
    SDL_Delay(500); //pause de 0.5 secondes
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); //on définit la couleur en bleu
    SDL_RenderDrawLine(renderer, (width * 20) - 1, (height * 20) - 20, (width * 20) - 1, (height * 20)); //la sortie en bleu
    SDL_RenderDrawLine(renderer, (width * 20) - 20, (height * 20) - 1, (width * 20), (height * 20) - 1); //la sortie en bleu

    SDL_RenderPresent(renderer); //on met à jour l'affichage
    SDL_Delay(3000); //pause de 3 secondes

    SDL_DestroyRenderer(renderer); //destruction du renderer (desallocation de la memoire)
    SDL_DestroyWindow(fenetre); //destruction de la fenetre (desallocation de la memoire)
    SDL_Quit(); //desalocation de la memoire
    return 0;
}
