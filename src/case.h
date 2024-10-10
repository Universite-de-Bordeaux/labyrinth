#ifndef CASE_H
#define CASE_H

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

bool_tab create_booltab(int width, int height);
void free_booltab(bool_tab tab);
void set_true(bool_tab tab, int x, int y);
void set_false(bool_tab tab, int x, int y);
bool get_bool(bool_tab tab, int x, int y);
maze_t create_basic_maze(int width, int height);
maze_t create_wall_maze(int width, int height);
void free_maze(maze_t maze);
int print_maze(maze_t maze, const char* filename);
void wall_up(maze_t maze, int x, int y);
void wall_down(maze_t maze, int x, int y);
void wall_left(maze_t maze, int x, int y);
void wall_right(maze_t maze, int x, int y);
void unwall_up(maze_t maze, int x, int y);
void unwall_down(maze_t maze, int x, int y);
void unwall_left(maze_t maze, int x, int y);
void unwall_right(maze_t maze, int x, int y);
bool is_wall_up(maze_t maze, int x, int y);
bool is_wall_down(maze_t maze, int x, int y);
bool is_wall_left(maze_t maze, int x, int y);
bool is_wall_right(maze_t maze, int x, int y);
#endif //CASE_H
