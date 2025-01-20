#include "mazemaker.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <time.h>
#include "outside.h"
#include "struct.h"

// Auxiliary functions
// update the connexity of the maze by adding a connexion to the cell (dx, dy) if it is connected to the cell (0, 0)
// return the number of visited cells
// maze : the maze
// is_connexe : the connexity of the maze
// dx, dy : coordinates of the cell to add
int static set_connexion(const maze_t maze, const bool_tab is_connexe, const int dx, const int dy)
{
    const bool_tab visited = create_booltab(maze.width, maze.height); // tableau de booléens pour savoir si une case a été traitée
    stack* s = create_stack(); // cette pile contiendra les coordonnées des cases visitées
    queue* q = create_queue(); // cette file contiendra les coordonnées des cases pour lesquelles on doit vérifier la connexité
    push(dx, dy, s);
    int x, y;
    bool connexion = false; // pour savoir si notre sous-graphe est lié à la case (0, 0)
    while (!isempty_stack(s))
    {
        pop(s, &x, &y);
        set_true(visited, x, y);
        if (get_bool(is_connexe, x, y))
        {
            connexion = true;
            continue; // on ne traite pas les cases déjà connexes
        }
        enqueue(x, y, q);
        if (!has_wall_down(maze, x, y) && !get_bool(visited, x, y + 1))
        {
            push(x, y + 1, s);
        }
        if (!has_wall_right(maze, x, y) && !get_bool(visited, x + 1, y))
        {
            push(x + 1, y, s);
        }
        if (!has_wall_up(maze, x, y) && !get_bool(visited, x, y - 1))
        {
            push(x, y - 1, s);
        }
        if (!has_wall_left(maze, x, y) && !get_bool(visited, x - 1, y))
        {
            push(x - 1, y, s);
        }
    }
    if (!connexion) // si notre sous-graphe n'est pas lié à la case (0, 0), c'est qu'il n'est pas connexe
    {
        free_stack(s);
        free_booltab(visited);
        free_queue(q);
        return 0; // on n'a rien modifié
    }
    int c = 0; // compteur de cases visitées
    while (!isempty_queue(q))
    {
        dequeue(q, &x, &y);
        set_true(is_connexe, x, y); // on marque la case comme connexe
        c++;
    }
    free_stack(s);
    free_booltab(visited);
    free_queue(q);
    return c; // on retourne le nombre de cases visitées
}

maze_t line_maze(const int width, const int height)
{
    const maze_t maze = create_basic_maze(width, height);
    if (height == 1 || width == 1)
    {
        return maze;
    }
    unsigned int break_wall;
    for (int i = 0; i < width - 1; i++)
    {
        getrandom(&break_wall, sizeof(break_wall), 0);
        break_wall %= height;
        for (int j = 0; j < height; j++)
        {
            if (j != break_wall)
            {
                wall_right(maze, i, j);
            }
        }
    }
    return maze;
}

maze_t column_maze(const int width, const int height)
{
    const maze_t maze = create_basic_maze(width, height);
    if (height == 1 || width == 1)
    {
        return maze;
    }
    unsigned int break_wall;
    for (int i = 0; i < height - 1; i++)
    {
        getrandom(&break_wall, sizeof(break_wall), 0);
        break_wall %= width;
        for (int j = 0; j < width; j++)
        {
            if (j != break_wall)
            {
                wall_down(maze, j, i);
            }
        }
    }
    return maze;
}

maze_t one_way_maze(const int width, const int height)
{
    const maze_t maze = create_wall_maze(width, height);
    const bool_tab annexe = create_booltab(width, height);
    int x, y;
    stack* s = create_stack();
    getrandom(&x, sizeof(x), 0);
    if (x < 0)
    {
        x = -x;
    }
    x %= width;
    getrandom(&y, sizeof(y), 0);
    if (y < 0)
    {
        y = -y;
    }
    y %= height;
    push(x, y, s); // on commence par une case aléatoire
    set_true(annexe, x, y);
    while (!isempty_stack(s))
    {
        pop(s, &x, &y);
        int dir[4] = {0, 1, 2, 3};
        unsigned int r;
        for (int i = 0; i < 4; i++)
        {
            // on mélange les directions
            getrandom(&r, sizeof(r), 0);
            r %= 4;
            const int tmp = dir[i];
            dir[i] = dir[r];
            dir[r] = tmp;
        }
        // on regarde les cases adjacentes (dans un ordre aléatoire)
        for (int i = 0; i < 4; i++)
        {
            if (dir[i] == 0)
            {
                if (y > 0 && !get_bool(annexe, x, y - 1)) // si la case n'a pas été visitée
                {
                    unwall_up(maze, x, y); // on crée un chemin
                    push(x, y - 1, s); // on poursuit la visite
                    set_true(annexe, x, y - 1); // on marque la case comme visitée
                }
            }
            else if (dir[i] == 1)
            {
                if (y < height - 1 && !get_bool(annexe, x, y + 1))
                {
                    unwall_down(maze, x, y);
                    push(x, y + 1, s);
                    set_true(annexe, x, y + 1);
                }
            }
            else if (dir[i] == 2)
            {
                if (x > 0 && !get_bool(annexe, x - 1, y))
                {
                    unwall_left(maze, x, y);
                    push(x - 1, y, s);
                    set_true(annexe, x - 1, y);
                }
            }
            else
            {
                if (x < width - 1 && !get_bool(annexe, x + 1, y))
                {
                    unwall_right(maze, x, y);
                    push(x + 1, y, s);
                    set_true(annexe, x + 1, y);
                }
            }
        }
    }
    free_stack(s);
    free_booltab(annexe);
    return maze;
}

maze_t octopus_maze(const int width, const int height)
{
    const maze_t maze = create_wall_maze(width, height);
    const bool_tab annexe = create_booltab(width, height);
    int x, y;
    queue* q = create_queue();
    getrandom(&x, sizeof(x), 0);
    if (x < 0)
    {
        x = -x;
    }
    x %= width;
    getrandom(&y, sizeof(y), 0);
    if (y < 0)
    {
        y = -y;
    }
    y %= height;
    enqueue(x, y, q); // on commence par une case aléatoire
    set_true(annexe, x, y);
    while (!isempty_queue(q))
    {
        dequeue(q, &x, &y);
        int dir[4] = {0, 1, 2, 3};
        unsigned int r;
        for (int i = 0; i < 4; i++)
        {
            // on mélange les directions
            getrandom(&r, sizeof(r), 0);
            r %= 4;
            const int tmp = dir[i];
            dir[i] = dir[r];
            dir[r] = tmp;
        }
        // on regarde les cases adjacentes (dans un ordre aléatoire)
        for (int i = 0; i < 4; i++)
        {
            if (dir[i] == 0)
            {
                if (y > 0 && !get_bool(annexe, x, y - 1)) // si la case n'a pas été visitée
                {
                    unwall_up(maze, x, y); // on crée un chemin
                    enqueue(x, y - 1, q); // on poursuit la visite
                    set_true(annexe, x, y - 1); // on marque la case comme visitée
                }
            }
            else if (dir[i] == 1)
            {
                if (y < height - 1 && !get_bool(annexe, x, y + 1))
                {
                    unwall_down(maze, x, y);
                    enqueue(x, y + 1, q);
                    set_true(annexe, x, y + 1);
                }
            }
            else if (dir[i] == 2)
            {
                if (x > 0 && !get_bool(annexe, x - 1, y))
                {
                    unwall_left(maze, x, y);
                    enqueue(x - 1, y, q);
                    set_true(annexe, x - 1, y);
                }
            }
            else
            {
                if (x < width - 1 && !get_bool(annexe, x + 1, y))
                {
                    unwall_right(maze, x, y);
                    enqueue(x + 1, y, q);
                    set_true(annexe, x + 1, y);
                }
            }
        }
    }
    free_queue(q);
    free_booltab(annexe);
    return maze;
}

maze_t my_octopus_maze(const int width, const int height)
{
    const maze_t maze = create_wall_maze(width, height);
    const bool_tab annexe = create_booltab(width, height);
    int x, y;
    queue* q = create_queue();
    stack* s = create_stack();
    getrandom(&x, sizeof(x), 0);
    if (x < 0)
    {
        x = -x;
    }
    x %= width;
    getrandom(&y, sizeof(y), 0);
    if (y < 0)
    {
        y = -y;
    }
    y %= height;
    enqueue(x, y, q); // on commence par une case aléatoire
    int r;
    set_true(annexe, x, y);
    while (!(isempty_queue(q) && isempty_stack(s)))
    {
        while (!isempty_stack(s))
        {
            pop(s, &x, &y);
            enqueue(x, y, q);
        }
        getrandom(&r, sizeof(r), 0);
        r = abs(r) % (size_queue(q) / 2);
        for (int i = 0; i < r; i++)
        {
            dequeue(q, &x, &y);
            push(x, y, s);
        }
        dequeue(q, &x, &y);
        int dir[4] = {0, 1, 2, 3};
        for (int i = 0; i < 4; i++)
        {
            // on mélange les directions
            getrandom(&r, sizeof(r), 0);
            r = abs(r) % 4;
            const int tmp = dir[i];
            dir[i] = dir[r];
            dir[r] = tmp;
        }
        // on regarde les cases adjacentes (dans un ordre aléatoire)
        for (int i = 0; i < 4; i++)
        {
            if (dir[i] == 0)
            {
                if (y > 0 && !get_bool(annexe, x, y - 1)) // si la case n'a pas été visitée
                {
                    unwall_up(maze, x, y); // on crée un chemin
                    enqueue(x, y - 1, q); // on poursuit la visite
                    set_true(annexe, x, y - 1); // on marque la case comme visitée
                }
            }
            else if (dir[i] == 1)
            {
                if (y < height - 1 && !get_bool(annexe, x, y + 1))
                {
                    unwall_down(maze, x, y);
                    enqueue(x, y + 1, q);
                    set_true(annexe, x, y + 1);
                }
            }
            else if (dir[i] == 2)
            {
                if (x > 0 && !get_bool(annexe, x - 1, y))
                {
                    unwall_left(maze, x, y);
                    enqueue(x - 1, y, q);
                    set_true(annexe, x - 1, y);
                }
            }
            else
            {
                if (x < width - 1 && !get_bool(annexe, x + 1, y))
                {
                    unwall_right(maze, x, y);
                    enqueue(x + 1, y, q);
                    set_true(annexe, x + 1, y);
                }
            }
        }
    }
    free_queue(q);
    free_booltab(annexe);
    return maze;
}

maze_t comb_maze(const int width, const int height)
{
    if (height == 1 || width == 1)
    {
        return create_basic_maze(width, height);
    }
    const maze_t maze = create_wall_maze(width, height);
    const bool_tab visited = create_booltab(width, height);

    // création du chemin principal
    int x = 0;
    int y = 0;
    while (x != width - 1 || y != height - 1)
    {
        if (y == height - 1)
        { // NOLINT(*-branch-clone)
            unwall_right(maze, x, y);
            set_true(visited, x, y);
            x++;
        }
        else if (x == width - 1 || rand() % 2 == 0) // NOLINT(*-msc50-cpp)
        {
            unwall_down(maze, x, y);
            set_true(visited, x, y);
            y++;
        }
        else
        {
            unwall_right(maze, x, y);
            set_true(visited, x, y);
            x++;
        }
    }
    set_true(visited, width - 1, height - 1);
    // création des embranchements
    bool is_done = false;
    while (!is_done)
    {
        is_done = true;
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                if (!get_bool(visited, i, j))
                {
                    x = i;
                    y = j;
                    is_done = false;
                    while (!get_bool(visited, x, y))
                    {
                        if (rand() % 2 == 0 && i > 0) // NOLINT(*-msc50-cpp)
                        {
                            unwall_left(maze, x, y);
                            set_true(visited, x, y);
                            x--;
                        }
                        else if (j > 0)
                        {
                            unwall_up(maze, x, y);
                            set_true(visited, x, y);
                            y--;
                        }
                    }
                }
            }
        }
    }
    free_booltab(visited);
    return maze;
}

// Prototypes of mazemaker //TODO: to be completed and added to the header file
maze_t proto_comb_maze(const int width, const int height)
{
    const maze_t maze = create_wall_maze(width, height);
    const bool_tab visited = create_booltab(width, height);

    // initialisation à la case (0, 0)
    int x = 0;
    int y = 0;
    set_true(visited, width - 1, height - 1);
    bool end = false;
    while (!end)
    {
        while (!get_bool(visited, x, y))
        {
            const bool right = x < width - 1;
            const bool down = y < height - 1;
            set_true(visited, x, y);
            if (!right) // on ne peut que descendre
            { // NOLINT(*-branch-clone)
                unwall_down(maze, x, y);
                y++;
                set_true(visited, x, y);
            }
            else if (!down || rand() % 2 == 1) // on ne peut que aller à droite ou on le tire aléatoirement NOLINT(*-msc50-cpp)
            {
                unwall_right(maze, x, y);
                x++;
                set_true(visited, x, y);
            }
            else // on a tiré au sort d'aller en bas
            {
                unwall_down(maze, x, y);
                y++;
                set_true(visited, x, y);
            }
        }
        // on cherche une case non visitée pour continuer
        end = true;
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; j++)
            {
                if (!get_bool(visited, i, j))
                {
                    x = i;
                    y = j;
                    end = false;
                    break;
                }
            }
            if (!end)
                break;
        }
    }
    for (int i = 0; i < maze.width; i++)
    {
        for (int j = 0; j < maze.height; j++)
        {
            set_false(visited, i, j);
        }
    }
    int r;
    int t = maze.height * maze.width - 1;
    set_true(visited, 0, 0);
    t -= set_connexion(maze, visited, x, y);
    bool dir[4];
    while (t > 0)
    {
        dir[0] = false, dir[1] = false, dir[2] = false, dir[3] = false;
        do
        {
            getrandom(&r, sizeof(r), 0);
            r %= maze.width * maze.height;
            x = abs(r) % maze.width;
            y = abs(r) / maze.width;
        }
        while (get_bool(visited, x, y));

        if (x > 0 && get_bool(visited, x - 1, y))
        {
            dir[0] = true;
        }
        if (x < maze.width - 1 && get_bool(visited, x + 1, y))
        {
            dir[1] = true;
        }
        if (y > 0 && get_bool(visited, x, y - 1))
        {
            dir[2] = true;
        }
        if (y < maze.height - 1 && get_bool(visited, x, y + 1))
        {
            dir[3] = true;
        }
        if (!dir[0] && !dir[1] && !dir[2] && !dir[3])
        {
            continue;
        }
        getrandom(&r, sizeof(r), 0);
        r = abs(r) % 4;
        while (!dir[r])
        {
            r = (r + 1) % 4;
        }
        if (r == 0)
        {
            unwall_left(maze, x, y);
            t -= set_connexion(maze, visited, x, y);
        }
        else if (r == 1)
        {
            unwall_right(maze, x, y);
            t -= set_connexion(maze, visited, x, y);
        }
        else if (r == 2)
        {
            unwall_up(maze, x, y);
            t -= set_connexion(maze, visited, x, y);
        }
        else
        {
            unwall_down(maze, x, y);
            t -= set_connexion(maze, visited, x, y);
        }
    }
    free_booltab(visited);
    return maze;
}

maze_t hunt_kill_maze(const int width, const int height)
{
    if (height == 1 || width == 1) // si le labyrinthe n'a qu'une dimension, il est parfait
    {
        return create_basic_maze(width, height);
    }
    // INITIALISATION
    // Créer un labyrithe avec tous les murs fermés
    // width : largeur du labyrinthe
    // height : hauteur du labyrinthe
    const maze_t maze = create_wall_maze(width, height);

    // création d'un tableau repertoriant si une cellule a été visitée
    const bool_tab visited = create_booltab(width, height);

    // choisir une cellule aléatoire
    int x, y;
    getrandom(&x, sizeof(x), 0), getrandom(&y, sizeof(y), 0);
    x = abs(x) % width, y = abs(y) % height;
    // cette cellule est visitée
    set_true(visited, x, y);

    while (true)
    {
        // tant que toutes les cellules n'ont pas été visitées
        // BOUCLE KILL
        // détruire un mur aléatoire tant que toutes les cellules voisines n'ont pas été visitées pour créer un long
        // couloir/chemin
        int size = 4;
        char c = '\0';
        while (size > 0)
        { // tant qu'il reste des directions possibles
            char dir[4] = {'R', 'D', 'L', 'U'}; // tableau des directions possibles
            size = 4;
            while (size > 0) // tant qu'il reste des directions possibles et qu'on n'en a pas tiré de valide
            {
                const int r = rand() % size; // NOLINT(*-msc50-cpp)
                c = dir[r];
                if (c == 'R' && x + 1 < width && !get_bool(visited, x + 1, y)) // si on a tiré la direction droite et qu'on peut y aller
                {
                    unwall_right(maze, x, y); // on détruit le mur
                    x++; // on se déplace
                    break;
                }
                if (c == 'L' && x > 0 && !get_bool(visited, x - 1, y)) // si on a tiré la direction gauche et qu'on peut y aller
                {
                    unwall_left(maze, x, y); // on détruit le mur
                    x--; // on se déplace
                    break;
                }
                if (c == 'D' && y + 1 < height && !get_bool(visited, x, y + 1)) // si on a tiré la direction bas et qu'on peut y aller
                {
                    unwall_down(maze, x, y); // on détruit le mur
                    y++; // on se déplace
                    break;
                }
                if (y > 0 && !get_bool(visited, x,
                                       y - 1)) // il ne reste plus que la direction haut, on regarde si on peut y aller
                {
                    unwall_up(maze, x, y); // on détruit le mur
                    y--; // on se déplace
                    break;
                }
                // sinon on supprime la direction prise du tableau
                {
                    for (int i = r; i < size - 1; i++)
                    {
                        dir[i] = dir[i + 1];
                    }
                    size--;
                }
            }
            set_true(visited, x, y);
        }

        // sinon on cherche une cellule non visitée adjacente à une cellule visitée
        // BOUCLE HUNT
        // find_hunt
        bool run = false;
        for (int i = 0; i < height; i++)
        {
            if (run)
                break;
            for (int j = 0; j < width; j++)
            {
                if (!get_bool(visited, j, i))
                {
                    if (j + 1 < width && get_bool(visited, j + 1, i))
                    {
                        x = j;
                        y = i;
                        run = true;
                        break;
                    }
                    if (i + 1 < height && get_bool(visited, j, i + 1))
                    {
                        x = j;
                        y = i;
                        run = true;
                        break;
                    }
                    if (j > 1 && get_bool(visited, j - 1, i))
                    {
                        x = j;
                        y = i;
                        run = true;
                        break;
                    }
                    if (i > 1 && get_bool(visited, j, i - 1))
                    {
                        x = j;
                        y = i;
                        run = true;
                        break;
                    }
                }
            }
        }
        if (!run)
            break;
        {
            char dir[4] = {0}; // tableau des directions possibles
            size = 0;
            if (x + 1 < width && get_bool(visited, x + 1, y))
            {
                dir[size] = 'R';
                size++;
            }
            if (y + 1 < height && get_bool(visited, x, y + 1))
            {
                dir[size] = 'D';
                size++;
            }
            if (x > 0 && get_bool(visited, x - 1, y))
            {
                dir[size] = 'L';
                size++;
            }
            if (y > 0 && get_bool(visited, x, y - 1))
            {
                dir[size] = 'U';
                size++;
            }
            // chercher une cellule visitée adjacente à notre cellule non visitée
            c = dir[rand() % size]; // NOLINT(*-msc50-cpp)
            if (c == 'R')
            {
                unwall_right(maze, x, y);
            }
            else if (c == 'L')
            {
                unwall_left(maze, x, y);
            }
            else if (c == 'D')
            {
                unwall_down(maze, x, y);
            }
            else if (c == 'U')
            {
                unwall_up(maze, x, y);
            }
            else
            {
                fprintf(stderr, "Erreur: direction hunt invalide\n");
                printf("x : %d, y : %d\n", x, y);
                free_booltab(visited);
                free_maze(maze);
                exit(EXIT_FAILURE);
            }
        }
        set_true(visited, x, y);
    }
    free_booltab(visited);
    return maze;
}


// Auxiliary functions for lab_by_path
// directions

#define CAN_MOVE_LEFT (*x > 0 && !get_bool(tab_visited, *x - 1, *y)) // si nous ne somme pas sur le bord gauche et que la case n'est pas visitée

#define CAN_MOVE_RIGHT (*x + 1 < maze->width && !get_bool(tab_visited, *x + 1, *y)) // si nous ne somme pas sur le bord droit et que la case n'est pas visitée

#define CAN_MOVE_UP (*y > 0 && !get_bool(tab_visited, *x, *y - 1)) // si ne nous somme pas sur le bord haut et que la case n'est pas visitée

#define CAN_MOVE_DOWN (*y + 1 < maze->height && !get_bool(tab_visited, *x, *y + 1)) // si nous ne somme pas sur le bord bas et que la case n'est pas visitée

static int can_move_dir(const maze_t* maze, int* x, int* y, const bool_tab tab_visited, int dir)
{
    switch (dir)
    {
    case 0:
        return CAN_MOVE_LEFT;
    case 1:
        return CAN_MOVE_RIGHT;
    case 2:
        return CAN_MOVE_UP;
    case 3:
        return CAN_MOVE_DOWN;
    default:
        fprintf(stderr, "Problem in the switch case in lbp_path_move");
        exit(EXIT_FAILURE);
    }
}

// Auxiliary functions for lab_by_path
// To apply a random move to the current cell
bool lbp_path_move(const maze_t* maze, int* x, int* y, const bool_tab tab_visited)
{
    if (!(CAN_MOVE_LEFT || CAN_MOVE_RIGHT || CAN_MOVE_UP || CAN_MOVE_DOWN))
    {
        return false;
    }
    // tant qu'il nous reste des directions
    int choice = rand() % 4; // choix d'une direction NOLINT(*-msc50-cpp)
    while (!can_move_dir(maze, x, y, tab_visited, choice)) // si notre direction n'est pas possible, on passe à la suivante
        choice = rand() % (4);
    switch (choice)
    {
    case 0:
        (*x)--;
        set_true(tab_visited, *x, *y);
        return true;

    case 1:
        (*x)++;
        set_true(tab_visited, *x, *y);
        return true;

    case 2:
        (*y)--;
        set_true(tab_visited, *x, *y);
        return true;

    case 3:
        (*y)++;
        set_true(tab_visited, *x, *y);
        return true;

    default:
        fprintf(stderr, "Problem in the switch case in lbp_path_move");
        exit(EXIT_FAILURE);
    }

    return false;
}
// Auxiliary functions for lab_by_path
// function to create a path in the maze by adding walls
void lbp_path(maze_t* maze, int* x, int* y, int* x_2, int* y_2, const bool_tab tab_visited)
{
    set_true(tab_visited, *x, *y);

    if (!CAN_MOVE_UP && !has_wall_up(*maze, *x, *y) && *y != *y_2 + 1)
    {
        wall_up(*maze, *x, *y);
    }

    if (!CAN_MOVE_DOWN && !has_wall_down(*maze, *x, *y) && *y != *y_2 - 1)
    {
        wall_down(*maze, *x, *y);
    }
    if (!CAN_MOVE_LEFT && !has_wall_left(*maze, *x, *y) && *x != *x_2 + 1)
    {
        wall_left(*maze, *x, *y);
    }
    if (!CAN_MOVE_RIGHT && !has_wall_right(*maze, *x, *y) && *x != *x_2 - 1)
    {
        wall_right(*maze, *x, *y);
    }
    *x_2 = *x, *y_2 = *y;
    if (lbp_path_move(maze, x, y, tab_visited)) // mouvement
        lbp_path(maze, x, y, x_2, y_2, tab_visited);
}

maze_t by_path_maze(const int width, const int height)
{
    maze_t maze = create_basic_maze(width, height);

    if (width == 1 || height == 1) // si le labyrinthe est une simple cellule, une ligne ou une collone, on la retourne
        return maze;

    const bool_tab tab_visited = create_booltab(width, height);
    int x_1 = 0, x_2 = 0, y_1 = 0, y_2 = 0; // création des coordonnées et coordonnées de la case précédente
    set_true(tab_visited, x_1, x_2); // on valide notre passage sur la case de départ

    for (int y = 0; y < height - 1; y++)
    {
        for (int x = 0; x < width - 1; x++)
        {
            if (get_bool(tab_visited, x, y))
            {
                x_1 = x, y_1 = y, x_2 = x, y_2 = y;
                if (!get_bool(tab_visited, x + 1, y))
                { // si la case de droite est nouvelle, on y va depuis notre case actuelle
                    x_1++;
                    lbp_path(&maze, &x_1, &y_1, &x_2, &y_2, tab_visited);
                }

                else if (!get_bool(tab_visited, x, y + 1))
                { // si la case du bas est nouvelle, on y va depuis notre case actuelle
                    y_1++;
                    lbp_path(&maze, &x_1, &y_1, &x_2, &y_2, tab_visited);
                }
            }
        }
    }
    if (!get_bool(tab_visited, width - 1, height - 1))
    { // si on n'est jamais passé par la sortie, alors on passe
        if (rand() % 2) // NOLINT(*-msc50-cpp)
            wall_up(maze, width - 1, height - 1);
        else
            wall_left(maze, width - 1, height - 1);
    }
    free_booltab(tab_visited);
    return maze;
}

maze_t cross_maze(const int width, const int height)
{
    if (height == 1 || width == 1)
    {
        return create_basic_maze(width, height);
    }
    const maze_t maze = create_wall_maze(width, height);
    const bool_tab annexe = create_booltab(width, height); // tableau de booléens pour savoir si une case a été traitée
    int t = width * height; // nombre de cases à traiter
    // on commence par crréer des chemins (en forme d'étoiles)
    int r;
    while (t > 0)
    {
        do
        {
            getrandom(&r, sizeof(r), 0);
            r = abs(r) % (width * height);
        }
        while (get_bool(annexe, r % width, r / width));
        const int x = abs(r) % width;
        const int y = abs(r) / width;
        set_true(annexe, x, y);
        t--;
        // on ouvre les murs
        if (y > 0 && !get_bool(annexe, x, y - 1)) // si la case adjacente n'a pas été traitée
        {
            unwall_up(maze, x, y);
            set_true(annexe, x, y - 1);
            t--;
        }
        if (y < height - 1 && !get_bool(annexe, x, y + 1)) // si la case adjacente n'a pas été traitée
        {
            unwall_down(maze, x, y);
            set_true(annexe, x, y + 1);
            t--;
        }
        if (x > 0 && !get_bool(annexe, x - 1, y)) // si la case adjacente n'a pas été traitée
        {
            unwall_left(maze, x, y);
            set_true(annexe, x - 1, y);
            t--;
        }
        if (x < width - 1 && !get_bool(annexe, x + 1, y)) // si la case adjacente n'a pas été traitée
        {
            unwall_right(maze, x, y);
            set_true(annexe, x + 1, y);
            t--;
        }
    }
    for (int i = 0; i < maze.height; i++)
    {
        for (int j = 0; j < maze.width; j++)
        {
            set_false(annexe, j, i); // on réinitialise le tableau
        }
    }
    // on va connexter les cases non connexes
    int x, y;
    getrandom(&x, sizeof(x), 0);
    getrandom(&y, sizeof(y), 0);
    x = abs(x) % width;
    y = abs(y) % height;
    set_true(annexe, x, y); // on marque la case comme celle de départ
    t = width * height - 1; // nombre de cases à traiter
    if (!has_wall_down(maze, x, y))
    {
        t -= set_connexion(maze, annexe, x, y + 1); // on marque la case adjacente comme connexe
    }
    if (!has_wall_up(maze, x, y))
    {
        t -= set_connexion(maze, annexe, x, y - 1); // on marque la case adjacente comme connexe
    }
    if (!has_wall_left(maze, x, y))
    {
        t -= set_connexion(maze, annexe, x - 1, y); // on marque la case adjacente comme connexe
    }
    if (!has_wall_right(maze, x, y))
    {
        t -= set_connexion(maze, annexe, x + 1, y); // on marque la case adjacente comme connexe
    }
    while (t > 0) // tant que toutes les cases ne sont pas connexes
    {
        do
        {
            getrandom(&r, sizeof(r), 0);
            r = abs(r) % (width * height);
        }
        while (get_bool(annexe, r % width, r / width));
        x = abs(r) % width;
        y = abs(r) / width;
        // on regarde les directions possibles
        int size = 0;
        bool dir[3] = {false, false, false}; // tableau des directions possibles (la 4eme est assuré par les 3 autres et le size)
        if (x > 0 && get_bool(annexe, x - 1, y)) // si la case adjacente est connexe
        {
            size++;
            dir[0] = true;
        }
        if (x < width - 1 && get_bool(annexe, x + 1, y)) // si la case adjacente est connexe
        {
            size++;
            dir[1] = true;
        }
        if (y > 0 && get_bool(annexe, x, y - 1)) // si la case adjacente est connexe
        {
            size++;
            dir[2] = true;
        }
        if (y < height - 1 && get_bool(annexe, x, y + 1)) // si la case adjacente est connexe
        {
            size++;
        }
        if (size == 0) // si la case n'est pas adjacente à une case visitée
        {
            continue; // on passe à la case suivante
        }
        getrandom(&r, sizeof(r), 0);
        r = abs(r) % size;
        if (r == 0) // la première direction peut être n'importe laquelle
        {
            if (dir[0]) // on regarde si la direction est possible
            {
                unwall_left(maze, x, y);
                t -= set_connexion(maze, annexe, x, y);
            }
            else if (dir[1]) // on regarde si la direction est possible
            {
                unwall_right(maze, x, y);
                t -= set_connexion(maze, annexe, x, y);
            }
            else if (dir[2]) // on regarde si la direction est possible
            {
                unwall_up(maze, x, y);
                t -= set_connexion(maze, annexe, x, y);
            }
            else // la quatrième direction est la seule restante
            {
                unwall_down(maze, x, y);
                t -= set_connexion(maze, annexe, x, y);
            }
        }
        else if (r == 1) // la deuxième direction ne peut pas être la 0 (qui est toujours première)
        {
            if (dir[1]) // on regarde si la direction est possible
            {
                unwall_right(maze, x, y);
                t -= set_connexion(maze, annexe, x, y);
            }
            else if (dir[2]) // on regarde si la direction est possible
            {
                unwall_up(maze, x, y);
                t -= set_connexion(maze, annexe, x, y);
            }
            else // la quatrième direction est la seule restante
            {
                unwall_down(maze, x, y);
                t -= set_connexion(maze, annexe, x, y);
            }
        }
        else if (r == 2) // la troisième direction ne peut pas être la 0 ou la 1
        {
            if (dir[2]) // on regarde si la direction est possible
            {
                unwall_up(maze, x, y);
                t -= set_connexion(maze, annexe, x, y);
            }
            else // la quatrième direction est la seule restante
            {
                unwall_down(maze, x, y);
                t -= set_connexion(maze, annexe, x, y);
            }
        }
        else // la quatrième direction est la seule restante
        {
            unwall_down(maze, x, y);
            t -= set_connexion(maze, annexe, x, y);
        }
    }
    free_booltab(annexe);
    return maze;
}

void tear(const maze_t maze, const unsigned int prop)
{
    if (prop == 0)
    {
        return;
    }
    if (prop > 100)
    {
        fprintf(stderr, "Erreur: la probabilité de déchirure doit être comprise entre 0 et 100, soumis : %d\n", prop);
        printf("sortie de tear sans intervention\n");
        return;
    }
    short unsigned int r;
    for (int i = 0; i < maze.width; i++)
    {
        for (int j = 0; j < maze.height; j++)
        {
            getrandom(&r, sizeof(r), 0);
            const bool tear = (r % 100) < prop;
            const bool wd = has_wall_down(maze, i, j) && j < maze.height - 1;
            const bool wr = has_wall_right(maze, i, j) && i < maze.width - 1;
            if (tear && wd && wr)
            {
                if (r % 2 == 0)
                {
                    unwall_down(maze, i, j);
                }
                else
                {
                    unwall_right(maze, i, j);
                }
            }
            else if (tear && wd)
            {
                unwall_down(maze, i, j);
            }
            else if (tear && wr)
            {
                unwall_right(maze, i, j);
            }
        }
    }
}
