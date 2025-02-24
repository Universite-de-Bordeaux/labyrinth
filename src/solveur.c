#include "solveur.h"
#include <SDL2/SDL.h>
#include <stdio.h>
#include <sys/random.h>

#include "struct.h"

// --- FONCTIONS SOLVEURS ---

// --- Solveur en profondeur ---

bool has_exit_deep_seeker(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width,
                                            maze.height); // ce tableau nous permettra de connaitre les cases déjà visitées pour éviter les boucles infinies
    stack* s = create_stack(); // cette pile contiendra les coordonnées des cases à visiter
    push(0, 0, s); // on commence par l'entrée
    int x, y;
    while (!isempty_stack(s)) // on essaie tous les chemins possibles
    {
        pop(s, &x, &y);
        if (x == maze.width - 1 && y == maze.height - 1) // si on est à la sortie
        {
            free_stack(s);
            free_booltab(visited);
            return true;
        }
        set_true(visited, x, y);
        if (!has_wall_up(maze, x, y) && !get_bool(visited, x, y - 1)) // si on peut aller en haut
        {
            push(x, y - 1, s);
        }
        if (!has_wall_down(maze, x, y) && !get_bool(visited, x, y + 1)) // si on peut aller en bas
        {
            push(x, y + 1, s);
        }
        if (!has_wall_left(maze, x, y) && !get_bool(visited, x - 1, y)) // si on peut aller à gauche
        {
            push(x - 1, y, s);
        }
        if (!has_wall_right(maze, x, y) && !get_bool(visited, x + 1, y)) // si on peut aller à droite
        {
            push(x + 1, y, s);
        }
    }
    free_booltab(visited);
    free_stack(s);
    return false;
}

bool is_connexe_deep_inspector(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width, maze.height); // on crée un tableau de booléens pour savoir si on est déjà passé par une case
    stack* s = create_stack(); // cette pile contiendra les coordonnées des cases à visiter
    push(0, 0, s); // on commence par l'entrée
    int x, y;
    while (!isempty_stack(s)) // on doit visiter toutes les cases
    {
        pop(s, &x, &y);
        if (get_bool(visited, x, y)) // si on est déjà passé par cette case, on ne la visite pas
        {
            continue;
        }
        set_true(visited, x, y);
        if (!has_wall_up(maze, x, y) && !get_bool(visited, x, y - 1)) // si on peut aller en haut
        {
            push(x, y - 1, s);
        }
        if (!has_wall_down(maze, x, y) && !get_bool(visited, x, y + 1)) // si on peut aller en bas
        {
            push(x, y + 1, s);
        }
        if (!has_wall_left(maze, x, y) && !get_bool(visited, x - 1, y)) // si on peut aller à gauche
        {
            push(x - 1, y, s);
        }
        if (!has_wall_right(maze, x, y) && !get_bool(visited, x + 1, y)) // si on peut aller à droite
        {
            push(x + 1, y, s);
        }
    }
    for (x = 0; x < maze.width; x++) // on vérifie que toutes les cases ont été visitées
    {
        for (y = 0; y < maze.height; y++)
        {
            if (!get_bool(visited, x, y)) // si une case n'a pas été visitée, c'est que le labyrinthe n'est pas connexe
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

bool is_perfect_deep_inspector(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width, maze.height); // on crée un tableau de booléens pour savoir si on est déjà passé par une case
    stack* s = create_stack(); // cette pile contiendra les coordonnées des cases à visiter
    push(0, 0, s); // on commence par l'entrée
    int x, y;
    int nb_way_used = 1; // on compte le nombre de chemin adjacent utilisé, en théorie il doit être égal à 1, on le met à 1 pour éviter un problème à la case de départ
    while (!isempty_stack(s)) // on doit visiter toutes les cases
    {
        pop(s, &x, &y);
        if (get_bool(visited, x, y)) // si on est déjà passé par cette case, c'est qu'il y a un cycle
        {
            free_stack(s);
            free_booltab(visited);
            return false;
        }
        set_true(visited, x, y);
        if (!has_wall_up(maze, x, y)) // si on peut aller en haut
        {
            if (!get_bool(visited, x, y - 1)) // on ne repasse pas par une case déjà visitée
            {
                push(x, y - 1, s);
            }
            else
            {
                nb_way_used++;
            }
        }
        if (!has_wall_down(maze, x, y)) // si on peut aller en bas
        {
            if (!get_bool(visited, x, y + 1)) // on ne repasse pas par une case déjà visitée
            {
                push(x, y + 1, s);
            }
            else
            {
                nb_way_used++;
            }
        }
        if (!has_wall_left(maze, x, y)) // si on peut aller à gauche
        {
            if (!get_bool(visited, x - 1, y)) // on ne repasse pas par une case déjà visitée
            {
                push(x - 1, y, s);
            }
            else
            {
                nb_way_used++;
            }
        }
        if (!has_wall_right(maze, x, y)) // si on peut aller à droite
        {
            if (!get_bool(visited, x + 1, y)) // on ne repasse pas par une case déjà visitée
            {
                push(x + 1, y, s);
            }
            else
            {
                nb_way_used++;
            }
        }
        if (nb_way_used != 1) // si on a plus d'un chemin adjacent utilisé, c'est qu'il y a un cycle
        {
            free_stack(s);
            free_booltab(visited);
            return false;
        }
        nb_way_used = 0;
    }
    for (x = 0; x < maze.width; x++) // on vérifie que toutes les cases ont été visitées
    {
        for (y = 0; y < maze.height; y++)
        {
            if (!get_bool(visited, x, y)) // si une case n'a pas été visitée, c'est que le labyrinthe n'est pas connexe
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

way* best_exit_deep_seeker(const maze_t maze)
{
    // on va constituer un tableau de way pour chaque case, qui contiendra le chemin le plus court pour y arriver, en
    // partant de l'entrée il nous permettra d'éviter les cycle (la taille du cycle serait plus grande) et de relier des
    // fragments de chemin plutôt que de tout recalculer
    const waytab ways = create_waytab(maze.width,
                                      maze.height); // ce tableau nous permettra de connaitre le chemin le plus court pour arriver à une case
    const bool_tab dead_end = create_booltab(maze.width, maze.height); // ce tableau nous permettra de savoir si une case est un
                                                                       // cul-de-sac, pour éviter de recalculer le chemin
    stack* s = create_stack(); // cette pile contiendra les coordonnées des cases à visiter
    push(0, 0, s); // on commence par l'entrée
    int x, y;
    unsigned int count = 0;
    while (!isempty_stack(s))
    {
        pop(s, &x, &y);
        if (x == maze.width - 1 && y == maze.height - 1) // si on est à la sortie, on s'arrête
        {
            continue;
        }
        const unsigned int l = length_waytab(ways, x, y) + 1; // on récupère la longueur du chemin actuel
        const int abs_dist = -x + maze.width - 1 - y + maze.height - 1; // distance de Manhattan à la sortie
        if (l + abs_dist > length_waytab(ways, maze.width - 1, maze.height - 1)) // si on a déjà trouvé un chemin plus court
        {
            continue;
        }
        if (!has_wall_up(maze, x, y) && !get_bool(dead_end, x, y - 1))
        {
            count++;
            if (l < length_waytab(ways, x, y - 1)) // si on peut aller en haut et que le chemin est plus court
            {
                push(x, y - 1, s); // on ajoute la case à celle à visiter
                connected_way(ways, x, y - 1, x, y); // on crée un chemin entre la case actuelle et la case en haut
            }
        }
        if (!has_wall_down(maze, x, y) && !get_bool(dead_end, x, y + 1))
        {
            count++;
            if (l < length_waytab(ways, x, y + 1)) // si on peut aller en bas et que le chemin est plus court
            {
                push(x, y + 1, s); // on ajoute la case à celle à visiter
                connected_way(ways, x, y + 1, x, y); // on crée un chemin entre la case actuelle et la case en bas
            }
        }
        if (!has_wall_left(maze, x, y) && !get_bool(dead_end, x - 1, y))
        {
            count++;
            if (l < length_waytab(ways, x - 1, y)) // si on peut aller à gauche et que le chemin est plus court
            {
                push(x - 1, y, s); // on ajoute la case à celle à visiter
                connected_way(ways, x - 1, y, x, y); // on crée un chemin entre la case actuelle et la case à gauche
            }
        }
        if (!has_wall_right(maze, x, y) && !get_bool(dead_end, x + 1, y))
        {
            count++;
            if (l < length_waytab(ways, x + 1, y)) // si on peut aller à droite et que le chemin est plus court
            {
                push(x + 1, y, s); // on ajoute la case à celle à visiter
                connected_way(ways, x + 1, y, x, y); // on crée un chemin entre la case actuelle et la case à droite
            }
        }
        if (count <= 1) // si on est dans un cul-de-sac
        {
            set_true(dead_end, x, y); // on marque la case comme un cul-de-sac
        }
        count = 0;
    }
    way* w = copy_way(get_way(ways, maze.width - 1,
                              maze.height - 1)); // on récupère le chemin pour arriver à la sortie (on le copie car on va libérer le tableau ways)
    free_booltab(dead_end);
    free_waytab(ways);
    free_stack(s);
    return w;
}

// --- Solveur en largeur ---

bool has_exit_breadth_seeker(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width, maze.height); // on crée un tableau de booléens pour savoir si on est déjà passé par une case
    queue* q = create_queue(); // cette file contiendra les coordonnées des cases à visiter
    enqueue(0, 0, q); // on commence par l'entrée
    int x, y;
    while (!isempty_queue(q))
    {
        dequeue(q, &x, &y);
        if (get_bool(visited, x, y)) // si on est déjà passé par cette case, on ne la visite pas
        {
            continue;
        }
        if (x == maze.width - 1 && y == maze.height - 1) // si on est à la sortie
        {
            free_queue(q);
            free_booltab(visited);
            return true;
        }
        set_true(visited, x, y);
        if (!has_wall_up(maze, x, y)) // si on peut aller en haut
        {
            enqueue(x, y - 1, q);
        }
        if (!has_wall_down(maze, x, y)) // si on peut aller en bas
        {
            enqueue(x, y + 1, q);
        }
        if (!has_wall_left(maze, x, y)) // si on peut aller à gauche
        {
            enqueue(x - 1, y, q);
        }
        if (!has_wall_right(maze, x, y)) // si on peut aller à droite
        {
            enqueue(x + 1, y, q);
        }
    }
    free_queue(q);
    free_booltab(visited);
    return false;
}

bool is_connexe_breadth_inspector(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width, maze.height); // on crée un tableau de booléens pour savoir si on est déjà passé par une case
    queue* q = create_queue(); // cette file contiendra les coordonnées des cases à visiter
    enqueue(0, 0, q); // on commence par l'entrée
    int x, y;
    while (!isempty_queue(q))
    {
        dequeue(q, &x, &y);
        if (get_bool(visited, x, y)) // si on est déjà passé par cette case, on ne la visite pas
        {
            continue;
        }
        set_true(visited, x, y);
        if (!has_wall_up(maze, x, y) && !get_bool(visited, x, y - 1)) // si on peut aller en haut
        {
            enqueue(x, y - 1, q);
        }
        if (!has_wall_down(maze, x, y) && !get_bool(visited, x, y + 1)) // si on peut aller en bas
        {
            enqueue(x, y + 1, q);
        }
        if (!has_wall_left(maze, x, y) && !get_bool(visited, x - 1, y)) // si on peut aller à gauche
        {
            enqueue(x - 1, y, q);
        }
        if (!has_wall_right(maze, x, y) && !get_bool(visited, x + 1, y)) // si on peut aller à droite
        {
            enqueue(x + 1, y, q);
        }
    }
    for (x = 0; x < maze.width; x++) // on vérifie que toutes les cases ont été visitées
    {
        for (y = 0; y < maze.height; y++)
        {
            if (!get_bool(visited, x, y)) // si une case n'a pas été visitée, c'est que le labyrinthe n'est pas connexe
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

bool is_perfect_breadth_inspector(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width, maze.height); // on crée un tableau de booléens pour savoir si on est déjà passé par une case
    queue* q = create_queue(); // cette file contiendra les coordonnées des cases à visiter
    enqueue(0, 0, q); // on commence par l'entrée
    int x, y;
    while (!isempty_queue(q))
    {
        dequeue(q, &x, &y);
        if (get_bool(visited, x, y)) // si on est déjà passé par cette case, c'est qu'il y a un cycle
        {
            free_queue(q);
            free_booltab(visited);
            return false;
        }
        set_true(visited, x, y);
        int nb_way_used = 0; // on compte le nombre de chemin adjacent utilisé, en théorie il doit être égal à 1 (notre
                             // père) ou 0 (la case de départ)
        if (!has_wall_up(maze, x, y)) // si on peut aller en haut
        {
            if (!get_bool(visited, x, y - 1)) // on ne repasse pas par une case déjà visitée
            {
                enqueue(x, y - 1, q);
            }
            else
            {
                nb_way_used++;
            }
        }
        if (!has_wall_down(maze, x, y)) // si on peut aller en bas
        {
            if (!get_bool(visited, x, y + 1)) // on ne repasse pas par une case déjà visitée
            {
                enqueue(x, y + 1, q);
            }
            else
            {
                nb_way_used++;
            }
        }
        if (!has_wall_left(maze, x, y)) // si on peut aller à gauche
        {
            if (!get_bool(visited, x - 1, y)) // on ne repasse pas par une case déjà visitée
            {
                enqueue(x - 1, y, q);
            }
            else
            {
                nb_way_used++;
            }
        }
        if (!has_wall_right(maze, x, y)) // si on peut aller à droite
        {
            if (!get_bool(visited, x + 1, y)) // on ne repasse pas par une case déjà visitée
            {
                enqueue(x + 1, y, q);
            }
            else
            {
                nb_way_used++;
            }
        }
        if (nb_way_used > 1) // si on a plus d'un chemin adjacent utilisé, c'est qu'il y a un cycle
        {
            free_queue(q);
            free_booltab(visited);
            return false;
        }
    }
    for (x = 0; x < maze.width; x++) // on vérifie que toutes les cases ont été visitées
    {
        for (y = 0; y < maze.height; y++)
        {
            if (!get_bool(visited, x, y)) // si une case n'a pas été visitée, c'est que le labyrinthe n'est pas connexe
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

way* best_exit_breadth_seeker(const maze_t maze)
{
    // on va constituer un tableau de way pour chaque case, qui contiendra le chemin le plus court pour y arriver, en
    // partant de l'entrée il nous permettra de savoir facilement si on est déjà passé par une case car un chemin en
    // largeur est toujours plus court qu'un autre
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
        if (x == maze.width - 1 && y == maze.height - 1) // si on est à la sortie
        {
            way* w = copy_way(get_way(ways, maze.width - 1, maze.height - 1)); // on récupère le chemin pour arriver à la sortie (on
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

// --- Solveur en tirage ---

bool has_exit_draw_seeker(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width,
                                            maze.height); // ce tableau nous permettra de connaitre les cases déjà visitées pour éviter les boucles infinies
    stack* s = create_stack(); // cette pile contiendra les coordonnées des cases à visiter
    push(0, 0, s); // on commence par l'entrée
    int x, y;
    while (!isempty_stack(s)) // on essaie tous les chemins possibles
    {
        rpop(s, &x, &y);
        if (x == maze.width - 1 && y == maze.height - 1) // si on est à la sortie
        {
            free_stack(s);
            free_booltab(visited);
            return true;
        }
        set_true(visited, x, y);
        if (!has_wall_up(maze, x, y) && !get_bool(visited, x, y - 1)) // si on peut aller en haut
        {
            push(x, y - 1, s);
        }
        if (!has_wall_down(maze, x, y) && !get_bool(visited, x, y + 1)) // si on peut aller en bas
        {
            push(x, y + 1, s);
        }
        if (!has_wall_left(maze, x, y) && !get_bool(visited, x - 1, y)) // si on peut aller à gauche
        {
            push(x - 1, y, s);
        }
        if (!has_wall_right(maze, x, y) && !get_bool(visited, x + 1, y)) // si on peut aller à droite
        {
            push(x + 1, y, s);
        }
    }
    free_booltab(visited);
    free_stack(s);
    return false;
}

bool is_connexe_draw_inspector(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width, maze.height); // on crée un tableau de booléens pour savoir si on est déjà passé par une case
    stack* s = create_stack(); // cette pile contiendra les coordonnées des cases à visiter
    push(0, 0, s); // on commence par l'entrée
    int x, y;
    while (!isempty_stack(s)) // on doit visiter toutes les cases
    {
        rpop(s, &x, &y);
        if (get_bool(visited, x, y)) // si on est déjà passé par cette case, on ne la visite pas
        {
            continue;
        }
        set_true(visited, x, y);
        if (!has_wall_up(maze, x, y) && !get_bool(visited, x, y - 1)) // si on peut aller en haut
        {
            push(x, y - 1, s);
        }
        if (!has_wall_down(maze, x, y) && !get_bool(visited, x, y + 1)) // si on peut aller en bas
        {
            push(x, y + 1, s);
        }
        if (!has_wall_left(maze, x, y) && !get_bool(visited, x - 1, y)) // si on peut aller à gauche
        {
            push(x - 1, y, s);
        }
        if (!has_wall_right(maze, x, y) && !get_bool(visited, x + 1, y)) // si on peut aller à droite
        {
            push(x + 1, y, s);
        }
    }
    for (x = 0; x < maze.width; x++) // on vérifie que toutes les cases ont été visitées
    {
        for (y = 0; y < maze.height; y++)
        {
            if (!get_bool(visited, x, y)) // si une case n'a pas été visitée, c'est que le labyrinthe n'est pas connexe
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

bool is_perfect_draw_inspector(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width, maze.height); // on crée un tableau de booléens pour savoir si on est déjà passé par une case
    stack* s = create_stack(); // cette pile contiendra les coordonnées des cases à visiter
    push(0, 0, s); // on commence par l'entrée
    int x, y;
    while (!isempty_stack(s)) // on doit visiter toutes les cases
    {
        rpop(s, &x, &y);
        if (get_bool(visited, x, y)) // si on est déjà passé par cette case, c'est qu'il y a un cycle
        {
            free_stack(s);
            free_booltab(visited);
            return false;
        }
        set_true(visited, x, y);
        int nb_way_used = 0; // on compte le nombre de chemin adjacent utilisé, en théorie il doit être égal à 1 (notre
                             // père) ou 0 (la case de départ)
        if (!has_wall_up(maze, x, y)) // si on peut aller en haut
        {
            if (!get_bool(visited, x, y - 1)) // on ne repasse pas par une case déjà visitée
            {
                push(x, y - 1, s);
            }
            else
            {
                nb_way_used++;
            }
        }
        if (!has_wall_down(maze, x, y)) // si on peut aller en bas
        {
            if (!get_bool(visited, x, y + 1)) // on ne repasse pas par une case déjà visitée
            {
                push(x, y + 1, s);
            }
            else
            {
                nb_way_used++;
            }
        }
        if (!has_wall_left(maze, x, y)) // si on peut aller à gauche
        {
            if (!get_bool(visited, x - 1, y)) // on ne repasse pas par une case déjà visitée
            {
                push(x - 1, y, s);
            }
            else
            {
                nb_way_used++;
            }
        }
        if (!has_wall_right(maze, x, y)) // si on peut aller à droite
        {
            if (!get_bool(visited, x + 1, y)) // on ne repasse pas par une case déjà visitée
            {
                push(x + 1, y, s);
            }
            else
            {
                nb_way_used++;
            }
        }
        if (nb_way_used > 1) // si on a plus d'un chemin adjacent utilisé, c'est qu'il y a un cycle
        {
            free_stack(s);
            free_booltab(visited);
            return false;
        }
        if (nb_way_used == 0 && (x != 0 || y != 0)) // si on est sur une case qui n'est pas la case de départ et qu'on a
                                                    // aucun chemin adjacent utilisé, c'est qu'il y a un problème
        {
            free_stack(s);
            free_booltab(visited);
            fprintf(stderr,
                    "Erreur dans la fonction perfect_deep_inspector : la case (%d, %d) a été inspectée mais n'a aucun "
                    "chemin parent.\n",
                    x, y);
            exit(EXIT_FAILURE);
        }
    }
    for (x = 0; x < maze.width; x++) // on vérifie que toutes les cases ont été visitées
    {
        for (y = 0; y < maze.height; y++)
        {
            if (!get_bool(visited, x, y)) // si une case n'a pas été visitée, c'est que le labyrinthe n'est pas connexe
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

way* best_exit_draw_seeker(const maze_t maze)
{
    // on va constituer un tableau de way pour chaque case, qui contiendra le chemin le plus court pour y arriver, en
    // partant de l'entrée il nous permettra d'éviter les cycle (la taille du cycle serait plus grande) et de relier des
    // fragments de chemin plutôt que de tout recalculer
    const waytab ways = create_waytab(maze.width,
                                      maze.height); // ce tableau nous permettra de connaitre le chemin le plus court pour arriver à une case
    const bool_tab dead_end = create_booltab(maze.width, maze.height); // ce tableau nous permettra de savoir si une case est un
                                                                       // cul-de-sac, pour éviter de recalculer le chemin
    stack* s = create_stack(); // cette pile contiendra les coordonnées des cases à visiter
    push(0, 0, s); // on commence par l'entrée
    int x, y;
    unsigned int count = 0;
    while (!isempty_stack(s))
    {
        rpop(s, &x, &y);
        if (x == maze.width - 1 && y == maze.height - 1) // si on est à la sortie, on s'arrête
        {
            continue;
        }
        const unsigned int l = length_waytab(ways, x, y) + 1; // on récupère la longueur du chemin actuel
        const int abs_dist = -x + maze.width - 1 - y + maze.height - 1; // distance de Manhattan à la sortie
        if (l + abs_dist > length_waytab(ways, maze.width - 1, maze.height - 1)) // si on a déjà trouvé un chemin plus court
        {
            continue;
        }
        if (!has_wall_up(maze, x, y) && !get_bool(dead_end, x, y - 1)) // si on peut aller en haut
        {
            count++;
            if (l < length_waytab(ways, x, y - 1)) // si on peut aller en haut et que le chemin est plus court
            {
                push(x, y - 1, s); // on ajoute la case à celle à visiter
                connected_way(ways, x, y - 1, x, y); // on crée un chemin entre la case actuelle et la case en haut
            }
        }
        if (!has_wall_down(maze, x, y) && !get_bool(dead_end, x, y + 1)) // si on peut aller en bas
        {
            count++;
            if (l < length_waytab(ways, x, y + 1)) // si on peut aller en bas et que le chemin est plus court
            {
                push(x, y + 1, s); // on ajoute la case à celle à visiter
                connected_way(ways, x, y + 1, x, y); // on crée un chemin entre la case actuelle et la case en bas
            }
        }
        if (!has_wall_left(maze, x, y) && !get_bool(dead_end, x - 1, y)) // si on peut aller à gauche
        {
            count++;
            if (l < length_waytab(ways, x - 1, y)) // si on peut aller à gauche et que le chemin est plus court
            {
                push(x - 1, y, s); // on ajoute la case à celle à visiter
                connected_way(ways, x - 1, y, x, y); // on crée un chemin entre la case actuelle et la case à gauche
            }
        }
        if (!has_wall_right(maze, x, y) && !get_bool(dead_end, x + 1, y)) // si on peut aller à droite
        {
            count++;
            if (l < length_waytab(ways, x + 1, y)) // si on peut aller à droite et que le chemin est plus court
            {
                push(x + 1, y, s); // on ajoute la case à celle à visiter
                connected_way(ways, x + 1, y, x, y); // on crée un chemin entre la case actuelle et la case à droite
            }
        }
        if (count <= 1) // si on est dans un cul-de-sac
        {
            set_true(dead_end, x, y); // on marque la case comme un cul-de-sac
        }
        count = 0;
    }
    way* w = copy_way(get_way(ways, maze.width - 1,
                              maze.height - 1)); // on récupère le chemin pour arriver à la sortie (on le copie car on va libérer le tableau ways)
    free_booltab(dead_end);
    free_waytab(ways);
    free_stack(s);
    return w;
}

// --- FONCTIONS DE VISUALISATION ---

// --- Solveur en profondeur ---

int show_has_exit_deep_seeker(const maze_t maze)
{
    int reach = maze.width * maze.height / 100; // on affichera un frame tous les 1% du labyrinthe
    if (reach == 0)
    {
        reach = 1;
    }
    int frame_count = 0;
    const bool_tab visited = create_booltab(maze.width,
                                            maze.height); // ce tableau nous permettra de connaitre les cases déjà visitées pour éviter les boucles infinies
    stack* s = create_stack(); // cette pile contiendra les coordonnées des cases à visiter
    push(0, 0, s); // on commence par l'entrée
    set_true(visited, 0, 0); // on marque la case de départ comme visitée
    int x, y;
    SDL_Renderer* renderer = NULL;
    SDL_Window* window = NULL;
    int dw, dh;
    initial_print_maze(maze, &renderer, &window, &dw, &dh);
    SDL_SetWindowTitle(window, "exit deep seeker");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    if (renderer == NULL || window == NULL)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de l'affichage du labyrinthe.\n");
        return -1;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // on dessine en bleu
    SDL_Event event = {0}; // on crée un event vide
    while (!isempty_stack(s)) // on essaie tous les chemins possibles
    {
        SDL_WaitEventTimeout(&event, 0); // on enregistre les events entrants
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
            (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)) // si l'utilisateur veut fermer la fenêtre
        {
            printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
            free_stack(s);
            free_booltab(visited);
            destroy_print_maze(renderer, window);
            return 1;
        }
        pop(s, &x, &y);
        if (x == maze.width - 1 && y == maze.height - 1) // si on est à la sortie
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // on dessine en vert
            const SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetWindowTitle(window, "exit found !"); // on change le titre de la fenêtre
            SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            wait_and_destroy_print_maze(renderer, window); // on attend que l'utilisateur ferme la fenêtre
            free_stack(s);
            free_booltab(visited);
            return 1;
        }
        SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
        SDL_RenderFillRect(renderer, &rect);
        if (!has_wall_up(maze, x, y)) // si on peut aller en haut
        {
            SDL_RenderDrawLine(renderer, x * dw + 1, y * dh, (x + 1) * dw - 2,
                               y * dh); // on dessine une ligne vers le haut
            if (!get_bool(visited, x, y - 1)) // on ne repasse pas par une case déjà visitée
            {
                push(x, y - 1, s);
                set_true(visited, x, y - 1); // on marque la case comme visitée por éviter de repasser par
            }
        }
        if (!has_wall_down(maze, x, y)) // si on peut aller en bas
        {
            SDL_RenderDrawLine(renderer, x * dw + 1, (y + 1) * dh - 1, (x + 1) * dw - 2,
                               (y + 1) * dh - 1); // on dessine une ligne vers le bas
            if (!get_bool(visited, x, y + 1)) // on ne repasse pas par une case déjà visitée
            {
                push(x, y + 1, s);
                set_true(visited, x, y + 1); // on marque la case comme visitée por éviter de repasser par là
            }
        }
        if (!has_wall_left(maze, x, y)) // si on peut aller à gauche
        {
            SDL_RenderDrawLine(renderer, x * dw, y * dh + 1, x * dw,
                               (y + 1) * dh - 2); // on dessine une ligne vers la gauche
            if (!get_bool(visited, x - 1, y)) // on ne repasse pas par une case déjà visitée
            {
                push(x - 1, y, s);
                set_true(visited, x - 1, y); // on marque la case comme visitée por éviter de repasser par là
            }
        }
        if (!has_wall_right(maze, x, y)) // si on peut aller à droite
        {
            SDL_RenderDrawLine(renderer, (x + 1) * dw - 1, y * dh + 1, (x + 1) * dw - 1,
                               (y + 1) * dh - 2); // on dessine une ligne vers la droite
            if (!get_bool(visited, x + 1, y)) // on ne repasse pas par une case déjà visitée
            {
                push(x + 1, y, s);
                set_true(visited, x + 1, y); // on marque la case comme visitée por éviter de repasser par là
            }
        }
        frame_count++;
        if (frame_count == reach)
        {
            SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            frame_count = 0;
        }
    }
    SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
    SDL_RenderPresent(renderer);
    SDL_SetWindowTitle(window, "exit do not exist"); // on change le titre de la fenêtre
    wait_and_destroy_print_maze(renderer, window);
    free_stack(s);
    free_booltab(visited);
    return 1;
}

int show_is_connexe_deep_inspector(const maze_t maze)
{
    int reach = maze.width * maze.height / 100; // on affichera un frame tous les 1% du labyrinthe
    if (reach == 0)
    {
        reach = 1;
    }
    int frame_count = 0;
    const bool_tab visited = create_booltab(maze.width, maze.height); // ce tableau nous permettra de connaitre les cases déjà visitées pou
                                                                      // repérer les cycles et la connexité
    stack* s = create_stack(); // cette pile contiendra les coordonnées des cases à visiter
    push(0, 0, s); // on commence par l'entrée
    int x, y;
    SDL_Renderer* renderer = NULL;
    SDL_Window* window = NULL;
    int dw, dh;
    initial_print_maze(maze, &renderer, &window, &dw, &dh);
    SDL_SetWindowTitle(window, "connected deep inspector");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    if (renderer == NULL || window == NULL)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de l'affichage du labyrinthe.\n");
        return -1;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // on dessine en bleu
    SDL_Event event = {0}; // on crée un event vide
    while (!isempty_stack(s)) // on essaie tous les chemins possibles
    {
        SDL_WaitEventTimeout(&event, 0); // on enregistre les events entrants
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
            (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)) // si l'utilisateur veut fermer la fenêtre
        {
            printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
            free_stack(s);
            free_booltab(visited);
            destroy_print_maze(renderer, window);
            return 1;
        }
        pop(s, &x, &y);
        if (get_bool(visited, x, y)) // si on est dejà passé par là, on continue
        {
            continue;
        }
        set_true(visited, x, y);
        SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
        SDL_RenderFillRect(renderer, &rect);
        if (!has_wall_up(maze, x, y)) // si on peut aller en haut
        {
            SDL_RenderDrawLine(renderer, x * dw + 1, y * dh, (x + 1) * dw - 2,
                               y * dh); // on dessine une ligne vers le haut
            if (!get_bool(visited, x, y - 1)) // si on n'est pas déjà passé par cette case
            {
                push(x, y - 1, s);
            }
        }
        if (!has_wall_down(maze, x, y)) // si on peut aller en bas
        {
            SDL_RenderDrawLine(renderer, x * dw + 1, (y + 1) * dh - 1, (x + 1) * dw - 2,
                               (y + 1) * dh - 1); // on dessine une ligne vers le bas
            if (!get_bool(visited, x, y + 1)) // si on n'est pas déjà passé par cette case
            {
                push(x, y + 1, s);
            }
        }
        if (!has_wall_left(maze, x, y)) // si on peut aller à gauche
        {
            SDL_RenderDrawLine(renderer, x * dw, y * dh + 1, x * dw,
                               (y + 1) * dh - 2); // on dessine une ligne vers la gauche
            if (!get_bool(visited, x - 1, y)) // si on n'est pas déjà passé par cette case
            {
                push(x - 1, y, s);
            }
        }
        if (!has_wall_right(maze, x, y)) // si on peut aller à droite
        {
            SDL_RenderDrawLine(renderer, (x + 1) * dw - 1, y * dh + 1, (x + 1) * dw - 1,
                               (y + 1) * dh - 2); // on dessine une ligne vers la droite
            if (!get_bool(visited, x + 1, y)) // si on n'est pas déjà passé par cette case
            {
                push(x + 1, y, s);
            }
        }
        frame_count++;
        if (frame_count == reach)
        {
            SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            frame_count = 0;
        }
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // on dessine en rouge
    bool is_connected = true;
    for (x = 0; x < maze.width; x++) // on vérifie que toutes les cases ont été visitées
    {
        for (y = 0; y < maze.height; y++)
        {
            if (!get_bool(visited, x, y)) // si une case n'a pas été visitée, c'est que le labyrinthe n'est pas connexe
            {
                const SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
                SDL_RenderFillRect(renderer, &rect);
                if (!has_wall_down(maze, x, y)) // si on peut aller en bas
                {
                    SDL_RenderDrawLine(renderer, x * dw + 1, (y + 1) * dh - 1, (x + 1) * dw - 2,
                                       (y + 1) * dh - 1); // on dessine une ligne vers le bas
                }
                if (!has_wall_right(maze, x, y)) // si on peut aller à droite
                {
                    SDL_RenderDrawLine(renderer, (x + 1) * dw - 1, y * dh + 1, (x + 1) * dw - 1,
                                       (y + 1) * dh - 2); // on dessine une ligne vers la droite
                }
                if (!has_wall_up(maze, x, y)) // si on peut aller en haut
                {
                    SDL_RenderDrawLine(renderer, x * dw + 1, y * dh, (x + 1) * dw - 2,
                                       y * dh); // on dessine une ligne vers le haut
                }
                if (!has_wall_left(maze, x, y)) // si on peut aller à gauche
                {
                    SDL_RenderDrawLine(renderer, x * dw, y * dh + 1, x * dw,
                                       (y + 1) * dh - 2); // on dessine une ligne vers la gauche
                }
                is_connected = false;
            }
        }
    }
    if (!is_connected)
    {
        SDL_SetWindowTitle(window, "not connected !"); // on change le titre de la fenêtre
        SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
        SDL_RenderPresent(renderer);
        wait_and_destroy_print_maze(renderer, window); // on attend que l'utilisateur ferme la fenêtre
        free_stack(s);
        free_booltab(visited);
        return 1;
    }
    SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
    SDL_RenderPresent(renderer);
    SDL_SetWindowTitle(window, "Maze connected !"); // on change le titre de la fenêtre
    wait_and_destroy_print_maze(renderer, window);
    free_stack(s);
    free_booltab(visited);
    return 1;
}

int show_is_perfect_deep_inspector(const maze_t maze)
{
    int reach = maze.width * maze.height / 100; // on affichera un frame tous les 1% du labyrinthe
    if (reach == 0)
    {
        reach = 1;
    }
    int frame_count = 0;
    const bool_tab visited = create_booltab(maze.width, maze.height); // ce tableau nous permettra de connaitre les cases déjà visitées pou
                                                                      // repérer les cycles et la connexité
    stack* s = create_stack(); // cette pile contiendra les coordonnées des cases à visiter
    push(0, 0, s); // on commence par l'entrée
    int x, y;
    SDL_Renderer* renderer = NULL;
    SDL_Window* window = NULL;
    int dw, dh;
    initial_print_maze(maze, &renderer, &window, &dw, &dh);
    SDL_SetWindowTitle(window, "is perfect deep inspector");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    if (renderer == NULL || window == NULL)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de l'affichage du labyrinthe.\n");
        return -1;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // on dessine en bleu
    SDL_Event event = {0}; // on crée un event vide
    while (!isempty_stack(s)) // on essaie tous les chemins possibles
    {
        SDL_WaitEventTimeout(&event, 0); // on enregistre les events entrants
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
            (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)) // si l'utilisateur veut fermer la fenêtre
        {
            printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
            free_stack(s);
            free_booltab(visited);
            destroy_print_maze(renderer, window);
            return 1;
        }
        pop(s, &x, &y);
        set_true(visited, x, y);
        SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
        SDL_RenderFillRect(renderer, &rect);
        int nb_way_used = 0; // on compte le nombre de chemin adjacent utilisé, en théorie il doit être égal à 1 (notre
                             // père) ou 0 (la case de départ)
        if (!has_wall_up(maze, x, y)) // si on peut aller en haut
        {
            SDL_RenderDrawLine(renderer, x * dw + 1, y * dh, (x + 1) * dw - 2,
                               y * dh); // on dessine une ligne vers le haut
            if (get_bool(visited, x, y - 1)) // si on est déjà passé par cette case
            {
                nb_way_used++;
            }
            else
            {
                push(x, y - 1, s);
            }
        }
        if (!has_wall_down(maze, x, y)) // si on peut aller en bas
        {
            SDL_RenderDrawLine(renderer, x * dw + 1, (y + 1) * dh - 1, (x + 1) * dw - 2,
                               (y + 1) * dh - 1); // on dessine une ligne vers le bas
            if (get_bool(visited, x, y + 1)) // si on est déjà passé par cette case
            {
                nb_way_used++;
            }
            else
            {
                push(x, y + 1, s);
            }
        }
        if (!has_wall_left(maze, x, y)) // si on peut aller à gauche
        {
            SDL_RenderDrawLine(renderer, x * dw, y * dh + 1, x * dw,
                               (y + 1) * dh - 2); // on dessine une ligne vers la gauche
            if (get_bool(visited, x - 1, y)) // si on est déjà passé par cette case
            {
                nb_way_used++;
            }
            else
            {
                push(x - 1, y, s);
            }
        }
        if (!has_wall_right(maze, x, y)) // si on peut aller à droite
        {
            SDL_RenderDrawLine(renderer, (x + 1) * dw - 1, y * dh + 1, (x + 1) * dw - 1,
                               (y + 1) * dh - 2); // on dessine une ligne vers la droite
            if (get_bool(visited, x + 1, y)) // si on est déjà passé par cette case
            {
                nb_way_used++;
            }
            else
            {
                push(x + 1, y, s);
            }
        }
        if (nb_way_used > 1) // si on a trop de chemin adjacent utilisé, c'est qu'il y a un cycle
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // on dessine en rouge
            const SDL_Rect rec = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
            SDL_RenderFillRect(renderer, &rec);
            SDL_SetWindowTitle(window, "cycle found !"); // on change le titre de la fenêtre
            SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            wait_and_destroy_print_maze(renderer, window); // on attend que l'utilisateur ferme la fenêtre
            free_stack(s);
            free_booltab(visited);
            return 1;
        }
        frame_count++;
        if (frame_count == reach)
        {
            SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            frame_count = 0;
        }
    }
    for (x = 0; x < maze.width; x++) // on vérifie que toutes les cases ont été visitées
    {
        for (y = 0; y < maze.height; y++)
        {
            if (!get_bool(visited, x, y)) // si une case n'a pas été visitée, c'est que le labyrinthe n'est pas connexe
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // on dessine en rouge
                const SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetWindowTitle(window, "not connected !"); // on change le titre de la fenêtre
                SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
                SDL_RenderPresent(renderer);
                wait_and_destroy_print_maze(renderer, window); // on attend que l'utilisateur ferme la fenêtre
                free_stack(s);
                free_booltab(visited);
                return 1;
            }
        }
    }
    SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
    SDL_RenderPresent(renderer);
    SDL_SetWindowTitle(window, "perfect maze !"); // on change le titre de la fenêtre
    wait_and_destroy_print_maze(renderer, window);
    free_stack(s);
    free_booltab(visited);
    return 1;
}

int show_best_exit_deep_seeker(const maze_t maze)
{
    // on va constituer un tableau de way pour chaque case, qui contiendra le chemin le plus court pour y arriver, en
    // partant de l'entrée il nous permettra d'éviter les cycle (la taille du cycle serait plus grande) et de relier des
    // fragments de chemin plutôt que de tout recalculer
    int frame_count = 0;
    unsigned int reach = maze.width * maze.height / 100; // on affichera un frame tous les 1% du labyrinthe
    if (reach == 0)
    {
        reach = 1;
    }
    const waytab ways = create_waytab(maze.width,
                                      maze.height); // ce tableau nous permettra de connaitre le chemin le plus court pour arriver à une case
    const bool_tab dead_end = create_booltab(maze.width, maze.height); // ce tableau nous permettra de savoir si une case est un
                                                                       // cul-de-sac, pour éviter de recalculer le chemin
    stack* s = create_stack(); // cette pile contiendra les coordonnées des cases à visiter
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    int dw, dh;
    unsigned int count = 0;
    initial_print_maze(maze, &renderer, &window, &dw, &dh);
    SDL_SetWindowTitle(window, "best exit deep seeker");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    if (renderer == NULL || window == NULL)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de l'affichage du labyrinthe.\n");
        return -1;
    }
    push(0, 0, s); // on commence par l'entrée
    int x, y;
    SDL_Event event = {0}; // on crée un event vide
    while (!isempty_stack(s))
    {
        SDL_WaitEventTimeout(&event, 0); // on enregistre les events entrants
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
            (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)) // si l'utilisateur veut fermer la fenêtre
        {
            printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
            free_stack(s);
            free_waytab(ways);
            free_booltab(dead_end);
            destroy_print_maze(renderer, window);
            return 1;
        }
        pop(s, &x, &y);
        SDL_SetRenderDrawColor(renderer, 125, 125, 125, 255); // on dessine en gris
        SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
        SDL_RenderFillRect(renderer, &rect);
        frame_count++;
        if (frame_count == reach)
        {
            SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // on dessine en bleu
        if (x == maze.width - 1 && y == maze.height - 1) // si on est à la sortie, on s'arrête
        {
            continue;
        }
        const unsigned int l = length_waytab(ways, x, y) + 1; // longueur du chemin actuel
        const int abs_dist = -x + maze.width - 1 - y + maze.height - 1; // distance de Manhattan à la sortie
        if (l + abs_dist > length_waytab(ways, maze.width - 1, maze.height - 1)) // si on a déjà trouvé un chemin plus court
        {
            continue;
        }
        if (!has_wall_up(maze, x, y) && !get_bool(dead_end, x, y - 1)) // si on peut aller en haut et que la case n'est pas un cul-de-sac
        {
            count++;
            SDL_RenderDrawLine(renderer, x * dw + 1, y * dh, (x + 1) * dw - 2,
                               y * dh); // on dessine une ligne vers le haut
            if (l < length_waytab(ways, x, y - 1))
            {
                push(x, y - 1, s); // on ajoute la case à celle à visiter
                connected_way(ways, x, y - 1, x, y); // on crée un chemin entre la case actuelle et la case en haut
            }
        }
        if (!has_wall_down(maze, x, y) && !get_bool(dead_end, x, y + 1)) // si on peut aller en bas et que la case n'est pas un cul-de-sac
        {
            count++;
            SDL_RenderDrawLine(renderer, x * dw + 1, (y + 1) * dh - 1, (x + 1) * dw - 2, (y + 1) * dh - 1);
            if (l < length_waytab(ways, x, y + 1))
            {
                push(x, y + 1, s); // on ajoute la case à celle à visiter
                connected_way(ways, x, y + 1, x, y); // on crée un chemin entre la case actuelle et la case en bas
            }
        }
        if (!has_wall_left(maze, x, y) && !get_bool(dead_end, x - 1, y)) // si on peut aller à gauche et que la case n'est pas un cul-de-sac
        {
            count++;
            SDL_RenderDrawLine(renderer, x * dw, y * dh + 1, x * dw,
                               (y + 1) * dh - 2); // on dessine une ligne vers la gauche
            if (l < length_waytab(ways, x - 1, y))
            {
                push(x - 1, y, s); // on ajoute la case à celle à visiter
                connected_way(ways, x - 1, y, x, y); // on crée un chemin entre la case actuelle et la case à gauche
            }
        }
        if (!has_wall_right(maze, x, y) && !get_bool(dead_end, x + 1, y)) // si on peut aller à droite et que la case n'est pas un cul-de-sac
        {
            count++;
            SDL_RenderDrawLine(renderer, (x + 1) * dw - 1, y * dh + 1, (x + 1) * dw - 1,
                               (y + 1) * dh - 2); // on dessine une ligne vers la droite
            if (l < length_waytab(ways, x + 1, y))
            {
                push(x + 1, y, s); // on ajoute la case à celle à visiter
                connected_way(ways, x + 1, y, x, y); // on crée un chemin entre la case actuelle et la case à droite
            }
        }
        SDL_RenderFillRect(renderer, &rect);
        if (count <= 1)
        {
            set_true(dead_end, x, y); // on marque la case comme une impasse
            SDL_SetRenderDrawColor(renderer, 125, 125, 125, 255); // on dessine en gris
            SDL_RenderFillRect(renderer, &rect);
        }
        count = 0;
        if (frame_count == reach)
        {
            SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            frame_count = 0;
        }
    }
    way* w = copy_way(get_way(ways, maze.width - 1,
                              maze.height - 1)); // on récupère le chemin pour arriver à la sortie (on le copie car on va libérer le tableau ways)
    free_waytab(ways);
    free_stack(s);
    free_booltab(dead_end);
    if (is_empty(w))
    {
        SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
        SDL_RenderPresent(renderer);
        SDL_SetWindowTitle(window, "exit do not exist"); // on change le titre de la fenêtre
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // on dessine en vert
        SDL_SetWindowTitle(window, "best exit found !"); // on change le titre de la fenêtre
        reach = length_way(w) / 10; // on affichera un frame tous les 10% du chemin
        if (reach == 0)
        {
            reach = 1;
        }
        frame_count = 0;
        while (w != NULL)
        {
            SDL_Rect rec = {get_x(w) * dw + 1, get_y(w) * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
            SDL_RenderFillRect(renderer, &rec);
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
            frame_count++;
            if (frame_count == reach)
            {
                SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
                SDL_RenderPresent(renderer);
                frame_count = 0;
            }
            w = get_dad(w);
        }
    }
    SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
    SDL_RenderPresent(renderer);
    wait_and_destroy_print_maze(renderer, window);
    free_way(w);
    return 1;
}

// --- Solveur en largeur ---

int show_has_exit_breadth_seeker(const maze_t maze)
{
    int frame_count = 0;
    int reach = 1;
    const bool_tab visited = create_booltab(maze.width,
                                            maze.height); // ce tableau nous permettra de connaitre les cases déjà visitées pour éviter les boucles infinies
    queue* q = create_queue(); // cette file contiendra les coordonnées des cases à visiter
    enqueue(0, 0, q); // on commence par l'entrée
    set_true(visited, 0, 0); // on marque la case de départ comme visitée
    int x, y;
    SDL_Renderer* renderer = NULL;
    SDL_Window* window = NULL;
    int dw, dh;
    initial_print_maze(maze, &renderer, &window, &dw, &dh);
    SDL_SetWindowTitle(window, "exit breadth seeker");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    if (renderer == NULL || window == NULL)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de l'affichage du labyrinthe.\n");
        return -1;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // on dessine en bleu
    SDL_Event event = {0}; // on crée un event vide
    while (!isempty_queue(q)) // on essaie tous les chemins possibles
    {
        SDL_WaitEventTimeout(&event, 0); // on enregistre les events entrants
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
            (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)) // si l'utilisateur veut fermer la fenêtre
        {
            printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
            free_queue(q);
            free_booltab(visited);
            destroy_print_maze(renderer, window);
            return 1;
        }
        dequeue(q, &x, &y);
        if (x == maze.width - 1 && y == maze.height - 1) // si on est à la sortie
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // on dessine en vert
            const SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetWindowTitle(window, "exit found !"); // on change le titre de la fenêtre
            SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            wait_and_destroy_print_maze(renderer, window); // on attend que l'utilisateur ferme la fenêtre
            free_queue(q);
            free_booltab(visited);
            return 1;
        }
        SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
        SDL_RenderFillRect(renderer, &rect);
        if (!has_wall_up(maze, x, y)) // si on peut aller en haut
        {
            SDL_RenderDrawLine(renderer, x * dw + 1, y * dh, (x + 1) * dw - 2,
                               y * dh); // on dessine une ligne vers le haut
            if (!get_bool(visited, x, y - 1)) // on ne repasse pas par une case déjà visitée
            {
                enqueue(x, y - 1, q);
                set_true(visited, x, y - 1); // on marque la case comme visitée pour éviter de repasser par
            }
        }
        if (!has_wall_down(maze, x, y)) // si on peut aller en bas
        {
            SDL_RenderDrawLine(renderer, x * dw + 1, (y + 1) * dh - 1, (x + 1) * dw - 2,
                               (y + 1) * dh - 1); // on dessine une ligne vers le bas
            if (!get_bool(visited, x, y + 1)) // on ne repasse pas par une case déjà visitée
            {
                enqueue(x, y + 1, q);
                set_true(visited, x, y + 1); // on marque la case comme visitée por éviter de repasser par là
            }
        }
        if (!has_wall_left(maze, x, y)) // si on peut aller à gauche
        {
            SDL_RenderDrawLine(renderer, x * dw, y * dh + 1, x * dw,
                               (y + 1) * dh - 2); // on dessine une ligne vers la gauche
            if (!get_bool(visited, x - 1, y)) // on ne repasse pas par une case déjà visitée
            {
                enqueue(x - 1, y, q);
                set_true(visited, x - 1, y); // on marque la case comme visitée por éviter de repasser par là
            }
        }
        if (!has_wall_right(maze, x, y)) // si on peut aller à droite
        {
            SDL_RenderDrawLine(renderer, (x + 1) * dw - 1, y * dh + 1, (x + 1) * dw - 1,
                               (y + 1) * dh - 2); // on dessine une ligne vers la droite
            if (!get_bool(visited, x + 1, y)) // on ne repasse pas par une case déjà visitée
            {
                enqueue(x + 1, y, q);
                set_true(visited, x + 1, y); // on marque la case comme visitée por éviter de repasser par là
            }
        }
        frame_count++;
        if (frame_count == reach)
        {
            SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            frame_count = 0;
            reach = size_queue(q) / 2;
        }
    }
    SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
    SDL_RenderPresent(renderer);
    SDL_SetWindowTitle(window, "exit do not exist"); // on change le titre de la fenêtre
    wait_and_destroy_print_maze(renderer, window);
    free_queue(q);
    free_booltab(visited);
    return 1;
}

int show_is_connexe_breadth_inspector(const maze_t maze)
{
    int frame_count = 0;
    int reach = 1;
    const bool_tab visited = create_booltab(maze.width, maze.height); // ce tableau nous permettra de connaitre les cases déjà visitées pou
                                                                      // repérer les cycles et la connexité
    queue* q = create_queue(); // cette file contiendra les coordonnées des cases à visiter
    enqueue(0, 0, q); // on commence par l'entrée
    int x, y;
    SDL_Renderer* renderer = NULL;
    SDL_Window* window = NULL;
    int dw, dh;
    initial_print_maze(maze, &renderer, &window, &dw, &dh);
    SDL_SetWindowTitle(window, "connected breath inspector");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    if (renderer == NULL || window == NULL)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de l'affichage du labyrinthe.\n");
        return -1;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // on dessine en bleu
    SDL_Event event = {0}; // on crée un event vide
    while (!isempty_queue(q)) // on essaie tous les chemins possibles
    {
        SDL_WaitEventTimeout(&event, 0); // on enregistre les events entrants
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
            (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)) // si l'utilisateur veut fermer la fenêtre
        {
            printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
            free_queue(q);
            free_booltab(visited);
            destroy_print_maze(renderer, window);
            return 1;
        }
        dequeue(q, &x, &y);
        if (get_bool(visited, x, y)) // si on est déjà passé par cette case
        {
            continue;
        }
        set_true(visited, x, y);
        SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
        SDL_RenderFillRect(renderer, &rect);
        if (!has_wall_up(maze, x, y)) // si on peut aller en haut
        {
            SDL_RenderDrawLine(renderer, x * dw + 1, y * dh, (x + 1) * dw - 2,
                               y * dh); // on dessine une ligne vers le haut
            enqueue(x, y - 1, q);
        }
        if (!has_wall_down(maze, x, y)) // si on peut aller en bas
        {
            SDL_RenderDrawLine(renderer, x * dw + 1, (y + 1) * dh - 1, (x + 1) * dw - 2,
                               (y + 1) * dh - 1); // on dessine une ligne vers le bas
            enqueue(x, y + 1, q);
        }
        if (!has_wall_left(maze, x, y)) // si on peut aller à gauche
        {
            SDL_RenderDrawLine(renderer, x * dw, y * dh + 1, x * dw,
                               (y + 1) * dh - 2); // on dessine une ligne vers la gauche
            enqueue(x - 1, y, q);
        }
        if (!has_wall_right(maze, x, y)) // si on peut aller à droite
        {
            SDL_RenderDrawLine(renderer, (x + 1) * dw - 1, y * dh + 1, (x + 1) * dw - 1,
                               (y + 1) * dh - 2); // on dessine une ligne vers la droite
            enqueue(x + 1, y, q);
        }
        frame_count++;
        if (frame_count == reach)
        {
            SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            frame_count = 0;
            reach = size_queue(q) / 2;
        }
    }
    bool connected = true;
    for (x = 0; x < maze.width; x++) // on vérifie que toutes les cases ont été visitées
    {
        for (y = 0; y < maze.height; y++)
        {
            if (!get_bool(visited, x, y)) // si une case n'a pas été visitée, c'est que le labyrinthe n'est pas connexe
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // on dessine en rouge
                const SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
                if (!has_wall_down(maze, x, y))
                {
                    SDL_RenderDrawLine(renderer, x * dw + 1, (y + 1) * dh - 1, (x + 1) * dw - 2,
                                       (y + 1) * dh - 1); // on dessine une ligne vers le bas
                }
                if (!has_wall_up(maze, x, y))
                {
                    SDL_RenderDrawLine(renderer, x * dw + 1, y * dh, (x + 1) * dw - 2,
                                       y * dh); // on dessine une ligne vers le haut
                }
                if (!has_wall_left(maze, x, y))
                {
                    SDL_RenderDrawLine(renderer, x * dw, y * dh + 1, x * dw,
                                       (y + 1) * dh - 2); // on dessine une ligne vers la gauche
                }
                if (!has_wall_right(maze, x, y))
                {
                    SDL_RenderDrawLine(renderer, (x + 1) * dw - 1, y * dh + 1, (x + 1) * dw - 1,
                                       (y + 1) * dh - 2); // on dessine une ligne vers la droite
                }
                SDL_RenderFillRect(renderer, &rect);
                connected = false;
            }
        }
    }
    if (!connected)
    {

        SDL_SetWindowTitle(window, "not connected !"); // on change le titre de la fenêtre
        SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
        SDL_RenderPresent(renderer);
        wait_and_destroy_print_maze(renderer, window); // on attend que l'utilisateur ferme la fenêtre
        free_queue(q);
        free_booltab(visited);
        return 1;
    }
    SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
    SDL_RenderPresent(renderer);
    SDL_SetWindowTitle(window, "Maze connected !"); // on change le titre de la fenêtre
    wait_and_destroy_print_maze(renderer, window);
    free_queue(q);
    free_booltab(visited);
    return 1;
}

int show_is_perfect_breadth_inspector(const maze_t maze)
{
    int frame_count = 0;
    int reach = 1;
    const bool_tab visited = create_booltab(maze.width, maze.height); // ce tableau nous permettra de connaitre les cases déjà visitées pou
                                                                      // repérer les cycles et la connexité
    queue* q = create_queue(); // cette file contiendra les coordonnées des cases à visiter
    enqueue(0, 0, q); // on commence par l'entrée
    int x, y;
    SDL_Renderer* renderer = NULL;
    SDL_Window* window = NULL;
    int dw, dh;
    initial_print_maze(maze, &renderer, &window, &dw, &dh);
    SDL_SetWindowTitle(window, "perfection breath inspector");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    if (renderer == NULL || window == NULL)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de l'affichage du labyrinthe.\n");
        return -1;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // on dessine en bleu
    SDL_Event event = {0}; // on crée un event vide
    while (!isempty_queue(q)) // on essaie tous les chemins possibles
    {
        SDL_WaitEventTimeout(&event, 0); // on enregistre les events entrants
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
            (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)) // si l'utilisateur veut fermer la fenêtre
        {
            printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
            free_queue(q);
            free_booltab(visited);
            destroy_print_maze(renderer, window);
            return 1;
        }
        dequeue(q, &x, &y);
        set_true(visited, x, y);
        SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
        SDL_RenderFillRect(renderer, &rect);
        int nb_way_used = 0; // on compte le nombre de chemin adjacent utilisé, en théorie il doit être égal à 1 (notre
                             // père) ou 0 (la case de départ)
        if (!has_wall_up(maze, x, y)) // si on peut aller en haut
        {
            SDL_RenderDrawLine(renderer, x * dw + 1, y * dh, (x + 1) * dw - 2,
                               y * dh); // on dessine une ligne vers le haut
            if (get_bool(visited, x, y - 1)) // si on est déjà passé par cette case
            {
                nb_way_used++;
            }
            else
            {
                enqueue(x, y - 1, q);
            }
        }
        if (!has_wall_down(maze, x, y)) // si on peut aller en bas
        {
            SDL_RenderDrawLine(renderer, x * dw + 1, (y + 1) * dh - 1, (x + 1) * dw - 2,
                               (y + 1) * dh - 1); // on dessine une ligne vers le bas
            if (get_bool(visited, x, y + 1)) // si on est déjà passé par cette case
            {
                nb_way_used++;
            }
            else
            {
                enqueue(x, y + 1, q);
            }
        }
        if (!has_wall_left(maze, x, y)) // si on peut aller à gauche
        {
            SDL_RenderDrawLine(renderer, x * dw, y * dh + 1, x * dw,
                               (y + 1) * dh - 2); // on dessine une ligne vers la gauche
            if (get_bool(visited, x - 1, y)) // si on est déjà passé par cette case
            {
                nb_way_used++;
            }
            else
            {
                enqueue(x - 1, y, q);
            }
        }
        if (!has_wall_right(maze, x, y)) // si on peut aller à droite
        {
            SDL_RenderDrawLine(renderer, (x + 1) * dw - 1, y * dh + 1, (x + 1) * dw - 1,
                               (y + 1) * dh - 2); // on dessine une ligne vers la droite
            if (get_bool(visited, x + 1, y)) // si on est déjà passé par cette case
            {
                nb_way_used++;
            }
            else
            {
                enqueue(x + 1, y, q);
            }
        }
        if (nb_way_used > 1) // si on a trop de chemin adjacent utilisé, c'est qu'il y a un cycle
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // on dessine en rouge
            const SDL_Rect rec = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
            SDL_RenderFillRect(renderer, &rec);
            SDL_SetWindowTitle(window, "cycle found !"); // on change le titre de la fenêtre
            SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            wait_and_destroy_print_maze(renderer, window); // on attend que l'utilisateur ferme la fenêtre
            free_queue(q);
            free_booltab(visited);
            return 1;
        }
        frame_count++;
        if (frame_count == reach)
        {
            SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            frame_count = 0;
            reach = size_queue(q) / 2;
        }
    }
    bool connected = true;
    for (x = 0; x < maze.width; x++) // on vérifie que toutes les cases ont été visitées
    {
        for (y = 0; y < maze.height; y++)
        {
            if (!get_bool(visited, x, y)) // si une case n'a pas été visitée, c'est que le labyrinthe n'est pas connexe
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // on dessine en rouge
                const SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
                if (!has_wall_down(maze, x, y))
                {
                    SDL_RenderDrawLine(renderer, x * dw + 1, (y + 1) * dh - 1, (x + 1) * dw - 2,
                                       (y + 1) * dh - 1); // on dessine une ligne vers le bas
                }
                if (!has_wall_up(maze, x, y))
                {
                    SDL_RenderDrawLine(renderer, x * dw + 1, y * dh, (x + 1) * dw - 2,
                                       y * dh); // on dessine une ligne vers le haut
                }
                if (!has_wall_left(maze, x, y))
                {
                    SDL_RenderDrawLine(renderer, x * dw, y * dh + 1, x * dw,
                                       (y + 1) * dh - 2); // on dessine une ligne vers la gauche
                }
                if (!has_wall_right(maze, x, y))
                {
                    SDL_RenderDrawLine(renderer, (x + 1) * dw - 1, y * dh + 1, (x + 1) * dw - 1,
                                       (y + 1) * dh - 2); // on dessine une ligne vers la droite
                }
                SDL_RenderFillRect(renderer, &rect);
                connected = false;
            }
        }
    }
    if (!connected)
    {

        SDL_SetWindowTitle(window, "not connected !"); // on change le titre de la fenêtre
        SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
        SDL_RenderPresent(renderer);
        wait_and_destroy_print_maze(renderer, window); // on attend que l'utilisateur ferme la fenêtre
        free_queue(q);
        free_booltab(visited);
        return 1;
    }
    SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
    SDL_RenderPresent(renderer);
    SDL_SetWindowTitle(window, "perfect maze !"); // on change le titre de la fenêtre
    wait_and_destroy_print_maze(renderer, window);
    free_queue(q);
    free_booltab(visited);
    return 1;
}

int show_best_exit_breadth_seeker(const maze_t maze)
{
    int frame_count = 0;
    unsigned int reach = 1;
    const waytab ways = create_waytab(maze.width, maze.height); // ce tableau nous permettra de connaitre le meilleur chemin pour sortir du labyrinthe
    queue* q = create_queue(); // cette file contiendra les coordonnées des cases à visiter
    enqueue(0, 0, q); // on commence par l'entrée
    int x, y;
    SDL_Renderer* renderer = NULL;
    SDL_Window* window = NULL;
    int dw, dh;
    initial_print_maze(maze, &renderer, &window, &dw, &dh);
    SDL_SetWindowTitle(window, "best exit breadth seeker");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    if (renderer == NULL || window == NULL)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de l'affichage du labyrinthe.\n");
        return -1;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // on dessine en bleu
    SDL_Event event = {0}; // on crée un event vide
    while (!isempty_queue(q)) // on essaie tous les chemins possibles
    {
        SDL_WaitEventTimeout(&event, 0); // on enregistre les events entrants
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
            (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)) // si l'utilisateur veut fermer la fenêtre
        {
            printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
            free_queue(q);
            free_waytab(ways);
            destroy_print_maze(renderer, window);
            return 1;
        }
        dequeue(q, &x, &y);
        if (x == maze.width - 1 && y == maze.height - 1) // si on est à la sortie
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // on dessine en vert
            SDL_SetWindowTitle(window, "best exit found !"); // on change le titre de la fenêtre
            const way* w = get_way(ways, x, y);
            frame_count = 0;
            reach = length_way(w) / 10; // on affichera un frame tous les 10% du chemin
            if (reach == 0)
            {
                reach = 1;
            }
            while (w != NULL)
            {
                SDL_Rect rec = {get_x(w) * dw + 1, get_y(w) * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
                SDL_RenderFillRect(renderer, &rec);
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
                frame_count++;
                if (frame_count == reach)
                {
                    SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
                    SDL_RenderPresent(renderer);
                    frame_count = 0;
                }
                w = get_dad(w);
            }
            SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            wait_and_destroy_print_maze(renderer, window); // on attend que l'utilisateur ferme la fenêtre
            free_queue(q);
            free_waytab(ways);
            return 1;
        }
        SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
        SDL_RenderFillRect(renderer, &rect);
        const unsigned int l = length_waytab(ways, x, y) + 1;
        if (!has_wall_up(maze, x, y)) // si on peut aller en haut
        {
            SDL_RenderDrawLine(renderer, x * dw + 1, y * dh, (x + 1) * dw - 2,
                               y * dh); // on dessine une ligne vers le haut
            if (l < length_waytab(ways, x, y - 1)) // si on a trouvé un chemin plus court
            {
                enqueue(x, y - 1, q);
                connected_way(ways, x, y - 1, x, y); // on met à jour le chemin
            }
        }
        if (!has_wall_down(maze, x, y)) // si on peut aller en bas
        {
            SDL_RenderDrawLine(renderer, x * dw + 1, (y + 1) * dh - 1, (x + 1) * dw - 2,
                               (y + 1) * dh - 1); // on dessine une ligne vers le bas
            if (l < length_waytab(ways, x, y + 1)) // si on a trouvé un chemin plus court
            {
                enqueue(x, y + 1, q);
                connected_way(ways, x, y + 1, x, y); // on met à jour le chemin
            }
        }
        if (!has_wall_left(maze, x, y)) // si on peut aller à gauche
        {
            SDL_RenderDrawLine(renderer, x * dw, y * dh + 1, x * dw,
                               (y + 1) * dh - 2); // on dessine une ligne vers la gauche
            if (l < length_waytab(ways, x - 1, y)) // si on a trouvé un chemin plus court
            {
                enqueue(x - 1, y, q);
                connected_way(ways, x - 1, y, x, y); // on met à jour le chemin
            }
        }
        if (!has_wall_right(maze, x, y)) // si on peut aller à droite
        {
            SDL_RenderDrawLine(renderer, (x + 1) * dw - 1, y * dh + 1, (x + 1) * dw - 1,
                               (y + 1) * dh - 2); // on dessine une ligne vers la droite
            if (l < length_waytab(ways, x + 1, y)) // si on a trouvé un chemin plus court
            {
                enqueue(x + 1, y, q);
                connected_way(ways, x + 1, y, x, y); // on met à jour le chemin
            }
        }
        frame_count++;
        if (frame_count == reach)
        {
            SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            frame_count = 0;
            reach = size_queue(q) / 2;
        }
    }
    SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
    SDL_RenderPresent(renderer);
    SDL_SetWindowTitle(window, "exit do not exist"); // on change le titre de la fenêtre
    wait_and_destroy_print_maze(renderer, window);
    free_queue(q);
    free_waytab(ways);
    return 1;
}

// --- Solveur en tirage ---

int show_has_exit_draw_seeker(const maze_t maze)
{
    int reach = maze.width * maze.height / 100; // on affichera un frame tous les 1% du labyrinthe
    if (reach == 0)
    {
        reach = 1;
    }
    int frame_count = 0;
    const bool_tab visited = create_booltab(maze.width,
                                            maze.height); // ce tableau nous permettra de connaitre les cases déjà visitées pour éviter les boucles infinies
    stack* s = create_stack(); // cette pile contiendra les coordonnées des cases à visiter
    push(0, 0, s); // on commence par l'entrée
    set_true(visited, 0, 0); // on marque la case de départ comme visitée
    int x, y;
    SDL_Renderer* renderer = NULL;
    SDL_Window* window = NULL;
    int dw, dh;
    initial_print_maze(maze, &renderer, &window, &dw, &dh);
    SDL_SetWindowTitle(window, "exit draw seeker");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    if (renderer == NULL || window == NULL)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de l'affichage du labyrinthe.\n");
        return -1;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // on dessine en bleu
    SDL_Event event = {0}; // on crée un event vide
    while (!isempty_stack(s)) // on essaie tous les chemins possibles
    {
        SDL_WaitEventTimeout(&event, 0); // on enregistre les events entrants
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
            (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)) // si l'utilisateur veut fermer la fenêtre
        {
            printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
            free_stack(s);
            free_booltab(visited);
            destroy_print_maze(renderer, window);
            return 1;
        }
        rpop(s, &x, &y);
        if (x == maze.width - 1 && y == maze.height - 1) // si on est à la sortie
        {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // on dessine en vert
            const SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetWindowTitle(window, "exit found !"); // on change le titre de la fenêtre
            SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            wait_and_destroy_print_maze(renderer, window); // on attend que l'utilisateur ferme la fenêtre
            free_stack(s);
            free_booltab(visited);
            return 1;
        }
        SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
        SDL_RenderFillRect(renderer, &rect);
        if (!has_wall_up(maze, x, y)) // si on peut aller en haut
        {
            SDL_RenderDrawLine(renderer, x * dw + 1, y * dh, (x + 1) * dw - 2,
                               y * dh); // on dessine une ligne vers le haut
            if (!get_bool(visited, x, y - 1)) // on ne repasse pas par une case déjà visitée
            {
                push(x, y - 1, s);
                set_true(visited, x, y - 1); // on marque la case comme visitée por éviter de repasser par
            }
        }
        if (!has_wall_down(maze, x, y)) // si on peut aller en bas
        {
            SDL_RenderDrawLine(renderer, x * dw + 1, (y + 1) * dh - 1, (x + 1) * dw - 2,
                               (y + 1) * dh - 1); // on dessine une ligne vers le bas
            if (!get_bool(visited, x, y + 1)) // on ne repasse pas par une case déjà visitée
            {
                push(x, y + 1, s);
                set_true(visited, x, y + 1); // on marque la case comme visitée por éviter de repasser par là
            }
        }
        if (!has_wall_left(maze, x, y)) // si on peut aller à gauche
        {
            SDL_RenderDrawLine(renderer, x * dw, y * dh + 1, x * dw,
                               (y + 1) * dh - 2); // on dessine une ligne vers la gauche
            if (!get_bool(visited, x - 1, y)) // on ne repasse pas par une case déjà visitée
            {
                push(x - 1, y, s);
                set_true(visited, x - 1, y); // on marque la case comme visitée por éviter de repasser par là
            }
        }
        if (!has_wall_right(maze, x, y)) // si on peut aller à droite
        {
            SDL_RenderDrawLine(renderer, (x + 1) * dw - 1, y * dh + 1, (x + 1) * dw - 1,
                               (y + 1) * dh - 2); // on dessine une ligne vers la droite
            if (!get_bool(visited, x + 1, y)) // on ne repasse pas par une case déjà visitée
            {
                push(x + 1, y, s);
                set_true(visited, x + 1, y); // on marque la case comme visitée por éviter de repasser par là
            }
        }
        frame_count++;
        if (frame_count == reach)
        {
            SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            frame_count = 0;
        }
    }
    SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
    SDL_RenderPresent(renderer);
    SDL_SetWindowTitle(window, "exit do not exist"); // on change le titre de la fenêtre
    wait_and_destroy_print_maze(renderer, window);
    free_stack(s);
    free_booltab(visited);
    return 1;
}

int show_is_connexe_draw_inspector(const maze_t maze)
{
    int reach = maze.width * maze.height / 100; // on affichera un frame tous les 1% du labyrinthe
    if (reach == 0)
    {
        reach = 1;
    }
    int frame_count = 0;
    const bool_tab visited = create_booltab(maze.width, maze.height); // ce tableau nous permettra de connaitre les cases déjà visitées pou
                                                                      // repérer les cycles et la connexité
    stack* s = create_stack(); // cette pile contiendra les coordonnées des cases à visiter
    push(0, 0, s); // on commence par l'entrée
    int x, y;
    SDL_Renderer* renderer = NULL;
    SDL_Window* window = NULL;
    int dw, dh;
    initial_print_maze(maze, &renderer, &window, &dw, &dh);
    SDL_SetWindowTitle(window, "connected draw inspector");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    if (renderer == NULL || window == NULL)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de l'affichage du labyrinthe.\n");
        return -1;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // on dessine en bleu
    SDL_Event event = {0}; // on crée un event vide
    while (!isempty_stack(s)) // on essaie tous les chemins possibles
    {
        SDL_WaitEventTimeout(&event, 0); // on enregistre les events entrants
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
            (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)) // si l'utilisateur veut fermer la fenêtre
        {
            printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
            free_stack(s);
            free_booltab(visited);
            destroy_print_maze(renderer, window);
            return 1;
        }
        rpop(s, &x, &y);
        set_true(visited, x, y);
        SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
        SDL_RenderFillRect(renderer, &rect);
        if (!has_wall_up(maze, x, y)) // si on peut aller en haut
        {
            SDL_RenderDrawLine(renderer, x * dw + 1, y * dh, (x + 1) * dw - 2,
                               y * dh); // on dessine une ligne vers le haut
            if (!get_bool(visited, x, y - 1)) // si on n'est pas déjà passé par cette case
            {
                push(x, y - 1, s);
            }
        }
        if (!has_wall_down(maze, x, y)) // si on peut aller en bas
        {
            SDL_RenderDrawLine(renderer, x * dw + 1, (y + 1) * dh - 1, (x + 1) * dw - 2,
                               (y + 1) * dh - 1); // on dessine une ligne vers le bas
            if (!get_bool(visited, x, y + 1)) // si on n'est pas déjà passé par cette case
            {
                push(x, y + 1, s);
            }
        }
        if (!has_wall_left(maze, x, y)) // si on peut aller à gauche
        {
            SDL_RenderDrawLine(renderer, x * dw, y * dh + 1, x * dw,
                               (y + 1) * dh - 2); // on dessine une ligne vers la gauche
            if (!get_bool(visited, x - 1, y)) // si on n'est pas déjà passé par cette case
            {
                push(x - 1, y, s);
            }
        }
        if (!has_wall_right(maze, x, y)) // si on peut aller à droite
        {
            SDL_RenderDrawLine(renderer, (x + 1) * dw - 1, y * dh + 1, (x + 1) * dw - 1,
                               (y + 1) * dh - 2); // on dessine une ligne vers la droite
            if (!get_bool(visited, x + 1, y)) // si on n'est pas déjà passé par cette case
            {
                push(x + 1, y, s);
            }
        }
        frame_count++;
        if (frame_count == reach)
        {
            SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            frame_count = 0;
        }
    }
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // on dessine en rouge
    bool is_connected = true;
    for (x = 0; x < maze.width; x++) // on vérifie que toutes les cases ont été visitées
    {
        for (y = 0; y < maze.height; y++)
        {
            if (!get_bool(visited, x, y)) // si une case n'a pas été visitée, c'est que le labyrinthe n'est pas connexe
            {
                const SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
                SDL_RenderFillRect(renderer, &rect);
                if (!has_wall_down(maze, x, y)) // si on peut aller en bas
                {
                    SDL_RenderDrawLine(renderer, x * dw + 1, (y + 1) * dh - 1, (x + 1) * dw - 2,
                                       (y + 1) * dh - 1); // on dessine une ligne vers le bas
                }
                if (!has_wall_right(maze, x, y)) // si on peut aller à droite
                {
                    SDL_RenderDrawLine(renderer, (x + 1) * dw - 1, y * dh + 1, (x + 1) * dw - 1,
                                       (y + 1) * dh - 2); // on dessine une ligne vers la droite
                }
                if (!has_wall_up(maze, x, y)) // si on peut aller en haut
                {
                    SDL_RenderDrawLine(renderer, x * dw + 1, y * dh, (x + 1) * dw - 2,
                                       y * dh); // on dessine une ligne vers le haut
                }
                if (!has_wall_left(maze, x, y)) // si on peut aller à gauche
                {
                    SDL_RenderDrawLine(renderer, x * dw, y * dh + 1, x * dw,
                                       (y + 1) * dh - 2); // on dessine une ligne vers la gauche
                }
                is_connected = false;
            }
        }
    }
    if (!is_connected)
    {
        SDL_SetWindowTitle(window, "not connected !"); // on change le titre de la fenêtre
        SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
        SDL_RenderPresent(renderer);
        wait_and_destroy_print_maze(renderer, window); // on attend que l'utilisateur ferme la fenêtre
        free_stack(s);
        free_booltab(visited);
        return 1;
    }
    SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
    SDL_RenderPresent(renderer);
    SDL_SetWindowTitle(window, "Maze connected !"); // on change le titre de la fenêtre
    wait_and_destroy_print_maze(renderer, window);
    free_stack(s);
    free_booltab(visited);
    return 1;
}

int show_is_perfect_draw_inspector(const maze_t maze)
{
    int reach = maze.width * maze.height / 100; // on affichera un frame tous les 1% du labyrinthe
    if (reach == 0)
    {
        reach = 1;
    }
    int frame_count = 0;
    const bool_tab visited = create_booltab(maze.width, maze.height); // ce tableau nous permettra de connaitre les cases déjà visitées pou
                                                                      // repérer les cycles et la connexité
    stack* s = create_stack(); // cette pile contiendra les coordonnées des cases à visiter
    push(0, 0, s); // on commence par l'entrée
    int x, y;
    SDL_Renderer* renderer = NULL;
    SDL_Window* window = NULL;
    int dw, dh;
    initial_print_maze(maze, &renderer, &window, &dw, &dh);
    SDL_SetWindowTitle(window, "is perfect draw inspector");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    if (renderer == NULL || window == NULL)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de l'affichage du labyrinthe.\n");
        return -1;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // on dessine en bleu
    SDL_Event event = {0}; // on crée un event vide
    while (!isempty_stack(s)) // on essaie tous les chemins possibles
    {
        SDL_WaitEventTimeout(&event, 0); // on enregistre les events entrants
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
            (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)) // si l'utilisateur veut fermer la fenêtre
        {
            printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
            free_stack(s);
            free_booltab(visited);
            destroy_print_maze(renderer, window);
            return 1;
        }
        rpop(s, &x, &y);
        set_true(visited, x, y);
        SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
        SDL_RenderFillRect(renderer, &rect);
        int nb_way_used = 0; // on compte le nombre de chemin adjacent utilisé, en théorie il doit être égal à 1 (notre
                             // père) ou 0 (la case de départ)
        if (!has_wall_up(maze, x, y)) // si on peut aller en haut
        {
            SDL_RenderDrawLine(renderer, x * dw + 1, y * dh, (x + 1) * dw - 2,
                               y * dh); // on dessine une ligne vers le haut
            if (get_bool(visited, x, y - 1)) // si on est déjà passé par cette case
            {
                nb_way_used++;
            }
            else
            {
                push(x, y - 1, s);
            }
        }
        if (!has_wall_down(maze, x, y)) // si on peut aller en bas
        {
            SDL_RenderDrawLine(renderer, x * dw + 1, (y + 1) * dh - 1, (x + 1) * dw - 2,
                               (y + 1) * dh - 1); // on dessine une ligne vers le bas
            if (get_bool(visited, x, y + 1)) // si on est déjà passé par cette case
            {
                nb_way_used++;
            }
            else
            {
                push(x, y + 1, s);
            }
        }
        if (!has_wall_left(maze, x, y)) // si on peut aller à gauche
        {
            SDL_RenderDrawLine(renderer, x * dw, y * dh + 1, x * dw,
                               (y + 1) * dh - 2); // on dessine une ligne vers la gauche
            if (get_bool(visited, x - 1, y)) // si on est déjà passé par cette case
            {
                nb_way_used++;
            }
            else
            {
                push(x - 1, y, s);
            }
        }
        if (!has_wall_right(maze, x, y)) // si on peut aller à droite
        {
            SDL_RenderDrawLine(renderer, (x + 1) * dw - 1, y * dh + 1, (x + 1) * dw - 1,
                               (y + 1) * dh - 2); // on dessine une ligne vers la droite
            if (get_bool(visited, x + 1, y)) // si on est déjà passé par cette case
            {
                nb_way_used++;
            }
            else
            {
                push(x + 1, y, s);
            }
        }
        if (nb_way_used > 1) // si on a trop de chemin adjacent utilisé, c'est qu'il y a un cycle
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // on dessine en rouge
            const SDL_Rect rec = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
            SDL_RenderFillRect(renderer, &rec);
            SDL_SetWindowTitle(window, "cycle found !"); // on change le titre de la fenêtre
            SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            wait_and_destroy_print_maze(renderer, window); // on attend que l'utilisateur ferme la fenêtre
            free_stack(s);
            free_booltab(visited);
            return 1;
        }
        frame_count++;
        if (frame_count == reach)
        {
            SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            frame_count = 0;
        }
    }
    for (x = 0; x < maze.width; x++) // on vérifie que toutes les cases ont été visitées
    {
        for (y = 0; y < maze.height; y++)
        {
            if (!get_bool(visited, x, y)) // si une case n'a pas été visitée, c'est que le labyrinthe n'est pas connexe
            {
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // on dessine en rouge
                const SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
                SDL_RenderFillRect(renderer, &rect);
                SDL_SetWindowTitle(window, "not connected !"); // on change le titre de la fenêtre
                SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
                SDL_RenderPresent(renderer);
                wait_and_destroy_print_maze(renderer, window); // on attend que l'utilisateur ferme la fenêtre
                free_stack(s);
                free_booltab(visited);
                return 1;
            }
        }
    }
    SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
    SDL_RenderPresent(renderer);
    SDL_SetWindowTitle(window, "perfect maze !"); // on change le titre de la fenêtre
    wait_and_destroy_print_maze(renderer, window);
    free_stack(s);
    free_booltab(visited);
    return 1;
}

int show_best_exit_draw_seeker(const maze_t maze)
{
    // on va constituer un tableau de way pour chaque case, qui contiendra le chemin le plus court pour y arriver, en
    // partant de l'entrée il nous permettra d'éviter les cycle (la taille du cycle serait plus grande) et de relier des
    // fragments de chemin plutôt que de tout recalculer
    int frame_count = 0;
    unsigned int reach = maze.width * maze.height / 100; // on affichera un frame tous les 1% du labyrinthe
    if (reach == 0)
    {
        reach = 1;
    }
    const waytab ways = create_waytab(maze.width,
                                      maze.height); // ce tableau nous permettra de connaitre le chemin le plus court pour arriver à une case
    const bool_tab dead_ends = create_booltab(maze.width, maze.height); // ce tableau nous permettra de connaitre les impasses
    stack* s = create_stack(); // cette pile contiendra les coordonnées des cases à visiter
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    int dw, dh;
    initial_print_maze(maze, &renderer, &window, &dw, &dh);
    SDL_SetWindowTitle(window, "best exit draw seeker");
    SDL_DisplayMode dm;
    SDL_GetCurrentDisplayMode(0, &dm);
    if (renderer == NULL || window == NULL)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de l'affichage du labyrinthe.\n");
        return -1;
    }
    push(0, 0, s); // on commence par l'entrée
    int x, y;
    unsigned int count = 0;
    SDL_Event event = {0}; // on crée un event vide
    while (!isempty_stack(s))
    {
        SDL_WaitEventTimeout(&event, 0); // on enregistre les events entrants
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
            (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)) // si l'utilisateur veut fermer la fenêtre
        {
            printf("L'utilisateur a demandé la fermeture de la fenêtre.\n");
            free_stack(s);
            free_booltab(dead_ends);
            free_waytab(ways);
            destroy_print_maze(renderer, window);
            return 1;
        }
        rpop(s, &x, &y);
        SDL_SetRenderDrawColor(renderer, 125, 125, 125, 255); // on dessine en gris
        SDL_Rect rect = {x * dw + 1, y * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
        SDL_RenderFillRect(renderer, &rect);
        frame_count++;
        if (frame_count == reach)
        {
            SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // on dessine en bleu
        if ((x == maze.width - 1 && y == maze.height - 1) || get_bool(dead_ends, x, y)) // si on est à la sortie ou dans une impasse
        {
            continue;
        }
        const unsigned int l = length_waytab(ways, x, y) + 1; // longueur du chemin actuel
        const int abs_dist = -x + maze.width - 1 - y + maze.height - 1; // distance de Manhattan à la sortie
        if (l + abs_dist > length_waytab(ways, maze.width - 1, maze.height - 1)) // si on a déjà trouvé un chemin plus court
        {
            continue;
        }
        if (!has_wall_up(maze, x, y) && !get_bool(dead_ends, x, y - 1)) // si on peut aller en haut et que la case n'est pas une impasse
        {
            count++;
            SDL_RenderDrawLine(renderer, x * dw + 1, y * dh, (x + 1) * dw - 2,
                               y * dh); // on dessine une ligne vers le haut
            if (l < length_waytab(ways, x, y - 1))
            {
                push(x, y - 1, s); // on ajoute la case à celle à visiter
                connected_way(ways, x, y - 1, x, y); // on crée un chemin entre la case actuelle et la case en haut
            }
        }
        if (!has_wall_down(maze, x, y) && !get_bool(dead_ends, x, y + 1)) // si on peut aller en bas et que la case n'est pas une impasse
        {
            count++;
            SDL_RenderDrawLine(renderer, x * dw + 1, (y + 1) * dh - 1, (x + 1) * dw - 2, (y + 1) * dh - 1);
            if (l < length_waytab(ways, x, y + 1))
            {
                push(x, y + 1, s); // on ajoute la case à celle à visiter
                connected_way(ways, x, y + 1, x, y); // on crée un chemin entre la case actuelle et la case en bas
            }
        }
        if (!has_wall_left(maze, x, y) && !get_bool(dead_ends, x - 1, y)) // si on peut aller à gauche et que la case n'est pas une impasse
        {
            count++;
            SDL_RenderDrawLine(renderer, x * dw, y * dh + 1, x * dw,
                               (y + 1) * dh - 2); // on dessine une ligne vers la gauche
            if (l < length_waytab(ways, x - 1, y))
            {
                push(x - 1, y, s); // on ajoute la case à celle à visiter
                connected_way(ways, x - 1, y, x, y); // on crée un chemin entre la case actuelle et la case à gauche
            }
        }
        if (!has_wall_right(maze, x, y) && !get_bool(dead_ends, x + 1, y)) // si on peut aller à droite et que la case n'est pas une impasse
        {
            count++;
            SDL_RenderDrawLine(renderer, (x + 1) * dw - 1, y * dh + 1, (x + 1) * dw - 1,
                               (y + 1) * dh - 2); // on dessine une ligne vers la droite
            if (l < length_waytab(ways, x + 1, y))
            {
                push(x + 1, y, s); // on ajoute la case à celle à visiter
                connected_way(ways, x + 1, y, x, y); // on crée un chemin entre la case actuelle et la case à droite
            }
        }
        if (count <= 1) // si on est dans une impasse
        {
            set_true(dead_ends, x, y); // on marque la case comme une impasse
            SDL_SetRenderDrawColor(renderer, 125, 125, 125, 255); // on dessine en gris
            SDL_RenderFillRect(renderer, &rect);
        }
        count = 0;
        SDL_RenderFillRect(renderer, &rect);
        if (frame_count == reach)
        {
            SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
            SDL_RenderPresent(renderer);
            frame_count = 0;
        }
    }
    way* w = copy_way(get_way(ways, maze.width - 1,
                              maze.height - 1)); // on récupère le chemin pour arriver à la sortie (on le copie car on va libérer le tableau ways)
    free_waytab(ways);
    free_stack(s);
    free_booltab(dead_ends);
    if (is_empty(w))
    {
        SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
        SDL_RenderPresent(renderer);
        SDL_SetWindowTitle(window, "exit do not exist"); // on change le titre de la fenêtre
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // on dessine en vert
        SDL_SetWindowTitle(window, "best exit found !"); // on change le titre de la fenêtre
        reach = length_way(w) / 10; // on affichera un frame tous les 10% du chemin
        if (reach == 0)
        {
            reach = 1;
        }
        frame_count = 0;
        while (w != NULL)
        {
            SDL_Rect rec = {get_x(w) * dw + 1, get_y(w) * dh + 1, dw - 2, dh - 2}; // on dessine un rectangle dans la case
            SDL_RenderFillRect(renderer, &rec);
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
            frame_count++;
            if (frame_count == reach)
            {
                SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
                SDL_RenderPresent(renderer);
                frame_count = 0;
            }
            w = get_dad(w);
        }
    }
    SDL_Delay(dm.refresh_rate); // pause pour laisser aux données le temps de s'afficher
    SDL_RenderPresent(renderer);
    free_way(w);
    wait_and_destroy_print_maze(renderer, window);
    return 1;
}

// --- Visualisation des chemins ---

int show_the_way(const maze_t maze, const way* w)
{
    SDL_Renderer* renderer = NULL;
    SDL_Window* window = NULL;
    int dw, dh;
    initial_print_maze(maze, &renderer, &window, &dw, &dh);
    if (renderer == NULL || window == NULL)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de l'affichage du labyrinthe.\n");
        return -1;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // on dessine en bleu
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
    wait_and_destroy_print_maze(renderer, window);
    return 1;
}
