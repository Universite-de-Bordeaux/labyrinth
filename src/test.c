#include "mazemaker.h"
#include "test.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


//FONCTION UTILISEES POUR LA GENERATION DE LABYRINTHE HUNT AND KILL
//fonction qui permet de s'assurer qu'on peut encore avancer dans la partie KILL de l'algo hunt and kill
//retourne true si on peut avancer (si on a des cases voisines non visité), false sinon
bool walk_possible(maze_t maze, const int x, const int y, const int width, const int height , const bool_tab visited){
    if (x+1 < width && !visited.tab[x+1][y]){
        return true;
    }
    if (y+1 < height && !visited.tab[x][y+1]){
        return true;
    }
    if (x > 2 && !visited.tab[x-1][y]){
        return true;
    }
    if (y > 2 && !visited.tab[x][y-1]){
        return true;
    }
    return false;
}

//fonction qui permet de s'assurer que toutes les cellules ont été visitées
//retourne true si toutes les cellules ont été visitées, false sinon
bool all_visited(int width, int height, bool_tab visited){
    for(int i = 0; i < width ; i++){
        for(int j = 0; j < height; j++){
            if (!visited.tab[i][j]){
                return false;
            }
        }
    }
    return true;
}

//fonction qui retourne une direction aléatoire possible vers une cellule non visitée
//retourne la direction sous le format d'un charactère
char rand_dir(int x, int y, int width, int height, bool_tab visited, int size, int dir_x[size], int dir_y[size], char dir[size]){
    int r = rand() % size;
    int new_x = x + dir_x[r];
    int new_y = y + dir_y[r];
    //la case designée par la direction random est possible
    if (new_x >= 0 && new_x < width && new_y >= 0 && new_y < height && !visited.tab[new_x][new_y]){
        printf("direction : %c\n", dir[r]);
        return dir[r];
    }
    //sinon on enlève la direction de la liste des directions possibles
    else {
        for (int i = r; i < size - 1; i++){
            dir[i] = dir[i+1];
            dir_x[i] = dir_x[i+1];
            dir_y[i] = dir_y[i+1];
        }
        //on rappelle la fonction avec une taille de tableau réduite
        return rand_dir(x, y, width, height, visited, size-1, dir_x, dir_y, dir);
    }
}

//fonction qui retourne une direction aléatoire possible vers une cellule visitée
//retourne la direction sous le format d'un charactère
char rand_dir_hunt(int x, int y, int width, int height, bool_tab visited, int size, int dir_x[size], int dir_y[size], char dir[size]){
    int r = rand() % size;
    //la case designée par la direction random est possible et a déjà été visitée
    if ((x+dir_x[r]>=0) && (x+dir_x[r]<width) && (y+dir_y[r]>=0) && (y+dir_y[r]<height) && visited.tab[x+dir_x[r]][y+dir_y[r]]){
        return dir[r];
    }
    //sinon on enlève la direction de la liste des directions possibles
    else {
        for (int i =r; i<=size; i++){
            dir[i] = dir[i+1];
            dir_x[i] = dir_x[i+1];
            dir_y[i] = dir_y[i+1];
        }
        return rand_dir_hunt(x, y, width, height, visited, size-1, dir_x, dir_y, dir);
    }
}

//FONCTION
//fonction qui permet de trouver une cellule non visitée adjacente à une cellule visitée
//modifie les coordonnées de la cellule non visitée et retourne true si une cellule a été trouvée, false sinon
bool finding_hunt(const int width, const int height, bool_tab visited, int *px, int *py){
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(!visited.tab[j][i])
            {
                if(j+1 < width && visited.tab[j+1][i]){
                    *px = j;
                    *py = i;
                    return true;
                }
                if(i+1 < height && visited.tab[j][i+1]){
                    *px = j;
                    *py = i;
                    return true;
                }
                if(j > 2 && visited.tab[j-1][i]){
                    *px = j;
                    *py = i;
                    return true;
                }
                if(i > 2 && visited.tab[j][i-1]){
                    *px = j;
                    *py = i;
                    return true;
                }
            }
        }
    }
    return false;
}

//Crée un labyrinthe parfait de taille width x height
//d'après la méthode du hunt and kill
maze_t hunt_kill_maze(const int width, const int height)
{
    const time_t t = time(NULL);
    srand(t);
    //Créer un labyrithe avec tous les murs fermés
    //width : largeur du labyrinthe
    //height : hauteur du labyrinthe
    maze_t maze = create_wall_maze(width, height);

    //création d'un tableau repertoriant si une cellule a été visitée
    bool_tab visited = create_booltab(width, height);

    printf("on a créé le labyrinthe\n");
    //choisir une cellule aléatoire
    int x = rand() % width;
    int y = rand() % height;
    //cette cellule est visitée
    printf("x : %d, y : %d\n", x, y);
    visited.tab[x][y] = true;
    printf("la case [%d][%d] est visitée\n", x, y);

    int end =0;
    int *px =&x;
    int *py =&y;

    while (end==0){ //tant que toutes les cellules n'ont pas été visitées
        //BOUCLE KILL
        //détruire un mur aléatoire tant que toutes les cellules voisines n'ont pas été visitées
        while(walk_possible(maze, x, y, width, height, visited)){
             // tableau des directions possibles (4 choix) (right, down, left, up)
            int dir_x_4[4] = {1, 0, -1, 0};   //tableau des déplacements possibles en x
            int dir_y_4[4] = {0, 1, 0, -1};   //tableau des déplacements possibles en y
            char dir_4[4] = {'R', 'D', 'L', 'U'}; //tableau des directions possibles
            char dir = rand_dir(x, y, width, height, visited, 4, dir_x_4, dir_y_4, dir_4);
            if (dir == 'R'){
                unwall_right(maze, x, y);
                x++;
            }
            else if (dir== 'D'){
                unwall_down(maze, x, y);
                y++;
            }
            else if (dir== 'L'){
                unwall_left(maze, x, y);
                x--;
            }
            else if (dir == 'U'){
                unwall_up(maze, x, y);
                y--;
            }
            else
            {
                fprintf(stderr, "Erreur: direction invalide\n");
                free_maze(maze);
                free_booltab(visited);
                exit(1);
            }
            visited.tab[x][y] = true;
        }
        printf("x : %d, y : %d\n", x, y);
        printf("il n'y a plus de chemin possible\n");
        print_maze(maze, "maze");

        //si toutes les cellules ont été visitées
        if (all_visited (width, height, visited)){
            end =1; //le labyrinthe est terminé
        }

        //sinon on cherche une cellule non visitée adjacente à une cellule visitée
        //BOUCLE HUNT
        else {
            printf ("on part à la chasse\n");
            if (!finding_hunt(width, height, visited, px, py)){
                end = 1;
                break;
            }
            else if (finding_hunt(width, height, visited, px, py)){
                // x = *px;
                // y = *py;
                // tableau des directions possibles (4 choix) (right, down, left, up)
                int dir_x_4[4] = {1, 0, -1, 0};   //tableau des déplacements possibles en x
                int dir_y_4[4] = {0, 1, 0, -1};   //tableau des déplacements possibles en y
                char dir_4[4] = {'R', 'D', 'L', 'U'}; //tableau des directions possibles
                //chercher une cellule non visitée adjacente à une cellule visitée
                char dir = rand_dir_hunt(x, y, width, height, visited, 4, dir_x_4, dir_y_4, dir_4);
                if (dir == 'R'){
                    unwall_right(maze, x, y);
                    x++;
                }
                else if (dir == 'D'){
                    unwall_down(maze, x, y);
                    y++;
                }
                else if (dir == 'L'){
                    unwall_left(maze, x, y);
                    x--;
                }
                else if (dir == 'U'){
                    unwall_up(maze, x, y);
                    y--;
                }
                visited.tab[x][y] = true;
                printf("x : %d, y : %d\n", x, y);
            }
        }
    }
    free_booltab(visited);
    printf("toutes les celules ont ete visitées\n");
    return maze;
}