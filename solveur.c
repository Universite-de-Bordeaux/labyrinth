#include "solveur.h"
#include <stdio.h>
#include <SDL2/SDL.h>

//une mini-jeu où il faut trouver la sortie 
//maze : le labyrinthe
int minigame1(const maze_t maze)
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) //initilisation de la SDL avec l'image et les events (comprends des malloc)
    {
        const char *error = SDL_GetError();
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", error);
        SDL_Quit();
        return -1;
    }
    SDL_Window *fenetre = SDL_CreateWindow("maze_game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, maze.width * 20, maze.height * 20, SDL_WINDOW_SHOWN); //creation d'une fenetre
    SDL_Renderer *renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED); //creation d'un renderer
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //

    int pos_x = 0;
    int pos_y = 0;
    bool is_gaming = true;
    SDL_Event event;
    while(is_gaming)
    {
        SDL_WaitEvent(&event); //attente d'un event
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            //l'utilisateur ferme la fenetre ou clique sur la croix
            is_gaming = false;
        }
        else if(event.type == SDL_WINDOWEVENT_MINIMIZED)
        {
            while(event.type != SDL_WINDOWEVENT_RESTORED)
            {
                SDL_WaitEvent(&event);
            }
        }
        else if(event.key.keysym.sym == SDLK_z)
        {
            if(!maze[y][x].wall_up)
            {
                pos_y--;
            }
        }
        else if(event.key.keysym.sym == SDLK_s)
        {
            if(!maze[y][x].wall_down)
            {
                pos_y++;
            }
        }
        else if(event.key.keysym.sym == SDLK_q)
        {
            if(!maze[y][x].wall_left)
            {
                pos_x--;
            }
        }
        else if(event.key.keysym.sym == SDLK_d)
        {
            if(!maze[y][x].wall_right)
            {
                pos_x--;
            }
        }
        if(maze.cells[pos_y][pos_x].wall_up)
            {
            SDL_RenderDrawLine(renderer, pos_x * 20, pos_y * 20, (pos_x * 20) + 19, pos_y * 20);
            }
        if(maze.cells[pos_y][pos_x].wall_down)
            {
            SDL_RenderDrawLine(renderer, pos_x * 20, (pos_y * 20) + 19, (pos_x * 20) + 19, (pos_y * 20) + 19);
            }
        if(maze.cells[pos_y][pos_x].wall_left)
            {
            SDL_RenderDrawLine(renderer, pos_x * 20, pos_y * 20, pos_x * 20, (pos_y * 20) + 19);
            }

        if(maze.cells[pos_y][pos_x].wall_right)
            {
            SDL_RenderDrawLine(renderer, (pos_x * 20) + 19, pos_y * 20, (pos_x * 20) + 19, (pos_y * 20) + 19);
            }
        SDL_RenderPresent(renderer); //on met à jour l'affichage
        SDL_Delay(250); //pause de 0.25 secondes
    }
    SDL_DestroyRenderer(renderer); //destruction du renderer (desallocation de la memoire)
    SDL_DestroyWindow(fenetre); //destruction de la fenetre (desallocation de la memoire)
    SDL_Quit(); //desalocation de la memoire
    return 0;
}