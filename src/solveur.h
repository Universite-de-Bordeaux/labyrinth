#ifndef SOLVEUR_H
#define SOLVEUR_H
#include "struct.h"

// --- mini-jeux ---

//une mini-jeu où il faut trouver la sortie
//version de base
//le chemin apparaît au fur et à mesure que le joueur avance
//maze : le labyrinthe
//appuier sur ECHAP ou la croix fermera proprement le jeu
//appuier sur ENTER abandonnera le jeu et affichera le labyrinthe en entier
int minigame1(maze_t maze);

//lance minigame1 avec un générateur de labyrinthe aléatoire
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
int rminigame1(int width, int height);

//une mini-jeu où il faut trouver la sortie
//le chemin n'apparaît que sur la case où le joueur est
//version mi-blind
//maze : le labyrinthe
//appuier sur ECHAP ou la croix fermera proprement le jeu
//appuier sur ENTER abandonnera le jeu et affichera le labyrinthe en entier
int minigame2(maze_t maze);

//lance minigame2 avec un générateur de labyrinthe aléatoire
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
//appuier sur ECHAP ou la croix fermera proprement le jeu
//appuier sur ENTER abandonnera le jeu et affichera le labyrinthe en entier
int rminigame2(int width, int height);

//une mini-jeu où il faut trouver la sortie
//le chemin n'apparaît pas, même sur la case où le joueur est
//version full-blind
//maze : le labyrinthe
int minigame3(maze_t maze);

//lance minigame3 avec un générateur de labyrinthe aléatoire
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
int rminigame3(int width, int height);

//lance un mini-jeu avec un générateur de labyrinthe et une difficulté aléatoires
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
int rminigame(int width, int height);

//lance un mini-jeu avec un labyrinthe donné et une difficulté aléatoire
//maze : le labyrinthe
int r_minigame(maze_t maze);

// --- solveur left hand (parcours en profondeur) ---


//renvoie true si le labyrinthe est solvable, false sinon
//maze : le labyrinthe
bool has_exit_right_hand(maze_t maze);

//renvoie true si le labyrinthe est parfait, false sinon
//maze : le labyrinthe
bool is_perfect_right_hand(maze_t maze);

//renvoie le chemin le plus court pour sortir du labyrinthe
//maze : le labyrinthe
//renvoie un chemin vide si le labyrinthe n'est pas solvable
way *shortest_exit_right_hand(const maze_t maze);

// --- visualisation ---


//fonction de visualisation
//affiche le labyrinthe et la progression du solveur
//renvoie -1 en cas d'erreur, 1 sinon
//maze : le labyrinthe
int show_has_exit_right_hand(maze_t maze);

//fonction de visualisation
//affiche le labyrinthe et la progression du solveur
//renvoie -1 en cas d'erreur, 1 sinon
//maze : le labyrinthe
//version rapide, risque de suppression de la visualisation active
//utilisation fortement déconseillée
int show_fast_has_exit_right_hand(maze_t maze);

//fonction de visualisation
//affiche le labyrinthe et la progression du solveur
//renvoie -1 en cas d'erreur, 1 sinon
//maze : le labyrinthe
int show_is_perfect_right_hand(maze_t maze);

//fonction de visualisation
//affiche le labyrinthe et la progression du solveur
//renvoie -1 en cas d'erreur, 1 sinon
//maze : le labyrinthe
//version rapide, risque de suppression de la visualisation active
//utilisation fortement déconseillée
int show_fast_is_perfect_right_hand(maze_t maze);

//fonction de visualisation
//affiche le labyrinthe et la progression du solveur
//renvoie -1 en cas d'erreur, 1 sinon
//maze : le labyrinthe
int show_shortest_exit_right_hand(maze_t maze); //TODO : en cours

//fonction de visualisation
//affiche le labyrinthe et la progression du solveur
//renvoie -1 en cas d'erreur, 1 sinon
//maze : le labyrinthe
//version rapide, risque de suppression de la visualisation active
//utilisation fortement déconseillée
int show_fast_shortest_exit_right_hand(maze_t maze); //TODO : en cours

//fonction de visualisation
//affiche le chemin dans le labyrinthe
//maze : le labyrinthe
//w : le chemin
//ne gère pas les cas d'erreur, ni les murs
int show_the_way(const maze_t maze, const way *w);
#endif //SOLVEUR_H
