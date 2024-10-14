#ifndef OUTSIDE_H
#define OUTSIDE_H
#include "struct.h"

// crée et renvoie un labyrinthe selon les configurations spécifiées dans le fichier
// filename : le nom du fichier
maze_t maze_from_file(const char *filename);

// écrit le labyrinthe dans un fichier texte
// maze : le labyrinthe
// filename : le nom du fichier où écrire le labyrinthe, sera écrasé s'il existe déjà
void maze_to_file(maze_t maze, const char *filename);

#endif //OUTSIDE_H
