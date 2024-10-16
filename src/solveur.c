#include "solveur.h"
#include "mazemaker.h"
#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#define NB_GENERATEURS 6
#define DELAY 5

// --- mini-jeux ---


int minigame1(const maze_t maze)
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) //initilisation de la SDL avec l'image et les events (comprends des malloc)
    {
        const char *error = SDL_GetError();
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", error);
        SDL_Quit();
        return -1;
    }
    SDL_Window *fenetre = SDL_CreateWindow("easy mode", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, maze.width * 20, maze.height * 20, SDL_WINDOW_SHOWN); //creation d'une fenetre
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
    SDL_RenderDrawLine(renderer, 0, 0, 19, 0); //on dessine le mur du haut
    SDL_RenderDrawLine(renderer, 0, 0, 0, 19); //on dessine le mur de gauche
    SDL_RenderDrawLine(renderer, 7, 10, 13, 10); //on dessine le joueur
    SDL_RenderDrawLine(renderer, 10, 7, 10, 13); //on dessine le joueur
    if(has_wall_right(maze, 0, 0))
    {
        SDL_RenderDrawLine(renderer, 19, 0, 19, 19); //on dessine le mur de droite
        SDL_RenderDrawLine(renderer, 20, 0, 20, 19); //on dessine le mur de droite
    }
    if(has_wall_down(maze, 0, 0))
    {
        SDL_RenderDrawLine(renderer, 0, 19, 19, 19); //on dessine le mur du bas
        SDL_RenderDrawLine(renderer, 0, 20, 19, 20); //on dessine le mur du bas
    }
    SDL_SetRenderDrawColor(renderer, 10, 235, 10, 255); //on choisit la couleur du renderer
    SDL_RenderDrawLine(renderer, (maze.width-1) * 20, ((maze.height-1) * 20) + 19, ((maze.width-1) * 20) + 19, ((maze.height-1) * 20) + 19); //on dessine la sortie
    SDL_RenderDrawLine(renderer, (maze.width-1) * 20 + 19, (maze.height-1) * 20, (maze.width-1) * 20 + 19, (maze.height-1) * 20 + 19); //on dessine la sortie
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //on remet la couleur du renderer en blanc
    SDL_Delay(10); //pause pour éviter que certains anacronismes ne se produisent
    SDL_RenderPresent(renderer); //on met à jour l'affichage

    int pos_x = 0;
    int pos_y = 0;
    bool is_gaming = true;
    SDL_Event event;
    while(is_gaming)
    {
        SDL_WaitEvent(&event); //attente d'un event
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
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
            if(event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN)
            {
                is_gaming = false;
                SDL_Delay(1);
                print_maze(maze);
            }
            SDL_Delay(1);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); //on définit la couleur en gomme
            SDL_RenderDrawLine(renderer, pos_x * 20 + 7, pos_y * 20 + 10, pos_x * 20 + 13, pos_y * 20 + 10); //on efface le joueur
            SDL_RenderDrawLine(renderer, pos_x * 20 + 10, pos_y * 20 + 7, pos_x * 20 + 10, pos_y * 20 + 13); //on efface le joueur
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //on choisit la couleur du renderer
            SDL_Delay(10);
            if(event.key.keysym.sym == SDLK_z || event.key.keysym.sym == SDLK_UP)
            {
                if(!has_wall_up(maze, pos_x, pos_y))
                {
                    pos_y--;
                }
            }
            else if(event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_DOWN)
            {
                if(!has_wall_down(maze, pos_x, pos_y))
                {
                    pos_y++;
                }
            }
            else if(event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_LEFT)
            {
                if(!has_wall_left(maze, pos_x, pos_y))
                {
                    pos_x--;
                }
            }
            else if(event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT)
            {
                if(!has_wall_right(maze, pos_x, pos_y))
                {
                    pos_x++;
                }
            }
            if(has_wall_up(maze, pos_x, pos_y))
            {
                SDL_RenderDrawLine(renderer, pos_x * 20, pos_y * 20, pos_x * 20 + 19, pos_y * 20);
                SDL_RenderDrawLine(renderer, pos_x * 20, pos_y * 20 -1, pos_x * 20 + 19, pos_y * 20 -1);
            }
            if(has_wall_down(maze, pos_x, pos_y))
            {
                SDL_RenderDrawLine(renderer, pos_x * 20, (pos_y * 20) + 19, (pos_x * 20) + 19, (pos_y * 20) + 19);
                SDL_RenderDrawLine(renderer, pos_x * 20, (pos_y * 20) + 20, (pos_x * 20) + 19, (pos_y * 20) + 20);
            }
            if(has_wall_left(maze, pos_x, pos_y))
            {
                SDL_RenderDrawLine(renderer, pos_x * 20, pos_y * 20, pos_x * 20, (pos_y * 20) + 19);
                SDL_RenderDrawLine(renderer, pos_x * 20 - 1, pos_y * 20, pos_x * 20 - 1, (pos_y * 20) + 19);
            }

            if(has_wall_right(maze, pos_x, pos_y))
            {
                SDL_RenderDrawLine(renderer, (pos_x * 20) + 19, pos_y * 20, (pos_x * 20) + 19, (pos_y * 20) + 19);
                SDL_RenderDrawLine(renderer, (pos_x * 20) + 20, pos_y * 20, (pos_x * 20) + 20, (pos_y * 20) + 19);
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
            print_maze(maze);
        }
    }
    SDL_DestroyRenderer(renderer); //destruction du renderer (desallocation de la memoire)
    SDL_DestroyWindow(fenetre); //destruction de la fenetre (desallocation de la memoire)
    SDL_Quit(); //desalocation de la memoire
    return 0;
}

int rminigame1(const int width, const int height)
{
    srand(time(NULL));
    const int r = rand() % NB_GENERATEURS;
    if(r == 0)
    {
        const maze_t maze = line_maze(width, height);
        const int s = minigame1(maze);
        free_maze(maze);
        return s;
    }
    else if(r == 1)
    {
        const maze_t maze = column_maze(width, height);
        const int s = minigame1(maze);
        free_maze(maze);
        return s;
    }
    else if(r == 2)
    {
        const maze_t maze = imperfect_one_way_maze(width, height);
        const int s = minigame1(maze);
        free_maze(maze);
        return s;
    }
    else if(r == 3)
    {
        const maze_t maze = perfect_one_way_maze(width, height);
        const int s = minigame1(maze);
        free_maze(maze);
        return s;
    }
    else if(r ==4)
    {
        const maze_t maze = hunt_kill_maze(width, height);
        const int s = minigame1(maze);
        free_maze(maze);
        return s;
    }
    else if(r == 5)
    {
        const maze_t maze = by_path_maze(width, height);
        const int s = minigame1(maze);
        free_maze(maze);
        return s;
    }
    fprintf(stderr, "Erreur de gestion de l'aléatoire\n");
    return -1;
}

int minigame2(const maze_t maze)
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) //initilisation de la SDL avec l'image et les events (comprends des malloc)
    {
        const char *error = SDL_GetError();
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", error);
        SDL_Quit();
        return -1;
    }
    SDL_Window *fenetre = SDL_CreateWindow("semi-blind mode", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, maze.width * 20, maze.height * 20, SDL_WINDOW_SHOWN); //creation d'une fenetre
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
    SDL_RenderDrawLine(renderer, 0, 0, 19, 0); //on dessine le mur du haut
    SDL_RenderDrawLine(renderer, 0, 0, 0, 19); //on dessine le mur de gauche
    SDL_RenderDrawLine(renderer, 7, 10, 13, 10); //on dessine le joueur
    SDL_RenderDrawLine(renderer, 10, 7, 10, 13); //on dessine le joueur
    if(has_wall_right(maze, 0, 0))
    {
        SDL_RenderDrawLine(renderer, 19, 0, 19, 19); //on dessine le mur de droite
    }
    if(has_wall_down(maze, 0, 0))
    {
        SDL_RenderDrawLine(renderer, 0, 19, 19, 19); //on dessine le mur du bas
    }
    SDL_SetRenderDrawColor(renderer, 10, 235, 10, 255); //on choisit la couleur du renderer
    SDL_RenderDrawLine(renderer, (maze.width-1) * 20, ((maze.height-1) * 20) + 19, ((maze.width-1) * 20) + 19, ((maze.height-1) * 20) + 19); //on dessine la sortie
    SDL_RenderDrawLine(renderer, (maze.width-1) * 20 + 19, (maze.height-1) * 20, (maze.width-1) * 20 + 19, (maze.height-1) * 20 + 19); //on dessine la sortie
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //on remet la couleur du renderer en blanc
    SDL_Delay(10); //pause pour éviter que certains anacronismes ne se produisent
    SDL_RenderPresent(renderer); //on met à jour l'affichage

    int pos_x = 0;
    int pos_y = 0;
    bool is_gaming = true;
    SDL_Event event;
    while(is_gaming)
    {
        SDL_WaitEvent(&event); //attente d'un event
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
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
            if(event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN)
            {
                is_gaming = false;
                SDL_Delay(1);
                print_maze(maze);
            }
            SDL_Delay(1);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); //on définit la couleur en gomme
            SDL_RenderDrawLine(renderer, pos_x * 20 + 7, pos_y * 20 + 10, pos_x * 20 + 13, pos_y * 20 + 10); //on efface le joueur
            SDL_RenderDrawLine(renderer, pos_x * 20 + 10, pos_y * 20 + 7, pos_x * 20 + 10, pos_y * 20 + 13); //on efface le joueur
            if(has_wall_up(maze, pos_x, pos_y))
            {
                SDL_RenderDrawLine(renderer, pos_x * 20, pos_y * 20, pos_x * 20 + 19, pos_y * 20); //on efface le mur du haut
            }
            if(has_wall_down(maze, pos_x, pos_y))
            {
                SDL_RenderDrawLine(renderer, pos_x * 20, (pos_y * 20) + 19, (pos_x * 20) + 19, (pos_y * 20) + 19); //on efface le mur du bas
            }
            if(has_wall_left(maze, pos_x, pos_y))
            {
                SDL_RenderDrawLine(renderer, pos_x * 20, pos_y * 20, pos_x * 20, (pos_y * 20) + 19); //on efface le mur de gauche
            }

            if(has_wall_right(maze, pos_x, pos_y))
            {
                SDL_RenderDrawLine(renderer, (pos_x * 20) + 19, pos_y * 20, (pos_x * 20) + 19, (pos_y * 20) + 19); //on efface le mur de droite
            }
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //on choisit la couleur du renderer
            SDL_Delay(10);
            if(event.key.keysym.sym == SDLK_z || event.key.keysym.sym == SDLK_UP)
            {
                if(!has_wall_up(maze, pos_x, pos_y))
                {
                    pos_y--;
                }
            }
            else if(event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_DOWN)
            {
                if(!has_wall_down(maze, pos_x, pos_y))
                {
                    pos_y++;
                }
            }
            else if(event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_LEFT)
            {
                if(!has_wall_left(maze, pos_x, pos_y))
                {
                    pos_x--;
                }
            }
            else if(event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT)
            {
                if(!has_wall_right(maze, pos_x, pos_y))
                {
                    pos_x++;
                }
            }
            if(has_wall_up(maze, pos_x, pos_y))
            {
                SDL_RenderDrawLine(renderer, pos_x * 20, pos_y * 20, pos_x * 20 + 19, pos_y * 20); //on dessine le mur du haut
            }
            if(has_wall_down(maze, pos_x, pos_y))
            {
                SDL_RenderDrawLine(renderer, pos_x * 20, (pos_y * 20) + 19, (pos_x * 20) + 19, (pos_y * 20) + 19); //on dessine le mur du bas
            }
            if(has_wall_left(maze, pos_x, pos_y))
            {
                SDL_RenderDrawLine(renderer, pos_x * 20, pos_y * 20, pos_x * 20, (pos_y * 20) + 19); //on dessine le mur de gauche
            }

            if(has_wall_right(maze, pos_x, pos_y))
            {
                SDL_RenderDrawLine(renderer, (pos_x * 20) + 19, pos_y * 20, (pos_x * 20) + 19, (pos_y * 20) + 19); //on dessine le mur de droite
            }
            SDL_RenderDrawLine(renderer, pos_x * 20 + 7, pos_y * 20 + 10, pos_x * 20 + 13, pos_y * 20 + 10); //on dessine le joueur
            SDL_RenderDrawLine(renderer, pos_x * 20 + 10, pos_y * 20 + 7, pos_x * 20 + 10, pos_y * 20 + 13); //on dessine le joueur
            SDL_Delay(250); //pause de 0.25 secondes
            SDL_RenderPresent(renderer); //on met à jour l'affichage
        }
        if(pos_x == maze.width - 1 && pos_y == maze.height - 1)
        {
            is_gaming = false; //on arrête le jeu
            SDL_Delay(100); //pause pour assurer le bon affichage
            print_maze(maze); //on affiche le labyrinthe en entier
        }
    }
    SDL_DestroyRenderer(renderer); //destruction du renderer (desallocation de la memoire)
    SDL_DestroyWindow(fenetre); //destruction de la fenetre (desallocation de la memoire)
    SDL_Quit(); //desalocation de la memoire
    return 0;
}

int rminigame2(const int width, const int height)
{
    srand(time(NULL));
    const int r = rand() % NB_GENERATEURS;
    if(r == 0)
    {
        const maze_t maze = line_maze(width, height);
        const int s = minigame2(maze);
        free_maze(maze);
        return s;
    }
    else if(r == 1)
    {
        const maze_t maze = column_maze(width, height);
        const int s = minigame2(maze);
        free_maze(maze);
        return s;
    }
    else if(r == 2)
    {
        const maze_t maze = imperfect_one_way_maze(width, height);
        const int s = minigame2(maze);
        free_maze(maze);
        return s;
    }
    else if(r == 3)
    {
        const maze_t maze = perfect_one_way_maze(width, height);
        const int s = minigame2(maze);
        free_maze(maze);
        return s;
    }
    else if(r ==4)
    {
        const maze_t maze = hunt_kill_maze(width, height);
        const int s = minigame2(maze);
        free_maze(maze);
        return s;
    }
    else if(r == 5)
    {
        const maze_t maze = by_path_maze(width, height);
        const int s = minigame2(maze);
        free_maze(maze);
        return s;
    }
    fprintf(stderr, "Erreur de gestion de l'aléatoire\n");
    return -1;
}

int minigame3(const maze_t maze)
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) //initilisation de la SDL avec l'image et les events (comprends des malloc)
    {
        const char *error = SDL_GetError();
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", error);
        SDL_Quit();
        return -1;
    }
    SDL_Window *fenetre = SDL_CreateWindow("full-blind mode", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, maze.width * 20, maze.height * 20, SDL_WINDOW_SHOWN); //creation d'une fenetre
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
    SDL_RenderDrawLine(renderer, 7, 10, 13, 10); //on dessine le joueur
    SDL_RenderDrawLine(renderer, 10, 7, 10, 13); //on dessine le joueur
    SDL_SetRenderDrawColor(renderer, 10, 235, 10, 255); //on choisit la couleur du renderer
    SDL_RenderDrawLine(renderer, (maze.width-1) * 20, ((maze.height-1) * 20) + 19, ((maze.width-1) * 20) + 19, ((maze.height-1) * 20) + 19); //on dessine la sortie
    SDL_RenderDrawLine(renderer, (maze.width-1) * 20 + 19, (maze.height-1) * 20, (maze.width-1) * 20 + 19, (maze.height-1) * 20 + 19); //on dessine la sortie
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //on remet la couleur du renderer en blanc
    SDL_Delay(10); //pause pour éviter que certains anachronismes ne se produisent
    SDL_RenderPresent(renderer); //on met à jour l'affichage

    int pos_x = 0;
    int pos_y = 0;
    bool is_gaming = true;
    SDL_Event event;
    while(is_gaming)
    {
        SDL_WaitEvent(&event); //attente d'un event
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
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
            if(event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN)
            {
                is_gaming = false;
                SDL_Delay(1);
                print_maze(maze);
            }
            SDL_Delay(1);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); //on définit la couleur en gomme
            SDL_RenderDrawLine(renderer, pos_x * 20 + 7, pos_y * 20 + 10, pos_x * 20 + 13, pos_y * 20 + 10); //on efface le joueur
            SDL_RenderDrawLine(renderer, pos_x * 20 + 10, pos_y * 20 + 7, pos_x * 20 + 10, pos_y * 20 + 13); //on efface le joueur
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //on choisit la couleur du renderer
            SDL_Delay(10);
            if(event.key.keysym.sym == SDLK_z || event.key.keysym.sym == SDLK_UP)
            {
                if(!has_wall_up(maze, pos_x, pos_y))
                {
                    pos_y--;
                }
            }
            else if(event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_DOWN)
            {
                if(!has_wall_down(maze, pos_x, pos_y))
                {
                    pos_y++;
                }
            }
            else if(event.key.keysym.sym == SDLK_q || event.key.keysym.sym == SDLK_LEFT)
            {
                if(!has_wall_left(maze, pos_x, pos_y))
                {
                    pos_x--;
                }
            }
            else if(event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT)
            {
                if(!has_wall_right(maze, pos_x, pos_y))
                {
                    pos_x++;
                }
            }
            SDL_RenderDrawLine(renderer, pos_x * 20 + 7, pos_y * 20 + 10, pos_x * 20 + 13, pos_y * 20 + 10); //on dessine le joueur
            SDL_RenderDrawLine(renderer, pos_x * 20 + 10, pos_y * 20 + 7, pos_x * 20 + 10, pos_y * 20 + 13); //on dessine le joueur
            SDL_Delay(250); //pause de 0.25 secondes
            SDL_RenderPresent(renderer); //on met à jour l'affichage
        }
        if(pos_x == maze.width - 1 && pos_y == maze.height - 1) //si le joueur est à la sortie
        {
            is_gaming = false; //on arrête le jeu
            SDL_Delay(10); //pause pour assurer le bon affichage
            print_maze(maze); //on affiche le labyrinthe en entier
        }
    }
    SDL_DestroyRenderer(renderer); //destruction du renderer (desallocation de la memoire)
    SDL_DestroyWindow(fenetre); //destruction de la fenetre (desallocation de la memoire)
    SDL_Quit(); //desalocation de la memoire
    return 0;
}

int rminigame3(const int width, const int height)
{
    srand(time(NULL));
    const int r = rand() % NB_GENERATEURS;
    if(r == 0)
    {
        const maze_t maze = line_maze(width, height);
        const int s = minigame3(maze);
        free_maze(maze);
        return s;
    }
    else if(r == 1)
    {
        const maze_t maze = column_maze(width, height);
        const int s = minigame3(maze);
        free_maze(maze);
        return s;
    }
    else if(r == 2)
    {
        const maze_t maze = imperfect_one_way_maze(width, height);
        const int s = minigame3(maze);
        free_maze(maze);
        return s;
    }
    else if(r == 3)
    {
        const maze_t maze = perfect_one_way_maze(width, height);
        const int s = minigame3(maze);
        free_maze(maze);
        return s;
    }
    else if(r == 4)
    {
        const maze_t maze = hunt_kill_maze(width, height);
        const int s = minigame3(maze);
        free_maze(maze);
        return s;
    }
    else if(r == 5)
    {
        const maze_t maze = by_path_maze(width, height);
        const int s = minigame3(maze);
        free_maze(maze);
        return s;
    }
    fprintf(stderr, "Erreur de gestion de l'aléatoire\n");
    return -1;
}

int minigame4(const maze_t maze)
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) //initilisation de la SDL avec l'image et les events (comprends des malloc)
    {
        const char *error = SDL_GetError();
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", error);
        SDL_Quit();
        return -1;
    }
    SDL_Window *fenetre = SDL_CreateWindow("full-blind mode", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 40, 40, SDL_WINDOW_SHOWN); //creation d'une fenetre
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
    SDL_RenderDrawLine(renderer, 7, 20, 33, 20); //on dessine le joueur
    SDL_RenderDrawLine(renderer, 20, 7, 20, 33); //on dessine le joueur
    int x = 0, y = 0;
    while(x != maze.width - 1 || y != maze.height - 1)
    {
        SDL_Event event;
        if(has_wall_up(maze, x, y))
        {
            SDL_RenderDrawLine(renderer, 0, 0, 39, 0); //on dessine le mur du haut
        }
        if(has_wall_down(maze, x, y))
        {
            SDL_RenderDrawLine(renderer, 0, 39, 39, 39); //on dessine le mur du bas
        }
        if(has_wall_left(maze, x, y))
        {
            SDL_RenderDrawLine(renderer, 0, 0, 0, 39); //on dessine le mur de gauche
        }
        if(has_wall_right(maze, x, y))
        {
            SDL_RenderDrawLine(renderer, 39, 0, 39, 39); //on dessine le mur de droite
        }
        SDL_Delay(1);
        SDL_RenderPresent(renderer); //on met à jour l'affichage
        SDL_WaitEvent(&event);
        if(event.type == SDLK_RETURN || event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
        {
            fprintf(stderr, "L'utilisateur a quitté le jeu\n");
            SDL_DestroyRenderer(renderer); //destruction du renderer (desallocation de la memoire)
            SDL_DestroyWindow(fenetre); //destruction de la fenetre (desallocation de la memoire)
            SDL_Quit(); //desalocation de la memoire
            return 1;
        }
        if(event.type == SDL_KEYUP)
        {
            if(event.key.keysym.sym == SDLK_UP && !has_wall_up(maze, x, y))
            {
                y--;
            }
            else if(event.key.keysym.sym == SDLK_DOWN && !has_wall_down(maze, x, y))
            {
                y++;
            }
            else if(event.key.keysym.sym == SDLK_LEFT && !has_wall_left(maze, x, y))
            {
                x--;
            }
            else if(event.key.keysym.sym == SDLK_RIGHT && !has_wall_right(maze, x, y))
            {
                x++;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); //on définit la couleur en gomme
        SDL_RenderDrawLine(renderer, 0, 0, 39, 0); //on efface le mur du haut
        SDL_RenderDrawLine(renderer, 0, 39, 39, 39); //on efface le mur du bas
        SDL_RenderDrawLine(renderer, 0, 0, 0, 39); //on efface le mur de gauche
        SDL_RenderDrawLine(renderer, 39, 0, 39, 39); //on efface le mur de droite
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //on choisit la couleur du renderer
    }
    SDL_Delay(100);
    print_maze(maze);
    SDL_DestroyRenderer(renderer); //destruction du renderer (desallocation de la memoire)
    SDL_DestroyWindow(fenetre); //destruction de la fenetre (desallocation de la memoire)
    SDL_Quit(); //desalocation de la memoire
    return 1;
}

int rminigame4(const int width, const int height)
{
    srand(time(NULL));
    const int r = rand() % NB_GENERATEURS;
    if(r == 0)
    {
        const maze_t maze = line_maze(width, height);
        const int s = minigame4(maze);
        free_maze(maze);
        return s;
    }
    if(r == 1)
    {
        const maze_t maze = column_maze(width, height);
        const int s = minigame4(maze);
        free_maze(maze);
        return s;
    }
    if(r == 2)
    {
        const maze_t maze = imperfect_one_way_maze(width, height);
        const int s = minigame4(maze);
        free_maze(maze);
        return s;
    }
    if(r == 3)
    {
        const maze_t maze = perfect_one_way_maze(width, height);
        const int s = minigame4(maze);
        free_maze(maze);
        return s;
    }
    if(r == 4)
    {
        const maze_t maze = hunt_kill_maze(width, height);
        const int s = minigame4(maze);
        free_maze(maze);
        return s;
    }
    if(r == 5)
    {
        const maze_t maze = by_path_maze(width, height);
        const int s = minigame4(maze);
        free_maze(maze);
        return s;
    }
    fprintf(stderr, "Erreur de gestion de l'aléatoire\n");
    return -1;
}

int rminigame(const int width, const int height)
{
    srand(time(NULL));
    const int r = rand() % 4;
    if(r == 0)
    {
        return rminigame1(width, height);
    }
    if(r == 1)
    {
        return rminigame2(width, height);
    }
    if (r == 2)
    {
        return rminigame3(width, height);
    }
    if(r == 3)
    {
        return rminigame4(width, height);
    }
    fprintf(stderr, "Erreur de gestion de l'aléatoire\n");
    return -1;
}

int r_minigame(const maze_t maze)
{
    srand(time(NULL));
    const int r = rand() % 3;
    if(r == 0)
    {
        return minigame1(maze);
    }
    else if(r == 1)
    {
        return minigame2(maze);
    }
    else if (r == 2)
    {
        return minigame3(maze);
    }
    fprintf(stderr, "Erreur de gestion de l'aléatoire\n");
    return -1;
}


// --- Solveur de labyrinthes ---

//fonction auxiliaire de right_hand
//renvoie true si on peut atteindre la sortie, false sinon
//maze : le labyrinthe
//visited : tableau de booléens pour savoir si on est déjà passé par une case
//x : abscisse de la case actuelle
//y : ordonnée de la case actuelle
bool has_exit_right_hand_aux(const maze_t maze, const bool_tab visited, const int x, const int y)
{
    //condition d'arrêt : atteindre la sortie
    if(x == maze.width - 1 && y == maze.height - 1)
    {
        return true;
    }

    //boucle : on part dans toutes les directions possibles et on regarde si on peut atteindre la sortie
    set_true(visited, x, y);
    if(!has_wall_up(maze, x, y) && !get_bool(visited, x, y-1))
    {
        if(has_exit_right_hand_aux(maze, visited, x, y - 1))
        {
            return true;
        }
    }
    if(!has_wall_down(maze, x, y) && !get_bool(visited, x, y + 1))
    {
        if(has_exit_right_hand_aux(maze, visited, x, y + 1))
        {
            return true;
        }
    }
    if(!has_wall_left(maze, x, y) && !get_bool(visited, x - 1, y))
    {
        if(has_exit_right_hand_aux(maze, visited, x - 1, y))
        {
            return true;
        }
    }
    if(!has_wall_right(maze, x, y) && !get_bool(visited, x + 1, y))
    {
        if(has_exit_right_hand_aux(maze, visited, x + 1, y))
        {
            return true;
        }
    }
    //condition d'arrêt : on ne peut plus avancer
    return false;
}

bool has_exit_right_hand(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width, maze.height);
    const bool s = has_exit_right_hand_aux(maze, visited, 0, 0);
    free_booltab(visited);
    return s;
}

//fonction auxiliaire de is_perfect_right_hand
//renvoie false si le labyrinth est évidemment non parfait, true sinon
//maze : le labyrinthe
//visited : tableau de booléens pour savoir si on est déjà passé par une case
//x : abscisse de la case actuelle
//y : ordonnée de la case actuelle
//p_x : abscisse de la case précédente
//p_y : ordonnée de la case précédente
bool is_perfect_right_hand_aux(const maze_t maze, const bool_tab visited, const int x, const int y, const int p_x,const int p_y)
{
    set_true(visited, x, y);
    if(!has_wall_up(maze, x, y) && (p_y != y - 1 || p_x != x))
    {
        if(get_bool(visited, x, y - 1) || !is_perfect_right_hand_aux(maze, visited, x, y - 1, x, y))
        {
            return false;
        }
    }
    if(!has_wall_down(maze, x, y) && (p_y != y + 1 || p_x != x))
    {
        if(get_bool(visited, x, y + 1) || !is_perfect_right_hand_aux(maze, visited, x, y + 1, x, y))
        {
            return false;
        }
    }
    if(!has_wall_left(maze, x, y) && (p_x != x - 1 || p_y != y))
    {
        if(get_bool(visited, x - 1, y) || !is_perfect_right_hand_aux(maze, visited, x - 1, y, x, y))
        {
            return false;
        }
    }
    if(!has_wall_right(maze, x, y) && (p_x != x + 1 || p_y != y))
    {
        if(get_bool(visited, x + 1, y) || !is_perfect_right_hand_aux(maze, visited, x + 1, y, x, y))
        {
            return false;
        }
    }
    return true;
}

bool is_perfect_right_hand(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width, maze.height);
    if(!is_perfect_right_hand_aux(maze, visited, 0, 0, 0, 0))
    {
        free_booltab(visited);
        return false;
    }
    for(int i = 0; i < maze.height; i++)
    {
        for(int j = 0; j < maze.width; j++)
        {
            if(!get_bool(visited, j, i))
            {
                free_booltab(visited);
                return false;
            }
        }
    }
    free_booltab(visited);
    return true;
}

//fonction auxiliaire de shortest_exit_right_hand
//renvoie le chemin le plus court pour atteindre la sortie
//maze : le labyrinthe
//visited : tableau de booléens pour savoir si on est déjà passé par une case
//x : abscisse de la case actuelle
//y : ordonnée de la case actuelle
//wayt : tableau de chemins
void shortest_exit_right_hand_aux(const maze_t maze, const int x, const int y, const waytab wayt)
{
    if(x == maze.width - 1 && y == maze.height - 1)
    {
        return;
    }
    const int l = length_waytab(wayt, x, y);
    if(l > length_waytab(wayt, maze.width - 1, maze.height - 1))
    {
        return;
    }
    if(!has_wall_up(maze, x, y) && length_waytab(wayt, x, y - 1) > l + 1)
    {
        connected_way(wayt, x, y - 1, x, y);
        shortest_exit_right_hand_aux(maze, x, y - 1, wayt);
    }
    if(!has_wall_down(maze, x, y) && length_waytab(wayt, x, y + 1) > l + 1)
    {
        connected_way(wayt, x, y + 1, x, y);
        shortest_exit_right_hand_aux(maze, x, y + 1, wayt);
    }
    if(!has_wall_left(maze, x, y) && length_waytab(wayt, x - 1, y) > l + 1)
    {
        connected_way(wayt, x - 1, y, x, y);
        shortest_exit_right_hand_aux(maze, x - 1, y, wayt);
    }
    if(!has_wall_right(maze, x, y) && length_waytab(wayt, x + 1, y) > l + 1)
    {
        connected_way(wayt, x + 1, y, x, y);
        shortest_exit_right_hand_aux(maze, x + 1, y, wayt);
    }
}


way *shortest_exit_right_hand(const maze_t maze)
{
    const waytab wayt = create_waytab(maze.width, maze.height);
    shortest_exit_right_hand_aux(maze, 0, 0, wayt);
    way *w = copy_way(get_way(wayt, maze.width - 1, maze.height - 1));
    free_waytab(wayt);
    return w;
}

// --- Visualisation des algorithmes de résolution de labyrinthes ---


//fonction auxiliaire de show_has_exit_right_hand
//colorie la case de coordonnées x et y
//renderer : le renderer
//x : abscisse de la case
//y : ordonnée de la case
void color_case(SDL_Renderer *renderer, const maze_t maze, const int x, const int y, const bool refresh)
{
    for(int i = 1; i < 20; i++)
    {
        SDL_RenderDrawLine(renderer, (x * 20) + 1, (y * 20) + i, (x * 20) + 19, (y * 20) + i);
    }
    if(!has_wall_down(maze, x, y))
    {
        SDL_RenderDrawLine(renderer, (x * 20) + 1, (y * 20) + 19, (x * 20) + 19, (y * 20) + 19);
    }
    if(!has_wall_up(maze, x, y))
    {
        SDL_RenderDrawLine(renderer, (x * 20) + 1, (y * 20), (x * 20) + 19, (y * 20));
    }
    if(!has_wall_left(maze, x, y))
    {
        SDL_RenderDrawLine(renderer, (x * 20), (y * 20) + 1, (x * 20), (y * 20) + 19);
    }
    if(!has_wall_right(maze, x, y))
    {
        SDL_RenderDrawLine(renderer, (x * 20) + 19, (y * 20) + 1, (x * 20) + 19, (y * 20) + 19);
    }
    if(refresh)
    {
        SDL_Delay(1); //pour éviter certains anachronismes
        SDL_RenderPresent(renderer); //on met à jour l'affichage
        SDL_Delay(DELAY); //pause de 0.01 secondes
    }
}

//fonction auxiliaire de show_has_exit_right_hand
//affiche le labyrinthe et la progression du solveur
//renvoie true si on peut atteindre la sortie, false sinon
//maze : le labyrinthe
//visited : tableau de booléens pour savoir si on est déjà passé par une case
//x : abscisse de la case actuelle
//y : ordonnée de la case actuelle
//renderer : le renderer
bool show_has_exit_right_hand_aux(const maze_t maze, const bool_tab visited, const int x, const int y, SDL_Renderer *renderer, bool fast)
{
    SDL_Event event;
    SDL_WaitEventTimeout(&event, DELAY); //attente d'un event
    if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
    {
        //l'utilisateur ferme la fenetre ou clique sur la croix
        fprintf(stderr, "L'utilisateur a manuellement réclamé l'arrêt du programme\n");
        exit(EXIT_SUCCESS);
    }
    else if(event.type == SDL_WINDOWEVENT_MINIMIZED)
    {
        while(event.type != SDL_WINDOWEVENT_RESTORED)
        {
            SDL_WaitEvent(&event);
        }
    }

    //première condition d'arrêt : atteindre la sortie
    if(x == maze.width - 1 && y == maze.height - 1)
    {
        SDL_SetRenderDrawColor(renderer, 10, 235, 10, 255); //on définit la couleur en vert
        color_case(renderer, maze, x, y, true);
        return true;
    }

    color_case(renderer, maze, x, y, !fast);
    set_true(visited, x, y);
    bool s = false;

    //boucle : on part dans toutes les directions possibles et on regarde si on peut atteindre la sortie
    if(!has_wall_up(maze, x, y) && !get_bool(visited, x, y - 1))
    {
         s = show_has_exit_right_hand_aux(maze, visited, x, y - 1, renderer, fast);
    }
    if(!has_wall_down(maze, x, y) && !get_bool(visited, x, y + 1))
    {
        s = s || show_has_exit_right_hand_aux(maze, visited, x, y + 1, renderer, fast);
    }
    if(!has_wall_left(maze, x, y) && !get_bool(visited, x - 1, y))
    {
        s = s || show_has_exit_right_hand_aux(maze, visited, x - 1, y, renderer, fast);
    }
    if(!has_wall_right(maze, x, y) && !get_bool(visited, x + 1, y))
    {
        s = s || show_has_exit_right_hand_aux(maze, visited, x + 1, y, renderer, fast);
    }
    //deuxième condition d'arrêt : on est bloqué
    if(s)
    {
        SDL_SetRenderDrawColor(renderer, 10, 235, 10, 255); //on définit la couleur en vert
        color_case(renderer, maze, x, y, fast);
        SDL_SetRenderDrawColor(renderer, 0, 55, 155, 255); //on définit la couleur en bleu
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //on définit la couleur en rouge
        color_case(renderer, maze, x, y, fast);
        SDL_SetRenderDrawColor(renderer, 0, 55, 155, 255); //on définit la couleur en bleu
    }
    return s;
}

//fonction de visualisation
//affiche le labyrinthe et la progression du solveur
//renvoie -1 en cas d'erreur, 1 sinon
//maze : le labyrinthe
//fast : true si on veut aller vite, false si on veut avoir un affichage fluide
int true_show_has_exit_right_hand(const maze_t maze, const bool fast)
{
    //on commence par afficher le labyrinthe
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) //initilisation de la SDL avec l'image et les events (comprends des malloc)
    {
        const char *error = SDL_GetError();
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", error);
        SDL_Quit();
        return -1;
    }
    SDL_Window *fenetre = SDL_CreateWindow("has_exit_right_hand", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, maze.width * 20, maze.height * 20, SDL_WINDOW_SHOWN); //creation d'une fenetre
    if(fenetre == NULL)
    {
        const char *error = SDL_GetError();
        fprintf(stderr, "Erreur de creation de la fenetre : %s\n", error);
        SDL_DestroyWindow(fenetre);
        SDL_Quit();
        return -1;
    }
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
            if(has_wall_up(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * 20, y * 20, (x * 20) + 19, y * 20); //on dessine un mur en haut
            }
            if(has_wall_down(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * 20, (y * 20) + 19, (x * 20) + 19, (y * 20) + 19); //on dessine un mur en bas
            }
            if(has_wall_left(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * 20, y * 20, x * 20, (y * 20) + 19); //on dessine un mur à gauche
            }
            if(has_wall_right(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, (x * 20) + 19, y * 20, (x * 20) + 19, (y * 20) + 19); //on dessine un mur à droite
            }
        }
    }

    SDL_RenderPresent(renderer); //on met à jour l'affichage
    SDL_Delay(1); //pause de 0.001 secondes
    SDL_SetRenderDrawColor(renderer, 0, 50, 255, 255); //on définit la couleur en bleu
    SDL_RenderDrawLine(renderer, 0, 0, 0, 20); //l'entrée en vert
    SDL_RenderDrawLine(renderer, 0, 0, 20, 0); //l'entrée en vert

    SDL_RenderPresent(renderer); //on met à jour l'affichage
    SDL_Delay(1); //pause de 0.001 secondes
    SDL_SetRenderDrawColor(renderer, 10, 235, 10, 255); //on définit la couleur en vert
    SDL_RenderDrawLine(renderer, (maze.width * 20) - 1, (maze.height * 20) - 20, (maze.width * 20) - 1, (maze.height * 20)); //la sortie en bleu
    SDL_RenderDrawLine(renderer, (maze.width * 20) - 20, (maze.height * 20) - 1, (maze.width * 20), (maze.height * 20) - 1); //la sortie en bleu
    SDL_RenderPresent(renderer); //on met à jour l'affichage

    //on commence la recherche
    const bool_tab visited = create_booltab(maze.width, maze.height);
    SDL_SetRenderDrawColor(renderer, 0, 55, 155, 255); //on définit la couleur en bleu
    color_case(renderer, maze, 0, 0, fast);
    if(show_has_exit_right_hand_aux(maze, visited, 0, 0, renderer, fast))
    {
        SDL_SetWindowTitle(fenetre, "maze solvable");
    }
    else
    {
        SDL_SetWindowTitle(fenetre, "maze unsolvable");
    }

    //à la fin
    SDL_RenderPresent(renderer); //on met à jour l'affichage
    SDL_Delay(100); //pause de 0.1 secondes
    SDL_Event event = {0}; //on crée un event
    while(!(event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || \
        (event.type == SDL_KEYUP && (event.key.keysym.sym == SDLK_ESCAPE || \
            event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN)))) //tant que l'utilisateur n'a pas fermé la fenetre
    {
        SDL_WaitEvent(&event); //on attend un event
    }
    free_booltab(visited);
    SDL_DestroyRenderer(renderer); //destruction du renderer (desallocation de la memoire)
    SDL_DestroyWindow(fenetre); //destruction de la fenetre (desallocation de la memoire)
    SDL_Quit(); //desalocation de la memoire
    return 1;
}

int show_has_exit_right_hand(const maze_t maze)
{
    if(true_show_has_exit_right_hand(maze, false) == -1)
    {
        fprintf(stderr, "Erreur de visualisation\n");
        return -1;
    }
    return 1;
}

int show_fast_has_exit_right_hand(const maze_t maze)
{
    if(true_show_has_exit_right_hand(maze, true) == -1)
    {
        fprintf(stderr, "Erreur de visualisation\n");
        return -1;
    }
    return 1;
}

//fonction auxiliaire de show_is_perfect_right_hand
//affiche le labyrinthe et la progression du solveur
//renvoie true si le labyrinthe est parfait, false sinon
//maze : le labyrinthe
//visited : tableau de booléens pour savoir si on est déjà passé par une case
//x : abscisse de la case actuelle
//y : ordonnée de la case actuelle
//p_x : abscisse de la case précédente
//p_y : ordonnée de la case précédente
//renderer : le renderer
//delay : le delay de refresh (2 * delay + 1 ms/case)
bool show_is_perfect_right_hand_aux(const maze_t maze, const bool_tab visited, const int x, const int y, const int p_x, const int p_y, SDL_Renderer *renderer, const int delay)
{
    SDL_Event event;
    SDL_WaitEventTimeout(&event, delay); //attente d'un event
    if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
    {
        //l'utilisateur ferme la fenetre ou clique sur la croix
        fprintf(stderr, "L'utilisateur a manuellement réclamé l'arrêt du programme\n");
        exit(EXIT_SUCCESS);
    }
    else if(event.type == SDL_WINDOWEVENT_MINIMIZED)
    {
        while(event.type != SDL_WINDOWEVENT_RESTORED)
        {
            SDL_WaitEvent(&event);
        }
    }
    set_true(visited, x, y);
    color_case(renderer, maze, x, y, delay);
    if(!has_wall_up(maze, x, y) && (p_y != y - 1 || p_x != x))
    {
        if(get_bool(visited, x, y - 1) || !show_is_perfect_right_hand_aux(maze, visited, x, y - 1, x, y, renderer, delay))
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //on définit la couleur en rouge
            color_case(renderer, maze, x, y, delay);
            SDL_SetRenderDrawColor(renderer, 0, 55, 155, 255); //on définit la couleur en bleu
            return false;
        }
    }
    if(!has_wall_down(maze, x, y) && (p_y != y + 1 || p_x != x))
    {
        if(get_bool(visited, x, y + 1) || !show_is_perfect_right_hand_aux(maze, visited, x, y + 1, x, y, renderer, delay))
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //on définit la couleur en rouge
            color_case(renderer, maze, x, y, delay);
            SDL_SetRenderDrawColor(renderer, 0, 55, 155, 255); //on définit la couleur en bleu
            return false;
        }
    }
    if(!has_wall_left(maze, x, y) && (p_x != x - 1 || p_y != y))
    {
        if(get_bool(visited, x - 1, y) || !show_is_perfect_right_hand_aux(maze, visited, x - 1, y, x, y, renderer, delay))
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //on définit la couleur en rouge
            color_case(renderer, maze, x, y, delay);
            SDL_SetRenderDrawColor(renderer, 0, 55, 155, 255); //on définit la couleur en bleu
            return false;
        }
    }
    if(!has_wall_right(maze, x, y) && (p_x != x + 1 || p_y != y))
    {
        if(get_bool(visited, x + 1, y) || !show_is_perfect_right_hand_aux(maze, visited, x + 1, y, x, y, renderer, delay))
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //on définit la couleur en rouge
            color_case(renderer, maze, x, y, delay);
            SDL_SetRenderDrawColor(renderer, 0, 55, 155, 255); //on définit la couleur en bleu
            return false;
        }
    }
    SDL_SetRenderDrawColor(renderer, 10, 235, 10, 255); //on définit la couleur en vert
    color_case(renderer, maze, x, y, delay);
    SDL_SetRenderDrawColor(renderer, 0, 55, 155, 255); //on définit la couleur en bleu
    return true;
}

//fonction de visualisation
//affiche le labyrinthe et la progression du solveur
//renvoie -1 en cas d'erreur, 1 sinon
//maze : le labyrinthe
//fast : true si on veut aller vite, false pour avoir un affichage fluide
int true_show_is_perfect_right_hand(const maze_t maze, const bool fast)
{
    //on commence par afficher le labyrinthe
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) //initilisation de la SDL avec l'image et les events (comprends des malloc)
    {
        const char *error = SDL_GetError();
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", error);
        SDL_Quit();
        return -1;
    }
    SDL_Window *fenetre = SDL_CreateWindow("is_perfect_right_hand", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, maze.width * 20, maze.height * 20, SDL_WINDOW_SHOWN); //creation d'une fenetre
    if(fenetre == NULL)
    {
        const char *error = SDL_GetError();
        fprintf(stderr, "Erreur de creation de la fenetre : %s\n", error);
        SDL_DestroyWindow(fenetre);
        SDL_Quit();
        return -1;
    }
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
            if(has_wall_up(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * 20, y * 20, (x * 20) + 19, y * 20); //on dessine un mur en haut
            }
            if(has_wall_down(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * 20, (y * 20) + 19, (x * 20) + 19, (y * 20) + 19); //on dessine un mur en bas
            }
            if(has_wall_left(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * 20, y * 20, x * 20, (y * 20) + 19); //on dessine un mur à gauche
            }
            if(has_wall_right(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, (x * 20) + 19, y * 20, (x * 20) + 19, (y * 20) + 19); //on dessine un mur à droite
            }
        }
    }

    SDL_RenderPresent(renderer); //on met à jour l'affichage
    SDL_Delay(1); //pause de 0.001 secondes
    SDL_SetRenderDrawColor(renderer, 0, 50, 255, 255); //on définit la couleur en bleu
    SDL_RenderDrawLine(renderer, 0, 0, 0, 20); //l'entrée en vert
    SDL_RenderDrawLine(renderer, 0, 0, 20, 0); //l'entrée en vert

    SDL_RenderPresent(renderer); //on met à jour l'affichage
    SDL_Delay(1); //pause de 0.001 secondes
    SDL_SetRenderDrawColor(renderer, 10, 235, 10, 255); //on définit la couleur en vert
    SDL_RenderDrawLine(renderer, (maze.width * 20) - 1, (maze.height * 20) - 20, (maze.width * 20) - 1, (maze.height * 20)); //la sortie en bleu
    SDL_RenderDrawLine(renderer, (maze.width * 20) - 20, (maze.height * 20) - 1, (maze.width * 20), (maze.height * 20) - 1); //la sortie en bleu
    SDL_RenderPresent(renderer); //on met à jour l'affichage

    //on commence la recherche
    const bool_tab visited = create_booltab(maze.width, maze.height);
    SDL_SetRenderDrawColor(renderer, 0, 55, 155, 255); //on définit la couleur en bleu
    color_case(renderer, maze, 0, 0, fast);
    if(show_is_perfect_right_hand_aux(maze, visited, 0, 0, 0, 0, renderer, fast))
    {
        bool b = false;
        for(int i =0; i < maze.height; i++)
        {
            for(int j = 0; j < maze.width; j++)
            {
                if(!get_bool(visited, j, i))
                {
                    SDL_SetWindowTitle(fenetre, "maze imperfect");
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //on définit la couleur en rouge
                    color_case(renderer, maze, j, i, false);
                    b = true;
                }
            }
        }
        if(!b)
        {
            SDL_SetWindowTitle(fenetre, "maze perfect");
        }
    }
    else
    {
        SDL_SetWindowTitle(fenetre, "maze imperfect");
    }

    //à la fin
    SDL_Delay(1); //pause de 1 ms pour éviter certains anachronismes
    SDL_RenderPresent(renderer); //on met à jour l'affichage
    SDL_Event event = {0}; //on crée un event
    while(!(event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || \
        (event.type == SDL_KEYUP && (event.key.keysym.sym == SDLK_ESCAPE || \
            event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN)))) //tant que l'utilisateur n'a pas fermé la fenetre
    {
        SDL_WaitEvent(&event); //on attend un event
    }
    free_booltab(visited);
    SDL_DestroyRenderer(renderer); //destruction du renderer (desallocation de la memoire)
    SDL_DestroyWindow(fenetre); //destruction de la fenetre (desallocation de la memoire)
    SDL_Quit(); //desalocation de la memoire
    return 1;
}

int show_is_perfect_right_hand(const maze_t maze)
{
    if(true_show_is_perfect_right_hand(maze, false) == -1)
    {
        fprintf(stderr, "Erreur de visualisation\n");
        return -1;
    }
    return 1;
}

int show_fast_is_perfect_right_hand(const maze_t maze)
{
    if(true_show_is_perfect_right_hand(maze, true) == -1)
    {
        fprintf(stderr, "Erreur de visualisation\n");
        return -1;
    }
    return 1;
}

//fonction auxiliaire de show_the_way
//colorie la case du chemin
//renderer : le renderer
//maze : le labyrinthe
//w : le chemin
//fast : true si on veut aller vite, false pour avoir un affichage fluide
void color_way(SDL_Renderer *renderer, const maze_t maze, const way *w, const bool fast)
{
    SDL_Event event;
    SDL_WaitEventTimeout(&event, 5); //attente d'un event
    if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
    {
        //l'utilisateur ferme la fenetre ou clique sur la croix
        fprintf(stderr, "L'utilisateur a manuellement réclamé l'arrêt du programme\n");
        exit(EXIT_SUCCESS);
    }
    else if(event.type == SDL_WINDOWEVENT_MINIMIZED)
    {
        while(event.type != SDL_WINDOWEVENT_RESTORED)
        {
            SDL_WaitEvent(&event);
        }
    }
    const way *s = w;
    if(s == NULL)
    {
        return;
    }
    color_way(renderer, maze, s->dad, fast);
    color_case(renderer, maze, s->x, s->y, !fast);
}



int show_the_way(const maze_t maze, const way *w)
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) //initilisation de la SDL avec l'image et les events (comprends des malloc)
    {
        const char *error = SDL_GetError();
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", error);
        SDL_Quit();
        return -1;
    }
    SDL_Window *fenetre = SDL_CreateWindow("maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, maze.width * 20, maze.height * 20, SDL_WINDOW_SHOWN); //creation d'une fenetre
    if(fenetre == NULL)
    {
        const char *error = SDL_GetError();
        fprintf(stderr, "Erreur de creation de la fenetre : %s\n", error);
        SDL_DestroyWindow(fenetre);
        SDL_Quit();
        return -1;
    }
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
                SDL_RenderDrawLine(renderer, x * 20, y * 20, (x * 20) + 19, y * 20); //on dessine un mur en haut
            }
            if(maze.cells[y][x].wall_down)
            {
                SDL_RenderDrawLine(renderer, x * 20, (y * 20) + 19, (x * 20) + 19, (y * 20) + 19); //on dessine un mur en bas
            }
            if(maze.cells[y][x].wall_left)
            {
                SDL_RenderDrawLine(renderer, x * 20, y * 20, x * 20, (y * 20) + 19); //on dessine un mur à gauche
            }
            if(maze.cells[y][x].wall_right)
            {
                SDL_RenderDrawLine(renderer, (x * 20) + 19, y * 20, (x * 20) + 19, (y * 20) + 19); //on dessine un mur à droite
            }
        }
    }

    SDL_RenderPresent(renderer); //on met à jour l'affichage
    SDL_Delay(1); //pause de 0.001 secondes
    SDL_SetRenderDrawColor(renderer, 0, 50, 255, 255); //on définit la couleur en bleu
    SDL_RenderDrawLine(renderer, 0, 0, 0, 20); //l'entrée en vert
    SDL_RenderDrawLine(renderer, 0, 0, 20, 0); //l'entrée en vert

    SDL_RenderPresent(renderer); //on met à jour l'affichage
    SDL_Delay(1); //pause de 0.001 secondes
    SDL_SetRenderDrawColor(renderer, 10, 235, 10, 255); //on définit la couleur en vert
    SDL_RenderDrawLine(renderer, (maze.width * 20) - 1, (maze.height * 20) - 20, (maze.width * 20) - 1, (maze.height * 20)); //la sortie en bleu
    SDL_RenderDrawLine(renderer, (maze.width * 20) - 20, (maze.height * 20) - 1, (maze.width * 20), (maze.height * 20) - 1); //la sortie en bleu

    SDL_SetRenderDrawColor(renderer, 0, 55, 155, 255); //on définit la couleur en bleu
    const way* s = w;
    color_way(renderer, maze, s, false);

    SDL_RenderPresent(renderer); //on met à jour l'affichage
    SDL_Delay(100); //pause de 0.1 secondes
    SDL_Event event = {0}; //on crée un event
    while(!(event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || \
        (event.type == SDL_KEYUP && (event.key.keysym.sym == SDLK_ESCAPE || \
            event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN)))) //tant que l'utilisateur n'a pas fermé la fenetre
    {
        SDL_WaitEvent(&event); //on attend un event
    }
    SDL_DestroyRenderer(renderer); //destruction du renderer (desallocation de la memoire)
    SDL_DestroyWindow(fenetre); //destruction de la fenetre (desallocation de la memoire)
    SDL_Quit(); //desalocation de la memoire
    return 1;
}

//fonction auxiliaire de show_shortest_exit_right_hand
//affiche le labyrinthe et la progression du solveur
//renvoie true si on peut atteindre la sortie, false sinon
//maze : le labyrinthe
//renderer : le renderer
//visited : tableau de booléens pour savoir si on est déjà passé par une case
//x : abscisse de la case actuelle
//y : ordonnée de la case actuelle
//wayt : tableau de chemins
//fast : true si on veut aller vite, false pour avoir un affichage fluide
bool show_shortest_exit_right_hand_aux(const maze_t maze, SDL_Renderer *renderer, const int x, const int y, const waytab wayt, const bool fast)
{
    SDL_Event event;
    SDL_WaitEventTimeout(&event, 5); //attente d'un event
    if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE))
    {
        //l'utilisateur ferme la fenetre ou clique sur la croix
        fprintf(stderr, "L'utilisateur a manuellement réclamé l'arrêt du programme\n");
        exit(EXIT_SUCCESS);
    }
    else if(event.type == SDL_WINDOWEVENT_MINIMIZED)
    {
        while(event.type != SDL_WINDOWEVENT_RESTORED)
        {
            SDL_WaitEvent(&event);
        }
    }
    bool s = false;
    if(x == maze.width - 1 && y == maze.height - 1)//première condition d'arrêt : atteindre la sortie
    {
        SDL_SetRenderDrawColor(renderer, 10, 200, 10, 255); //on définit la couleur en vert
        color_case(renderer, maze, x, y, fast);
        return true;
    }
    const int l = length_waytab(wayt, x, y);
    if(!has_wall_up(maze, x, y) &&  length_waytab(wayt, x, y - 1) > l + 1) //si on peut aller en haut et que ça crée un chemin plus court que l'actuel
    {
        connected_way(wayt, x, y - 1, x, y); //on connecte les deux cases
        color_case(renderer, maze, x, y, !fast); //on colorie la case
        s = show_shortest_exit_right_hand_aux(maze, renderer, x, y - 1, wayt, fast); //on continue la recherche
    }
    if(!has_wall_down(maze, x, y) && length_waytab(wayt, x, y + 1) > l + 1)
    {
        connected_way(wayt, x, y + 1, x, y); //on connecte les deux cases
        color_case(renderer, maze, x, y, !fast);
        s = show_shortest_exit_right_hand_aux(maze, renderer, x, y + 1, wayt, fast) || s;
    }
    if(!has_wall_left(maze, x, y) && length_waytab(wayt, x - 1, y) > l + 1)
    {
        connected_way(wayt, x - 1, y, x, y);
        color_case(renderer, maze, x, y, !fast);
        s = show_shortest_exit_right_hand_aux(maze, renderer, x - 1, y, wayt, fast) || s;
    }
    if(!has_wall_right(maze, x, y) && length_waytab(wayt, x + 1, y) > l + 1)
    {
        connected_way(wayt, x + 1, y, x, y);
        color_case(renderer, maze, x, y, !fast);
        s = show_shortest_exit_right_hand_aux(maze, renderer, x + 1, y, wayt, fast) || s;
    }
    SDL_SetRenderDrawColor(renderer, 50, 0, 0, 255); //on définit la couleur en rouge
    color_case(renderer, maze, x, y, !fast);
    SDL_SetRenderDrawColor(renderer, 0, 55, 155, 255); //on définit la couleur en bleu
    if(s) //si on a un chemin correcte, on l'affiche en vert, sinon en rouge
    {
        SDL_SetRenderDrawColor(renderer, 10, 235, 200, 255); //on définit la couleur en cyan
        color_case(renderer, maze, x, y, fast);
        SDL_SetRenderDrawColor(renderer, 0, 55, 155, 255); //on définit la couleur en bleu
        return true;
    }
    return false;
}

//fonction de visualisation
//affiche le labyrinthe et la progression du solveur
//renvoie -1 en cas d'erreur, 1 sinon
//maze : le labyrinthe
//fast : true si on veut aller vite, false pour avoir un affichage fluide
int true_show_shortest_exit_right_hand(const maze_t maze, const bool fast)
{
    //on commence par afficher le labyrinthe
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) //initilisation de la SDL avec l'image et les events (comprends des malloc)
    {
        const char *error = SDL_GetError();
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", error);
        SDL_Quit();
        return -1;
    }
    SDL_Window *fenetre = SDL_CreateWindow("shortest_exit_right_hand", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, maze.width * 20, maze.height * 20, SDL_WINDOW_SHOWN); //creation d'une fenetre
    if(fenetre == NULL)
    {
        const char *error = SDL_GetError();
        fprintf(stderr, "Erreur de creation de la fenetre : %s\n", error);
        SDL_DestroyWindow(fenetre);
        SDL_Quit();
        return -1;
    }
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
            if(has_wall_up(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * 20, y * 20, (x * 20) + 19, y * 20); //on dessine un mur en haut
            }
            if(has_wall_down(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * 20, (y * 20) + 19, (x * 20) + 19, (y * 20) + 19); //on dessine un mur en bas
            }
            if(has_wall_left(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * 20, y * 20, x * 20, (y * 20) + 19); //on dessine un mur à gauche
            }
            if(has_wall_right(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, (x * 20) + 19, y * 20, (x * 20) + 19, (y * 20) + 19); //on dessine un mur à droite
            }
        }
    }

    SDL_RenderPresent(renderer); //on met à jour l'affichage
    SDL_Delay(1); //pause de 0.001 secondes
    SDL_SetRenderDrawColor(renderer, 0, 50, 255, 255); //on définit la couleur en bleu
    SDL_RenderDrawLine(renderer, 0, 0, 0, 20); //l'entrée en vert
    SDL_RenderDrawLine(renderer, 0, 0, 20, 0); //l'entrée en vert

    SDL_RenderPresent(renderer); //on met à jour l'affichage
    SDL_Delay(1); //pause de 0.001 secondes
    SDL_SetRenderDrawColor(renderer, 10, 235, 10, 255); //on définit la couleur en vert
    SDL_RenderDrawLine(renderer, (maze.width * 20) - 1, (maze.height * 20) - 20, (maze.width * 20) - 1, (maze.height * 20)); //la sortie en bleu
    SDL_RenderDrawLine(renderer, (maze.width * 20) - 20, (maze.height * 20) - 1, (maze.width * 20), (maze.height * 20) - 1); //la sortie en bleu
    SDL_RenderPresent(renderer); //on met à jour l'affichage
    SDL_SetRenderDrawColor(renderer, 0, 55, 155, 255); //on définit la couleur en bleu

    //on commence la recherche
    const waytab wayt = create_waytab(maze.width, maze.height);
    show_shortest_exit_right_hand_aux(maze, renderer, 0, 0, wayt, fast);
    SDL_SetRenderDrawColor(renderer, 10, 235, 10, 255); //on définit la couleur en vert
    color_way(renderer, maze, get_way(wayt, maze.width - 1, maze.height - 1), fast); //on affiche le chemin le plus court
    free_waytab(wayt);

    //à la fin
    SDL_Delay(1); //pause de 1 ms pour éviter certains anachronismes
    SDL_RenderPresent(renderer); //on met à jour l'affichage
    SDL_Event event = {0}; //on crée un event
    while(!(event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || \
        (event.type == SDL_KEYUP && (event.key.keysym.sym == SDLK_ESCAPE || \
            event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN)))) //tant que l'utilisateur n'a pas fermé la fenetre
    {
        SDL_WaitEvent(&event); //on attend un event
    }
    SDL_DestroyRenderer(renderer); //destruction du renderer (desallocation de la memoire)
    SDL_DestroyWindow(fenetre); //destruction de la fenetre (desallocation de la memoire)
    SDL_Quit(); //desalocation de la memoire
    return 1;
}

int show_shortest_exit_right_hand(const maze_t maze)
{
    {
        if(true_show_shortest_exit_right_hand(maze, false) == -1)
        {
            fprintf(stderr, "Erreur de visualisation\n");
            return -1;
        }
        return 1;
    }
}

int show_fast_shortest_exit_right_hand(const maze_t maze)
{
    {
        if(true_show_shortest_exit_right_hand(maze, true) == -1)
        {
            fprintf(stderr, "Erreur de visualisation\n");
            return -1;
        }
        return 1;
    }
}