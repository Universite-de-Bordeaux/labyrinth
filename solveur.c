#include "solveur.h"
#include "mazemaker.h"
#include <stdio.h>
#include <time.h>
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
    if(fenetre == NULL)
    {
        const char *error = SDL_GetError();
        fprintf(stderr, "Erreur de création de la fenetre : %s\n", error);
        SDL_Quit();
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED); //creation d'un renderer
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //on choisit la couleur du renderer

    if(renderer == NULL)
    {
        const char *error = SDL_GetError();
        fprintf(stderr, "Erreur de création du renderer : %s\n", error);
        SDL_DestroyWindow(fenetre);
        SDL_Quit();
        return -1;
    }

    SDL_RenderDrawLine(renderer, 0, 0, 19, 0);
    SDL_RenderDrawLine(renderer, 0, 0, 0, 19);
    SDL_RenderDrawLine(renderer, 7, 10, 13, 10);
    SDL_RenderDrawLine(renderer, 10, 7, 10, 13);
    if(maze.cells[0][0].wall_right)
    {
        SDL_RenderDrawLine(renderer, 19, 0, 19, 19);
    }
    if(maze.cells[0][0].wall_down)
    {
        SDL_RenderDrawLine(renderer, 0, 19, 19, 19);
    }
    SDL_SetRenderDrawColor(renderer, 10, 235, 10, 255);
    SDL_RenderDrawLine(renderer, (maze.width-1) * 20, ((maze.height-1) * 20) + 19, ((maze.width-1) * 20) + 19, ((maze.height-1) * 20) + 19);
    SDL_RenderDrawLine(renderer, (maze.width-1) * 20 + 19, (maze.height-1) * 20, (maze.width-1) * 20 + 19, (maze.height-1) * 20 + 19);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Delay(10);
    SDL_RenderPresent(renderer);

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
        if(event.type == SDL_KEYUP)
        {
            SDL_Delay(1);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); //on définit la couleur en gomme
            SDL_RenderDrawLine(renderer, pos_x * 20 + 7, pos_y * 20 + 10, pos_x * 20 + 13, pos_y * 20 + 10);
            SDL_RenderDrawLine(renderer, pos_x * 20 + 10, pos_y * 20 + 7, pos_x * 20 + 10, pos_y * 20 + 13);
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //on choisit la couleur du renderer
            SDL_Delay(10);
            if(event.key.keysym.sym == SDLK_z || event.key.keysym.sym == SDLK_UP)
            {
                if(!maze.cells[pos_y][pos_x].wall_up)
                {
                    pos_y--;
                }
            }
            else if(event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_DOWN)
            {
                if(!maze.cells[pos_y][pos_x].wall_down)
                {
                    pos_y++;
                }
            }
            else if(event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_LEFT)
            {
                if(!maze.cells[pos_y][pos_x].wall_left)
                {
                    pos_x--;
                }
            }
            else if(event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT)
            {
                if(!maze.cells[pos_y][pos_x].wall_right)
                {
                    pos_x++;
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
            SDL_RenderDrawLine(renderer, pos_x * 20 + 7, pos_y * 20 + 10, pos_x * 20 + 13, pos_y * 20 + 10);
            SDL_RenderDrawLine(renderer, pos_x * 20 + 10, pos_y * 20 + 7, pos_x * 20 + 10, pos_y * 20 + 13);
            SDL_Delay(250); //pause de 0.25 secondes
            SDL_RenderPresent(renderer); //on met à jour l'affichage
        }
        if(pos_x == maze.width - 1 && pos_y == maze.height - 1)
        {
            is_gaming = false;
            SDL_Delay(100);
            print_maze(maze, "victoire !!");
        }
    }
    SDL_DestroyRenderer(renderer); //destruction du renderer (desallocation de la memoire)
    SDL_DestroyWindow(fenetre); //destruction de la fenetre (desallocation de la memoire)
    SDL_Quit(); //desalocation de la memoire
    return 0;
}

//lance minigame1 avec un générateur de labyrinthe aléatoire
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
int rminigame1(const int width, const int height)
{
    srand(time(NULL));
    const int r = rand() % 2; // NOLINT(*-msc50-cpp)
    if(r == 0)
    {
        const maze_t maze = line_maze(width, height);
        return minigame1(maze);
    }
    else if(r == 1)
    {
        const maze_t maze = column_maze(width, height);
        return minigame1(maze);
    }
    fprintf(stderr, "Erreur de gestion de l'aléatoire\n");
    return -1;
}
