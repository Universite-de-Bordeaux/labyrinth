#ifndef SOLVEUR_H
#define SOLVEUR_H
#include "case.h"

//mini-jeu
int minigame1(maze_t maze);
int rminigame1(int width, int height);
int minigame2(maze_t maze);
int rminigame2(int width, int height);
int minigame3(maze_t maze);
int rminigame3(int width, int height);
int rminigame(int width, int height);
int r_minigame(maze_t maze);
int minigame4(maze_t maze);
int rminigame4(int width, int height);

//solveur
bool right_hand(maze_t maze);

//solveur cascade (parcours en profondeur)
bool has_exit_cascade(maze_t maze);
bool is_perfect_cascade(maze_t maze);
//visualisation des solveurs cascade
int show_has_exit_cascade(maze_t maze);
#endif //SOLVEUR_H
