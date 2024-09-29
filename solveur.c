#include "solveur.h"
#include "mazemaker.h"
#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>
#define NB_GENERATEURS 4

//une mini-jeu où il faut trouver la sortie
//version de base
//le chemin apparaît au fur et à mesure que le joueur avance
//maze : le labyrinthe
//appuier sur ECHAP, ENTER ou la croix fermera proprement le jeu
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
    if(maze.cells[0][0].wall_right)
    {
        SDL_RenderDrawLine(renderer, 19, 0, 19, 19); //on dessine le mur de droite
        SDL_RenderDrawLine(renderer, 20, 0, 20, 19); //on dessine le mur de droite
    }
    if(maze.cells[0][0].wall_down)
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
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || (event.type == SDL_KEYUP && \
                (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDL_SCANCODE_KP_ENTER)))
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
                SDL_RenderDrawLine(renderer, pos_x * 20, pos_y * 20, pos_x * 20 + 19, pos_y * 20);
                SDL_RenderDrawLine(renderer, pos_x * 20, pos_y * 20 -1, pos_x * 20 + 19, pos_y * 20 -1);
            }
            if(maze.cells[pos_y][pos_x].wall_down)
            {
                SDL_RenderDrawLine(renderer, pos_x * 20, (pos_y * 20) + 19, (pos_x * 20) + 19, (pos_y * 20) + 19);
                SDL_RenderDrawLine(renderer, pos_x * 20, (pos_y * 20) + 20, (pos_x * 20) + 19, (pos_y * 20) + 20);
            }
            if(maze.cells[pos_y][pos_x].wall_left)
            {
                SDL_RenderDrawLine(renderer, pos_x * 20, pos_y * 20, pos_x * 20, (pos_y * 20) + 19);
                SDL_RenderDrawLine(renderer, pos_x * 20 - 1, pos_y * 20, pos_x * 20 - 1, (pos_y * 20) + 19);
            }

            if(maze.cells[pos_y][pos_x].wall_right)
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
    fprintf(stderr, "Erreur de gestion de l'aléatoire\n");
    return -1;
}

//une mini-jeu où il faut trouver la sortie
//le chemin n'apparaît que sur la case où le joueur est
//version mi-blind
//maze : le labyrinthe
//appuier sur ECHAP, ENTER ou la croix fermera proprement le jeu
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
    if(maze.cells[0][0].wall_right)
    {
        SDL_RenderDrawLine(renderer, 19, 0, 19, 19); //on dessine le mur de droite
    }
    if(maze.cells[0][0].wall_down)
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
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || (event.type == SDL_KEYUP && \
                (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDL_SCANCODE_KP_ENTER)))
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
            SDL_RenderDrawLine(renderer, pos_x * 20 + 7, pos_y * 20 + 10, pos_x * 20 + 13, pos_y * 20 + 10); //on efface le joueur
            SDL_RenderDrawLine(renderer, pos_x * 20 + 10, pos_y * 20 + 7, pos_x * 20 + 10, pos_y * 20 + 13); //on efface le joueur
            if(maze.cells[pos_y][pos_x].wall_up)
            {
                SDL_RenderDrawLine(renderer, pos_x * 20, pos_y * 20, pos_x * 20 + 19, pos_y * 20); //on efface le mur du haut
            }
            if(maze.cells[pos_y][pos_x].wall_down)
            {
                SDL_RenderDrawLine(renderer, pos_x * 20, (pos_y * 20) + 19, (pos_x * 20) + 19, (pos_y * 20) + 19); //on efface le mur du bas
            }
            if(maze.cells[pos_y][pos_x].wall_left)
            {
                SDL_RenderDrawLine(renderer, pos_x * 20, pos_y * 20, pos_x * 20, (pos_y * 20) + 19); //on efface le mur de gauche
            }

            if(maze.cells[pos_y][pos_x].wall_right)
            {
                SDL_RenderDrawLine(renderer, (pos_x * 20) + 19, pos_y * 20, (pos_x * 20) + 19, (pos_y * 20) + 19); //on efface le mur de droite
            }
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
                SDL_RenderDrawLine(renderer, pos_x * 20, pos_y * 20, pos_x * 20 + 19, pos_y * 20); //on dessine le mur du haut
            }
            if(maze.cells[pos_y][pos_x].wall_down)
            {
                SDL_RenderDrawLine(renderer, pos_x * 20, (pos_y * 20) + 19, (pos_x * 20) + 19, (pos_y * 20) + 19); //on dessine le mur du bas
            }
            if(maze.cells[pos_y][pos_x].wall_left)
            {
                SDL_RenderDrawLine(renderer, pos_x * 20, pos_y * 20, pos_x * 20, (pos_y * 20) + 19); //on dessine le mur de gauche
            }

            if(maze.cells[pos_y][pos_x].wall_right)
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
            print_maze(maze, "victoire !!"); //on affiche le labyrinthe en entier
        }
    }
    SDL_DestroyRenderer(renderer); //destruction du renderer (desallocation de la memoire)
    SDL_DestroyWindow(fenetre); //destruction de la fenetre (desallocation de la memoire)
    SDL_Quit(); //desalocation de la memoire
    return 0;
}

//lance minigame2 avec un générateur de labyrinthe aléatoire
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
//appuier sur ECHAP, ENTER ou la croix fermera proprement le jeu
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
    fprintf(stderr, "Erreur de gestion de l'aléatoire\n");
    return -1;
}

//une mini-jeu où il faut trouver la sortie
//le chemin n'apparaît pas, même sur la case où le joueur est
//version full-blind
//maze : le labyrinthe
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
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || (event.type == SDL_KEYUP && \
                (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDL_SCANCODE_KP_ENTER)))
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
            SDL_RenderDrawLine(renderer, pos_x * 20 + 7, pos_y * 20 + 10, pos_x * 20 + 13, pos_y * 20 + 10); //on efface le joueur
            SDL_RenderDrawLine(renderer, pos_x * 20 + 10, pos_y * 20 + 7, pos_x * 20 + 10, pos_y * 20 + 13); //on efface le joueur
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
            SDL_RenderDrawLine(renderer, pos_x * 20 + 7, pos_y * 20 + 10, pos_x * 20 + 13, pos_y * 20 + 10); //on dessine le joueur
            SDL_RenderDrawLine(renderer, pos_x * 20 + 10, pos_y * 20 + 7, pos_x * 20 + 10, pos_y * 20 + 13); //on dessine le joueur
            SDL_Delay(250); //pause de 0.25 secondes
            SDL_RenderPresent(renderer); //on met à jour l'affichage
        }
        if(pos_x == maze.width - 1 && pos_y == maze.height - 1) //si le joueur est à la sortie
        {
            is_gaming = false; //on arrête le jeu
            SDL_Delay(10); //pause pour assurer le bon affichage
            print_maze(maze, "victoire !!"); //on affiche le labyrinthe en entier
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
    fprintf(stderr, "Erreur de gestion de l'aléatoire\n");
    return -1;
}

//lance un mini-jeu avec un générateur de labyrinthe et une difficulté aléatoires
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
int rminigame(const int width, const int height)
{
    srand(time(NULL));
    const int r = rand() % 3;
    if(r == 0)
    {
        return rminigame1(width, height);
    }
    else if(r == 1)
    {
        return rminigame2(width, height);
    }
    else if (r == 2)
    {
        return rminigame3(width, height);
    }
    fprintf(stderr, "Erreur de gestion de l'aléatoire\n");
    return -1;
}

//lance un mini-jeu avec un labyrinthe donné et une difficulté aléatoire
//maze : le labyrinthe
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