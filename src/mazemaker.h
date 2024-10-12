#ifndef MAZEMAKER_H
#define MAZEMAKER_H
#include "case.h"

//un labyrinth parfait est un labyrinthe où il existe un unique chemin entre chaque paire de cellules
//un labyrinthe est parfait si et seulement si il existe un chemin entre chaque paire de cellules et qu'il n'existe pas de boucle (principe des solveurs)
//un labyrinthe solvable est un labyrinthe où il existe au moins un chemin entre le point de départ (0, 0) et le point d'arrivée (width-1, height-1)
//tout labyrinthe doit être solvable (cahier des charges)

//Crée un labyrinthe parfait de taille width x height
//tous les murs sont verticaux
maze_t line_maze(const int width, const int height);

//Crée un labyrinthe parfait de taille width x height
//tous les murs sont horizontaux
maze_t column_maze(const int width, const int height);

//Crée un labyrinthe imparfait de taille width x height
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
//seuls le chemins de sortie possèdent des embranchements
maze_t imperfect_one_way_maze(const int width, const int height);

//Crée un labyrinthe parfait de taille width x height
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
//tous les embranchements vont seulement vers la droite ou le bas
maze_t perfect_one_way_maze(const int width, const int height);

//Crée un labyrinthe parfait de taille width x height
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
//d'après la méthode du hunt and kill
maze_t hunt_kill_maze(const int width, const int height);

//Crée un labyrinthe parfait de taille width x height
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
maze_t by_path_maze(int width, int height);

//Crée un labyrinthe à l'aide d'une méthode existante chosie aléatoirement
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
maze_t r_maze(const int width, const int height);

//Crée un labyrinthe parfait de taille width x height à l'aide d'une méthode existante chosie aléatoirement
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
maze_t rperfect_maze(const int width, const int height);

//Crée un labyrinthe imparfait de taille width x height à l'aide d'une méthode existante chosie aléatoirement
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
maze_t rimperfect_maze(const int width, const int height);

#endif //MAZEMAKER_H
