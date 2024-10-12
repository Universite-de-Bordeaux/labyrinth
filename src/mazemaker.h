#ifndef MAZEMAKER_H
#define MAZEMAKER_H
#include "case.h"

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
