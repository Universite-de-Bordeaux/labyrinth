#ifndef CASE_H
#define CASE_H
#include <stdbool.h>
#include <SDL2/SDL.h>
#define L_TP 3000000000

//Structure - typedef - fonction (dans l'ordre)

// --- Structures ---

struct cell {
    bool wall_down;
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
    unsigned int length;
};

struct waytab
{
    int width;
    int height;
    struct way **tab;
};

typedef struct {
    int *array;     // Tableau des valeurs.
    int size_array; // Taille du tableau des valeurs.
    int left;       // Indice de la valeur à gauche de la file (si non-vide).
    int right;      // Indice qui suit celui de la valeur à droite de la fil (si elle est non-vide).
    bool empty;     // Booléen indiquant si la file est vide.
} queue;

typedef struct {
    int size_array; // Taille totale du tableau alloué en mémoire.
    int size_stack; // Nombre d'éléments dans la pile représentée.
    int *array;     // Tableau contenant les valeurs de la pile représentée.
} stack;

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
//length : le longueur du chemin depuis le départ, on peut se téléporter pour un coût de L_TP
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

// --- VISUALISATION ---


//affiche le labyrinthe, renvoie -1 en cas d'erreur, 1 sinon
//maze : le labyrinthe à afficher
//les murs d'entré sont en bleu, les murs de sortie en vert
//la taille de chaque cellules est automatiquement ajustée pour remplir la fenêtre
//le programme se ferme quand on appuie sur ECHAP, ENTRER, ou tente de fermer la fenetre
int print_maze(maze_t maze);

//affiche le labyrinthe, renvoie -1 en cas d'erreur, 1 sinon
//assigne les valeurs utiles aux variables d'entrées
//maze : le labyrinthe à afficher
//renderer : le renderer de la fenêtre résultante
//window : la fenêtre résultante
//dw : la largeur résultante de chaque cellule
//dh : la hauteur résultante de chaque cellule
//les murs d'entré sont en bleu, les murs de sortie en vert
//la taille de chaque cellules est automatiquement ajustée pour remplir la fenêtre
int initialisde_print_maze(const maze_t maze, const SDL_Renderer *renderer, const SDL_Window *window, int *dw, int *dh);

//désalloue la mémoire allouée pour l'affichage du labyrinthe et ferme la fenêtre
void destroy_print_maze(SDL_DisplayMode *display, const SDL_Renderer *renderer, const SDL_Window *window);

//désalloue la mémoire allouée pour l'affichage du labyrinthe et ferme la fenêtre quand on appuie sur les touchent ECHAP, ENTRER, ou tente de fermer la fenetre
void wait_and_destroy_print_maze(SDL_Renderer *renderer, SDL_Window *window);

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
unsigned int length_waytab(waytab tab, int x, int y);


// --- WAY ---

//crée un chemin vide
//renvoie le chemin
way *create_way();

//écrit le chemin de la case x, y du tableau à la case (0, 0)
//w : le chemin
void print_way(const way *w);

//renvoie la longueur du chemin
//w : le chemin
unsigned int length_way(const way *w);

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

// --- QUEUE ---

// Crée une file vide.
queue *create_queue(void);

// Libère la mémoire allouée pour la file.
void delete_queue(queue *);

// Renvoie le nombre d'éléments dans la file.
int size_queue(const queue *);

// Renvoie vrai si la file est vide, faux sinon.
bool isempty_queue(const queue *);

// Ajoute une valeur dans la file, à gauche de la valeur la plus à gauche.
void enqueue(int, queue *);

// Renvoie la valeur la plus à droite de la file et la retire.
int dequeue(queue *);

// --- STACK ---

// Crée une pile vide.
stack *create_stack(void);

// Libère la mémoire allouée pour la pile.
void delete_stack(stack *);

// Renvoie vrai si la pile est vide, faux sinon.
bool isempty_stack(const stack *);

// Renvoie le nombre d'éléments dans la pile.
int size_stack(const stack *);

// Renvoie la valeur au sommet de la pile et la retire. Si la pile est vide, la
// fonction affiche un message sur la sortie erreur standard et termine le
// programme. Si l'occupation du tableau tombe à 25% après le pop(), le tableau
// est redimensionné par la fonction shrink_stack().
int pop(stack *);

// Ajoute une valeur au sommet de la pile. Si le tableau est plein, il est
// redimensionné au préalable, par la fonction grow_stack.
void push(int, stack *);

#endif //CASE_H
