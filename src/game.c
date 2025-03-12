#include "game.h"

static void reprint_maze(const maze_t maze, SDL_Renderer *renderer, const int dh, const int dw)
{
    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0)
    { // on obtient le mode d'affichage de l'écran
        fprintf(stderr, "Erreur lors de l'obtention du mode d'affichage : %s\n", SDL_GetError());
        SDL_Quit();
        return;
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // on définit la couleur de fond en blanc
    for (int x = 0; x < maze.width; x++)
    {
        for (int y = 0; y < maze.height; y++)
        {
            if (has_wall_down(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, x * dw, (y + 1) * dh - 1, (x + 1) * dw - 1,
                                   (y + 1) * dh - 1); // on dessine un mur en bas
            }
            if (has_wall_right(maze, x, y))
            {
                SDL_RenderDrawLine(renderer, (x + 1) * dw - 1, y * dh, (x + 1) * dw - 1,
                                   (y + 1) * dh - 1); // on dessine un mur à droite
            }
        }
    }
    SDL_RenderDrawLine(renderer, 0, 0, dw * maze.width, 0); // on dessine l'entrée
    SDL_RenderDrawLine(renderer, 0, 0, 0, dh * maze.height); // on dessine l'entrée
    SDL_Delay(displayMode.refresh_rate); // pause pour laisser aux données le temps de s'afficher
    SDL_RenderPresent(renderer); // on met à jour l'affichage
}

static way *local_best_exit_breadth_seeker(const maze_t maze, const int end_x, const int end_y)
{
    const waytab ways = create_waytab(maze.width,
                                      maze.height); // ce tableau nous permettra de connaitre le chemin le plus court pour arriver à une case
    const bool_tab visited = create_booltab(maze.width,
                                            maze.height); // ce tableau nous permettra de connaitre les cases déjà visitées pour éviter les boucles infinies
    queue* q = create_queue(); // cette file contiendra les coordonnées des cases à visiter
    enqueue(0, 0, q); // on commence par l'entrée
    set_true(visited, 0, 0);
    int x, y;
    while (!isempty_queue(q))
    {
        dequeue(q, &x, &y);
        if (x == end_x && y == end_y) // si on est à la sortie
        {
            way* w = copy_way(get_way(ways, end_x, end_y)); // on récupère le chemin pour arriver à la sortie (on
                                                                               // le copie car on va libérer le tableau ways)
            free_waytab(ways);
            free_queue(q);
            return w;
        }
        if (!has_wall_up(maze, x, y) && !get_bool(visited, x, y - 1)) // si on peut aller en haut et que le chemin est plus court
        {
            enqueue(x, y - 1, q); // on ajoute la case à celle à visiter
            connected_way(ways, x, y - 1, x, y); // on crée un chemin entre la case actuelle et la case en haut
            set_true(visited, x, y - 1);
        }
        if (!has_wall_down(maze, x, y) && !get_bool(visited, x, y + 1)) // si on peut aller en bas et que le chemin est plus court
        {
            enqueue(x, y + 1, q); // on ajoute la case à celle à visiter
            connected_way(ways, x, y + 1, x, y); // on crée un chemin entre la case actuelle et la case en bas
            set_true(visited, x, y + 1);
        }
        if (!has_wall_left(maze, x, y) && !get_bool(visited, x - 1, y)) // si on peut aller à gauche et que le chemin est plus court
        {
            enqueue(x - 1, y, q); // on ajoute la case à celle à visiter
            connected_way(ways, x - 1, y, x, y); // on crée un chemin entre la case actuelle et la case à gauche
            set_true(visited, x - 1, y);
        }
        if (!has_wall_right(maze, x, y) && !get_bool(visited, x + 1, y)) // si on peut aller à droite et que le chemin est plus court
        {
            enqueue(x + 1, y, q); // on ajoute la case à celle à visiter
            connected_way(ways, x + 1, y, x, y); // on crée un chemin entre la case actuelle et la case à droite
            set_true(visited, x + 1, y);
        }
    }
    free_waytab(ways);
    free_queue(q);
    free_booltab(visited);
    return create_way(); // si on n'a pas trouvé de chemin, on renvoie un chemin vide
}

static void local_show_the_way(const maze_t maze, const way *w, SDL_Renderer* renderer, const int dh, const int dw)
{
    SDL_SetRenderDrawColor(renderer, 25, 100, 20, 255); // on dessine en bleu
    while (w != NULL)
    {
        SDL_Rect rect = {get_x(w) * dw + 1, get_y(w) * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
        SDL_RenderFillRect(renderer, &rect);
        if (!has_wall_down(maze, get_x(w), get_y(w)))
        {
            SDL_RenderDrawLine(renderer, get_x(w) * dw + 1, (get_y(w) + 1) * dh - 1, (get_x(w) + 1) * dw - 2,
                               (get_y(w) + 1) * dh - 1); // on dessine une ligne vers le bas
        }
        if (!has_wall_up(maze, get_x(w), get_y(w)))
        {
            SDL_RenderDrawLine(renderer, get_x(w) * dw + 1, get_y(w) * dh, (get_x(w) + 1) * dw - 2,
                               get_y(w) * dh); // on dessine une ligne vers le haut
        }
        if (!has_wall_left(maze, get_x(w), get_y(w)))
        {
            SDL_RenderDrawLine(renderer, get_x(w) * dw, get_y(w) * dh + 1, get_x(w) * dw,
                               (get_y(w) + 1) * dh - 2); // on dessine une ligne vers la gauche
        }
        if (!has_wall_right(maze, get_x(w), get_y(w)))
        {
            SDL_RenderDrawLine(renderer, (get_x(w) + 1) * dw - 1, get_y(w) * dh + 1, (get_x(w) + 1) * dw - 1,
                               (get_y(w) + 1) * dh - 2); // on dessine une ligne vers la droite
        }
        w = get_dad(w); // on passe à la case suivante
    }
    const SDL_Rect rect = {1, 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case de départ
    SDL_RenderFillRect(renderer, &rect);
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
    SDL_RenderPresent(renderer);
}

static void draw_border(const maze_t maze, SDL_Renderer *renderer, const int x, const int y, const int dw, const int dh)
{
    if (!has_wall_right(maze, x, y))
    {
        SDL_RenderDrawLine(renderer, (x + 1) * dw - 1, y * dh, (x + 1) * dw - 1, (y + 1) * dh - 2); // on dessine un mur à droite
    }
    if (!has_wall_down(maze, x, y))
    {
        SDL_RenderDrawLine(renderer, x * dw, (y + 1) * dh - 1, (x + 1) * dw - 2, (y + 1) * dh - 1); // on dessine un mur en bas
    }
    if (!has_wall_left(maze, x, y))
    {
        SDL_RenderDrawLine(renderer, x * dw, y * dh, x * dw, (y + 1) * dh - 2); // on dessine un mur à gauche
    }
    if (!has_wall_up(maze, x, y))
    {
        SDL_RenderDrawLine(renderer, x * dw, y * dh, (x + 1) * dw - 2, y * dh); // on dessine un mur en haut
    }
}

static void draw_wall(const maze_t maze, SDL_Renderer *renderer, const int x, const int y, const int dw, const int dh)
{
    if (has_wall_right(maze, x, y))
    {
        SDL_RenderDrawLine(renderer, (x + 1) * dw - 1, y * dh, (x + 1) * dw - 1,
                                   (y + 1) * dh - 1); // on dessine un mur à droite
    }
    if (has_wall_down(maze, x, y))
    {
        SDL_RenderDrawLine(renderer, x * dw, (y + 1) * dh - 1, (x + 1) * dw - 1,
                                   (y + 1) * dh - 1); // on dessine un mur en bas
    }
    if (has_wall_left(maze, x, y))
    {
        SDL_RenderDrawLine(renderer, (x) * dw - 1, y * dh, (x) * dw - 1,
                                   (y + 1) * dh - 1); // on dessine un mur à droite de la cellule de gauche
    }
    if (has_wall_up(maze, x, y))
    {
        SDL_RenderDrawLine(renderer, x * dw, (y) * dh - 1, (x + 1) * dw - 1,
                                   (y) * dh - 1); // on dessine un mur en bas de la cellule du haut
    }
}

void game_show(const maze_t maze, int x, int y){
    SDL_Renderer* renderer;
    SDL_Window* window;
    int dw, dh;
    if (initial_print_maze(maze, &renderer, &window, &dw, &dh) != 1)
    {
        return;
    }
    SDL_SetWindowTitle(window, "game");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
    SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // la position actuelle
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer); // on affiche la position actuelle
    char direction = NORD;

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event))
    {
        // on vide la file d'attente des événements
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // on dessine en blanc
    SDL_RenderDrawLine(renderer, maze.width * dw - dw, maze.height * dh - 1, maze.width * dw,
                       maze.height * dh - 1); // on dessine la sortie
    SDL_RenderDrawLine(renderer, maze.width * dw - 1, maze.height * dh - dh, maze.width * dw - 1,
                       maze.height * dh); // on dessine la sortie
    while (x != 0 || y != 0)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // on efface la position actuelle
        SDL_RenderFillRect(renderer, &rect);
        draw_border(maze, renderer, x, y, dw, dh);
        SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
        SDL_WaitEvent(&event);
        {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
                (event.type == SDL_KEYUP &&
                 (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER ||
                  event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
            {
                printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                destroy_print_maze(renderer, window);
                return;
            }
            if (event.type == SDL_KEYUP){
                if (event.key.keysym.sym == SDLK_c)
                {
                    switch (direction)
                    {
                        case NORD:
                            if (y > 0 && has_wall_up(maze, x, y))
                            {
                                y--;
                            }
                        break;
                        case SUD:
                            if (y < maze.height - 1 && has_wall_down(maze, x, y))
                            {
                                y++;
                            }
                        break;
                        case EST:
                            if (x < maze.width - 1 && has_wall_right(maze, x, y))
                            {
                                x++;
                            }
                        break;
                        case OUEST:
                            if (x > 0 && has_wall_left(maze, x, y))
                            {
                                x--;
                            }
                        break;
                        // ReSharper disable once CppDFAUnreachableCode
                        default:
                            fprintf(stderr, "Error : direction %c is not a valid direction\n", direction);
                            destroy_print_maze(renderer, window);
                            exit(EXIT_FAILURE);
                    }
                }
                else if (event.key.keysym.sym == SDLK_UP)
                {
                    direction = NORD;
                    if (!has_wall_up(maze, x, y))
                    {
                        y--;
                    }
                }
                else if (event.key.keysym.sym == SDLK_DOWN)
                {
                    direction = SUD;
                    if (!has_wall_down(maze, x, y))
                    {
                        y++;
                    }
                }
                else if (event.key.keysym.sym == SDLK_LEFT)
                {
                    direction = OUEST;
                    if (!has_wall_left(maze, x, y))
                    {
                        x--;
                    }
                }
                else if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    direction = EST;
                    if (!has_wall_right(maze, x, y))
                    {
                        x++;
                    }
                }
                else if (event.key.keysym.sym == SDLK_s)
                {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderClear(renderer);
                    reprint_maze(maze, renderer, dw, dh);
                    SDL_Delay(dm.refresh_rate);
                    SDL_RenderPresent(renderer);
                    way *w = local_best_exit_breadth_seeker(maze, x, y);
                    local_show_the_way(maze, w, renderer, dh, dw);
                    free_way(w);
                }
            }
}
        SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
        rect.x = x * dw + 1;
        rect.y = y * dh + 1;
        SDL_RenderFillRect(renderer, &rect); // on dessine la nouvelle position
        draw_border(maze, renderer, x, y, dw, dh);
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

void game(const maze_t maze, int x, int y){
    SDL_Renderer* renderer;
    SDL_Window* window;
    int dw, dh;
    if (pre_print_maze(maze, &renderer, &window, &dw, &dh) != 1)
    {
        return;
    }
    SDL_SetWindowTitle(window, "game");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    draw_wall(maze, renderer, x, y, dw, dh);
    SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
    SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // la position actuelle
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer); // on affiche la position actuelle
    char direction = NORD;

    SDL_Event event = {0}; // on crée un event vide
    while (SDL_PollEvent(&event))
    {
        // on vide la file d'attente des événements
    }
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // on dessine en blanc
    SDL_RenderDrawLine(renderer, maze.width * dw - dw, maze.height * dh - 1, maze.width * dw,
                       maze.height * dh - 1); // on dessine la sortie
    SDL_RenderDrawLine(renderer, maze.width * dw - 1, maze.height * dh - dh, maze.width * dw - 1,
                       maze.height * dh); // on dessine la sortie
    while (x != 0 || y != 0)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // on efface la position actuelle
        SDL_RenderFillRect(renderer, &rect);
        draw_border(maze, renderer, x, y, dw, dh);
        SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
        SDL_WaitEvent(&event);
        {
            if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
                (event.type == SDL_KEYUP &&
                 (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER ||
                  event.key.keysym.sym == SDLK_RETURN))) // si l'utilisateur veut fermer la fenêtre
            {
                printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
                destroy_print_maze(renderer, window);
                return;
            }
            if (event.type == SDL_KEYUP){
                if (event.key.keysym.sym == SDLK_c)
                {
                    switch (direction)
                    {
                        case NORD:
                            if (y > 0 && has_wall_up(maze, x, y))
                            {
                                y--;
                            }
                        break;
                        case SUD:
                            if (y < maze.height - 1 && has_wall_down(maze, x, y))
                            {
                                y++;
                            }
                        break;
                        case EST:
                            if (x < maze.width - 1 && has_wall_right(maze, x, y))
                            {
                                x++;
                            }
                        break;
                        case OUEST:
                            if (x > 0 && has_wall_left(maze, x, y))
                            {
                                x--;
                            }
                        break;
                        // ReSharper disable once CppDFAUnreachableCode
                        default:
                            fprintf(stderr, "Error : direction %c is not a valid direction\n", direction);
                            destroy_print_maze(renderer, window);
                            exit(EXIT_FAILURE);
                    }
                }
                else if (event.key.keysym.sym == SDLK_UP)
                {
                    direction = NORD;
                    if (!has_wall_up(maze, x, y))
                    {
                        y--;
                    }
                }
                else if (event.key.keysym.sym == SDLK_DOWN)
                {
                    direction = SUD;
                    if (!has_wall_down(maze, x, y))
                    {
                        y++;
                    }
                }
                else if (event.key.keysym.sym == SDLK_LEFT)
                {
                    direction = OUEST;
                    if (!has_wall_left(maze, x, y))
                    {
                        x--;
                    }
                }
                else if (event.key.keysym.sym == SDLK_RIGHT)
                {
                    direction = EST;
                    if (!has_wall_right(maze, x, y))
                    {
                        x++;
                    }
                }
                else if (event.key.keysym.sym == SDLK_s)
                {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                    SDL_RenderClear(renderer);
                    SDL_Delay(dm.refresh_rate);
                    SDL_RenderPresent(renderer);
                    way *w = local_best_exit_breadth_seeker(maze, x, y);
                    local_show_the_way(maze, w, renderer, dh, dw);
                    free_way(w);
                }
            }
}
        SDL_SetRenderDrawColor(renderer, 0, 100, 200, 255);
        rect.x = x * dw + 1;
        rect.y = y * dh + 1;
        SDL_RenderFillRect(renderer, &rect); // on dessine la nouvelle position
        draw_border(maze, renderer, x, y, dw, dh);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        draw_wall(maze, renderer, x, y, dw, dh);
        SDL_Delay(dm.refresh_rate);
        SDL_RenderPresent(renderer);
    }
    reprint_maze(maze, renderer, dw, dh);
    SDL_SetRenderDrawColor(renderer, 0, 250, 0, 255);
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetWindowTitle(window, "escaped");
    SDL_Delay(dm.refresh_rate);
    SDL_RenderPresent(renderer);
    wait_and_destroy_print_maze(renderer, window);
}

void game_half_blind(maze_t, int, int);

void game_blind(maze_t, int, int);

void game_quarter_blind(maze_t, int, int);