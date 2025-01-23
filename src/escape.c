#include "escape.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>

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
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255); //couleur customisable
    SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; //la position actuelle
    SDL_RenderFillRect(renderer, &rect);
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer); //on affiche la position actuelle

    //vos variables à déclarer avant la boucle
    int truc = 18;
    char machin = 5;

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event)) {
        //on vide la file d'attente des événements
    }
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
            (event.type == SDL_KEYUP && (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
            {
                printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                destroy_print_maze(renderer, window);
                return;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //on efface la position actuelle
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);

        //le corps de l'algorithme
        //votre déplacement
        truc++;
        if (truc == 49)
        {
            machin = 3;
            truc = machin + 1;
        }

        rect.x = x * dw + 1;
        rect.y = y * dh + 1;
        SDL_RenderFillRect(renderer, &rect);
        SDL_Delay(dm.refresh_rate); //delay customisable (actuellement à sa vitesse maximale)
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
    SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; //la position actuelle
    SDL_RenderFillRect(renderer, &rect);
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer); //on affiche la position actuelle

    unsigned char choix; //la direction qu'on va suivre le plus possible
    getrandom(&choix, sizeof(char), 0);
    choix %= 4;

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event)) {
        //on vide la file d'attente des événements
    }
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
            (event.type == SDL_KEYUP && (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
            {
                printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                destroy_print_maze(renderer, window);
                return;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //on efface la position actuelle
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
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255); //couleur customisable
    SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; //la position actuelle
    SDL_RenderFillRect(renderer, &rect);
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer); //on affiche la position actuelle

    unsigned char tendance = 0; //la direction qu'on va suivre le plus possible
    const int CONF = 50; //le nombre de fois qu'on va conserver la tendance si elle n'est pas possible
    int confiance = CONF; //le nombre de fois qu'on va conserver la tendance si elle n'est pas possible

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event)) {
        //on vide la file d'attente des événements
    }
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
            (event.type == SDL_KEYUP && (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
            {
                printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                destroy_print_maze(renderer, window);
                return;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //on efface la position actuelle
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);

        switch (tendance)
        {
            case 0: //plutot bas droite
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
            case 1: //plutot bas gauche
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
            case 2: //plutot haut droite
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
            case 3: //plutot haut gauche
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
            case 4: //plutot gauche bas
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
            case 5: //plutot gauche haut
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
            case 6: //plutot droite bas
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
            case 7: //plutot droite haut
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
                wait_and_destroy_print_maze(renderer, window);
                return;
            }

            do
            {
                getrandom(&tendance, sizeof(char), 0);
                tendance %= 8;
            }while (!trust[tendance / 2]);

            getrandom(&confiance, sizeof(int), 0);
            confiance = abs(confiance) % CONF;
        }

        rect.x = x * dw + 1;
        rect.y = y * dh + 1;
        SDL_RenderFillRect(renderer, &rect);
        SDL_Delay(dm.refresh_rate); //delay customisable (actuellement à sa vitesse maximale)
        SDL_RenderPresent(renderer);
    }
    SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetWindowTitle(window, "escaped");
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer);

    wait_and_destroy_print_maze(renderer, window);
}

void cheat_escape(const maze_t maze, int x, int y)
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
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255); //couleur customisable
    SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; //la position actuelle
    SDL_RenderFillRect(renderer, &rect);
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer); //on affiche la position actuelle

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event)) {
        //on vide la file d'attente des événements
    }
    while (x != maze.width - 1 || y != maze.height - 1)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
            (event.type == SDL_KEYUP && (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
            {
                printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                destroy_print_maze(renderer, window);
                return;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //on efface la position actuelle
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);

        //le corps de l'algorithme
        //votre déplacement
        if (x < maze.width - 1 && rand () % 2 == 0)
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
        SDL_Delay(dm.refresh_rate); //delay customisable (actuellement à sa vitesse maximale)
        SDL_RenderPresent(renderer);
    }
    SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetWindowTitle(window, "escaped");
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer);
    wait_and_destroy_print_maze(renderer, window);
}

