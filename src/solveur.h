#ifndef SOLVEUR_H
#define SOLVEUR_H
#include "struct.h"

// --- solveurs ---

//renvoie true si le labyrinthe est solvable, false sinon
//maze : le labyrinthe
bool exit_deep_seeker(maze_t maze); //TODO : réecrire à l'aide de piles

//renvoie true si le labyrinthe est parfait, false sinon
//maze : le labyrinthe
bool perfect_deep_inspector(maze_t maze); //TODO : réecrire à l'aide de piles

//renvoie le chemin le plus court pour sortir du labyrinthe
//maze : le labyrinthe
//renvoie un chemin vide si le labyrinthe n'est pas solvable
way *best_exit_deep_seeker(const maze_t maze); //TODO : réecrire à l'aide de piles


// --- visualisation ---

//fonction de visualisation
//affiche le labyrinthe et la progression du solveur
//renvoie -1 en cas d'erreur, 1 sinon
//maze : le labyrinthe
int show_exit_deep_seeker(maze_t maze); //TODO

//fonction de visualisation
//affiche le labyrinthe et la progression du solveur
//renvoie -1 en cas d'erreur, 1 sinon
//maze : le labyrinthe
int show_perfect_deep_inspector(maze_t maze); //TODO

//fonction de visualisation
//affiche le labyrinthe et la progression du solveur
//renvoie -1 en cas d'erreur, 1 sinon
//maze : le labyrinthe
int show_best_exit_deep_seeker(maze_t maze); //TODO

//fonction de visualisation
//affiche le chemin dans le labyrinthe
//maze : le labyrinthe
//w : le chemin
//ne gère pas les cas d'erreur, ni les murs
int show_the_way(const maze_t maze, const way *w); //TODO
#endif //SOLVEUR_H
