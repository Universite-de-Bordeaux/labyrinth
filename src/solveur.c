#include "solveur.h"
#include "struct.h"
#include <stdio.h>
#include <SDL2/SDL.h>

// --- Solveur en profondeur ---


bool has_exit_deep_seeker(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width, maze.height); //ce tableau nous permettra de connaitre les cases déjà visitées pour éviter les boucles infinies
    stack *s = create_stack(); //cette pile contiendra les coordonnées des cases à visiter
    push(0, 0, s); //on commence par l'entrée
    int x, y;
    while(!isempty_stack(s)) //on essaie tous les chemins possibles
    {
        pop(s, &x, &y);
        if(x == maze.width - 1 && y == maze.height - 1) //si on est à la sortie
        {
            free_stack(s);
            free_booltab(visited);
            return true;
        }
        set_true(visited, x, y);
        if(!has_wall_up(maze, x, y) && !get_bool(visited, x, y - 1)) //si on peut aller en haut
        {
            push(x, y - 1, s);
        }
        if(!has_wall_down(maze, x, y) && !get_bool(visited, x, y + 1)) //si on peut aller en bas
        {
            push(x, y + 1, s);
        }
        if(!has_wall_left(maze, x, y) && !get_bool(visited, x - 1, y)) //si on peut aller à gauche
        {
            push(x - 1, y, s);
        }
        if(!has_wall_right(maze, x, y) && !get_bool(visited, x + 1, y)) //si on peut aller à droite
        {
            push(x + 1, y, s);
        }
    }
    free_booltab(visited);
    free_stack(s);
    return false;
}

bool is_perfect_deep_inspector(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width, maze.height); //on crée un tableau de booléens pour savoir si on est déjà passé par une case
    stack *s = create_stack(); //cette pile contiendra les coordonnées des cases à visiter
    push(0, 0, s); //on commence par l'entrée
    int x, y;
    while(!isempty_stack(s)) //on doit visiter toutes les cases
    {
        pop(s, &x, &y);
        if(get_bool(visited, x, y)) //si on est déjà passé par cette case, c'est qu'il y a un cycle
        {
            free_stack(s);
            free_booltab(visited);
            return false;
        }
        set_true(visited, x, y);
        int nb_way_used = 0; //on compte le nombre de chemin adjacent utilisé, en théorie il doit être égal à 1 (notre père) ou 0 (la case de départ)
        if(!has_wall_up(maze, x, y)) //si on peut aller en haut
        {
            if(!get_bool(visited, x, y - 1)) //on ne repasse pas par une case déjà visitée
            {
                push(x, y - 1, s);
            }
            else
            {
                nb_way_used++;
            }
        }
        if(!has_wall_down(maze, x, y)) //si on peut aller en bas
        {
            if(!get_bool(visited, x, y + 1)) //on ne repasse pas par une case déjà visitée
            {
                push(x, y + 1, s);
            }
            else
            {
                nb_way_used++;
            }
        }
        if(!has_wall_left(maze, x, y)) //si on peut aller à gauche
        {
            if(!get_bool(visited, x - 1, y)) //on ne repasse pas par une case déjà visitée
            {
                push(x - 1, y, s);
            }
            else
            {
                nb_way_used++;
            }
        }
        if(!has_wall_right(maze, x, y)) //si on peut aller à droite
        {
            if(!get_bool(visited, x + 1, y)) //on ne repasse pas par une case déjà visitée
            {
                push(x + 1, y, s);
            }
            else
            {
                nb_way_used++;
            }
        }
        if(nb_way_used > 1) //si on a plus d'un chemin adjacent utilisé, c'est qu'il y a un cycle
        {
            free_stack(s);
            free_booltab(visited);
            return false;
        }
        if(nb_way_used == 0 && (x != 0 || y != 0)) //si on est sur une case qui n'est pas la case de départ et qu'on a aucun chemin adjacent utilisé, c'est qu'il y a un problème
        {
            free_stack(s);
            free_booltab(visited);
            fprintf(stderr, "Erreur dans la fonction perfect_deep_inspector : la case (%d, %d) a été inspectée mais n'a aucun chemin parent.\n", x, y);
            exit(EXIT_FAILURE);
        }
    }
    for(x = 0; x < maze.width; x++) //on vérifie que toutes les cases ont été visitées
    {
        for(y = 0; y < maze.height; y++)
        {
            if(!get_bool(visited, x, y)) //si une case n'a pas été visitée, c'est que le labyrinthe n'est pas connexe
            {
                free_stack(s);
                free_booltab(visited);
                return false;
            }
        }
    }
    free_stack(s);
    free_booltab(visited);
    return true;
}

way *best_exit_deep_seeker(const maze_t maze)
{
    //on va constituer un tableau de way pour chaque case, qui contiendra le chemin le plus court pour y arriver, en partant de l'entrée
    //il nous permettra d'éviter les cycle (la taille du cycle serait plus grande) et de relier des fragments de chemin plutôt que de tout recalculer
    const waytab ways = create_waytab(maze.width, maze.height); //ce tableau nous permettra de connaitre le chemin le plus court pour arriver à une case
    stack *s = create_stack(); //cette pile contiendra les coordonnées des cases à visiter
    push(0, 0, s); //on commence par l'entrée
    int x, y;
    while(!isempty_stack(s))
    {
        pop(s, &x, &y);
        if(x == maze.width - 1 && y == maze.height - 1) //si on est à la sortie, on ne va pas plus loin
        {
            continue;
        }
        if(!has_wall_up(maze, x, y) && length_waytab(ways, x, y) + 1 < length_waytab(ways, x, y - 1)) //si on peut aller en haut et que le chemin est plus court
        {
            push(x, y - 1, s); //on ajoute la case à celle à visiter
            connected_way(ways, x, y - 1, x, y); //on crée un chemin entre la case actuelle et la case en haut
        }
        if(!has_wall_down(maze, x, y) && length_waytab(ways, x, y) + 1 < length_waytab(ways, x, y + 1)) //si on peut aller en bas et que le chemin est plus court
        {
            push(x, y + 1, s); //on ajoute la case à celle à visiter
            connected_way(ways, x, y + 1, x, y); //on crée un chemin entre la case actuelle et la case en bas
        }
        if(!has_wall_left(maze, x, y) && length_waytab(ways, x, y) + 1 < length_waytab(ways, x - 1, y)) //si on peut aller à gauche et que le chemin est plus court
        {
            push(x - 1, y, s); //on ajoute la case à celle à visiter
            connected_way(ways, x - 1, y, x, y); //on crée un chemin entre la case actuelle et la case à gauche
        }
        if(!has_wall_right(maze, x, y) && length_waytab(ways, x, y) + 1 < length_waytab(ways, x + 1, y)) //si on peut aller à droite et que le chemin est plus court
        {
            push(x + 1, y, s); //on ajoute la case à celle à visiter
            connected_way(ways, x + 1, y, x, y); //on crée un chemin entre la case actuelle et la case à droite
        }
    }
    way *w = copy_way(get_way(ways, maze.width - 1, maze.height - 1)); //on récupère le chemin pour arriver à la sortie (on le copie car on va libérer le tableau ways)
    free_waytab(ways);
    free_stack(s);
    return w;
}

// --- Solveur en largeur ---

bool has_exit_breadth_seeker(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width, maze.height); //on crée un tableau de booléens pour savoir si on est déjà passé par une case
    queue *q = create_queue(); //cette file contiendra les coordonnées des cases à visiter
    enqueue(0, 0, q); //on commence par l'entrée
    int x, y;
    while(!isempty_queue(q))
    {
        dequeue(q, &x, &y);
        if(get_bool(visited, x, y)) //si on est déjà passé par cette case, on ne la visite pas
        {
            continue;
        }
        if(x == maze.width - 1 && y == maze.height - 1) //si on est à la sortie
        {
            free_queue(q);
            free_booltab(visited);
            return true;
        }
        set_true(visited, x, y);
        if(!has_wall_up(maze, x, y)) //si on peut aller en haut
        {
            enqueue(x, y - 1, q);
        }
        if(!has_wall_down(maze, x, y)) //si on peut aller en bas
        {
            enqueue(x, y + 1, q);
        }
        if(!has_wall_left(maze, x, y)) //si on peut aller à gauche
        {
            enqueue(x - 1, y, q);
        }
        if(!has_wall_right(maze, x, y)) //si on peut aller à droite
        {
            enqueue(x + 1, y, q);
        }
    }
    free_queue(q);
    free_booltab(visited);
    return false;
}

bool is_perfect_breadth_inspector(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width, maze.height); //on crée un tableau de booléens pour savoir si on est déjà passé par une case
    queue *q = create_queue(); //cette file contiendra les coordonnées des cases à visiter
    enqueue(0, 0, q); //on commence par l'entrée
    int x, y;
    while(!isempty_queue(q))
    {
        dequeue(q, &x, &y);
        if(get_bool(visited, x, y)) //si on est déjà passé par cette case, c'est qu'il y a un cycle
        {
            free_queue(q);
            free_booltab(visited);
            return false;
        }
        set_true(visited, x, y);
        int nb_way_used = 0; //on compte le nombre de chemin adjacent utilisé, en théorie il doit être égal à 1 (notre père) ou 0 (la case de départ)
        if(!has_wall_up(maze, x, y)) //si on peut aller en haut
        {
            if(!get_bool(visited, x, y - 1)) //on ne repasse pas par une case déjà visitée
            {
                enqueue(x, y - 1, q);
            }
            else
            {
                nb_way_used++;
            }
        }
        if(!has_wall_down(maze, x, y)) //si on peut aller en bas
        {
            if(!get_bool(visited, x, y + 1)) //on ne repasse pas par une case déjà visitée
            {
                enqueue(x, y + 1, q);
            }
            else
            {
                nb_way_used++;
            }
        }
        if(!has_wall_left(maze, x, y)) //si on peut aller à gauche
        {
            if(!get_bool(visited, x - 1, y)) //on ne repasse pas par une case déjà visitée
            {
                enqueue(x - 1, y, q);
            }
            else
            {
                nb_way_used++;
            }
        }
        if(!has_wall_right(maze, x, y)) //si on peut aller à droite
        {
            if(!get_bool(visited, x + 1, y)) //on ne repasse pas par une case déjà visitée
            {
                enqueue(x + 1, y, q);
            }
            else
            {
                nb_way_used++;
            }
        }
        if(nb_way_used > 1) //si on a plus d'un chemin adjacent utilisé, c'est qu'il y a un cycle
        {
            free_queue(q);
            free_booltab(visited);
            return false;
        }
    }
    for(x = 0; x < maze.width; x++) //on vérifie que toutes les cases ont été visitées
    {
        for(y = 0; y < maze.height; y++)
        {
            if(!get_bool(visited, x, y)) //si une case n'a pas été visitée, c'est que le labyrinthe n'est pas connexe
            {
                free_queue(q);
                free_booltab(visited);
                return false;
            }
        }
    }
    free_queue(q);
    free_booltab(visited);
    return true;
}

way *best_exit_breadth_seeker(const maze_t maze)
{
    //on va constituer un tableau de way pour chaque case, qui contiendra le chemin le plus court pour y arriver, en partant de l'entrée
    //il nous permettra de savoir facilement si on est déjà passé par une case car un chemin en largeur est toujours plus court qu'un autre
    const waytab ways = create_waytab(maze.width, maze.height); //ce tableau nous permettra de connaitre le chemin le plus court pour arriver à une case
    queue *q = create_queue(); //cette file contiendra les coordonnées des cases à visiter
    enqueue(0, 0, q); //on commence par l'entrée
    int x, y;
    while(!isempty_queue(q))
    {
        dequeue(q, &x, &y);
        if(x == maze.width - 1 && y == maze.height - 1) //si on est à la sortie, on ne va pas plus loin
        {
            continue;
        }
        if(!has_wall_up(maze, x, y) && is_empty(get_way(ways, x, y))) //si on peut aller en haut et que le chemin est plus court
        {
            enqueue(x, y - 1, q); //on ajoute la case à celle à visiter
            connected_way(ways, x, y - 1, x, y); //on crée un chemin entre la case actuelle et la case en haut
        }
        if(!has_wall_down(maze, x, y) && is_empty(get_way(ways, x, y))) //si on peut aller en bas et que le chemin est plus court
        {
            enqueue(x, y + 1, q); //on ajoute la case à celle à visiter
            connected_way(ways, x, y + 1, x, y); //on crée un chemin entre la case actuelle et la case en bas
        }
        if(!has_wall_left(maze, x, y) && is_empty(get_way(ways, x, y))) //si on peut aller à gauche et que le chemin est plus court
        {
            enqueue(x - 1, y, q); //on ajoute la case à celle à visiter
            connected_way(ways, x - 1, y, x, y); //on crée un chemin entre la case actuelle et la case à gauche
        }
        if(!has_wall_right(maze, x, y) && is_empty(get_way(ways, x, y))) //si on peut aller à droite et que le chemin est plus court
        {
            enqueue(x + 1, y, q); //on ajoute la case à celle à visiter
            connected_way(ways, x + 1, y, x, y); //on crée un chemin entre la case actuelle et la case à droite
        }
    }
    way *w = copy_way(get_way(ways, maze.width - 1, maze.height - 1)); //on récupère le chemin pour arriver à la sortie (on le copie car on va libérer le tableau ways)
    free_waytab(ways);
    free_queue(q);
    return w;
}

// --- Visualisation des algorithmes de résolution de labyrinthes ---

int show_has_exit_deep_seeker(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width, maze.height); //ce tableau nous permettra de connaitre les cases déjà visitées pour éviter les boucles infinies
    stack *s = create_stack(); //cette pile contiendra les coordonnées des cases à visiter
    push(0, 0, s); //on commence par l'entrée
    int x, y;
    SDL_Renderer *renderer = NULL;
    SDL_Window *window = NULL;
    int dw, dh;
    initial_print_maze(maze, &renderer, &window, &dw, &dh);
    SDL_SetWindowTitle(window, "exit deep seeker");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    if(renderer == NULL || window == NULL)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de l'affichage du labyrinthe.\n");
        return -1;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); //on dessine en bleu
    SDL_Event event = {0}; //on crée un event vide
    while(!isempty_stack(s)) //on essaie tous les chemins possibles
    {
        SDL_WaitEventTimeout(&event, 10); //on enregistre les events entrants
        if(event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || \
            (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)) //si l'utilisateur veut fermer la fenêtre
        {
            printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
            free_stack(s);
            free_booltab(visited);
            destroy_print_maze(renderer, window);
            return 1;
        }
        pop(s, &x, &y);
        if(x == maze.width - 1 && y == maze.height - 1) //si on est à la sortie
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); //on dessine en vert
            const SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; //on dessine un rectangle dans la case
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetWindowTitle(window, "exit found !"); //on change le titre de la fenêtre
            SDL_Delay(dm.refresh_rate); //pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            wait_and_destroy_print_maze(renderer, window); //on attend que l'utilisateur ferme la fenêtre
            free_stack(s);
            free_booltab(visited);
            return 1;
        }
        if(get_bool(visited, x, y)) //si on est déjà passé par cette case, on ne la visite pas
        {

            continue;
        }
        set_true(visited, x, y);
        SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; //on dessine un rectangle dans la case
        SDL_RenderFillRect(renderer, &rect);
        SDL_Delay(dm.refresh_rate); //pause pour laisser aux données le temps de s'afficher
        SDL_RenderPresent(renderer);
        if(!has_wall_up(maze, x, y) && !get_bool(visited, x, y - 1)) //si on peut aller en haut
        {
            push(x, y - 1, s);
        }
        if(!has_wall_down(maze, x, y) && !get_bool(visited, x, y + 1)) //si on peut aller en bas
        {
            push(x, y + 1, s);
        }
        if(!has_wall_left(maze, x, y) && !get_bool(visited, x - 1, y)) //si on peut aller à gauche
        {
            push(x - 1, y, s);
        }
        if(!has_wall_right(maze, x, y) && !get_bool(visited, x + 1, y)) //si on peut aller à droite
        {
            push(x + 1, y, s);
        }
    }
    SDL_SetWindowTitle(window, "exit do not exist"); //on change le titre de la fenêtre
    wait_and_destroy_print_maze(renderer, window);
    free_stack(s);
    free_booltab(visited);
    return 1;
}

int show_is_perfect_deep_inspector(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width, maze.height); //ce tableau nous permettra de connaitre les cases déjà visitées pou repérer les cycles et la connexité
    stack *s = create_stack(); //cette pile contiendra les coordonnées des cases à visiter
    push(0, 0, s); //on commence par l'entrée
    int x, y;
    SDL_Renderer *renderer = NULL;
    SDL_Window *window = NULL;
    int dw, dh;
    initial_print_maze(maze, &renderer, &window, &dw, &dh);
    SDL_SetWindowTitle(window, "is perfect deep inspector");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    if(renderer == NULL || window == NULL)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de l'affichage du labyrinthe.\n");
        return -1;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); //on dessine en bleu
    SDL_Event event = {0}; //on crée un event vide
    while(!isempty_stack(s)) //on essaie tous les chemins possibles
    {
        SDL_WaitEventTimeout(&event, 10); //on enregistre les events entrants
        if(event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || \
            (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)) //si l'utilisateur veut fermer la fenêtre
        {
            printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
            free_stack(s);
            free_booltab(visited);
            destroy_print_maze(renderer, window);
            return 1;
        }
        pop(s, &x, &y);
        set_true(visited, x, y);
        SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; //on dessine un rectangle dans la case
        SDL_RenderFillRect(renderer, &rect);
        SDL_Delay(dm.refresh_rate); //pause pour laisser aux données le temps de s'afficher
        SDL_RenderPresent(renderer);
        int nb_way_used = 0; //on compte le nombre de chemin adjacent utilisé, en théorie il doit être égal à 1 (notre père) ou 0 (la case de départ)
        if(!has_wall_up(maze, x, y)) //si on peut aller en haut
        {
            if(get_bool(visited, x, y - 1)) //si on est déjà passé par cette case
            {
                nb_way_used++;
            }
            else
            {
                push(x, y - 1, s);
            }
        }
        if(!has_wall_down(maze, x, y)) //si on peut aller en bas
        {
            if(get_bool(visited, x, y + 1)) //si on est déjà passé par cette case
            {
                nb_way_used++;
            }
            else
            {
                push(x, y + 1, s);
            }
        }
        if(!has_wall_left(maze, x, y)) //si on peut aller à gauche
        {
            if(get_bool(visited, x - 1, y)) //si on est déjà passé par cette case
            {
                nb_way_used++;
            }
            else
            {
                push(x - 1, y, s);
            }
        }
        if(!has_wall_right(maze, x, y)) //si on peut aller à droite
        {
            if(get_bool(visited, x + 1, y)) //si on est déjà passé par cette case
            {
                nb_way_used++;
            }
            else
            {
                push(x + 1, y, s);
            }
        }
        if(nb_way_used > 1) //si on a trop de chemin adjacent utilisé, c'est qu'il y a un cycle
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //on dessine en rouge
            const SDL_Rect rec = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; //on dessine un rectangle dans la case
            SDL_RenderFillRect(renderer, &rec);
            SDL_SetWindowTitle(window, "cycle found !"); //on change le titre de la fenêtre
            SDL_Delay(dm.refresh_rate); //pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            wait_and_destroy_print_maze(renderer, window); //on attend que l'utilisateur ferme la fenêtre
            free_stack(s);
            free_booltab(visited);
            return 1;
        }
    }
    for(x = 0; x < maze.width; x++) //on vérifie que toutes les cases ont été visitées
    {
        for(y = 0; y < maze.height; y++)
        {
            if(!get_bool(visited, x, y)) //si une case n'a pas été visitée, c'est que le labyrinthe n'est pas connexe
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); //on dessine en rouge
                const SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; //on dessine un rectangle dans la case
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetWindowTitle(window, "not connected !"); //on change le titre de la fenêtre
                SDL_Delay(dm.refresh_rate); //pause pour laisser aux données le temps de s'afficher
                SDL_RenderPresent(renderer);
                wait_and_destroy_print_maze(renderer, window); //on attend que l'utilisateur ferme la fenêtre
                free_stack(s);
                free_booltab(visited);
                return 1;
            }
        }
    }
    SDL_SetWindowTitle(window, "perfect maze !"); //on change le titre de la fenêtre
    wait_and_destroy_print_maze(renderer, window);
    free_stack(s);
    free_booltab(visited);
    return 1;
}

int show_best_exit_deep_seeker(maze_t maze)
{
    printf("Le visualisateur du meilleur chemin pour sortir du labyrinthe n'est pas encore implémenté.\n");
    return 1;
}

int show_has_exit_breadth_seeker(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width, maze.height); //ce tableau nous permettra de connaitre les cases déjà visitées pour éviter les boucles infinies
    queue *q = create_queue(); //cette file contiendra les coordonnées des cases à visiter
    enqueue(0, 0, q); //on commence par l'entrée
    set_true(visited, 0, 0); //on marque la case de départ comme visitée
    int x, y;
    SDL_Renderer *renderer = NULL;
    SDL_Window *window = NULL;
    int dw, dh;
    initial_print_maze(maze, &renderer, &window, &dw, &dh);
    SDL_SetWindowTitle(window, "exit breadth seeker");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    if(renderer == NULL || window == NULL)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de l'affichage du labyrinthe.\n");
        return -1;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); //on dessine en bleu
    SDL_Event event = {0}; //on crée un event vide
    while(!isempty_queue(q)) //on essaie tous les chemins possibles
    {
        SDL_WaitEventTimeout(&event, 1); //on enregistre les events entrants
        if(event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE || \
            (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)) //si l'utilisateur veut fermer la fenêtre
        {
            printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
            free_queue(q);
            free_booltab(visited);
            destroy_print_maze(renderer, window);
            return 1;
        }
        dequeue(q, &x, &y);
        if(x == maze.width - 1 && y == maze.height - 1) //si on est à la sortie
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); //on dessine en vert
            const SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; //on dessine un rectangle dans la case
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetWindowTitle(window, "exit found !"); //on change le titre de la fenêtre
            SDL_Delay(dm.refresh_rate); //pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            wait_and_destroy_print_maze(renderer, window); //on attend que l'utilisateur ferme la fenêtre
            free_queue(q);
            free_booltab(visited);
            return 1;
        }
        SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; //on dessine un rectangle dans la case
        SDL_RenderFillRect(renderer, &rect);
        if(!has_wall_up(maze, x, y) && !get_bool(visited, x, y - 1)) //si on peut aller en haut
        {
            enqueue(x, y - 1, q);
            set_true(visited, x, y - 1); //on marque la case comme visitée por éviter de repasser par là
        }
        if(!has_wall_down(maze, x, y) && !get_bool(visited, x, y + 1)) //si on peut aller en bas
        {
            enqueue(x, y + 1, q);
            set_true(visited, x, y + 1); //on marque la case comme visitée por éviter de repasser par là
        }
        if(!has_wall_left(maze, x, y) && !get_bool(visited, x - 1, y)) //si on peut aller à gauche
        {
            enqueue(x - 1, y, q);
            set_true(visited, x - 1, y); //on marque la case comme visitée por éviter de repasser par là
        }
        if(!has_wall_right(maze, x, y) && !get_bool(visited, x + 1, y)) //si on peut aller à droite
        {
            enqueue(x + 1, y, q);
            set_true(visited, x + 1, y); //on marque la case comme visitée por éviter de repasser par là
        }
        SDL_Delay(dm.refresh_rate); //pause pour laisser aux données le temps de s'afficher
        SDL_RenderPresent(renderer);
    }
    SDL_SetWindowTitle(window, "exit do not exist"); //on change le titre de la fenêtre
    wait_and_destroy_print_maze(renderer, window);
    free_queue(q);
    free_booltab(visited);
    return 1;
}

int show_is_perfect_breadth_inspector(maze_t maze)
{
    printf("Le visualisateur de la perfection du labyrinthe n'est pas encore implémenté.\n");
    return 1;
}

int show_best_exit_breadth_seeker(maze_t maze)
{
    printf("Le visualisateur du meilleur chemin pour sortir du labyrinthe n'est pas encore implémenté.\n");
    return 1;
}

// --- Visualisation des chemins ---

int show_the_way(const maze_t maze, const way *w)
{
    SDL_Renderer *renderer = NULL;
    SDL_Window *window = NULL;
    int dw, dh;
    initial_print_maze(maze, &renderer, &window, &dw, &dh);
    if(renderer == NULL || window == NULL)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de l'affichage du labyrinthe.\n");
        return -1;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); //on dessine en bleu
    while(!is_origin(w))
    {
        SDL_Rect rect = {w->x * dw + 1, w->y * dh + 1, dw - 2, dh - 2}; //on dessine un rectangle dans la case
        SDL_RenderFillRect(renderer, &rect);
        w = get_dad(w); //on passe à la case suivante
    }
    const SDL_Rect rect = {1, 1, dw - 2, dh - 2}; //on dessine un rectangle dans la case de départ
    SDL_RenderFillRect(renderer, &rect);
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    SDL_Delay(dm.refresh_rate); //pause pour laisser aux données le temps de s'afficher
    SDL_RenderPresent(renderer);
    wait_and_destroy_print_maze(renderer, window);
    return 1;
}