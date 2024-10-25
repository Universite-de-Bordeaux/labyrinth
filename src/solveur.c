#include "solveur.h"
#include "mazemaker.h"
#include <stdio.h>
#include <time.h>
#include <SDL2/SDL.h>

// --- Solveur de labyrinthes ---

//fonction auxiliaire de right_hand
//renvoie true si on peut atteindre la sortie, false sinon
//maze : le labyrinthe
//visited : tableau de booléens pour savoir si on est déjà passé par une case
//x : abscisse de la case actuelle
//y : ordonnée de la case actuelle
bool has_exit_right_hand_aux(const maze_t maze, const bool_tab visited, const int x, const int y)
{
    //condition d'arrêt : atteindre la sortie
    if(x == maze.width - 1 && y == maze.height - 1)
    {
        return true;
    }

    //boucle : on part dans toutes les directions possibles et on regarde si on peut atteindre la sortie
    set_true(visited, x, y);
    if(!has_wall_up(maze, x, y) && !get_bool(visited, x, y-1))
    {
        if(has_exit_right_hand_aux(maze, visited, x, y - 1))
        {
            return true;
        }
    }
    if(!has_wall_down(maze, x, y) && !get_bool(visited, x, y + 1))
    {
        if(has_exit_right_hand_aux(maze, visited, x, y + 1))
        {
            return true;
        }
    }
    if(!has_wall_left(maze, x, y) && !get_bool(visited, x - 1, y))
    {
        if(has_exit_right_hand_aux(maze, visited, x - 1, y))
        {
            return true;
        }
    }
    if(!has_wall_right(maze, x, y) && !get_bool(visited, x + 1, y))
    {
        if(has_exit_right_hand_aux(maze, visited, x + 1, y))
        {
            return true;
        }
    }
    //condition d'arrêt : on ne peut plus avancer
    return false;
}

bool has_exit_right_hand(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width, maze.height);
    const bool s = has_exit_right_hand_aux(maze, visited, 0, 0);
    free_booltab(visited);
    return s;
}

//fonction auxiliaire de is_perfect_right_hand
//renvoie false si le labyrinth est évidemment non parfait, true sinon
//maze : le labyrinthe
//visited : tableau de booléens pour savoir si on est déjà passé par une case
//x : abscisse de la case actuelle
//y : ordonnée de la case actuelle
//p_x : abscisse de la case précédente
//p_y : ordonnée de la case précédente
bool is_perfect_right_hand_aux(const maze_t maze, const bool_tab visited, const int x, const int y, const int p_x,const int p_y)
{
    set_true(visited, x, y);
    if(!has_wall_up(maze, x, y) && (p_y != y - 1 || p_x != x))
    {
        if(get_bool(visited, x, y - 1) || !is_perfect_right_hand_aux(maze, visited, x, y - 1, x, y))
        {
            return false;
        }
    }
    if(!has_wall_down(maze, x, y) && (p_y != y + 1 || p_x != x))
    {
        if(get_bool(visited, x, y + 1) || !is_perfect_right_hand_aux(maze, visited, x, y + 1, x, y))
        {
            return false;
        }
    }
    if(!has_wall_left(maze, x, y) && (p_x != x - 1 || p_y != y))
    {
        if(get_bool(visited, x - 1, y) || !is_perfect_right_hand_aux(maze, visited, x - 1, y, x, y))
        {
            return false;
        }
    }
    if(!has_wall_right(maze, x, y) && (p_x != x + 1 || p_y != y))
    {
        if(get_bool(visited, x + 1, y) || !is_perfect_right_hand_aux(maze, visited, x + 1, y, x, y))
        {
            return false;
        }
    }
    return true;
}

bool is_perfect_right_hand(const maze_t maze)
{
    const bool_tab visited = create_booltab(maze.width, maze.height);
    if(!is_perfect_right_hand_aux(maze, visited, 0, 0, 0, 0))
    {
        free_booltab(visited);
        return false;
    }
    for(int i = 0; i < maze.height; i++)
    {
        for(int j = 0; j < maze.width; j++)
        {
            if(!get_bool(visited, j, i))
            {
                free_booltab(visited);
                return false;
            }
        }
    }
    free_booltab(visited);
    return true;
}

//fonction auxiliaire de shortest_exit_right_hand
//renvoie le chemin le plus court pour atteindre la sortie
//maze : le labyrinthe
//visited : tableau de booléens pour savoir si on est déjà passé par une case
//x : abscisse de la case actuelle
//y : ordonnée de la case actuelle
//wayt : tableau de chemins
void shortest_exit_right_hand_aux(const maze_t maze, const int x, const int y, const waytab wayt)
{
    if(x == maze.width - 1 && y == maze.height - 1)
    {
        return;
    }
    const int l = length_waytab(wayt, x, y);
    if(l > length_waytab(wayt, maze.width - 1, maze.height - 1))
    {
        return;
    }
    if(!has_wall_up(maze, x, y) && length_waytab(wayt, x, y - 1) > l + 1)
    {
        connected_way(wayt, x, y - 1, x, y);
        shortest_exit_right_hand_aux(maze, x, y - 1, wayt);
    }
    if(!has_wall_down(maze, x, y) && length_waytab(wayt, x, y + 1) > l + 1)
    {
        connected_way(wayt, x, y + 1, x, y);
        shortest_exit_right_hand_aux(maze, x, y + 1, wayt);
    }
    if(!has_wall_left(maze, x, y) && length_waytab(wayt, x - 1, y) > l + 1)
    {
        connected_way(wayt, x - 1, y, x, y);
        shortest_exit_right_hand_aux(maze, x - 1, y, wayt);
    }
    if(!has_wall_right(maze, x, y) && length_waytab(wayt, x + 1, y) > l + 1)
    {
        connected_way(wayt, x + 1, y, x, y);
        shortest_exit_right_hand_aux(maze, x + 1, y, wayt);
    }
}


way *shortest_exit_right_hand(const maze_t maze)
{
    const waytab wayt = create_waytab(maze.width, maze.height);
    shortest_exit_right_hand_aux(maze, 0, 0, wayt);
    way *w = copy_way(get_way(wayt, maze.width - 1, maze.height - 1));
    free_waytab(wayt);
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