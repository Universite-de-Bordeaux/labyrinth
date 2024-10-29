#include "solveur.h"
#include "mazemaker.h"
#include <stdio.h>
#include <SDL2/SDL.h>

// --- Solveur de labyrinthes ---


bool exit_deep_seeker(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width, maze.height); //ce tableau nous permettra de connaitre les cases déjà visitées pour éviter les boucles infinies
    queue *q = create_queue(); //cette queue contiendra les coordonnées des cases à visiter
    enqueue(0, 0, q); //on commence par l'entrée
    int x, y;
    while(!isempty_queue(q)) //on essaie tous les chemins possibles
    {
        dequeue(q, &x, &y);
        if(x == maze.width - 1 && y == maze.height - 1) //si on est à la sortie
        {
            free_queue(q);
            free_booltab(visited);
            return true;
        }
        set_true(visited, x, y);
        if(!has_wall_up(maze, x, y) && !get_bool(visited, x, y - 1)) //si on peut aller en haut
        {
            enqueue(x, y - 1, q);
        }
        if(!has_wall_down(maze, x, y) && !get_bool(visited, x, y + 1)) //si on peut aller en bas
        {
            enqueue(x, y + 1, q);
        }
        if(!has_wall_left(maze, x, y) && !get_bool(visited, x - 1, y)) //si on peut aller à gauche
        {
            enqueue(x - 1, y, q);
        }
        if(!has_wall_right(maze, x, y) && !get_bool(visited, x + 1, y)) //si on peut aller à droite
        {
            enqueue(x + 1, y, q);
        }
    }
    free_booltab(visited);
    free_queue(q);
    return false;
}

bool perfect_deep_inspector(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width, maze.height); //on crée un tableau de booléens pour savoir si on est déjà passé par une case
    queue *q = create_queue(); //cette queue contiendra les coordonnées des cases à visiter
    enqueue(0, 0, q); //on commence par l'entrée
    int x, y;
    while(!isempty_queue(q)) //on doit visiter toutes les cases
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
        if(nb_way_used == 0 && (x != 0 || y != 0)) //si on est sur une case qui n'est pas la case de départ et qu'on a aucun chemin adjacent utilisé, c'est qu'il y a un problème
        {
            free_queue(q);
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

way *best_exit_deep_seeker(const maze_t maze)
{
    //on va constituer un tableau de way pour chaque case, qui contiendra le chemin le plus court pour y arriver, en partant de l'entrée
    //il nous permettra d'éviter les cycle (la taille du cycle serait plus grande) et de relier des fragments de chemin plutôt que de tout recalculer
    const waytab ways = create_waytab(maze.width, maze.height); //ce tableau nous permettra de connaitre le chemin le plus court pour arriver à une case
    queue *q = create_queue(); //cette queue contiendra les coordonnées des cases à visiter
    enqueue(0, 0, q); //on commence par l'entrée
    int x, y;
    while(!isempty_queue(q))
    {
        dequeue(q, &x, &y);
        if(x == maze.width - 1 && y == maze.height - 1) //si on est à la sortie, on ne va pas plus loin
        {
            continue;
        }
        if(!has_wall_up(maze, x, y) && length_waytab(ways, x, y) + 1 < length_waytab(ways, x, y - 1)) //si on peut aller en haut et que le chemin est plus court
        {
            enqueue(x, y - 1, q); //on ajoute la case à celle à visiter
            connected_way(ways, x, y - 1, x, y); //on crée un chemin entre la case actuelle et la case en haut
        }
        if(!has_wall_down(maze, x, y) && length_waytab(ways, x, y) + 1 < length_waytab(ways, x, y + 1)) //si on peut aller en bas et que le chemin est plus court
        {
            enqueue(x, y + 1, q); //on ajoute la case à celle à visiter
            connected_way(ways, x, y + 1, x, y); //on crée un chemin entre la case actuelle et la case en bas
        }
        if(!has_wall_left(maze, x, y) && length_waytab(ways, x, y) + 1 < length_waytab(ways, x - 1, y)) //si on peut aller à gauche et que le chemin est plus court
        {
            enqueue(x - 1, y, q); //on ajoute la case à celle à visiter
            connected_way(ways, x - 1, y, x, y); //on crée un chemin entre la case actuelle et la case à gauche
        }
        if(!has_wall_right(maze, x, y) && length_waytab(ways, x, y) + 1 < length_waytab(ways, x + 1, y)) //si on peut aller à droite et que le chemin est plus court
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

int show_has_exit_right_hand(maze_t maze)
{
    printf("Le visualisateur de l'algorithme de résolution de labyrinthes n'est pas encore implémenté.\n");
    return 1;
}

int show_is_perfect_right_hand(maze_t maze)
{
    printf("Le visualisateur de l'algorithme de recherche de labyrinthes parfaits n'est pas encore implémenté.\n");
    return 1;
}

int show_shortest_exit_right_hand(maze_t maze)
{
    printf("Le visualisateur de l'algorithme de recherche du chemin le plus court pour sortir du labyrinthe n'est pas encore implémenté.\n");
    return 1;
}

int show_the_way(const maze_t maze, const way *w)
{
    printf("Le visualisateur du chemin dans le labyrinthe n'est pas encore implémenté.\n");
    return 1;
}