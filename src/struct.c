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
    for (int i = 0; i < height - 1; i++) {
        for (int j = 0; j < width - 1; j++) //cellules non bordure
        {
            start[i][j].wall_down = false;
            start[i][j].wall_right = false;
        }
        start[i][width - 1].wall_down = false; //cellules bordure droite
        start[i][width - 1].wall_right = true;
    }
    for (int j = 0; j < width - 1; j++) {
        start[height - 1][j].wall_down = true; //cellules bordure bas
        start[height - 1][j].wall_right = false;
    }
    start[height - 1][width - 1].wall_down = true; //sortie
    start[height - 1][width - 1].wall_right = true;
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
            start[i][j].wall_down = true;
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
    if(x == 0)
    {
        return false;
    }
    return maze.cells[y][x - 1].wall_down;
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
    if(y == 0)
    {
        return false;
    }
    return maze.cells[y - 1][x].wall_right;
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
            way_tab[i][j] = *create_way();
            way_tab[i][j].x = j;
            way_tab[i][j].y = i;
        }
    }
    way_tab[0][0].length = 0;
    const waytab tab = {width, height, way_tab};
    // ReSharper disable once CppDFAMemoryLeak
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

unsigned int length_waytab(const waytab tab, const int x, const int y)
{
    if(y < 0 || x < 0 || x >= tab.width || y >= tab.height)
    {
        fprintf(stderr, "Erreur dans la fonction length_waytab : \nles coordonnées de la case sont en dehors des limites du tableau, cible : %d, %d\n", x, y);
        exit(EXIT_FAILURE);
    }
    return length_way(&tab.tab[y][x]);
}

// --- WAY FUNCTIONS ---

way *create_way()
{
    // ReSharper disable once CppDFAMemoryLeak
    way *w = malloc(sizeof(way));
    w -> dad = NULL;
    w -> x = -1;
    w -> y = -1;
    w -> length = L_TP;
    return w;
}

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
    printf("(%d, %d)\n", w->x, w->y);
}

unsigned int length_way(const way *w)
{
    return w -> x == 0 && w -> y == 0 ? 0 : w -> length;
}

void new_dad(way *son, way *dad)
{
    if(son -> x == 0 && son -> y == 0)
    {
        fprintf(stderr, "Erreur dans la fonction new_dad : \nla case de départ ne peut pas avoir de père\n");
        return;
    }
    son->dad = dad;
    son->length = dad->length + 1;
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
    copy->length = w->length;
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
        return w -> x != 0 || w -> y != 0; //un chemin est vide s'il n'est pas relié à la case (0, 0)
    }
    if(w->length == L_TP)
    {
        return false;
    }
    return is_empty(w->dad);
}

// --- MAZE PRINTING ---

int print_maze(maze_t const maze)
{
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) //initilisation de la SDL avec l'image et les events (comprends des malloc)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0) { //on obtient le mode d'affichage de l'écran
        fprintf(stderr, "Erreur lors de l'obtention du mode d'affichage : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    int d_h = displayMode.h/maze.height; int d_w = displayMode.w/maze.width; //on définie le ratio de la taille des cellules
    printf("h : %d, w : %d\n", d_h, d_w);
    d_h = d_h > d_w ? d_w : d_h; //on prend le plus petit ratio pour que le labyrinthe tienne dans l'écran
    d_w = d_h;
    if(d_w < 2) //la taille minimale des cellules est de 2 pixels (1 pixel de vide et 2 pixels pour chaque mur)
    {
        fprintf(stderr, "Warning : la taille des cellules est trop petite pour être affichée correctement, l'affichage va dépasser de l'écran\n");
        d_w = 2;
        d_h = 2;
    }
    printf("d_h : %d, d_w : %d\n", d_h, d_w);

    SDL_Window *fenetre = SDL_CreateWindow("maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, maze.width * d_w, maze.height * d_h, SDL_WINDOW_SHOWN); //creation d'une fenetre
    if(fenetre == NULL)
    {
        fprintf(stderr, "Erreur de creation de la fenetre : %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }
    SDL_Renderer *renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED); //creation d'un renderer
    if(renderer == NULL)
    {
        renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_SOFTWARE);
        if(renderer == NULL)
        {
            fprintf(stderr, "Erreur de creation du renderer : %s\n", SDL_GetError());
            SDL_DestroyWindow(fenetre);
            SDL_Quit();
            return -1;
        }
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //on définit la couleur de fond en blanc

    for(int x = 0; x < maze.width; x++)
    {
        for(int y = 0; y < maze.height; y++)
        {
            if(maze.cells[y][x].wall_down)
            {
                SDL_RenderDrawLine(renderer, x * d_w, (y + 1) * d_h - 1, (x + 1) * d_w - 1, (y + 1) * d_h - 1); //on dessine un mur en bas
            }
            if(maze.cells[y][x].wall_right)
            {
                SDL_RenderDrawLine(renderer, (x + 1) * d_w - 1, y * d_h, (x + 1) * d_w - 1, (y + 1) * d_h - 1); //on dessine un mur à droite
            }
        }
    }
    SDL_RenderDrawLine(renderer, 0, 0, 0, maze.height * d_h); //on dessine les murs de la bordure gauche
    SDL_RenderDrawLine(renderer, 0, 0, maze.width * d_w, 0); //on dessine les murs de la bordure haute

    SDL_SetRenderDrawColor(renderer, 0, 50, 255, 255); //on définit la couleur en bleu
    SDL_RenderDrawLine(renderer, 0, 0, d_w, 0); //on dessine l'entrée
    SDL_RenderDrawLine(renderer, 0, 0, 0, d_h); //on dessine l'entrée

    SDL_SetRenderDrawColor(renderer, 10, 235, 10, 255); //on définit la couleur en vert
    SDL_RenderDrawLine(renderer, maze.width * d_w - d_w, maze.height * d_h - 1, maze.width * d_w, maze.height * d_h - 1); //on dessine la sortie
    SDL_RenderDrawLine(renderer, maze.width * d_w - 1, maze.height * d_h - d_h, maze.width * d_w - 1, maze.height * d_h); //on dessine la sortie

    SDL_Delay(displayMode.refresh_rate); //pause pour laisser aux données le temps de s'afficher
    SDL_RenderPresent(renderer); //on met à jour l'affichage
    SDL_Event event = {0}; //on crée un event vide
    while(!(event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || \
        (event.type == SDL_KEYUP && (event.key.keysym.sym == SDLK_ESCAPE || \
            event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN)))) //tant que l'utilisateur n'a pas fermé la fenetre
    {
        SDL_WaitEvent(&event); //on enregistre les events entrants
    }
    SDL_DestroyRenderer(renderer); //destruction du renderer (desallocation de la memoire)
    SDL_DestroyWindow(fenetre); //destruction de la fenetre (desallocation de la memoire)
    SDL_Quit(); //desalocation de la memoire
    return 1;
}
