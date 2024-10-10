#include "mazemaker.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#define NB_MAZE_GENERATOR 5
#define NB_PERFECT_MAZE_GENERATOR 4
#define NB_IMPERFECT_MAZE_GENERATOR 1

//Crée un labyrinthe parfait de taille width x height
//tous les murs sont verticaux
maze_t line_maze(const int width, const int height)
{
    const time_t t = time(NULL);
    srand(t);
    const maze_t maze = create_basic_maze(width, height);
    for(int i = 0; i < width - 1; i++)
    {
        const int break_wall = rand() % height;
        for(int j = 0; j < height; j++)
        {
            if(j != break_wall)
            {
                wall_right(maze, i, j);
            }
        }
    }
    return maze;
}

//Crée un labyrinthe parfait de taille width x height
//tous les murs sont horizontaux
maze_t column_maze(const int width, const int height)
{
    const time_t t = time(NULL);
    srand(t);
    const maze_t maze = create_basic_maze(width, height);
    for(int i = 0; i < height - 1; i++)
    {
        const int break_wall = rand() % width;
        for(int j = 0; j < width; j++)
        {
            if(j != break_wall)
            {
                wall_down(maze, j, i);
            }
        }
    }
    return maze;
}

//Crée un labyrinthe imparfait de taille width x height
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
//seuls le chemins de sortie possèdent des embranchements
maze_t imperfect_one_way_maze(const int width, const int height)
{
    const maze_t maze = create_wall_maze(width, height);
    const bool_tab visited = create_booltab(width, height);
    const time_t t = time(NULL);
    srand(t);
    int c = 0;
    int x = 0;
    int y = 0;
    while(!(x == width - 1 && y == height - 1))
    {
        if(c % 2 == 0)
        {
            const int next_x = (rand() % (width - x)) + x;
            for (int i = x; i < next_x; i++)
            {
                unwall_right(maze, i, y);
                set_true(visited, i, y);
            }
            x = next_x;
            c++;
        }
        else
        {
            const int next_y = (rand() % (height - y)) + y;
            for (int i = y; i < next_y; i++)
            {
                unwall_down(maze, x, i);
                set_true(visited, x, i);
            }
            y = next_y;
            c++;
        }
    }
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            if(!get_bool(visited, i, j))
            {
                const int direction = rand() % 4;
                if(direction == 0 && i > 0)
                {
                    unwall_left(maze, i, j);
                }
                else if(direction == 1 && i < width - 1)
                {
                    unwall_right(maze, i, j);
                }
                else if(direction == 2 && j > 0)
                {
                    unwall_up(maze, i, j);
                }
                else if(direction == 3 && j < height - 1)
                {
                    unwall_down(maze, i, j);
                }
            }
        }
    }
    free_booltab(visited);
    return maze;
}

//Crée un labyrinthe parfait de taille width x height
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
//seuls le chemins de sortie possèdent des embranchements
maze_t perfect_one_way_maze(const int width, const int height)
{
    const maze_t maze = create_wall_maze(width, height);
    const bool_tab visited = create_booltab(width, height);
    const time_t t = time(NULL);
    srand(t);
    int x = 0;
    int y = 0;
    while(!(x == width - 1 && y == height - 1))
    {
        if(rand() % 2 == 0 || y == height - 1)
        {
            const int next_x = (rand() % (width - x)) + x;
            for (int i = x; i < next_x; i++)
            {
                unwall_right(maze, i, y);
                set_true(visited, i, y);
            }
            x = next_x;
        }
        else
        {
            const int next_y = (rand() % (height - y)) + y;
            for (int i = y; i < next_y; i++)
            {
                unwall_down(maze, x, i);
                set_true(visited, x, i);
            }
            y = next_y;
        }
    }
    set_true(visited, width - 1, height - 1);
    bool is_done = false;
    while(!is_done)
    {
        is_done = true;
        for(int i = 0; i < width; i++)
        {
            for(int j = 0; j < height; j++)
            {
                if(!get_bool(visited, i, j))
                {
                    is_done = false;
                    if(j < height - 1 && get_bool(visited, i, j + 1))
                    {
                        unwall_down(maze, i, j);
                        set_true(visited, i, j);
                    }
                    else if(j > 0 && get_bool(visited, i, j - 1))
                    {
                        unwall_up(maze, i, j);
                        set_true(visited, i, j);
                    }
                    else if(i < width - 1 && get_bool(visited, i + 1, j))
                    {
                        unwall_right(maze, i, j);
                        set_true(visited, i, j);
                    }
                    else if(i > 0 && get_bool(visited, i - 1, j))
                    {
                        unwall_left(maze, i, j);
                        set_true(visited, i, j);
                    }
                }
            }
        }
    }
    free_booltab(visited);
    return maze;
}

//FONCTION AUXILIAIRE (hunt_kill_maze)
//fonction qui permet de trouver une cellule non visitée adjacente à une cellule visitée
//modifie les coordonnées de la cellule non visitée et retourne true si une cellule a été trouvée, false sinon
bool finding_hunt(const int width, const int height, const bool_tab visited, int *px, int *py){
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(!get_bool(visited, i, j))
            {
                if(j+1 < width && get_bool(visited, i, j+1)){
                    *px = j;
                    *py = i;
                    return true;
                }
                if(i+1 < height && get_bool(visited, i + 1, j)){
                    *px = j;
                    *py = i;
                    return true;
                }
                if(j > 2 && get_bool(visited, i, j - 1)){
                    *px = j;
                    *py = i;
                    return true;
                }
                if(i > 2 && get_bool(visited, i-  1, j)){
                    *px = j;
                    *py = i;
                    return true;
                }
            }
        }
    }
    return false;
}

//d'après la méthode du hunt and kill
//Crée un labyrinthe parfait de taille width x height
maze_t hunt_kill_maze(const int width, const int height)
{
    //INITIALISATION
    const time_t t = time(NULL);
    srand(t);
    //Créer un labyrithe avec tous les murs fermés
    //width : largeur du labyrinthe
    //height : hauteur du labyrinthe
    const maze_t maze = create_wall_maze(width, height);

    //création d'un tableau repertoriant si une cellule a été visitée
    const bool_tab visited = create_booltab(width, height);

    //choisir une cellule aléatoire
    int x = rand() % width;
    int y = rand() % height;
    //cette cellule est visitée
    set_true(visited, x, y);

    int end = 0;
    int *px = &x;
    int *py = &y;

    while(end == 0)
    {
        //tant que toutes les cellules n'ont pas été visitées
        //BOUCLE KILL
        //détruire un mur aléatoire tant que toutes les cellules voisines n'ont pas été visitées
        int size = 4;
        char c = '\0';
        while(size > 0){
            char dir[4] = {'R', 'D', 'L', 'U'}; //tableau des directions possibles
            size = 4;
            c = '\0';
            while(c == '\0' && size > 0)
            {
                const int r = rand() % size;
                c = dir[r];
                if (c == 'R' && x + 1 < width && !get_bool(visited, x + 1, y))
                {
                    unwall_right(maze, x, y);
                    x++;
                }
                else if(c == 'L' && x > 0 && !get_bool(visited, x - 1, y))
                {
                    unwall_left(maze, x, y);
                    x--;
                }
                else if(c == 'D' && y + 1 < height && !get_bool(visited, x, y + 1))
                {
                    unwall_down(maze, x, y);
                    y++;
                }
                else if(c == 'U' && y > 0 && !get_bool(visited, x, y - 1))
                {
                    unwall_up(maze, x, y);
                    y--;
                }
                else
                {
                    for (int i = r; i < size - 1; i++){
                        dir[i] = dir[i+1];
                        c = '\0';
                    }
                    size--;
                }
            }
            set_true(visited, x, y);
        }

        //sinon on cherche une cellule non visitée adjacente à une cellule visitée
        //BOUCLE HUNT
        if(!finding_hunt(width, height, visited, px, py)){
            end = 1;
        }
        else{
            char dir[4] = {'R', 'D', 'L', 'U'}; //tableau des directions possibles
            //chercher une cellule non visitée adjacente à une cellule visitée
            c = '\0';
            size = 4;
            while(c == '\0')
            {
                const int r = rand() % size;
                c = dir[r];
                if (c == 'R' && x + 1 < width && get_bool(visited, x + 1, y))
                {
                    unwall_right(maze, x, y);
                }
                else if(c == 'L' && x > 0 && get_bool(visited, x - 1, y))
                {
                    unwall_left(maze, x, y);
                }
                else if(c == 'D' && y + 1 < height && get_bool(visited, x, y + 1))
                {
                    unwall_down(maze, x, y);
                }
                else if(c == 'U' && y > 0 && get_bool(visited, x, y - 1))
                {
                    unwall_up(maze, x, y);
                }
                else
                {
                    for (int i = r; i < size; i++){
                        dir[i] = dir[i+1];
                    }
                    size--;
                    c = '\0';
                    if(size == 0){
                        fprintf(stderr, "Erreur: direction hunt invalide\n");
                        printf("x : %d, y : %d\n", x, y);
                        print_maze(maze, "maze");
                        free_booltab(visited);
                        free_maze(maze);
                        exit(1);
                    }
                }
            }
            set_true(visited, x, y);
        }
    }
    free_booltab(visited);
    return maze;
}

//Crée un labyrinthe à l'aide d'une méthode existante chosie aléatoirement
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
maze_t rmaze(const int width, const int height)
{
    const int choice = rand() % NB_MAZE_GENERATOR;
    if(choice == 0)
    {
        return line_maze(width, height);
    }
    else if(choice == 1)
    {
        return column_maze(width, height);
    }
    else if(choice == 2)
    {
        return imperfect_one_way_maze(width, height);
    }
    else if(choice == 3)
    {
        return perfect_one_way_maze(width, height);
    }
    else
    {
        return hunt_kill_maze(width, height);
    }
}

//Crée un labyrinthe parfait de taille width x height à l'aide d'une méthode existante chosie aléatoirement
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
maze_t rperfect_maze(const int width, const int height)
{
    const int choice = rand() % NB_PERFECT_MAZE_GENERATOR;
    if(choice == 0)
    {
        return line_maze(width, height);
    }
    else if(choice == 1)
    {
        return column_maze(width, height);
    }
    else if(choice == 2)
    {
        return perfect_one_way_maze(width, height);
    }
    return hunt_kill_maze(width, height);
}

//Crée un labyrinthe imparfait de taille width x height à l'aide d'une méthode existante chosie aléatoirement
//width : largeur du labyrinthe
//height : hauteur du labyrinthe
maze_t rimperfect_maze(const int width, const int height)
{
    const int choice = rand() % NB_IMPERFECT_MAZE_GENERATOR;
    if(choice == 0)
    {
        return imperfect_one_way_maze(width, height);
    }
    return create_basic_maze(width, height);
}

