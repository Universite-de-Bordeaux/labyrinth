#include "escape.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <threads.h>
#include "limits.h"
#include "struct.h"

int (*escape[SIZE])(maze_t maze, int x, int y) = {&random_escape,   &right_hand, &right_hand_r, &hunt_kill_escape, &right_hand_p, &right_hand_de,
                                                  &right_hand_p_de, &random_p,   &random_de,    &random_p_de,      &cheat_escape};

const char* escape_name[SIZE] = {"random",          "right_hand", "right_hand_r", "hunt_kill",   "right_hand_p", "right_hand_de",
                                 "right_hand_p_de", "random_p",   "random_de",    "random_p_de", "cheat"};

int ESCAPE_TYPE(const maze_t maze, int x, int y)
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    int dw, dh;
    if (initial_print_maze(maze, &renderer, &window, &dw, &dh) != 1)
    {
        return -1;
    }
    SDL_SetWindowTitle(window, "escaping");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255); // couleur customisable
    SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // la position actuelle
    SDL_RenderFillRect(renderer, &rect);
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer); // on affiche la position actuelle
    bool show = true;

    // vos variables à déclarer avant la boucle
    int truc = 18;
    char machin = 5;

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event))
    {
        // on vide la file d'attente des événements
    }
    int steps = 0;
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        if (show)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
                    (event.type == SDL_KEYUP &&
                     (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER ||
                      event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
                {
                    printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                    destroy_print_maze(renderer, window);
                    return -1;
                }
                else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE)
                {
                    show = false;
                    destroy_print_maze(renderer, window);
                }
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // on efface la position actuelle
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
        }

        // le corps de l'algorithme
        // votre déplacement
        truc++;
        if (truc == 49)
        {
            machin = 3;
            truc = machin + 1;
        }

        if (show)
        {
            rect.x = x * dw + 1;
            rect.y = y * dh + 1;
            SDL_RenderFillRect(renderer, &rect);
            SDL_Delay(dm.refresh_rate); // delay customisable (actuellement à sa vitesse maximale)
            SDL_RenderPresent(renderer);
        }
        steps++;
    }
    if (show)
    {
        SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetWindowTitle(window, "escaped");
        SDL_Delay(dm.refresh_rate);
        SDL_RenderPresent(renderer);
        wait_and_destroy_print_maze(renderer, window);
    }
    return steps;
}

int random_escape(const maze_t maze, int x, int y)
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    int dw, dh;
    if (initial_print_maze(maze, &renderer, &window, &dw, &dh) != 1)
    {
        return -1;
    }
    SDL_SetWindowTitle(window, "escaping");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
    SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // la position actuelle
    SDL_RenderFillRect(renderer, &rect);
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer); // on affiche la position actuelle
    bool show = true;

    unsigned char choix; // la direction qu'on va suivre le plus possible
    getrandom(&choix, sizeof(char), 0);
    choix %= 4;

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event))
    {
        // on vide la file d'attente des événements
    }
    int steps = 0;
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        if (show)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
                    (event.type == SDL_KEYUP &&
                     (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER ||
                      event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
                {
                    printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                    destroy_print_maze(renderer, window);
                    return -1;
                }
                else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE)
                {
                    show = false;
                    destroy_print_maze(renderer, window);
                }
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // on efface la position actuelle
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
        }

        switch (choix)
        {
        case 0:
            if (!has_wall_up(maze, x, y))
            {
                y--;
            }
            break;
        case 1:
            if (!has_wall_down(maze, x, y))
            {
                y++;
            }
            break;
        case 2:
            if (!has_wall_left(maze, x, y))
            {
                x--;
            }
            break;
        case 3:
            if (!has_wall_right(maze, x, y))
            {
                x++;
            }
            break;
        default:
            fprintf(stderr, "Erreur dans la fonction try_to_escape : choix invalide.\n");
            printf("choix = %d\n", choix);
            destroy_print_maze(renderer, window);
            exit(EXIT_FAILURE);
            break;
        }

        getrandom(&choix, sizeof(char), 0);
        choix %= 4;

        if (show)
        {
            rect.x = x * dw + 1;
            rect.y = y * dh + 1;
            SDL_RenderFillRect(renderer, &rect);
            SDL_Delay(dm.refresh_rate);
            SDL_RenderPresent(renderer);
        }
        steps++;
    }
    if (show)
    {
        SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetWindowTitle(window, "escaped");
        SDL_Delay(dm.refresh_rate);
        SDL_RenderPresent(renderer);
        wait_and_destroy_print_maze(renderer, window);
    }
    return steps;
}

int cheat_escape(const maze_t maze, int x, int y)
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    int dw, dh;
    if (initial_print_maze(maze, &renderer, &window, &dw, &dh) != 1)
    {
        return -1;
    }
    SDL_SetWindowTitle(window, "escaping");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255); // couleur customisable
    SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // la position actuelle
    SDL_RenderFillRect(renderer, &rect);
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer); // on affiche la position actuelle
    bool show = true;

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event))
    {
        // on vide la file d'attente des événements
    }
    int steps = 0;
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        if (show)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
                    (event.type == SDL_KEYUP &&
                     (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER ||
                      event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
                {
                    printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                    destroy_print_maze(renderer, window);
                    return -1;
                }
                else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE)
                {
                    show = false;
                    destroy_print_maze(renderer, window);
                }
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // on efface la position actuelle
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
        }

        // le corps de l'algorithme
        // votre déplacement
        unsigned int random_value;
        getrandom(&random_value, sizeof(random_value), 0);
        if (x < maze.width - 1 && random_value % 2 == 0)
        {
            x++;
        }
        else if (y < maze.height - 1)
        {
            y++;
        }
        else
        {
            x++;
        }

        if (show)
        {
            rect.x = x * dw + 1;
            rect.y = y * dh + 1;
            SDL_RenderFillRect(renderer, &rect);
            SDL_Delay(dm.refresh_rate); // delay customisable (actuellement à sa vitesse maximale)
            SDL_RenderPresent(renderer);
        }
        steps++;
    }
    if (show)
    {
        SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetWindowTitle(window, "escaped");
        SDL_Delay(dm.refresh_rate);
        SDL_RenderPresent(renderer);
        wait_and_destroy_print_maze(renderer, window);
    }
    return steps;
}

// fonction pour savoir si une cellule a des cellules adjacentes accessibles
// renvoie true si il y a des cellules accessibles sinon false
static bool has_accessible_cells(int x, int y, bool_tab visited, maze_t maze)
{
    if (!has_wall_right(maze, x, y) && !get_bool(visited, x + 1, y))
    {
        return true;
    }
    if (!has_wall_down(maze, x, y) && !get_bool(visited, x, y + 1))
    {
        return true;
    }
    if (!has_wall_left(maze, x, y) && !get_bool(visited, x - 1, y))
    {
        return true;
    }
    if (!has_wall_up(maze, x, y) && !get_bool(visited, x, y - 1))
    {
        return true;
    }
    return false;
}

enum direction
{
    DROITE = 0,
    BAS,
    GAUCHE,
    HAUT
};

// Fonction qui retourne un booléen correspondant à la possibilité d'aller dans une direction donnée
// elle modifie les variables x_next et y_next pour les coordonnées de la cellule suivante
static bool get_adj(int x, int y, int* x_next, int* y_next, maze_t maze, char dir)
{
    switch (dir)
    {
    case DROITE:
        if (!has_wall_right(maze, x, y))
        {
            *x_next = x + 1;
            *y_next = y;
            return true;
        }
        return false;
    case BAS:
        if (!has_wall_down(maze, x, y))
        {
            *x_next = x;
            *y_next = y + 1;
            return true;
        }
        return false;
    case GAUCHE:
        if (!has_wall_left(maze, x, y))
        {
            *x_next = x - 1;
            *y_next = y;
            return true;
        }
        return false;
    case HAUT:
        if (!has_wall_up(maze, x, y))
        {
            *x_next = x;
            *y_next = y - 1;
            return true;
        }
        return false;
    default:
        fprintf(stderr, "Error in get_adj, dir must be within 0 and 3, received %d\n", dir);
        exit(EXIT_FAILURE);
    }
}

// Fonction qui retourne un booléen correspondant à la possibilité d'aller dans une direction donnée
static bool can_go(int x, int y, maze_t maze, char dir)
{
    // On renvoie faux si il y a un mur dans la direction donnée
    switch (dir)
    {
    case DROITE:
        return (!has_wall_right(maze, x, y));
    case BAS:
        return (!has_wall_down(maze, x, y));
    case GAUCHE:
        return (!has_wall_left(maze, x, y));
    case HAUT:
        return (!has_wall_up(maze, x, y));
    default:
        fprintf(stderr, "Error in can_go, dir must be within 0 and 3, received %d\n", dir);
        exit(EXIT_FAILURE);
    }
}

// Fonction change les valeurs de x et y en fonction de la direction donnée
static void go(int* x, int* y, int dir)
{
    // On incrémente ou décrémente x ou y en fonction de la direction donnée
    switch (dir)
    {
    case DROITE:
        (*x)++;
        break;
    case BAS:
        (*y)++;
        break;
    case GAUCHE:
        (*x)--;
        break;
    case HAUT:
        (*y)--;
        break;
    default:
        fprintf(stderr, "Error in go, dir must be within 0 and 3, received %d\n", dir);
        exit(EXIT_FAILURE);
    }
}

int hunt_kill_escape(maze_t maze, int x, int y)
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    int dw, dh;
    int step = 0;
    if (pre_print_maze(maze, &renderer, &window, &dw, &dh) != 1)
    {
        return -1;
    }

    SDL_SetWindowTitle(window, "escaping");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255); // couleur customisable
    SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // la position actuelle
    SDL_RenderFillRect(renderer, &rect);
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer); // on affiche la position actuelle
    bool show = true;


    // on crée un bool_tab pour stocker les cellules visitées
    // on marque la cellule actuelle comme visitée
    bool_tab visited = create_booltab(maze.width, maze.height);
    bool_tab dead_end = create_booltab(maze.width, maze.height);
    int x_next = x;
    int y_next = y;
    int not_go;
    unsigned char dir;

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event))
    {
        // on vide la file d'attente des événements
    }
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        if (show)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // on définit la couleur de fond en blanc
            if (has_wall_up(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, y * dh, (x + 1) * dw, y * dh);
            }
            if (has_wall_down(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, (y + 1) * dh, (x + 1) * dw, (y + 1) * dh);
            }
            if (has_wall_left(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, y * dh, x * dw, (y + 1) * dh);
            }
            if (has_wall_right(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, (x + 1) * dw, y * dh, (x + 1) * dw, (y + 1) * dh);
            }
            SDL_Delay(dm.refresh_rate);
            SDL_RenderPresent(renderer);
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
                    (event.type == SDL_KEYUP &&
                     (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER ||
                      event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
                {
                    printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                    destroy_print_maze(renderer, window);
                    return -1;
                }
                else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE)
                {
                    show = false;
                    destroy_print_maze(renderer, window);
                }
            }
            SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255); // on efface la position actuelle
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
        }
        step++;

        set_true(visited, x, y);
        // toutes les cellules accessibles ont été visitées
        // HUNT
        if (!has_accessible_cells(x, y, visited, maze))
        {
            int nb_entry = 0;
            for (int i = 0; i < 4; i++)
            {
                // on compte le nombre de cellules adjacentes accessibles
                if (get_adj(x, y, &x_next, &y_next, maze, i) && !get_bool(dead_end, x_next, y_next))
                {
                    nb_entry++;
                }
            }
            if (nb_entry == 1)
            {
                set_true(dead_end, x, y);
                not_go = -1;
            }
            else
            {
                not_go = (dir + 2) % 4;
            }
            do
            {
                getrandom(&dir, sizeof(dir), 0);
                dir %= 4;
            }
            while (dir == not_go || !get_adj(x, y, &x_next, &y_next, maze, dir) || get_bool(dead_end, x_next, y_next));
        }
        // tant qu'on a des cellules adjacentes non visitées
        // on en choisit une aléatoirement
        // KILL
        else
        {
            do
            {
                getrandom(&dir, sizeof(dir), 0);
                dir %= 4;
            }
            while (!get_adj(x, y, &x_next, &y_next, maze, dir) || get_bool(visited, x_next, y_next));
        }
        go(&x, &y, dir);

        if (show)
        {
            rect.x = x * dw + 1;
            rect.y = y * dh + 1;
            SDL_RenderFillRect(renderer, &rect);
            SDL_Delay(dm.refresh_rate); // delay customisable (actuellement à sa vitesse maximale)
            SDL_RenderPresent(renderer);
        }
    }
    free_booltab(visited);
    free_booltab(dead_end);
    if (show)
    {
        SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetWindowTitle(window, "escaped");
        SDL_Delay(dm.refresh_rate);
        SDL_RenderPresent(renderer);
        wait_and_destroy_print_maze(renderer, window);
    }
    return step;
}

static int visited_value(int** visited, int x, int y, int dir)
{
    // On renvoie la valeur de la case adjacente dans la direction donnée
    switch (dir)
    {
    case DROITE:
        return visited[x + 1][y];
    case BAS:
        return visited[x][y + 1];
    case GAUCHE:
        return visited[x - 1][y];
    case HAUT:
        return visited[x][y - 1];
    default:
        fprintf(stderr, "Error in visited_value, dir must be within 0 and 3, received %d\n", dir);
        exit(EXIT_FAILURE);
    }
}

// Algorithme de résolution main droite
int right_hand(const maze_t maze, int x, int y)
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    int dw, dh;
    if (initial_print_maze(maze, &renderer, &window, &dw, &dh) != 1)
    {
        return -1;
    }
    SDL_SetWindowTitle(window, "escaping");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255); // couleur customisable
    SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // la position actuelle
    SDL_RenderFillRect(renderer, &rect);
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer); // on affiche la position actuelle
    bool show = true;

    // vos variables à déclarer avant la boucle
    unsigned int dir;
    getrandom(&dir, sizeof(dir), 0);
    dir %= 4; // direction aléatoire

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event))
    {
        // on vide la file d'attente des événements
    }
    int steps = 0;
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        if (show)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
                    (event.type == SDL_KEYUP &&
                     (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER ||
                      event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
                {
                    printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                    destroy_print_maze(renderer, window);
                    return -1;
                }
                else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE)
                {
                    show = false;
                    destroy_print_maze(renderer, window);
                }
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // on efface la position actuelle
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
        }
        // le corps de l'algorithme
        // votre déplacement

        if (can_go(x, y, maze, (dir + 1) % 4)) // Déplacement à droite
        {
            dir = (dir + 1) % 4;
        }
        else if (can_go(x, y, maze, dir)) // Déplacement en face
        {
            // On ne change pas dir
        }
        else if (can_go(x, y, maze, (dir + 3) % 4)) // Déplacement à gauche
        {
            dir = (dir + 3) % 4;
        }
        else if (can_go(x, y, maze, (dir + 2) % 4)) // Déplacement en arrière
        {
            dir = (dir + 2) % 4;
        }
        go(&x, &y, dir);


        if (show)
        {
            rect.x = x * dw + 1;
            rect.y = y * dh + 1;
            SDL_RenderFillRect(renderer, &rect);
            SDL_Delay(dm.refresh_rate); // delay customisable (actuellement à sa vitesse maximale)
            SDL_RenderPresent(renderer);
        }
        steps++;
    }
    if (show)
    {
        SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetWindowTitle(window, "escaped");
        SDL_Delay(dm.refresh_rate);
        SDL_RenderPresent(renderer);
        wait_and_destroy_print_maze(renderer, window);
    }
    return steps;
}


// Algorithme de résolution main droite quand le terrain n'est pas connu sinon des déplacements aléatoires
int right_hand_r(const maze_t maze, int x, int y)
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    int dw, dh;
    if (initial_print_maze(maze, &renderer, &window, &dw, &dh) != 1)
    {
        return -1;
    }
    SDL_SetWindowTitle(window, "escaping");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255); // couleur customisable
    SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // la position actuelle
    SDL_RenderFillRect(renderer, &rect);
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer); // on affiche la position actuelle
    bool show = true;

    // Variables à déclarer avant la boucle
    unsigned int dir;
    getrandom(&dir, sizeof(dir), 0);
    dir %= 4; // direction aléatoire
    int count = 0; // Compteur de direction disponibles
    int min = 0; // Valeur de la cellule avec la valeur plus basse disponible ajdacente à la cellule actuelle
    // Création d'un tableau de taille width.height pour savoir si on est déjà passé par une case initialisé à 0
    int** visited = malloc(maze.width * sizeof(int*));
    for (int i = 0; i < maze.width; i++)
    {
        visited[i] = malloc(maze.height * sizeof(int));
    }
    for (int i = 0; i < maze.width; i++)
    {
        for (int j = 0; j < maze.height; j++)
        {
            visited[i][j] = 0;
        }
    }

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event))
    {
        // on vide la file d'attente des événements
    }
    int steps = 0;
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        if (show)
        {
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
                    (event.type == SDL_KEYUP &&
                     (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER ||
                      event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
                {
                    printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                    destroy_print_maze(renderer, window);
                    return -1;
                }
                else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE)
                {
                    show = false;
                    destroy_print_maze(renderer, window);
                }
            }
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // on efface la position actuelle
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
        }

        // Boucle de l'algo
        if (visited[x][y])
        {
            // Algorithme aléatoire dans le cas où on est déjà passé par une case
            // On choisira de préférence les cellules adjacentes sur lesquelles nous ne sommes jamais allés
            count = 0, min = 0;
            for (int i = 0; i < 4; i++) // Vérifier toutes les directions
            {
                if (can_go(x, y, maze, i))
                {
                    if (min > visited_value(visited, x, y, i) || count == 0)
                    {
                        min = visited_value(visited, x, y, i); // min vaudra donc la valeur la plus petite des cellules adjacentes
                    }
                    count += 1;
                }
            }
            if (count > 0) // Si une case est dispo
            {
                do
                {
                    getrandom(&dir, sizeof(dir), 0);
                    dir %= 4; // direction aléatoire
                }
                while ((visited_value(visited, x, y, dir) != min) || !can_go(x, y, maze, dir)); // on choisit une des cellules avec le moins de passages
            }
        }
        else
        {
            // Algorithme de main droite le cas échéant
            if (can_go(x, y, maze, (dir + 1) % 4)) // Déplacement à droite
            {
                dir = (dir + 1) % 4;
            }
            else if (can_go(x, y, maze, dir)) // Déplacement en face
            {
                // On ne change pas dir
            }
            else if (can_go(x, y, maze, (dir + 3) % 4)) // Déplacement à gauche
            {
                dir = (dir + 3) % 4;
            }
            else if (can_go(x, y, maze, (dir + 2) % 4)) // Déplacement en arrière
            {
                dir = (dir + 2) % 4;
            }
        }
        visited[x][y] = 1; // On marque la case actuelle comme visitée
        go(&x, &y, dir);


        if (show)
        {
            rect.x = x * dw + 1;
            rect.y = y * dh + 1;
            SDL_RenderFillRect(renderer, &rect);
            SDL_Delay(dm.refresh_rate); // delay customisable (actuellement à sa vitesse maximale)
            SDL_RenderPresent(renderer);
        }
        steps++;
    }

    // On désaloue notre double tableau
    for (int i = 0; i < maze.width; i++)
    {
        free(visited[i]);
    }
    free(visited);


    if (show)
    {
        SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetWindowTitle(window, "escaped");
        SDL_Delay(dm.refresh_rate);
        SDL_RenderPresent(renderer);
        wait_and_destroy_print_maze(renderer, window);
    }
    return steps;
}

// Algorithme de résolution main droite avec pondération sur le nombre de passage sur une cellule
int right_hand_p(const maze_t maze, int x, int y)
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    int dw, dh;
    if (pre_print_maze(maze, &renderer, &window, &dw, &dh) != 1)
    {
        return -1;
    }
    SDL_SetWindowTitle(window, "escaping");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255); // couleur customisable
    SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // la position actuelle
    SDL_RenderFillRect(renderer, &rect);
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer); // on affiche la position actuelle
    bool show = true;

    // Variables à déclarer avant la boucle
    unsigned int dir;
    getrandom(&dir, sizeof(dir), 0);
    dir %= 4; // direction aléatoire
    int count = 0;
    int** visited = malloc(maze.width * sizeof(int*));
    int min = 0;
    for (int i = 0; i < maze.width; i++)
    {
        visited[i] = malloc(maze.height * sizeof(int));
    }
    for (int i = 0; i < maze.width; i++)
    {
        for (int j = 0; j < maze.height; j++)
        {
            visited[i][j] = 0;
        }
    }

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event))
    {
        // on vide la file d'attente des événements
    }
    int step = 0;
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        if (show)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // on définit la couleur de fond en blanc
            if (has_wall_up(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, y * dh, (x + 1) * dw, y * dh);
            }
            if (has_wall_down(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, (y + 1) * dh, (x + 1) * dw, (y + 1) * dh);
            }
            if (has_wall_left(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, y * dh, x * dw, (y + 1) * dh);
            }
            if (has_wall_right(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, (x + 1) * dw, y * dh, (x + 1) * dw, (y + 1) * dh);
            }
            SDL_Delay(dm.refresh_rate);
            SDL_RenderPresent(renderer);
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
                    (event.type == SDL_KEYUP &&
                     (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER ||
                      event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
                {
                    printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                    destroy_print_maze(renderer, window);
                    return -1;
                }
                else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE)
                {
                    show = false;
                    destroy_print_maze(renderer, window);
                }
            }
            SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255); // on efface la position actuelle
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
        }
        step++;

        // Boucle de l'algo
        count = 0, min = 0;
        for (int i = 0; i < 4; i++) // Vérifier toutes les directions
        {
            if (can_go(x, y, maze, i))
            {
                if (min > visited_value(visited, x, y, i) || count == 0)
                {
                    min = visited_value(visited, x, y, i);
                }
                count += 1;
            }
        }

        if (can_go(x, y, maze, (dir + 1) % 4) && visited_value(visited, x, y, (dir + 1) % 4) == min) // Déplacement à droite
        {
            dir = (dir + 1) % 4;
        }
        else if (can_go(x, y, maze, dir) && visited_value(visited, x, y, dir) == min) // Déplacement en face
        {
            // On ne change pas dir
        }
        else if (can_go(x, y, maze, (dir + 3) % 4) && visited_value(visited, x, y, (dir + 3) % 4) == min) // Déplacement à gauche
        {
            dir = (dir + 3) % 4;
        }
        else if (can_go(x, y, maze, (dir + 2) % 4) && visited_value(visited, x, y, (dir + 2) % 4) == min) // Déplacement en arrière
        {
            dir = (dir + 2) % 4;
        }
        visited[x][y] += 1;
        go(&x, &y, dir);

        if (show)
        {
            rect.x = x * dw + 1;
            rect.y = y * dh + 1;
            SDL_RenderFillRect(renderer, &rect);
            SDL_Delay(dm.refresh_rate); // delay customisable (actuellement à sa vitesse maximale)
            SDL_RenderPresent(renderer);
        }
    }

    // Supression des tableaux
    for (int i = 0; i < maze.width; i++)
    {
        free(visited[i]);
    }
    free(visited);

    if (show)
    {
        SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetWindowTitle(window, "escaped");
        SDL_Delay(dm.refresh_rate);
        SDL_RenderPresent(renderer);
        wait_and_destroy_print_maze(renderer, window);
    }
    return step;
}

// Algorithme de résolution main droite avec pondération sur les cul de sac
int right_hand_de(const maze_t maze, int x, int y)
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    int dw, dh;
    if (pre_print_maze(maze, &renderer, &window, &dw, &dh) != 1)
    {
        return -1;
    }
    SDL_SetWindowTitle(window, "escaping");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255); // couleur customisable
    SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // la position actuelle
    SDL_RenderFillRect(renderer, &rect);
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer); // on affiche la position actuelle
    bool show = true;

    // Variables à déclarer avant la boucle
    unsigned int dir;
    getrandom(&dir, sizeof(dir), 0);
    dir %= 4; // direction aléatoire
    int count = 0;
    int** visited = malloc(maze.width * sizeof(int*));
    for (int i = 0; i < maze.width; i++)
    {
        visited[i] = malloc(maze.height * sizeof(int));
    }
    for (int i = 0; i < maze.width; i++)
    {
        for (int j = 0; j < maze.height; j++)
        {
            visited[i][j] = 0;
        }
    }

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event))
    {
        // on vide la file d'attente des événements
    }
    int step = 0;
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        if (show)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // on définit la couleur de fond en blanc
            if (has_wall_up(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, y * dh, (x + 1) * dw, y * dh);
            }
            if (has_wall_down(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, (y + 1) * dh, (x + 1) * dw, (y + 1) * dh);
            }
            if (has_wall_left(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, y * dh, x * dw, (y + 1) * dh);
            }
            if (has_wall_right(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, (x + 1) * dw, y * dh, (x + 1) * dw, (y + 1) * dh);
            }
            SDL_Delay(dm.refresh_rate);
            SDL_RenderPresent(renderer);
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
                    (event.type == SDL_KEYUP &&
                     (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER ||
                      event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
                {
                    printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                    destroy_print_maze(renderer, window);
                    return -1;
                }
                else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE)
                {
                    show = false;
                    destroy_print_maze(renderer, window);
                }
            }
            SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255); // on efface la position actuelle
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
        }
        step++;

        // Boucle de l'algo
        count = 0;
        for (int i = 0; i < 4; i++) // Vérifier toutes les directions
        {
            if (can_go(x, y, maze, i) && (visited_value(visited, x, y, i) != INT_MAX))
            {
                count += 1;
            }
        }

        if (count == 1)
        {
            visited[x][y] = INT_MAX;
        }

        if (can_go(x, y, maze, (dir + 1) % 4) && visited_value(visited, x, y, (dir + 1) % 4) != INT_MAX) // Déplacement à droite
        {
            dir = (dir + 1) % 4;
        }
        else if (can_go(x, y, maze, dir) && visited_value(visited, x, y, dir) != INT_MAX) // Déplacement en face
        {
            // On ne change pas dir
        }
        else if (can_go(x, y, maze, (dir + 3) % 4) && visited_value(visited, x, y, (dir + 3) % 4) != INT_MAX) // Déplacement à gauche
        {
            dir = (dir + 3) % 4;
        }
        else if (can_go(x, y, maze, (dir + 2) % 4) && visited_value(visited, x, y, (dir + 2) % 4) != INT_MAX) // Déplacement en arrière
        {
            dir = (dir + 2) % 4;
        }
        go(&x, &y, dir);

        if (show)
        {
            rect.x = x * dw + 1;
            rect.y = y * dh + 1;
            SDL_RenderFillRect(renderer, &rect);
            SDL_Delay(dm.refresh_rate); // delay customisable (actuellement à sa vitesse maximale)
            SDL_RenderPresent(renderer);
        }
    }

    // Supression des tableaux
    for (int i = 0; i < maze.width; i++)
    {
        free(visited[i]);
    }
    free(visited);

    if (show)
    {
        SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetWindowTitle(window, "escaped");
        SDL_Delay(dm.refresh_rate);
        SDL_RenderPresent(renderer);
        wait_and_destroy_print_maze(renderer, window);
    }
    return step;
}

// Algorithme de résolution main droite avec pondération sur le nombre de passage sur une cellule et les cul de sac
int right_hand_p_de(const maze_t maze, int x, int y)
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    int dw, dh;
    if (pre_print_maze(maze, &renderer, &window, &dw, &dh) != 1)
    {
        return -1;
    }
    SDL_SetWindowTitle(window, "escaping");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255); // couleur customisable
    SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // la position actuelle
    SDL_RenderFillRect(renderer, &rect);
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer); // on affiche la position actuelle
    bool show = true;

    // Variables à déclarer avant la boucle
    unsigned int dir;
    getrandom(&dir, sizeof(dir), 0);
    dir %= 4; // direction aléatoire
    int count = 0;
    int** visited = malloc(maze.width * sizeof(int*));
    int min = 0;
    for (int i = 0; i < maze.width; i++)
    {
        visited[i] = malloc(maze.height * sizeof(int));
    }
    for (int i = 0; i < maze.width; i++)
    {
        for (int j = 0; j < maze.height; j++)
        {
            visited[i][j] = 0;
        }
    }

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event))
    {
        // on vide la file d'attente des événements
    }
    int step = 0;
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        if (show)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // on définit la couleur de fond en blanc
            if (has_wall_up(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, y * dh, (x + 1) * dw, y * dh);
            }
            if (has_wall_down(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, (y + 1) * dh, (x + 1) * dw, (y + 1) * dh);
            }
            if (has_wall_left(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, y * dh, x * dw, (y + 1) * dh);
            }
            if (has_wall_right(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, (x + 1) * dw, y * dh, (x + 1) * dw, (y + 1) * dh);
            }
            SDL_Delay(dm.refresh_rate);
            SDL_RenderPresent(renderer);
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
                    (event.type == SDL_KEYUP &&
                     (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER ||
                      event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
                {
                    printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                    destroy_print_maze(renderer, window);
                    return -1;
                }
                else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE)
                {
                    show = false;
                    destroy_print_maze(renderer, window);
                }
            }
            SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255); // on efface la position actuelle
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
        }
        step++;

        // Boucle de l'algo
        count = 0, min = 0;
        for (int i = 0; i < 4; i++) // Vérifier toutes les directions
        {
            if (can_go(x, y, maze, i) && (visited_value(visited, x, y, i) != INT_MAX))
            {
                if (min > visited_value(visited, x, y, i) || count == 0)
                {
                    min = visited_value(visited, x, y, i);
                }
                count += 1;
            }
        }

        if (count == 1)
        {
            visited[x][y] = INT_MAX;
        }

        if (can_go(x, y, maze, (dir + 1) % 4) && visited_value(visited, x, y, (dir + 1) % 4) == min) // Déplacement à droite
        {
            dir = (dir + 1) % 4;
        }
        else if (can_go(x, y, maze, dir) && visited_value(visited, x, y, dir) == min) // Déplacement en face
        {
            // On ne change pas dir
        }
        else if (can_go(x, y, maze, (dir + 3) % 4) && visited_value(visited, x, y, (dir + 3) % 4) == min) // Déplacement à gauche
        {
            dir = (dir + 3) % 4;
        }
        else if (can_go(x, y, maze, (dir + 2) % 4) && visited_value(visited, x, y, (dir + 2) % 4) == min) // Déplacement en arrière
        {
            dir = (dir + 2) % 4;
        }
        if (visited[x][y] != INT_MAX)
            visited[x][y] += 1;
        go(&x, &y, dir);

        if (show)
        {
            rect.x = x * dw + 1;
            rect.y = y * dh + 1;
            SDL_RenderFillRect(renderer, &rect);
            SDL_Delay(dm.refresh_rate); // delay customisable (actuellement à sa vitesse maximale)
            SDL_RenderPresent(renderer);
        }
    }

    // Supression des tableaux
    for (int i = 0; i < maze.width; i++)
    {
        free(visited[i]);
    }
    free(visited);

    if (show)
    {
        SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetWindowTitle(window, "escaped");
        SDL_Delay(dm.refresh_rate);
        SDL_RenderPresent(renderer);
        wait_and_destroy_print_maze(renderer, window);
    }
    return step;
}

// Algorithme random avec pondération sur le nombre de passage sur une cellule
int random_p(const maze_t maze, int x, int y)
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    int dw, dh;
    if (pre_print_maze(maze, &renderer, &window, &dw, &dh) != 1)
    {
        return -1;
    }
    SDL_SetWindowTitle(window, "escaping");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255); // couleur customisable
    SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // la position actuelle
    SDL_RenderFillRect(renderer, &rect);
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer); // on affiche la position actuelle
    bool show = true;

    // Variables à déclarer avant la boucle
    unsigned int dir;
    getrandom(&dir, sizeof(dir), 0);
    dir %= 4; // direction aléatoire
    int count = 0;
    int** visited = malloc(maze.width * sizeof(int*));
    int min = 0;
    for (int i = 0; i < maze.width; i++)
    {
        visited[i] = malloc(maze.height * sizeof(int));
    }
    for (int i = 0; i < maze.width; i++)
    {
        for (int j = 0; j < maze.height; j++)
        {
            visited[i][j] = 0;
        }
    }

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event))
    {
        // on vide la file d'attente des événements
    }
    int step = 0;
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        if (show)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // on définit la couleur de fond en blanc
            if (has_wall_up(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, y * dh, (x + 1) * dw, y * dh);
            }
            if (has_wall_down(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, (y + 1) * dh, (x + 1) * dw, (y + 1) * dh);
            }
            if (has_wall_left(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, y * dh, x * dw, (y + 1) * dh);
            }
            if (has_wall_right(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, (x + 1) * dw, y * dh, (x + 1) * dw, (y + 1) * dh);
            }
            SDL_Delay(dm.refresh_rate);
            SDL_RenderPresent(renderer);
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
                    (event.type == SDL_KEYUP &&
                     (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER ||
                      event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
                {
                    printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                    destroy_print_maze(renderer, window);
                    return -1;
                }
                else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE)
                {
                    show = false;
                    destroy_print_maze(renderer, window);
                }
            }
            SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255); // on efface la position actuelle
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
        }
        step++;

        // La boucle
        count = 0, min = 0;
        for (int i = 0; i < 4; i++) // Vérifier toutes les directions
        {
            if (can_go(x, y, maze, i))
            {
                if (min > visited_value(visited, x, y, i) || count == 0)
                {
                    min = visited_value(visited, x, y, i);
                }
                count += 1;
            }
        }

        do
        {
            getrandom(&dir, sizeof(dir), 0);
            dir %= 4; // direction aléatoire
        }
        while (!(can_go(x, y, maze, dir) && visited_value(visited, x, y, dir) == min));
        visited[x][y] += 1;
        go(&x, &y, dir);

        if (show)
        {
            rect.x = x * dw + 1;
            rect.y = y * dh + 1;
            SDL_RenderFillRect(renderer, &rect);
            SDL_Delay(dm.refresh_rate); // delay customisable (actuellement à sa vitesse maximale)
            SDL_RenderPresent(renderer);
        }
    }

    // Supression des tableaux
    for (int i = 0; i < maze.width; i++)
    {
        free(visited[i]);
    }
    free(visited);

    if (show)
    {
        SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetWindowTitle(window, "escaped");
        SDL_Delay(dm.refresh_rate);
        SDL_RenderPresent(renderer);
        wait_and_destroy_print_maze(renderer, window);
    }
    return step;
}

// Algorithme random avec pondération sur les cul de sac
int random_de(const maze_t maze, int x, int y)
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    int dw, dh;
    if (pre_print_maze(maze, &renderer, &window, &dw, &dh) != 1)
    {
        return -1;
    }
    SDL_SetWindowTitle(window, "escaping");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255); // couleur customisable
    SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // la position actuelle
    SDL_RenderFillRect(renderer, &rect);
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer); // on affiche la position actuelle
    bool show = true;

    // Variables à déclarer avant la boucle
    unsigned int dir;
    getrandom(&dir, sizeof(dir), 0);
    dir %= 4; // direction aléatoire
    int count = 0;
    int** visited = malloc(maze.width * sizeof(int*));
    for (int i = 0; i < maze.width; i++)
    {
        visited[i] = malloc(maze.height * sizeof(int));
    }
    for (int i = 0; i < maze.width; i++)
    {
        for (int j = 0; j < maze.height; j++)
        {
            visited[i][j] = 0;
        }
    }


    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event))
    {
        // on vide la file d'attente des événements
    }
    int step = 0;
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        if (show)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // on définit la couleur de fond en blanc
            if (has_wall_up(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, y * dh, (x + 1) * dw, y * dh);
            }
            if (has_wall_down(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, (y + 1) * dh, (x + 1) * dw, (y + 1) * dh);
            }
            if (has_wall_left(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, y * dh, x * dw, (y + 1) * dh);
            }
            if (has_wall_right(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, (x + 1) * dw, y * dh, (x + 1) * dw, (y + 1) * dh);
            }
            SDL_Delay(dm.refresh_rate);
            SDL_RenderPresent(renderer);
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
                    (event.type == SDL_KEYUP &&
                     (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER ||
                      event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
                {
                    printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                    destroy_print_maze(renderer, window);
                    return -1;
                }
                else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE)
                {
                    show = false;
                    destroy_print_maze(renderer, window);
                }
            }
            SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255); // on efface la position actuelle
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
        }
        step++;

        if (count == 1)
        {
            visited[x][y] = INT_MAX;
        }

        do
        {
            getrandom(&dir, sizeof(dir), 0);
            dir %= 4; // direction aléatoire
        }
        while (!(can_go(x, y, maze, dir) && visited_value(visited, x, y, dir) != INT_MAX));
        go(&x, &y, dir);

        if (show)
        {
            rect.x = x * dw + 1;
            rect.y = y * dh + 1;
            SDL_RenderFillRect(renderer, &rect);
            SDL_Delay(dm.refresh_rate); // delay customisable (actuellement à sa vitesse maximale)
            SDL_RenderPresent(renderer);
        }
    }

    // Supression des tableaux
    for (int i = 0; i < maze.width; i++)
    {
        free(visited[i]);
    }
    free(visited);

    if (show)
    {
        SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetWindowTitle(window, "escaped");
        SDL_Delay(dm.refresh_rate);
        SDL_RenderPresent(renderer);
        wait_and_destroy_print_maze(renderer, window);
    }
    return step;
}

// Algorithme random avec pondération sur les cul de sac et le nombre de passage sur une cellule
int random_p_de(const maze_t maze, int x, int y)
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    int dw, dh;
    if (pre_print_maze(maze, &renderer, &window, &dw, &dh) != 1)
    {
        return -1;
    }
    SDL_SetWindowTitle(window, "escaping");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255); // couleur customisable
    SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // la position actuelle
    SDL_RenderFillRect(renderer, &rect);
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer); // on affiche la position actuelle
    bool show = true;

    // Variables à déclarer avant la boucle
    unsigned int dir;
    getrandom(&dir, sizeof(dir), 0);
    dir %= 4; // direction aléatoire
    int count = 0;
    int** visited = malloc(maze.width * sizeof(int*));
    int min = 0;
    for (int i = 0; i < maze.width; i++)
    {
        visited[i] = malloc(maze.height * sizeof(int));
    }
    for (int i = 0; i < maze.width; i++)
    {
        for (int j = 0; j < maze.height; j++)
        {
            visited[i][j] = 0;
        }
    }


    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event))
    {
        // on vide la file d'attente des événements
    }
    int step = 0;
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        if (show)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // on définit la couleur de fond en blanc
            if (has_wall_up(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, y * dh, (x + 1) * dw, y * dh);
            }
            if (has_wall_down(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, (y + 1) * dh, (x + 1) * dw, (y + 1) * dh);
            }
            if (has_wall_left(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, y * dh, x * dw, (y + 1) * dh);
            }
            if (has_wall_right(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, (x + 1) * dw, y * dh, (x + 1) * dw, (y + 1) * dh);
            }
            SDL_Delay(dm.refresh_rate);
            SDL_RenderPresent(renderer);
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
                    (event.type == SDL_KEYUP &&
                     (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER ||
                      event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
                {
                    printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                    destroy_print_maze(renderer, window);
                    return -1;
                }
                else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE)
                {
                    show = false;
                    destroy_print_maze(renderer, window);
                }
            }
            SDL_SetRenderDrawColor(renderer, 70, 70, 70, 255); // on efface la position actuelle
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
        }
        step++;

        // La boucle
        count = 0, min = 0;
        for (int i = 0; i < 4; i++) // Vérifier toutes les directions
        {
            if (can_go(x, y, maze, i) && visited_value(visited, x, y, i) != INT_MAX)
            {
                if (min > visited_value(visited, x, y, i) || count == 0)
                {
                    min = visited_value(visited, x, y, i);
                }
                count += 1;
            }
        }

        // Création de cul de sac
        if (count == 1)
        {
            visited[x][y] = INT_MAX;
        }

        do
        {
            getrandom(&dir, sizeof(dir), 0);
            dir %= 4; // direction aléatoire
        }
        while (!(can_go(x, y, maze, dir) && visited_value(visited, x, y, dir) == min));
        if (visited[x][y] != INT_MAX)
            visited[x][y] += 1;
        go(&x, &y, dir);

        if (show)
        {
            rect.x = x * dw + 1;
            rect.y = y * dh + 1;
            SDL_RenderFillRect(renderer, &rect);
            SDL_Delay(dm.refresh_rate); // delay customisable (actuellement à sa vitesse maximale)
            SDL_RenderPresent(renderer);
        }
    }

    // Supression des tableaux
    for (int i = 0; i < maze.width; i++)
    {
        free(visited[i]);
    }
    free(visited);

    if (show)
    {
        SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetWindowTitle(window, "escaped");
        SDL_Delay(dm.refresh_rate);
        SDL_RenderPresent(renderer);
        wait_and_destroy_print_maze(renderer, window);
    }
    return step;
}
