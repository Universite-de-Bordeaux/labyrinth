#include "struct.h"

#include <sys/random.h>

// --- MAZE FUNCTIONS ---

maze_t create_basic_maze(const int width, const int height)
{
    if (width < 1 || height < 1)
    {
        fprintf(stderr,
                "Erreur dans la fonction create_basic_maze : \nles dimensions du labyrinthe doivent être strictements "
                "positives, width : %d, height : %d\n",
                width, height);
        exit(1);
    }
    char** start = malloc(sizeof(char*) * height);
    for (int i = 0; i < height; i++)
    {
        start[i] = malloc(sizeof(char) * width);
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++) // cellules non bordure
        {
            start[i][j] = 0;
        }
        start[i][width - 1] += 8; // cellules bordure droite
        start[i][0] += 4; // cellules bordure gauche
    }
    for (int j = 0; j < width; j++)
    {
        start[height - 1][j] += 2; // cellules bordure bas
        start[0][j] += 1; // cellules bordure haut
    }

    const maze_t maze = {width, height, start};
    return maze;
}

maze_t create_wall_maze(const int width, const int height)
{
    if (width < 1 || height < 1)
    {
        fprintf(stderr,
                "Erreur dans la fonction create_wall_maze : \nles dimensions du labyrinthe doivent être strictements "
                "positives, width : %d, height : %d\n",
                width, height);
        exit(1);
    }
    char** start = malloc(sizeof(char*) * height);
    for (int i = 0; i < height; i++)
    {
        start[i] = malloc(sizeof(char) * width);
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            start[i][j] = 15; // toutes les cellules ont les 4 murs
        }
    }
    const maze_t maze = {width, height, start};
    return maze;
}

void free_maze(const maze_t maze)
{
    for (int i = 0; i < maze.height; i++)
    {
        free(maze.walls[i]);
    }
    free(maze.walls);
}

void wall_up(maze_t const maze, const int x, const int y)
{
    if (y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr,
                "Erreur dans la fonction wall_up : \nles coordonnées de la cellule sont en dehors des limites du "
                "labyrinthe, cible : %d, %d\n",
                x, y);
        return;
    }
    if (y > 0)
    {
        maze.walls[y][x] |= 1; // ajoute le mur du haut
        maze.walls[y - 1][x] |= 2; // ajoute le mur du bas de la cellule du haut
    }
    else
    {
        fprintf(stderr, "Impossible de modifier un mur de délimitation du labyrinthe, cible : %d, %d up\n", x, y);
    }
}

void wall_down(const maze_t maze, const int x, const int y)
{
    if (y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr,
                "Erreur dans la fonction wall_down : \nles coordonnées de la cellule sont en dehors des limites du "
                "labyrinthe, cible : %d, %d\n",
                x, y);
        return;
    }
    if (y < maze.height - 1)
    {
        maze.walls[y][x] |= 2; // ajoute le mur du bas
        maze.walls[y + 1][x] |= 1; // ajoute le mur du haut de la cellule du bas
    }
    else
    {
        fprintf(stderr, "Impossible de modifier un mur de délimitation du labyrinthe, cible : %d, %d down\n", x, y);
    }
}

void wall_left(const maze_t maze, const int x, const int y)
{
    if (y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr,
                "Erreur dans la fonction wall_left : \nles coordonnées de la cellule sont en dehors des limites du "
                "labyrinthe, cible : %d, %d\n",
                x, y);
        return;
    }
    if (x > 0)
    {
        maze.walls[y][x] |= 4; // ajoute le mur de gauche
        maze.walls[y][x - 1] |= 8; // ajoute le mur de droite de la cellule de gauche
    }
    else
    {
        fprintf(stderr, "Impossible de modifier un mur de délimitation du labyrinthe, cible : %d, %d left\n", x, y);
    }
}

void wall_right(const maze_t maze, const int x, const int y)
{
    if (y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr,
                "Erreur dans la fonction wall_right : \nles coordonnées de la cellule sont en dehors des limites du "
                "labyrinthe, cible : %d, %d\n",
                x, y);
        return;
    }
    if (x < maze.width - 1)
    {
        maze.walls[y][x] |= 8; // ajoute le mur de droite
        maze.walls[y][x + 1] |= 4; // ajoute le mur de gauche de la cellule de droite
    }
    else
    {
        fprintf(stderr, "Impossible de modifier un mur de délimitation du labyrinthe, cible : %d, %d right\n", x, y);
    }
}

void unwall_up(const maze_t maze, const int x, const int y)
{
    if (y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr,
                "Erreur dans la fonction unwall_up : \nles coordonnées de la cellule sont en dehors des limites du "
                "labyrinthe, cible : %d, %d\n",
                x, y);
        return;
    }
    if (y > 0)
    {
        maze.walls[y][x] &= 14; // retire le mur du haut
        maze.walls[y - 1][x] &= 13; // retire le mur du bas de la cellule du haut
    }
    else
    {
        fprintf(stderr, "Impossible de modifier un mur de délimitation du labyrinthe, cible : %d, %d up\n", x, y);
    }
}

void unwall_down(const maze_t maze, const int x, const int y)
{
    if (y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr,
                "Erreur dans la fonction unwall_down : \nles coordonnées de la cellule sont en dehors des limites du "
                "labyrinthe, cible : %d, %d\n",
                x, y);
        return;
    }
    if (y < maze.height - 1)
    {
        maze.walls[y][x] &= 13; // retire le mur du bas
        maze.walls[y + 1][x] &= 14; // retire le mur du haut de la cellule du bas
    }
    else
    {
        fprintf(stderr, "Impossible de modifier un mur de délimitation du labyrinthe, cible : %d, %d down\n", x, y);
    }
}

void unwall_left(const maze_t maze, const int x, const int y)
{
    if (y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr,
                "Erreur dans la fonction unwall_left : \nles coordonnées de la cellule sont en dehors des limites du "
                "labyrinthe, cible : %d, %d\n",
                x, y);
        return;
    }
    if (x > 0)
    {
        maze.walls[y][x] &= 11; // retire le mur de gauche
        maze.walls[y][x - 1] &= 7; // retire le mur de droite de la cellule de gauche
    }
    else
    {
        fprintf(stderr, "Impossible de modifier un mur de délimitation du labyrinthe, cible : %d, %d left\n", x, y);
    }
}

void unwall_right(const maze_t maze, const int x, const int y)
{
    if (y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr,
                "Erreur dans la fonction unwall_right : \nles coordonnées de la cellule sont en dehors des limites du "
                "labyrinthe, cible : %d, %d\n",
                x, y);
        return;
    }
    if (x < maze.width - 1)
    {
        maze.walls[y][x] &= 7; // retire le mur de droite
        maze.walls[y][x + 1] &= 11; // retire le mur de gauche de la cellule de droite
    }
    else
    {
        fprintf(stderr, "Impossible de modifier un mur de délimitation du labyrinthe, cible : %d, %d right\n", x, y);
    }
}

bool has_wall_up(const maze_t maze, const int x, const int y)
{
    if (y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr,
                "Erreur dans la fonction has_wall_up : \nles coordonnées de la cellule sont en dehors des limites du "
                "labyrinthe, cible : %d, %d\n",
                x, y);
        exit(EXIT_FAILURE);
    }
    return maze.walls[y][x] & 1;
}

bool has_wall_down(const maze_t maze, const int x, const int y)
{
    if (y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr,
                "Erreur dans la fonction has_wall_down : \nles coordonnées de la cellule sont en dehors des limites du "
                "labyrinthe, cible : %d, %d\n",
                x, y);
        exit(EXIT_FAILURE);
    }
    return maze.walls[y][x] & 2;
}

bool has_wall_left(const maze_t maze, const int x, const int y)
{
    if (y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr,
                "Erreur dans la fonction has_wall_left : \nles coordonnées de la cellule sont en dehors des limites du "
                "labyrinthe, cible : %d, %d\n",
                x, y);
        exit(EXIT_FAILURE);
    }
    return maze.walls[y][x] & 4;
}

bool has_wall_right(const maze_t maze, const int x, const int y)
{
    if (y < 0 || x < 0 || x >= maze.width || y >= maze.height)
    {
        fprintf(stderr,
                "Erreur dans la fonction has_wall_right : \nles coordonnées de la cellule sont en dehors des limites "
                "du labyrinthe, cible : %d, %d\n",
                x, y);
        exit(EXIT_FAILURE);
    }
    return maze.walls[y][x] & 8;
}


// --- BOOLTAB FUNCTIONS ---

bool_tab create_booltab(const int width, const int height)
{
    if (width < 1 || height < 1)
    {
        fprintf(stderr,
                "Erreur dans la fonction create_booltab : \nles dimensions du tableau de booléens doivent être "
                "strictements positives, width : %d, height : %d\n",
                width, height);
        exit(EXIT_FAILURE);
    }
    char** tab = malloc(sizeof(char*) * height);
    const int size = width % 8 == 0 ? width / 8 : width / 8 + 1;
    for (int i = 0; i < height; i++)
    {
        tab[i] = malloc(sizeof(char) * size);
        for (int j = 0; j < size; j++)
        {
            tab[i][j] = 0;
        }
    }
    const bool_tab btab = {width, height, tab};
    return btab;
}

void free_booltab(const bool_tab tab)
{
    for (int i = 0; i < tab.height; i++)
    {
        free(tab.tab[i]);
    }
    free(tab.tab);
}

void set_true(const bool_tab tab, const int x, const int y)
{
    if (y < 0 || x < 0 || x >= tab.width || y >= tab.height)
    {
        fprintf(stderr,
                "Erreur dans la fonction set_true : \nles coordonnées de la case sont en dehors des limites du "
                "tableau, cible : %d, %d\n",
                x, y);
        return;
    }
    const int nx = x / 8;
    tab.tab[y][nx] |= 1 << x % 8; // set the bit to 1 NOLINT(*-narrowing-conversions)
}

void set_false(const bool_tab tab, const int x, const int y)
{
    if (y < 0 || x < 0 || x >= tab.width || y >= tab.height)
    {
        fprintf(stderr,
                "Erreur dans la fonction set_false : \nles coordonnées de la case sont en dehors des limites du "
                "tableau, cible : %d, %d\n",
                x, y);
        return;
    }
    const int nx = x / 8;
    tab.tab[y][nx] &= ~(1 << x % 8); // set the bit to 0 NOLINT(*-narrowing-conversions)
}

bool get_bool(const bool_tab tab, const int x, const int y)
{
    if (y < 0 || x < 0 || x >= tab.width || y >= tab.height)
    {
        fprintf(stderr,
                "Erreur dans la fonction get_bool : \nLes coordonnées de la case sont en dehors des limites du "
                "tableau, cible : %d, %d\n",
                x, y);
        exit(1);
    }
    const int nx = x / 8;
    const char temp = tab.tab[y][nx];
    return temp >> x % 8 & 1;
}


// --- WAYTAB FUNCTIONS ---

waytab create_waytab(const int width, const int height)
{
    if (width < 1 || height < 1)
    {
        fprintf(stderr,
                "Erreur dans la fonction create_waytab : \nles dimensions du tableau de chemin doivent être "
                "strictements positives, width : %d, height : %d\n",
                width, height);
        exit(EXIT_FAILURE);
    }
    way** way_tab = malloc(sizeof(way*) * height);
    for (int i = 0; i < height; i++)
    {
        way_tab[i] = malloc(sizeof(way) * width);
        for (int j = 0; j < width; j++)
        {
            way_tab[i][j] = *create_way();
            way_tab[i][j].x = j;
            way_tab[i][j].y = i;
        }
    }
    way_tab[0][0].length = 0;
    const waytab tab = {way_tab, width, height};
    // ReSharper disable once CppDFAMemoryLeak
    return tab;
}

void free_waytab(const waytab tab)
{
    for (int i = 0; i < tab.height; i++)
    {
        free(tab.tab[i]);
    }
    free(tab.tab);
}

bool has_way(const waytab tab, const int x, const int y)
{
    if (y < 0 || x < 0 || x >= tab.width || y >= tab.height)
    {
        fprintf(stderr,
                "Erreur dans la fonction has_way : \nles coordonnées de la case sont en dehors des limites du tableau, "
                "cible : %d, %d\n",
                x, y);
        exit(EXIT_FAILURE);
    }
    return !is_empty(&tab.tab[y][x]);
}

way* get_way(const waytab tab, const int x, const int y)
{
    if (y < 0 || x < 0 || x >= tab.width || y >= tab.height)
    {
        fprintf(stderr,
                "Erreur dans la fonction get_way : \nles coordonnées de la case sont en dehors des limites du tableau, "
                "cible : %d, %d\n",
                x, y);
        exit(EXIT_FAILURE);
    }
    return &tab.tab[y][x];
}

void connected_way(const waytab tab, const int x, const int y, const int dad_x, const int dad_y)
{
    if (y < 0 || x < 0 || x >= tab.width || y >= tab.height)
    {
        fprintf(stderr,
                "Erreur dans la fonction connected_way : \nles coordonnées de la case sont en dehors des limites du "
                "tableau, cible : %d, %d\n",
                x, y);
        return;
    }
    if (dad_x < 0 || dad_y < 0 || dad_x >= tab.width || dad_y >= tab.height)
    {
        fprintf(stderr,
                "Erreur dans la fonction connected_way : \nles coordonnées du père sont en dehors des limites du "
                "tableau, cible : %d, %d\n",
                dad_x, dad_y);
        return;
    }
    new_dad(tab.tab[y] + x, tab.tab[dad_y] + dad_x);
}

unsigned int length_waytab(const waytab tab, const int x, const int y)
{
    if (y < 0 || x < 0 || x >= tab.width || y >= tab.height)
    {
        fprintf(stderr,
                "Erreur dans la fonction length_waytab : \nles coordonnées de la case sont en dehors des limites du "
                "tableau, cible : %d, %d\n",
                x, y);
        exit(EXIT_FAILURE);
    }
    return length_way(&tab.tab[y][x]);
}

// --- WAY FUNCTIONS ---

way* create_way()
{
    // ReSharper disable once CppDFAMemoryLeak
    way* w = malloc(sizeof(way));
    w->dad = NULL;
    w->x = -1;
    w->y = -1;
    w->length = L_TP;
    return w;
}

void print_way(const way* w)
{
    if (is_empty(w))
    {
        printf("Chemin vide\n");
        return;
    }
    if (w->dad != NULL)
    {
        print_way(w->dad);
    }
    printf("(%d, %d)\n", w->x, w->y);
}

unsigned int length_way(const way* w) { return w->x == 0 && w->y == 0 ? 0 : w->length; }

void new_dad(way* son, way* dad)
{
    if (son->x == 0 && son->y == 0)
    {
        fprintf(stderr, "Erreur dans la fonction new_dad : \nla case de départ ne peut pas avoir de père\n");
        return;
    }
    son->dad = dad;
    son->length = dad->length + 1;
}

way* copy_way(const way* w)
{
    way* copy = malloc(sizeof(way));
    if (w->dad != NULL)
    {
        copy->dad = copy_way(w->dad);
    }
    else
    {
        copy->dad = NULL;
    }
    copy->x = w->x;
    copy->y = w->y;
    copy->length = w->length;
    return copy;
}

void free_way(way* w)
{
    if (!w)
    {
        return;
    }
    if (w->dad != NULL)
    {
        free_way(w->dad);
    }
    free(w);
}

bool is_empty(const way* w)
{
    if (w->length == L_TP)
    {
        return true;
    }
    if (w->dad == NULL)
    {
        return !is_origin(w); // un chemin est vide s'il n'est pas relié à la case (0, 0)
    }
    return is_empty(w->dad);
}

way* get_dad(const way* w) { return w->dad; }

int get_x(const way* w) { return w->x; }

int get_y(const way* w) { return w->y; }

bool is_origin(const way* w) { return w->x == 0 && w->y == 0; }

void fix_size(way* w)
{
    if (w->dad != NULL)
    {
        fix_size(w->dad);
        w->length = w->dad->length + 1;
        return;
    }
    w->length = w->x == 0 && w->y == 0 ? 0 : L_TP;
}

// --- QUEUE FUNCTIONS ---

// Auxilary function of grow_queue
// Double the size of the array used in the representation.
// This function will be used when the array is full and we want to add a value to it.
static void grow_queue(queue* q)
{
    int* new_array = malloc(sizeof(int) * q->size_array * 2);
    for (int i = 0; i < q->size_array; i++)
    {
        new_array[i] = q->array[(i + q->left) % q->size_array];
    }
    free(q->array);
    q->array = new_array;
    q->left = 0;
    q->right = q->size_array;
    q->size_array *= 2;
}

// Auxilary function of shrink_queue
// Divide by two the size of the array used in the representation.
// This function will be used when the array is filled at less than a fourth of its capacity.
static void shrink_queue(queue* p)
{
    if (p->size_array < 3)
    {
        return;
    }
    const int t = size_queue(p);
    int* new_array = malloc(sizeof(int) * p->size_array / 2);
    for (int i = 0; i < t; i++)
    {
        new_array[i] = p->array[(p->left + i) % p->size_array];
    }
    free(p->array);
    p->left = 0;
    p->right = t;
    p->size_array /= 2;
    p->array = new_array;
}

queue* create_queue(void)
{
    int* array = malloc(sizeof(int) * 2);
    queue* q = malloc(sizeof(queue));
    if (q == NULL || array == NULL)
    {
        fprintf(stderr, "Erreur malloc\n");
        exit(1);
    }
    q->size_array = 2;
    q->left = 0;
    q->right = 0;
    q->empty = true;
    q->array = array;
    return q;
}

void free_queue(queue* q)
{
    free(q->array);
    q->array = NULL;
    q->size_array = 0;
    q->left = 0;
    q->right = 0;
    q->empty = true;
    free(q);
}

bool isempty_queue(const queue* q) { return q->empty; }

int size_queue(const queue* q)
{
    const int t = q->right - q->left;
    if (t == 0)
    {
        return isempty_queue(q) ? 0 : q->size_array;
    }
    if (t > 0)
    {
        return t;
    }
    return t + q->size_array;
}

void enqueue(const int x, const int y, queue* q)
{
    if (q->right == q->left && !isempty_queue(q))
    {
        grow_queue(q);
    }
    q->array[q->right] = x;
    q->right = (q->right + 1) % q->size_array;
    q->empty = false;
    if (q->right == q->left)
    {
        grow_queue(q);
    }
    q->array[q->right] = y;
    q->right = (q->right + 1) % q->size_array;
}

void dequeue(queue* q, int* x, int* y)
{
    if (q->empty)
    {
        fprintf(stderr, "Error : try to dequeue an empty queue\n");
        exit(1);
    }
    *x = q->array[q->left];
    q->left = (q->left + 1) % q->size_array;
    if (q->left == q->right)
    {
        fprintf(stderr, "Error : the queue has only one element\n");
        exit(1);
    }
    *y = q->array[q->left];
    q->left = (q->left + 1) % q->size_array;
    if (q->left == q->right)
    {
        q->empty = true;
    }
    if (size_queue(q) < q->size_array / 4)
    {
        shrink_queue(q);
    }
}

void print_queue(const queue* q)
{
    const int s = size_queue(q);
    printf("queue :\n");
    if (q->empty) // si la queue est vide
    {
        printf("empty queue\n");
        return;
    }
    for (int i = 0; i < s; i = i + 2)
    {
        printf("x : %d y : %d\n", q->array[(q->left + i) % q->size_array], q->array[(q->left + i + 1) % q->size_array]);
    }
}

// --- STACK FUNCTIONS ---

// Auxilary function of grow_stack
// Double the size of the array used in the representation.
// This function will be used when the array is full and we want to add a value to it.
static void grow_stack(stack* s)
{
    s->size_array *= 2;
    s->array = realloc(s->array, sizeof(int) * s->size_array); // NOLINT(*-suspicious-realloc-usage)
    if (s->array == NULL)
    {
        fprintf(stderr, "Erreur realloc\n");
        exit(1);
    }
}

// Auxilary function of shrink_stack
// Divide by two the size of the array used in the representation.
// This function will be used when the array is filled at less than a fourth of its capacity.
static void shrink_stack(stack* s)
{
    if (s->size_array < 3)
        return;
    s->size_array /= 2;
    s->array = realloc(s->array, sizeof(int) * s->size_array); // NOLINT(*-suspicious-realloc-usage)
    if (s->array == NULL)
    {
        fprintf(stderr, "Erreur realloc\n");
        exit(1);
    }
}

stack* create_stack(void)
{
    stack* p = malloc(sizeof(stack));
    if (p == NULL)
    {
        fprintf(stderr, "Erreur maloc\n");
        exit(1);
    }
    p->size_array = 2;
    p->size_stack = 0;
    p->array = (int*)malloc(sizeof(int) * p->size_array);
    if (p->array == NULL)
    {
        fprintf(stderr, "Erreur malloc\n");
        exit(1);
    }
    return p;
}

void free_stack(stack* s)
{
    free(s->array);
    free(s);
}

bool isempty_stack(const stack* s) { return s->size_stack == 0; }

int size_stack(const stack* s) { return s->size_stack / 2; }

void pop(stack* s, int* x, int* y)
{
    if (s->size_stack == 0)
    {
        fprintf(stderr, "Error : try to pop en empty pill\n");
        exit(1);
    }
    if (s->size_stack == 1)
    {
        fprintf(stderr, "Error : the pill has only one element\n");
        exit(1);
    }
    *y = s->array[s->size_stack - 1];
    *x = s->array[s->size_stack - 2];
    s->size_stack -= 2;
    if (s->size_stack <= s->size_array / 4 && s->size_array > 1)
    {
        shrink_stack(s);
    }
}

void rpop(stack* s, int* x, int* y)
{
    if (s->size_stack == 0)
    {
        fprintf(stderr, "Error : try to pop en empty pill\n");
        exit(1);
    }
    queue* q = create_queue();
    unsigned int tirage;
    getrandom(&tirage, sizeof(unsigned int), 0);
    tirage = tirage % (s->size_stack / 2);
    int a, b;
    for (int i = 0; i < tirage; i++)
    {
        pop(s, &a, &b);
        enqueue(a, b, q);
    }
    pop(s, x, y);
    while (!isempty_queue(q))
    {
        dequeue(q, &a, &b);
        push(a, b, s);
    }
    free_queue(q);
}

void push(const int x, const int y, stack* s)
{
    if (s->size_stack == s->size_array)
    {
        grow_stack(s);
    }
    s->array[s->size_stack] = x;
    if (++s->size_stack == s->size_array)
    {
        grow_stack(s);
    }
    s->array[s->size_stack] = y;
    s->size_stack++;
}

// --- MAZE PRINTING ---

int pre_print_maze(const maze_t maze, SDL_Renderer** renderer, SDL_Window** window, int* dw, int* dh)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0) // initilisation de la SDL avec l'image et les events (comprends des malloc)
    {
        fprintf(stderr, "Erreur d'initialisation de la SDL : %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0)
    { // on obtient le mode d'affichage de l'écran
        fprintf(stderr, "Erreur lors de l'obtention du mode d'affichage : %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    int d_h = (displayMode.h - 40) / maze.height;
    int d_w = displayMode.w / maze.width; // on définie le ratio de la taille des cellules
    // le -40 est pour les 40 pixels du haut de mon écran qui sont occupés par la barre de titre et empêche l'affichage,
    // Vincent
    d_h = d_h - 1 > d_w ? d_w : d_h - 1; // on prend le plus petit ratio pour que le labyrinthe tienne dans l'écran
    d_w = d_h;
    if (d_w < 3) // la taille minimale des cellules est de 2 pixels (1 pixel de vide et 2 pixels pour chaque mur)
    {
        fprintf(stderr,
                "Warning : la taille des cellules est trop petite pour être affichée correctement, l'affichage va "
                "dépasser de l'écran\n");
        d_w = 3;
        d_h = 3;
    }

    *window = SDL_CreateWindow("maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, maze.width * d_w, maze.height * d_h, SDL_WINDOW_SHOWN); // creation d'une fenetre
    if (*window == NULL)
    {
        fprintf(stderr, "Erreur de creation de la fenetre : %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED); // creation d'un renderer
    if (*renderer == NULL)
    {
        *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_SOFTWARE);
        if (*renderer == NULL)
        {
            fprintf(stderr, "Erreur de creation du renderer : %s\n", SDL_GetError());
            SDL_DestroyWindow(*window);
            SDL_Quit();
            return -1;
        }
    }

    SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255); // on définit la couleur de fond en blanc

    SDL_RenderDrawLine(*renderer, 0, 0, 0, (maze.height * d_h) - 1); // on dessine les murs de la bordure gauche
    SDL_RenderDrawLine(*renderer, 0, 0, (maze.width * d_w) - 1, 0); // on dessine les murs de la bordure haute
    SDL_RenderDrawLine(*renderer, (maze.width * d_w) - 1, 0, (maze.width * d_w) - 1, (maze.height * d_h) - 1); // on dessine les murs de la bordure droite
    SDL_RenderDrawLine(*renderer, 0, (maze.height * d_h) - 1, (maze.width * d_w) - 1, maze.height * d_h - 1); // on dessine les murs de la bordure basse

    SDL_SetRenderDrawColor(*renderer, 0, 50, 255, 255); // on définit la couleur en bleu
    SDL_RenderDrawLine(*renderer, 0, 0, d_w, 0); // on dessine l'entrée
    SDL_RenderDrawLine(*renderer, 0, 0, 0, d_h); // on dessine l'entrée

    SDL_SetRenderDrawColor(*renderer, 10, 235, 10, 255); // on définit la couleur en vert
    SDL_RenderDrawLine(*renderer, maze.width * d_w - d_w, maze.height * d_h - 1, maze.width * d_w,
                       maze.height * d_h - 1); // on dessine la sortie
    SDL_RenderDrawLine(*renderer, maze.width * d_w - 1, maze.height * d_h - d_h, maze.width * d_w - 1,
                       maze.height * d_h); // on dessine la sortie

    SDL_Delay(displayMode.refresh_rate); // pause pour laisser aux données le temps de s'afficher
    SDL_RenderPresent(*renderer); // on met à jour l'affichage

    // ReSharper disable twice CppDFANullDereference
    *dw = d_w;
    *dh = d_h;
    return 1;
}

int initial_print_maze(const maze_t maze, SDL_Renderer** renderer, SDL_Window** window, int* dw, int* dh)
{
    if (pre_print_maze(maze, renderer, window, dw, dh) == -1)
    {
        return -1;
    }

    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0)
    { // on obtient le mode d'affichage de l'écran
        fprintf(stderr, "Erreur lors de l'obtention du mode d'affichage : %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    SDL_SetRenderDrawColor(*renderer, 255, 255, 255, 255); // on définit la couleur de fond en blanc
    for (int x = 0; x < maze.width; x++)
    {
        for (int y = 0; y < maze.height; y++)
        {
            if ((x == 0 && y == 0) || (x == maze.width - 1 && y == maze.height - 1))
            {
                continue;
            }
            if (has_wall_down(maze, x, y))
            {
                SDL_RenderDrawLine(*renderer, x * *dw, (y + 1) * *dh - 1, (x + 1) * *dw - 1,
                                   (y + 1) * *dh - 1); // on dessine un mur en bas
            }
            if (has_wall_right(maze, x, y))
            {
                SDL_RenderDrawLine(*renderer, (x + 1) * *dw - 1, y * *dh, (x + 1) * *dw - 1,
                                   (y + 1) * *dh - 1); // on dessine un mur à droite
            }
        }
    }
    SDL_Delay(displayMode.refresh_rate); // pause pour laisser aux données le temps de s'afficher
    SDL_RenderPresent(*renderer); // on met à jour l'affichage
    return 1;
}

void destroy_print_maze(SDL_Renderer* renderer, SDL_Window* window)
{
    SDL_DestroyRenderer(renderer); // destruction du renderer (desallocation de la memoire)
    SDL_DestroyWindow(window); // destruction de la fenetre (desallocation de la memoire)
    SDL_Quit(); // desalocation de la memoire
}

void wait_and_destroy_print_maze(SDL_Renderer* renderer, SDL_Window* window)
{
    SDL_Event event = {0}; // on crée un event vide
    while (!(
        event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE ||
        (event.type == SDL_KEYUP &&
         (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_KP_ENTER || event.key.keysym.sym == SDLK_RETURN)))) // tant que l'utilisateur n'a pas fermé la fenetre
    {
        SDL_WaitEvent(&event); // on enregistre les events entrants
    }
    destroy_print_maze(renderer, window);
}

int print_maze(const maze_t maze)
{
    SDL_Renderer* renderer = 0;
    SDL_Window* window = 0;
    int dw = 0;
    int dh = 0;
    if (initial_print_maze(maze, &renderer, &window, &dw, &dh) == -1)
    {
        return -1;
    }
    wait_and_destroy_print_maze(renderer, window);
    return 1;
}
