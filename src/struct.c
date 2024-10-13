#include "struct.h"
#include <SDL2/SDL.h>


// --- MAZE FUNCTIONS ---

maze_t create_basic_maze(const int width, const int height) {
    if(width < 1 || height < 1)
    {
        fprintf(stderr, "Erreur dans la fonction create_basic_maze : \nles dimensions du labyrinthe doivent être strictements positives, width : %d, height : %d\n", width, height);
        exit(1);
    }
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

maze_t create_wall_maze(const int width, const int height) {
    if(width < 1 || height < 1)
    {
        fprintf(stderr, "Erreur dans la fonction create_wall_maze : \nles dimensions du labyrinthe doivent être strictements positives, width : %d, height : %d\n", width, height);
        exit(1);
    }
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

void free_maze(const maze_t maze) {
  for (int i = 0; i < maze.height; i++) {
    free(maze.cells[i]);
  }
  free(maze.cells);
}

void wall_up(maze_t const maze, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr, "Erreur dans la fonction wall_up : \nles coordonnées de la cellule sont en dehors des limites du labyrinthe, cible : %d, %d\n", x, y);
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

void wall_down(const maze_t maze, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr, "Erreur dans la fonction wall_down : \nles coordonnées de la cellule sont en dehors des limites du labyrinthe, cible : %d, %d\n", x, y);
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

void wall_left(const maze_t maze, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr, "Erreur dans la fonction wall_left : \nles coordonnées de la cellule sont en dehors des limites du labyrinthe, cible : %d, %d\n", x, y);
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

void wall_right(const maze_t maze, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr, "Erreur dans la fonction wall_right : \nles coordonnées de la cellule sont en dehors des limites du labyrinthe, cible : %d, %d\n", x, y);
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

void unwall_up(const maze_t maze, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr, "Erreur dans la fonction unwall_up : \nles coordonnées de la cellule sont en dehors des limites du labyrinthe, cible : %d, %d\n", x, y);
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

void unwall_down(const maze_t maze, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr, "Erreur dans la fonction unwall_down : \nles coordonnées de la cellule sont en dehors des limites du labyrinthe, cible : %d, %d\n", x, y);
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

void unwall_left(const maze_t maze, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr, "Erreur dans la fonction unwall_left : \nles coordonnées de la cellule sont en dehors des limites du labyrinthe, cible : %d, %d\n", x, y);
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

void unwall_right(const maze_t maze, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr, "Erreur dans la fonction unwall_right : \nles coordonnées de la cellule sont en dehors des limites du labyrinthe, cible : %d, %d\n", x, y);
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

bool has_wall_up(const maze_t maze, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr, "Erreur dans la fonction has_wall_up : \nles coordonnées de la cellule sont en dehors des limites du labyrinthe, cible : %d, %d\n", x, y);
        exit(EXIT_FAILURE);
    }
    return maze.cells[y][x].wall_up;
}

bool has_wall_down(const maze_t maze, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr, "Erreur dans la fonction has_wall_down : \nles coordonnées de la cellule sont en dehors des limites du labyrinthe, cible : %d, %d\n", x, y);
        exit(EXIT_FAILURE);
    }
    return maze.cells[y][x].wall_down;
}

bool has_wall_left(const maze_t maze, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr, "Erreur dans la fonction has_wall_left : \nles coordonnées de la cellule sont en dehors des limites du labyrinthe, cible : %d, %d\n", x, y);
        exit(EXIT_FAILURE);
    }
    return maze.cells[y][x].wall_left;
}

bool has_wall_right(const maze_t maze, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr, "Erreur dans la fonction has_wall_right : \nles coordonnées de la cellule sont en dehors des limites du labyrinthe, cible : %d, %d\n", x, y);
        exit(EXIT_FAILURE);
    }
    return maze.cells[y][x].wall_right;
}


// --- BOOLTAB FUNCTIONS ---

bool_tab create_booltab(const int width, const int height)
{
    if(width < 1 || height < 1)
    {
        fprintf(stderr, "Erreur dans la fonction create_booltab : \nles dimensions du tableau de booléens doivent être strictements positives, width : %d, height : %d\n", width, height);
        exit(EXIT_FAILURE);
    }
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

void free_booltab(const bool_tab tab)
{
    for(int i = 0; i < tab.height; i++)
    {
        free(tab.tab[i]);
    }
    free(tab.tab);
}

void set_true(const bool_tab tab, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= tab.width || y >= tab.height)
    {
        fprintf(stderr, "Erreur dans la fonction set_true : \nles coordonnées de la case sont en dehors des limites du tableau, cible : %d, %d\n", x, y);
        return;
    }
    tab.tab[y][x] = true;
}

void set_false(const bool_tab tab, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= tab.width || y >= tab.height)
    {
        fprintf(stderr, "Erreur dans la fonction set_false : \nles coordonnées de la case sont en dehors des limites du tableau, cible : %d, %d\n", x, y);
        return;
    }
    tab.tab[y][x] = false;
}

bool get_bool(const bool_tab tab, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= tab.width || y >= tab.height)
    {
        fprintf(stderr, "Erreur dans la fonction get_bool : \nLes coordonnées de la case sont en dehors des limites du tableau, cible : %d, %d\n", x, y);
        exit(1);
    }
    return tab.tab[y][x];
}


// --- WAYTAB FUNCTIONS ---

waytab create_waytab(const int width,const int height)
{
    if(width < 1 || height < 1)
    {
        fprintf(stderr, "Erreur dans la fonction create_waytab : \nles dimensions du tableau de chemin doivent être strictements positives, width : %d, height : %d\n", width, height);
        exit(EXIT_FAILURE);
    }
    way** way_tab = malloc(sizeof(way*) * height);
    for(int i = 0; i < height; i++)
    {
        way_tab[i] = malloc(sizeof(way) * width);
        for(int j = 0; j < width; j++)
        {
            way_tab[i][j].dad = NULL;
            way_tab[i][j].x = j;
            way_tab[i][j].y = i;
        }
    }
    const waytab tab = {width, height, way_tab};
    return tab;
}

void free_waytab(const waytab tab)
{
    for(int i = 0; i < tab.height; i++)
    {
        free(tab.tab[i]);
    }
    free(tab.tab);
}

bool has_way(const waytab tab, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= tab.width || y >= tab.height)
    {
        fprintf(stderr, "Erreur dans la fonction has_way : \nles coordonnées de la case sont en dehors des limites du tableau, cible : %d, %d\n", x, y);
        exit(EXIT_FAILURE);
    }
    return !is_empty(&tab.tab[y][x]);
}

way *get_way(const waytab tab, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= tab.width || y >= tab.height)
    {
        fprintf(stderr, "Erreur dans la fonction get_way : \nles coordonnées de la case sont en dehors des limites du tableau, cible : %d, %d\n", x, y);
        exit(EXIT_FAILURE);
    }
    return &tab.tab[y][x];
}

void connected_way(const waytab tab, const int x, const int y, const int dad_x, const int dad_y)
{
    if(y < 0 || x < 0 || x >= tab.width || y >= tab.height)
    {
        fprintf(stderr, "Erreur dans la fonction connected_way : \nles coordonnées de la case sont en dehors des limites du tableau, cible : %d, %d\n", x, y);
        return;
    }
    if(dad_x < 0 || dad_y < 0 || dad_x >= tab.width || dad_y >= tab.height)
    {
        fprintf(stderr, "Erreur dans la fonction connected_way : \nles coordonnées du père sont en dehors des limites du tableau, cible : %d, %d\n", dad_x, dad_y);
        return;
    }
    new_dad(tab.tab[y] + x, tab.tab[dad_y] + dad_x);
}

int length_waytab(const waytab tab, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= tab.width || y >= tab.height)
    {
        fprintf(stderr, "Erreur dans la fonction length_waytab : \nles coordonnées de la case sont en dehors des limites du tableau, cible : %d, %d\n", x, y);
        exit(EXIT_FAILURE);
    }
    return length_way(&tab.tab[y][x]);
}

// --- WAY FUNCTIONS ---

void print_way(const way *w)
{
    if(is_empty(w))
    {
        printf("Chemin vide\n");
        return;
    }
    if(w->dad != NULL)
    {
        print_way(w->dad);
    }
    printf("(%d, %d) ", w->x, w->y);
}

int length_way(const way *w)
{
    if(is_empty(w))
    {
        return 4294967295;
    }
    if(w->dad == NULL)
    {
        return 0;
    }
    return 1 + length_way(w->dad);
}

void new_dad(way *son, way *dad)
{
    if(son -> x == 0 && son -> y == 0)
    {
        fprintf(stderr, "Erreur dans la fonction new_dad : \nla case de départ ne peut pas avoir de père\n");
        return;
    }
    son->dad = dad;
}

way *copy_way(const way *w)
{
    way *copy = malloc(sizeof(way));
    if(w->dad != NULL)
    {
        copy->dad = copy_way(w->dad);
    }
    else
    {
        copy->dad = NULL;
    }
    copy->x = w->x;
    copy->y = w->y;
    return copy;
}

void free_way(way *w)
{
    if(w->dad != NULL)
    {
        free_way(w->dad);
    }
    free(w);
}

bool is_empty(const way *w)
{
    if(w->dad == NULL)
    {
        return !(w -> x == 0 && w -> y == 0); //un chemin vide est un chemin qui ne pointe pas vers la case (0, 0)
    }
    return is_empty(w->dad);
}

// --- MAZE PRINTING ---

int print_maze(maze_t const maze)
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