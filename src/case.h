#ifndef CASE_H
#define CASE_H
#include <stdio.h>
#include <stdbool.h>

struct cell {
    bool wall_up;
    bool wall_down;
    bool wall_left;
    bool wall_right;
};
struct maze {
    int width;
    int height;
    struct cell **cells;
};
struct bool_tab {
    int width;
    int height;
    bool **tab;
};

//Structure représentant une cellule
//contient 4 booléens représentant les murs de la cellule :
//wall_up, wall_down, wall_left, wall_right
typedef struct cell cell;
//Structure représentant un labyrinthe
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
//cells : tableau de tableau de cellules
//malloc utilisés, penser à free_maze pour libérer la mémoire
typedef struct maze maze_t;
//Structure représentant un tableau de booléens
//width : largeur du tableau
//height : hauteur du tableau
//tab : tableau de tableau de booléens
//malloc utilisés, penser à free_booltab pour libérer la mémoire
typedef struct bool_tab bool_tab;

//maze_t

// Create a maze of width x height cells
// width : width of the maze
// height : height of the maze
// tous les murs sont ouverts (hors limites)
// returns the maze
// gère les cas d'erreur et affiche un message d'erreur avant d'arrêter le programme
maze_t create_basic_maze(int width, int height);

// Create a maze of width x height cells
// width : width of the maze
// height : height of the maze
// tous les murs sont fermés
// returns the maze
// gère les cas d'erreur et affiche un message d'erreur avant d'arrêter le programme
maze_t create_wall_maze(int width, int height);

//désalloue la mémoire allouée pour le labyrinthe
//maze : le labyrinthe à désallouer
void free_maze(maze_t maze);

//ajoute un mur en haut de la cellule x, y
//maze : le labyrinthe
//x : la coordonnée x de la cellule
//y : la coordonnée y de la cellule
//gère les cas d'erreur et affiche un message d'erreur sans arrêter le programme
void wall_up(maze_t maze, int x, int y);

//ajoute un mur en bas de la cellule x, y
//maze : le labyrinthe
//x : la coordonnée x de la cellule
//y : la coordonnée y de la cellule
//gère les cas d'erreur et affiche un message d'erreur sans arrêter le programme
void wall_down(maze_t maze, int x, int y);

//ajoute un mur à gauche de la cellule x, y
//maze : le labyrinthe
//x : la coordonnée x de la cellule
//y : la coordonnée y de la cellule
//gère les cas d'erreur et affiche un message d'erreur sans arrêter le programme
void wall_left(maze_t maze, int x, int y);

//ajoute un mur à droite de la cellule x, y
//maze : le labyrinthe
//x : la coordonnée x de la cellule
//y : la coordonnée y de la cellule
//gère les cas d'erreur et affiche un message d'erreur sans arrêter le programme
void wall_right(maze_t maze, int x, int y);

//ajoute un mur en haut de la cellule x, y
//maze : le labyrinthe
//x : la coordonnée x de la cellule
//y : la coordonnée y de la cellule
//gère les cas d'erreur et affiche un message d'erreur sans arrêter le programme
void unwall_up(maze_t maze, int x, int y);

//retire le mur du bas de la cellule x, y
//maze : le labyrinthe
//x : la coordonnée x de la cellule
//y : la coordonnée y de la cellule
//gère les cas d'erreur et affiche un message d'erreur sans arrêter le programme
void unwall_down(maze_t maze, int x, int y);

//retire le mur à gauche de la cellule x, y
//maze : le labyrinthe
//x : la coordonnée x de la cellule
//y : la coordonnée y de la cellule
//gère les cas d'erreur et affiche un message d'erreur sans arrêter le programme
void unwall_left(maze_t maze, int x, int y);

//retire le mur à droite de la cellule x, y
//maze : le labyrinthe
//x : la coordonnée x de la cellule
//y : la coordonnée y de la cellule
//gère les cas d'erreur et affiche un message d'erreur sans arrêter le programme
void unwall_right(maze_t maze, int x, int y);

//renvoie vrai si le mur du haut de la cellule x, y est présent
//renvoie faux sinon
//maze : le labyrinthe
//x : la coordonnée x de la cellule
//y : la coordonnée y de la cellule
//gère les cas d'erreur et affiche un message d'erreur avant d'arrêter le programme
bool has_wall_up(maze_t maze, int x, int y);

//renvoie vrai si le mur du bas de la cellule x, y est présent
//renvoie faux sinon
//maze : le labyrinthe
//x : la coordonnée x de la cellule
//y : la coordonnée y de la cellule
//gère les cas d'erreur et affiche un message d'erreur avant d'arrêter le programme
bool has_wall_down(maze_t maze, int x, int y);

//renvoie vrai si le mur de gauche de la cellule x, y est présent
//renvoie faux sinon
//maze : le labyrinthe
//x : la coordonnée x de la cellule
//y : la coordonnée y de la cellule
//gère les cas d'erreur et affiche un message d'erreur avant d'arrêter le programme
bool has_wall_left(maze_t maze, int x, int y);

//renvoie vrai si le mur de droite de la cellule x, y est présent
//renvoie faux sinon
//maze : le labyrinthe
//x : la coordonnée x de la cellule
//y : la coordonnée y de la cellule
//gère les cas d'erreur et affiche un message d'erreur avant d'arrêter le programme
bool has_wall_right(maze_t maze, int x, int y);

//affiche le labyrinthe, renvoie -1 en cas d'erreur, 1 sinon
//maze : le labyrinthe à afficher
//les murs d'entré sont en bleu, les murs de sortie en vert
//chaque cellule est de taille 20x20 pixels
//le programme se ferme quand on appuie sur ECHAP, ENTRER, ou tente de fermer la fenetre
int print_maze(maze_t maze);

//bool_tab

//crée un tableau de booléens sur false de taille width x height
//width : largeur du tableau
//height : hauteur du tableau
//renvoie le tableau de booléens
bool_tab create_booltab(int width, int height);

//désalloue la mémoire allouée pour le tableau de booléens
//tab : le tableau de booléens à désallouer
void free_booltab(bool_tab tab);

//met la case x, y du tableau de booléens à vrai
//tab : le tableau de booléens
//x : la coordonnée x de la case
//y : la coordonnée y de la case
//gère les cas d'erreur et affiche un message d'erreur sans arrêter le programme
void set_true(bool_tab tab, int x, int y);

//met la case x, y du tableau de booléens à faux
//tab : le tableau de booléens
//x : la coordonnée x de la case
//y : la coordonnée y de la case
//gère les cas d'erreur et affiche un message d'erreur sans arrêter le programme
void set_false(bool_tab tab, int x, int y);

//renvoie la valeur de la case x, y du tableau de booléens
//tab : le tableau de booléens
//x : la coordonnée x de la case
//y : la coordonnée y de la case
//gère les cas d'erreur et affiche un message d'erreur avant d'arrêter le programme
bool get_bool(bool_tab tab, int x, int y);
#endif //CASE_H
