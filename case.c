#include "case.h"
#include <stdio.h>
#include <SDL2/SDL.h>

// Create a maze of width x height cells
// width : width of the maze
// height : height of the maze
// tous les murs sont ouverts (hors limites)
// returns the maze
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
    const maze_t maze = {width, height, start};
    return maze;
}

// Create a maze of width x height cells
// width : width of the maze
// height : height of the maze
// tous les murs sont fermés
// returns the maze
maze_t create_wall_maze(const int width, const int height) {
    cell** start = malloc(sizeof(cell *) * height);
    for (int i = 0; i < height; i++) {
        start[i] = malloc(sizeof(cell) * width);
    }
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            start[i][j].wall_up = true;
            start[i][j].wall_down = true;
            start[i][j].wall_left = true;
            start[i][j].wall_right = true;
        }
    }
    const maze_t maze = {width, height, start};
    return maze;
}

//désalloue la mémoire allouée pour le labyrinthe
//maze : le labyrinthe à désallouer
void free_maze(const maze_t maze) {
  for (int i = 0; i < maze.height; i++) {
    free(maze.cells[i]);
  }
  free(maze.cells);
}

//ajoute un mur en haut de la cellule x, y
//maze : le labyrinthe
//x : la coordonnée x de la cellule
//y : la coordonnée y de la cellule
//gère les cas d'erreur et affiche un message d'erreur sans arrêter le programme
void wall_up(maze_t const maze, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr, "les coordonnées de la cellule sont en dehors des limites du labyrinthe, cible : %d, %d\n", x, y);
        return;
    }
    if(y > 0)
    {
        maze.cells[y][x].wall_up = true;
        maze.cells[y - 1][x].wall_down = true;
    }
    else
      {
        fprintf(stderr, "Impossible de modifier un mur de délimitation du labyrinthe, cible : %d, %d up\n", x, y);
      }
}

//ajoute un mur en bas de la cellule x, y
//maze : le labyrinthe
//x : la coordonnée x de la cellule
//y : la coordonnée y de la cellule
//gère les cas d'erreur et affiche un message d'erreur sans arrêter le programme
void wall_down(const maze_t maze, int x, int y)
{
    if(y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr, "les coordonnées de la cellule sont en dehors des limites du labyrinthe, cible : %d, %d\n", x, y);
        return;
    }
    if(y < maze.height - 1)
    {
        maze.cells[y][x].wall_down = true;
        maze.cells[y + 1][x].wall_up = true;
    }
    else
    {
        fprintf(stderr, "Impossible de modifier un mur de délimitation du labyrinthe, cible : %d, %d down\n", x, y);
    }
}

//ajoute un mur à gauche de la cellule x, y
//maze : le labyrinthe
//x : la coordonnée x de la cellule
//y : la coordonnée y de la cellule
//gère les cas d'erreur et affiche un message d'erreur sans arrêter le programme
void wall_left(const maze_t maze, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr, "les coordonnées de la cellule sont en dehors des limites du labyrinthe, cible : %d, %d\n", x, y);
        return;
    }
    if(x > 0)
    {
        maze.cells[y][x].wall_left = true;
        maze.cells[y][x - 1].wall_right = true;
    }
    else
    {
        fprintf(stderr, "Impossible de modifier un mur de délimitation du labyrinthe, cible : %d, %d left\n", x, y);
    }
}

//ajoute un mur à droite de la cellule x, y
//maze : le labyrinthe
//x : la coordonnée x de la cellule
//y : la coordonnée y de la cellule
//gère les cas d'erreur et affiche un message d'erreur sans arrêter le programme
void wall_right(const maze_t maze, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr, "les coordonnées de la cellule sont en dehors des limites du labyrinthe, cible : %d, %d\n", x, y);
        return;
    }
    if(x < maze.width - 1)
    {
        maze.cells[y][x].wall_right = true;
        maze.cells[y][x + 1].wall_left = true;
    }
    else
    {
        fprintf(stderr, "Impossible de modifier un mur de délimitation du labyrinthe, cible : %d, %d right\n", x, y);
    }
}

//retire le mur du haut de la cellule x, y
//maze : le labyrinthe
//x : la coordonnée x de la cellule
//y : la coordonnée y de la cellule
//gère les cas d'erreur et affiche un message d'erreur sans arrêter le programme
void unwall_up(const maze_t maze, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr, "les coordonnées de la cellule sont en dehors des limites du labyrinthe, cible : %d, %d\n", x, y);
        return;
    }
    if(y > 0)
    {
        maze.cells[y][x].wall_up = false;
        maze.cells[y - 1][x].wall_down = false;
    }
    else
    {
        fprintf(stderr, "Impossible de modifier un mur de délimitation du labyrinthe, cible : %d, %d up\n", x, y);
    }
}

//retire le mur du bas de la cellule x, y
//maze : le labyrinthe
//x : la coordonnée x de la cellule
//y : la coordonnée y de la cellule
//gère les cas d'erreur et affiche un message d'erreur sans arrêter le programme
void unwall_down(const maze_t maze, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr, "les coordonnées de la cellule sont en dehors des limites du labyrinthe, cible : %d, %d\n", x, y);
        return;
    }
    if(y < maze.height - 1)
    {
        maze.cells[y][x].wall_down = false;
        maze.cells[y + 1][x].wall_up = false;
    }
    else
    {
        fprintf(stderr, "Impossible de modifier un mur de délimitation du labyrinthe, cible : %d, %d down\n", x, y);
    }
}

//retire le mur à gauche de la cellule x, y
//maze : le labyrinthe
//x : la coordonnée x de la cellule
//y : la coordonnée y de la cellule
//gère les cas d'erreur et affiche un message d'erreur sans arrêter le programme
void unwall_left(const maze_t maze, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr, "les coordonnées de la cellule sont en dehors des limites du labyrinthe, cible : %d, %d\n", x, y);
        return;
    }
    if(x > 0)
    {
        maze.cells[y][x].wall_left = false;
        maze.cells[y][x - 1].wall_right = false;
    }
    else
    {
        fprintf(stderr, "Impossible de modifier un mur de délimitation du labyrinthe, cible : %d, %d left\n", x, y);
    }
}

//retire le mur à droite de la cellule x, y
//maze : le labyrinthe
//x : la coordonnée x de la cellule
//y : la coordonnée y de la cellule
//gère les cas d'erreur et affiche un message d'erreur sans arrêter le programme
void unwall_right(const maze_t maze, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr, "les coordonnées de la cellule sont en dehors des limites du labyrinthe, cible : %d, %d\n", x, y);
        return;
    }
    if(x < maze.width - 1)
    {
        maze.cells[y][x].wall_right = false;
        maze.cells[y][x + 1].wall_left = false;
    }
    else
    {
        fprintf(stderr, "Impossible de modifier un mur de délimitation du labyrinthe, cible : %d, %d right\n", x, y);
    }
}

//crée un tableau de booléens sur false de taille width x height
//width : largeur du tableau
//height : hauteur du tableau
//renvoie le tableau de booléens
bool_tab create_booltab(const int width, const int height)
{
    bool** booltab = malloc(sizeof(bool *) * height);
    for(int i = 0; i < height; i++)
    {
        booltab[i] = malloc(sizeof(bool) * width);
        for(int j = 0; j < width; j++)
        {
            booltab[i][j] = false;
        }
    }
    const bool_tab tab = {width, height, booltab};
    return tab;
}

//désalloue la mémoire allouée pour le tableau de booléens
//tab : le tableau de booléens à désallouer
void free_booltab(const bool_tab tab)
{
    for(int i = 0; i < tab.height; i++)
    {
        free(tab.tab[i]);
    }
    free(tab.tab);
}

//affiche le labyrinthe, renvoie -1 en cas d'erreur
//maze : le labyrinthe à afficher
//filename : le nom de la fenêtre
//les murs d'entré sont en vert, les murs de sortie en bleu
//chaque cellule est de taille 20x20 pixels
//le programme se ferme uand on appuie sur ECHAP, ENTRER, ou tente de fermer la fenetre
int print_maze(maze_t const maze, const char* filename)
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) //initilisation de la SDL avec l'image et les events (comprends des malloc)
    {
        const char *error = SDL_GetError();
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", error);
        SDL_Quit();
        return -1;
    }
    SDL_Window *fenetre = SDL_CreateWindow(filename, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, maze.width * 20, maze.height * 20, SDL_WINDOW_SHOWN); //creation d'une fenetre
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
    SDL_Delay(100); //pause de 0.1 secondes
    SDL_Event event; //on crée un event
    while(!(event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || \
        (event.type == SDL_KEYUP && (event.key.keysym.sym == SDLK_ESCAPE || \
            event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDL_SCANCODE_KP_ENTER)))) //tant que l'utilisateur n'a pas fermé la fenetre
    {
        SDL_WaitEvent(&event); //on attend un event
    }
    SDL_DestroyRenderer(renderer); //destruction du renderer (desallocation de la memoire)
    SDL_DestroyWindow(fenetre); //destruction de la fenetre (desallocation de la memoire)
    SDL_Quit(); //desalocation de la memoire
    return 0;
}