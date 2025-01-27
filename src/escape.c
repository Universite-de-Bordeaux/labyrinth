#include "escape.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include "struct.h"

void ESCAPE_TYPE(const maze_t maze, int x, int y)
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    int dw, dh;
    if (initial_print_maze(maze, &renderer, &window, &dw, &dh) != 1)
    {
        return;
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
    int truc = 18;
    char machin = 5;

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event))
    {
        // on vide la file d'attente des événements
    }
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
                return;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // on efface la position actuelle
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);

        // le corps de l'algorithme
        // votre déplacement
        truc++;
        if (truc == 49)
        {
            machin = 3;
            truc = machin + 1;
        }

        rect.x = x * dw + 1;
        rect.y = y * dh + 1;
        SDL_RenderFillRect(renderer, &rect);
        SDL_Delay(dm.refresh_rate); // delay customisable (actuellement à sa vitesse maximale)
        SDL_RenderPresent(renderer);
    }
    SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetWindowTitle(window, "escaped");
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer);
    wait_and_destroy_print_maze(renderer, window);
}

void random_escape(const maze_t maze, int x, int y)
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    int dw, dh;
    if (initial_print_maze(maze, &renderer, &window, &dw, &dh) != 1)
    {
        return;
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
                return;
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
    }
    SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetWindowTitle(window, "escaped");
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer);
    wait_and_destroy_print_maze(renderer, window);
}

void try_direction(const maze_t maze, int x, int y)
{
    SDL_Renderer* renderer;
    SDL_Window* window;
    int dw, dh;
    if (initial_print_maze(maze, &renderer, &window, &dw, &dh) != 1)
    {
        return;
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
    int conf[4] = {CONF, CONF, CONF, CONF}; // la probabilité de récuper une tendance

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event))
    {
        // on vide la file d'attente des événements
    }
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
                return;
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
                confiance--;
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
                confiance--;
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
                confiance--;
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
                confiance--;
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
            }
            else
            {
                confiance = 0;
            }
            break;
        case 4: // plutot gauche bas
            if (!has_wall_left(maze, x, y))
            {
                x--;
                confiance--;
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
            }
            else
            {
                confiance = 0;
            }
            break;
        case 5: // plutot gauche haut
            if (!has_wall_left(maze, x, y))
            {
                x--;
                confiance--;
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
            }
            else
            {
                confiance = 0;
            }
            break;
        case 6: // plutot droite bas
            if (!has_wall_right(maze, x, y))
            {
                x++;
                confiance--;
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
            }
            else
            {
                confiance = 0;
            }
            break;
        case 7: // plutot droite haut
            if (!has_wall_right(maze, x, y))
            {
                x++;
                confiance--;
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
            }
            else
            {
                confiance = 0;
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
            conf[temp / 2]--;
            if(conf[temp / 2] <= 0)
            {
                for(int i = 0; i < 4; i++)
                {
                    conf[i] += CONF;
                }
            }

            int sum = 0;
            int temp2[4] = {0};
            if (trust[0])
            {
                sum += conf[0];
                temp2[0] = conf[0];
            }
            else
            {
                temp2[0] = conf[0];
                conf[0] = 0;
            }
            if (trust[1])
            {
                sum += conf[1];
                temp2[1] = conf[1];
            }
            else
            {
                temp2[1] = conf[1];
                conf[1] = 0;
            }
            if (trust[2])
            {
                sum += conf[2];
                temp2[2] = conf[2];
            }
            else
            {
                temp2[2] = conf[2];
                conf[2] = 0;
            }
            if (trust[3])
            {
                sum += conf[3];
                temp2[3] = conf[3];
            }
            else
            {
                temp2[3] = conf[3];
                conf[3] = 0;
            }
            sum -= conf[temp / 2];
            if (!trust[0] && !trust[1] && !trust[2] && !trust[3])
            {
                printf("Aucune direction possible\n");
                wait_and_destroy_print_maze(renderer, window);
                return;
            }

            do
            {
                getrandom(&tendance, sizeof(char), 0);
                tendance %= sum;
                if(tendance < conf[0])
                {
                    tendance = 0;
                }
                else if(tendance < conf[0] + conf[1])
                {
                    tendance = 1;
                }
                else if(tendance < conf[0] + conf[1] + conf[2])
                {
                    tendance = 2;
                }
                else
                {
                    tendance = 3;
                }
            }
            while (!trust[tendance]);

            getrandom(&confiance, sizeof(int), 0);
            confiance = (abs(confiance) % CONF) + 1;
            tendance = tendance * 2 + (confiance % 2);
            for(int i = 0; i < 4; i++){
                conf[i] = temp2[i];
            }
        }
        rect.x = x * dw + 1;
        rect.y = y * dh + 1;
        SDL_RenderFillRect(renderer, &rect);
        SDL_Delay(dm.refresh_rate); // delay customisable (actuellement à sa vitesse maximale)
        SDL_RenderPresent(renderer);
    }
    SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetWindowTitle(window, "escaped");
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer);
    wait_and_destroy_print_maze(renderer, window);
}

void cheat_escape(const maze_t maze, int x, int y){
    SDL_Renderer* renderer;
    SDL_Window* window;
    int dw, dh;
    if (initial_print_maze(maze, &renderer, &window, &dw, &dh) != 1)
    {
        return;
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
                return;
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
    }
    SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetWindowTitle(window, "escaped");
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer);
    wait_and_destroy_print_maze(renderer, window);
}

void left_hand(const maze_t maze, int x, int y){
    SDL_Renderer* renderer;
    SDL_Window* window;
    int dw, dh;
    if (initial_print_maze(maze, &renderer, &window, &dw, &dh) != 1)
    {
        return;
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
                return;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // on efface la position actuelle
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);



        rect.x = x * dw + 1;
        rect.y = y * dh + 1;
        SDL_RenderFillRect(renderer, &rect);
        SDL_Delay(dm.refresh_rate); // delay customisable (actuellement à sa vitesse maximale)
        SDL_RenderPresent(renderer);
    }
    SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetWindowTitle(window, "escaped");
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer);
    wait_and_destroy_print_maze(renderer, window);
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

void hunt_kill_escape(maze_t maze, int x, int y)
{
    // on crée un bool_tab pour stocker les cellules visitées
    // on marque la cellule actuelle comme visitée
    bool_tab visited = create_booltab(maze.width, maze.height);
    set_true(visited, x, y);
    int* dir_acc = malloc(4 * sizeof(int));
    int old_dir = -1;
    while (x != maze.width - 1 || y != maze.height)
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
