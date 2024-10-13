#ifndef CASE_H
#define CASE_H
#include <stdbool.h>

//Structure - typedef - fonction (dans l'ordre)

// --- Structures ---

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

struct way
{
    int x;
    int y;
    struct way *dad;
};

struct waytab
{
    int width;
    int height;
    struct way **tab;
};


// --- Typedef ---

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

//Structure représentant un chemin depuis le début du labyrinthe
//dad : cellule précédente
//x, y : coordonnées de la cellule
//malloc utilisés, penser à free_way pour libérer la mémoire
//toujours privilégier les fonctions de waytab pour manipuler les chemins
typedef struct way way;

//Structure représentant un tableau de chemins
//width : largeur du tableau
//height : hauteur du tableau
//tab : tableau de tableau de chemins
//malloc utilisés, penser à free_way_tab pour libérer la mémoire
typedef struct waytab waytab;


// --- Fonctions ---
// --- MAZE ---

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


// --- BOOL_TAB ---

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


// --- WAYTAB ---

//crée un tableau de chemin vide de taille width x height
//width : largeur du tableau
//height : hauteur du tableau
//renvoie le tableau de chemin
waytab create_waytab(int width, int height);

//désalloue la mémoire allouée pour le tableau de chemin
//way : le tableau de chemin à désallouer
void free_waytab(waytab tab);

//renvoie true si la case x, y du tableau connait un chemin vers le début du labyrinthe, false sinon
//tab : le tableau de chemin
//x : la coordonnée x de la case
//y : la coordonnée y de la case
//gère les cas d'erreur et affiche un message d'erreur avant d'arrêter le programme
bool has_way(waytab tab, int x, int y);

//renvoie le chemin de la case x, y du tableau
//tab : le tableau de chemin
//x : la coordonnée x de la case
//y : la coordonnée y de la case
//gère les cas d'erreur et affiche un message d'erreur avant d'arrêter le programme
way *get_way(waytab tab, int x, int y);

//impose un chemin à la case x, y du tableau
//tab : le tableau de chemin
//x : la coordonnée x de la case
//y : la coordonnée y de la case
//dad_x : la coordonnée x du nouveau père
//dad_y : la coordonnée y du nouveau père
//gère les cas d'erreur et affiche un message d'erreur sans arrêter le programme
//ne vérifie pas si le chemin est valide
void connected_way(waytab tab, int x, int y, int dad_x, int dad_y);

//renvoie la longueur du chemin de la case x, y du tableau
//tab : le tableau de chemin
//x : la coordonnée x de la case
//y : la coordonnée y de la case
//renvoie 4294967295 si le chemin n'existe pas
int length_waytab(waytab tab, int x, int y);


// --- WAY ---

//écrit le chemin de la case x, y du tableau à la case (0, 0)
//w : le chemin
void print_way(const way *w);

//renvoie la longueur du chemin
//w : le chemin
//renvoie 4294967295 si le chemin n'existe pas
int length_way(const way *w);

//change le père du chemin
//son : le chemin
//dad : le nouveau père
//mettre dad à NULL pour supprimer le chemin
void new_dad(way *son, way *dad);

//crée une copie du chemin
//w : le chemin à copier
//renvoie la copie du chemin
way *copy_way(const way *w);

//désalloue la mémoire allouée pour le chemin
//w : le chemin à désallouer
//NE PAS UTILISER DANS UN WAYTAB !!!
void free_way(way *w);

//renvoie true si le chemin est vide (c'est à dire qu'il n'est pas relié à la case (0, 0)), false sinon
//w : le chemin
bool is_empty(const way *w);
#endif //CASE_H
