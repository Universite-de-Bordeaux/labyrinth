#include "escape.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include "limits.h"
#include "struct.h"

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
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
                (event.type == SDL_KEYUP &&
                 (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
            {
                printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                destroy_print_maze(renderer, window);
                return -1;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // on efface la position actuelle
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);

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


        rect.x = x * dw + 1;
        rect.y = y * dh + 1;
        SDL_RenderFillRect(renderer, &rect);
        SDL_Delay(dm.refresh_rate);
        SDL_RenderPresent(renderer);
        steps++;
    }
    SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetWindowTitle(window, "escaped");
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer);
    wait_and_destroy_print_maze(renderer, window);
    return steps;
}

int try_direction(const maze_t maze, int x, int y)
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

    unsigned char tendance = 0; // la direction qu'on va suivre le plus possible
    const int CONF = 50; // le nombre de fois qu'on va conserver la tendance si elle n'est pas possible
    int confiance = CONF; // le nombre de fois qu'on va conserver la tendance si elle n'est pas possible

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event))
    {
        // on vide la file d'attente des événements
    }
    int steps = 0;
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
                (event.type == SDL_KEYUP &&
                 (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
            {
                printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                destroy_print_maze(renderer, window);
                return -1;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // on efface la position actuelle
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);

        switch (tendance)
        {
        case 0: // plutot bas droite
            if (!has_wall_down(maze, x, y))
            {
                y++;
            }
            else if (!has_wall_right(maze, x, y))
            {
                x++;
                confiance--;
            }
            else if (!has_wall_left(maze, x, y))
            {
                x--;
                confiance -= 2;
                tendance = 1;
            }
            else
            {
                confiance = 0;
            }
            break;
        case 1: // plutot bas gauche
            if (!has_wall_down(maze, x, y))
            {
                y++;
            }
            else if (!has_wall_left(maze, x, y))
            {
                x--;
                confiance--;
            }
            else if (!has_wall_right(maze, x, y))
            {
                x++;
                confiance -= 2;
                tendance = 0;
            }
            else
            {
                confiance = 0;
            }
            break;
        case 2: // plutot haut droite
            if (!has_wall_up(maze, x, y))
            {
                y--;
            }
            else if (!has_wall_right(maze, x, y))
            {
                x++;
                confiance--;
            }
            else if (!has_wall_left(maze, x, y))
            {
                x--;
                confiance -= 2;
                tendance = 3;
            }
            else
            {
                confiance = 0;
            }
            break;
        case 3: // plutot haut gauche
            if (!has_wall_up(maze, x, y))
            {
                y--;
            }
            else if (!has_wall_left(maze, x, y))
            {
                x--;
                confiance--;
            }
            else if (!has_wall_right(maze, x, y))
            {
                x++;
                confiance -= 2;
                tendance = 2;
            }
            else
            {
                confiance = 0;
            }
            break;
        case 4: // plutot gauche bas
            {
                if (!has_wall_left(maze, x, y))
                {
                    x--;
                }
                else if (!has_wall_down(maze, x, y))
                {
                    y++;
                    confiance--;
                }
                else if (!has_wall_up(maze, x, y))
                {
                    y--;
                    confiance -= 2;
                    tendance = 5;
                }
                else
                {
                    confiance = 0;
                }
            }
            break;
        case 5: // plutot gauche haut
            {
                if (!has_wall_left(maze, x, y))
                {
                    x--;
                }
                else if (!has_wall_up(maze, x, y))
                {
                    y--;
                    confiance--;
                }
                else if (!has_wall_down(maze, x, y))
                {
                    y++;
                    confiance -= 2;
                    tendance = 4;
                }
                else
                {
                    confiance = 0;
                }
            }
            break;
        case 6: // plutot droite bas
            {
                if (!has_wall_right(maze, x, y))
                {
                    x++;
                }
                else if (!has_wall_down(maze, x, y))
                {
                    y++;
                    confiance--;
                }
                else if (!has_wall_up(maze, x, y))
                {
                    y--;
                    confiance -= 2;
                    tendance = 7;
                }
                else
                {
                    confiance = 0;
                }
            }
            break;
        case 7: // plutot droite haut
            {
                if (!has_wall_right(maze, x, y))
                {
                    x++;
                }
                else if (!has_wall_up(maze, x, y))
                {
                    y--;
                    confiance--;
                }
                else if (!has_wall_down(maze, x, y))
                {
                    y++;
                    confiance -= 2;
                    tendance = 6;
                }
                else
                {
                    confiance = 0;
                }
            }
            break;

        default:
            fprintf(stderr, "Erreur dans la fonction try_direction : tendance invalide.\n");
            printf("tendance = %d\n", tendance);
            destroy_print_maze(renderer, window);
            exit(EXIT_FAILURE);
            break;
        }

        if (confiance <= 0)
        {
            const unsigned char temp = tendance % 2 == 0 ? tendance : tendance - 1;
            bool trust[4];
            trust[0] = !has_wall_down(maze, x, y);
            trust[1] = !has_wall_up(maze, x, y);
            trust[2] = !has_wall_left(maze, x, y);
            trust[3] = !has_wall_right(maze, x, y);
            trust[temp / 2] = false;
            if (!trust[0] && !trust[1] && !trust[2] && !trust[3])
            {
                printf("Aucune direction possible\n");
                SDL_SetWindowTitle(window, "failure");
                wait_and_destroy_print_maze(renderer, window);
                return -1;
            }

            do
            {
                getrandom(&tendance, sizeof(char), 0);
                tendance %= 8;
            }
            while (!trust[tendance / 2]);

            getrandom(&confiance, sizeof(int), 0);
            confiance = abs(confiance) % CONF;
        }

        rect.x = x * dw + 1;
        rect.y = y * dh + 1;
        SDL_RenderFillRect(renderer, &rect);
        SDL_Delay(dm.refresh_rate); // delay customisable (actuellement à sa vitesse maximale)
        SDL_RenderPresent(renderer);
        steps++;
    }
    SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetWindowTitle(window, "escaped");
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer);

    wait_and_destroy_print_maze(renderer, window);
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

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event))
    {
        // on vide la file d'attente des événements
    }
    int steps = 0;
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
                (event.type == SDL_KEYUP &&
                 (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
            {
                printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                destroy_print_maze(renderer, window);
                return -1;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // on efface la position actuelle
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);

        // le corps de l'algorithme
        // votre déplacement
        if (x < maze.width - 1 && rand() % 2 == 0)
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

        rect.x = x * dw + 1;
        rect.y = y * dh + 1;
        SDL_RenderFillRect(renderer, &rect);
        SDL_Delay(dm.refresh_rate); // delay customisable (actuellement à sa vitesse maximale)
        SDL_RenderPresent(renderer);
        steps++;
    }
    SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetWindowTitle(window, "escaped");
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer);
    wait_and_destroy_print_maze(renderer, window);
    return steps;
}

// fonction pour savoir si une cellule a des cellules adjacentes accessibles
// renvoie true si la cellule n'a pas de cellules adjacentes accessibles
// renvoie false si la cellule a des cellules adjacentes accessibles
static bool has_accessible_cells(int x, int y, bool_tab visited, maze_t maze, int* dir_acc)
{
    if (!has_wall_right(maze, x, y) && !get_bool(visited, x + 1, y))
    {
        dir_acc[0] = 1;
    }
    if (!has_wall_down(maze, x, y) && !get_bool(visited, x, y + 1))
    {
        dir_acc[1] = 1;
    }
    if (!has_wall_left(maze, x, y) && !get_bool(visited, x - 1, y))
    {
        dir_acc[2] = 1;
    }
    if (!has_wall_up(maze, x, y) && !get_bool(visited, x, y - 1))
    {
        dir_acc[3] = 1;
    }
    return dir_acc[0] && dir_acc[1] && dir_acc[2] && dir_acc[3];
}

int hunt_kill_escape(maze_t maze, int x, int y)
{
    // on crée un bool_tab pour stocker les cellules visitées
    // on marque la cellule actuelle comme visitée
    bool_tab visited = create_booltab(maze.width, maze.height);
    set_true(visited, x, y);
    int* dir_acc = malloc(4 * sizeof(int));
    int old_dir = -1;
    int steps = 0;
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        // on cherche une cellule accessible
        // tant qu'on est dans une boucle
        while (has_accessible_cells(x, y, visited, maze, dir_acc))
        {
            // on va dans une direction aléatoire autre que notre direction d'origine si possible
            int size = 4;
            char c = '\0';
            while (size > 0)
            { // tant qu'il reste des directions possibles
                char dir[4] = {'R', 'D', 'L', 'U'}; // tableau des directions possibles
                size = 4;
                while (size > 0) // tant qu'il reste des directions possibles et qu'on n'en a pas tiré de valide
                {
                    const int r = rand() % size; // NOLINT(*-msc50-cpp)
                    c = dir[r];
                    if (r == old_dir)
                    {
                        break;
                    }
                    if (c == 'R' && x + 1 < maze.width && !has_wall_right(maze, x + 1, y)) // si on a tiré la direction droite et qu'on peut y aller
                    {
                        x++; // on se déplace
                        old_dir = 2;
                        break;
                    }
                    if (c == 'L' && x > 0 && !has_wall_left(maze, x - 1, y)) // si on a tiré la direction gauche et qu'on peut y aller
                    {
                        x--; // on se déplace
                        old_dir = 0;
                        break;
                    }
                    if (c == 'D' && y + 1 < maze.height && !has_wall_down(maze, x, y + 1)) // si on a tiré la direction bas et qu'on peut y aller
                    {
                        y++; // on se déplace
                        old_dir = 3;
                        break;
                    }
                    if (y > 0 && !has_wall_up(maze, x, y)) // il ne reste plus que la direction haut, on regarde si on peut y aller
                    {
                        y--; // on se déplace
                        old_dir = 1;
                        break;
                    }
                    // sinon on supprime la direction prise du tableau
                    {
                        for (int i = r; i < size - 1; i++)
                        {
                            dir[i] = dir[i + 1];
                        }
                        size--;
                    }
                }
                if (size == 0) // la seule direction possible est la direction de là ou on vient
                {
                    if (old_dir == 0)
                    {
                        x++;
                        old_dir = 2;
                    }
                    else if (old_dir == 1)
                    {
                        y++;
                        old_dir = 3;
                    }
                    else if (old_dir == 2)
                    {
                        x--;
                        old_dir = 0;
                    }
                    else if (old_dir == 3)
                    {
                        y--;
                        old_dir = 1;
                    }
                    break;
                }
                set_true(visited, x, y);
            }
        }
        // tant qu'on a des cellules adjacentes non visitées
        // on essaie d'aller à droite, puis en bas, puis à gauche, puis en haut
        while (has_accessible_cells(x, y, visited, maze, dir_acc))
        {
            int i = 0;
            while (dir_acc[i] == 0)
            {
                i++;
            }
            if (i == 0)
            {
                x++;
                old_dir = 2;
            }
            else if (i == 1)
            {
                y++;
                old_dir = 3;
            }
            else if (i == 2)
            {
                x--;
                old_dir = 0;
            }
            else if (i == 3)
            {
                y--;
                old_dir = 1;
            }
        }
    }
    free_booltab(visited);
    free(dir_acc);
}

enum direction
{
    EAST = 0,
    SUD,
    OUEST,
    NORD
};

// Fonction qui retourne un booléen correspondant à la possibilité d'aller dans une direction donnée
static bool can_go(int x, int y, maze_t maze, char dir)
{
    switch (dir)
    {
    case EAST:
        return (!has_wall_right(maze, x, y));
    case SUD:
        return (!has_wall_down(maze, x, y));
    case OUEST:
        return (!has_wall_left(maze, x, y));
    case NORD:
        return (!has_wall_up(maze, x, y));
    default:
        fprintf(stderr, "Error in can_go, dir must be within 0 and 3, received %d\n", dir);
        exit(EXIT_FAILURE);
    }
}

static int go(int* x, int* y, char dir)
{
    switch (dir)
    {
    case EAST:
        (*x)++;
        break;
    case SUD:
        (*y)++;
        break;
    case OUEST:
        (*x)--;
        break;
    case NORD:
        (*y)--;
        break;
    default:
        fprintf(stderr, "Error in go, dir must be within 0 and 3, received %d\n", dir);
        exit(EXIT_FAILURE);
    }
}

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

    // vos variables à déclarer avant la boucle
    char dir = 1; // on commence par aller à droite

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event))
    {
        // on vide la file d'attente des événements
    }
    int steps = 0;
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
                (event.type == SDL_KEYUP &&
                 (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
            {
                printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                destroy_print_maze(renderer, window);
                return -1;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // on efface la position actuelle
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
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


        rect.x = x * dw + 1;
        rect.y = y * dh + 1;
        SDL_RenderFillRect(renderer, &rect);
        SDL_Delay(dm.refresh_rate); // delay customisable (actuellement à sa vitesse maximale)
        SDL_RenderPresent(renderer);
        steps++;
    }
    SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetWindowTitle(window, "escaped");
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer);
    wait_and_destroy_print_maze(renderer, window);
    return steps;
}

int right_hand_random(const maze_t maze, int x, int y)
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

    // Variables à déclarer avant la boucle
    char dir = rand() % 4; // on commence par une direction aléatoire
    char count = 0;
    bool_tab visited = create_booltab(maze.width, maze.height);

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event))
    {
        // on vide la file d'attente des événements
    }
    int steps = 0;
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
                (event.type == SDL_KEYUP &&
                 (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
            {
                printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                destroy_print_maze(renderer, window);
                return -1;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // on efface la position actuelle
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);

        // Boucle de l'algo

        if (get_bool(visited, x, y))
        {
            int possible_dirs[4] = {0, 0, 0, 0};
            count = 0;
            for (int i = 0; i < 4; i++) // Vérifier toutes les directions
            {
                if (can_go(x, y, maze, i))
                {
                    go(&x, &y, i);
                    if (!get_bool(visited, x, y))
                    {
                        possible_dirs[i] = 1;
                        count += 1;
                    }
                    go(&x, &y, (i + 2) % 4);
                }
            }
            if (count > 0) // Si une case est dispo
            {
                do
                {
                    dir = rand() % 4;
                }
                while (!possible_dirs[(int)dir]);
            }
        }
        if (count == 0)
        {
            int possible_dirs[4] = {0, 0, 0, 0};
            for (int i = 0; i < 4; i++) // Vérifier toutes les directions
            {
                if (can_go(x, y, maze, i))
                {
                    possible_dirs[i] = 1;
                }
            }
            do
            {
                dir = rand() % 4;
            }
            while (!possible_dirs[(int)dir]);
        }
        if (!get_bool(visited, x, y))
        {
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
        set_true(visited, x, y);
        go(&x, &y, dir);


        rect.x = x * dw + 1;
        rect.y = y * dh + 1;
        SDL_RenderFillRect(renderer, &rect);
        SDL_Delay(dm.refresh_rate); // delay customisable (actuellement à sa vitesse maximale)
        SDL_RenderPresent(renderer);
        steps++;
    }
    free_booltab(visited);
    SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetWindowTitle(window, "escaped");
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer);
    wait_and_destroy_print_maze(renderer, window);
    return steps;
}

int right_hand_random_pond(const maze_t maze, int x, int y)
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

    // Variables à déclarer avant la boucle
    char dir = rand() % 4; // on commence par une direction aléatoire
    char count = 0;
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
    int steps = 0;
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
                (event.type == SDL_KEYUP &&
                 (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
            {
                printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                destroy_print_maze(renderer, window);
                return -1;
            }
        }

        // Boucle de l'algo

        if (visited[x][y])
        {
            int possible_dirs[4] = {0, 0, 0, 0};
            count = 0, min = 0;
            for (int i = 0; i < 4; i++) // Vérifier toutes les directions
            {
                if (can_go(x, y, maze, i))
                {
                    go(&x, &y, i);
                    if (min > visited[x][y] || count == 0)
                    {
                        min = visited[x][y];
                    }
                    possible_dirs[i] = visited[x][y];
                    count += 1;

                    go(&x, &y, (i + 2) % 4);
                }
                else
                {
                    possible_dirs[i] = INT_MAX;
                }
            }
            if (count > 0) // Si une case est dispo
            {
                do
                {
                    dir = rand() % 4;
                }
                while (!(possible_dirs[(int)dir] == min));
            }
        }
        if (count == 0)
        {
            int possible_dirs[4] = {0, 0, 0, 0};
            for (int i = 0; i < 4; i++) // Vérifier toutes les directions
            {
                if (can_go(x, y, maze, i))
                {
                    possible_dirs[i] = 1;
                }
            }
            do
            {
                dir = rand() % 4;
            }
            while (!possible_dirs[(int)dir]);
        }
        if (!visited[x][y])
        {
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
        visited[x][y] += 1;
        go(&x, &y, dir);

        rect.x = x * dw + 1;
        rect.y = y * dh + 1;
        SDL_RenderFillRect(renderer, &rect);
        SDL_Delay(dm.refresh_rate); // delay customisable (actuellement à sa vitesse maximale)
        SDL_RenderPresent(renderer);
        steps++;
    }

    // Supression des tableaux
    for (int i = 0; i < maze.width; i++)
    {
        free(visited[i]);
    }
    free(visited);

    SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetWindowTitle(window, "escaped");
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer);
    wait_and_destroy_print_maze(renderer, window);
    return steps;
}
