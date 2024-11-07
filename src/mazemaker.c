#include "mazemaker.h"
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

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

maze_t perfect_one_way_maze(const int width, const int height)
{
    const maze_t maze = create_wall_maze(width, height);
    const bool_tab visited = create_booltab(width, height);
    const time_t t = time(NULL);
    srand(t);

    //création du chemin principal
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

    //création des embranchements
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
                    x = i;
                    y = j;
                    is_done = false;
                    while(!get_bool(visited, x, y))
                    {
                        if(rand() % 2 == 0 && i > 0)
                        {
                            unwall_left(maze, x, y);
                            set_true(visited, x, y);
                            x--;
                        }
                        else if(j > 0)
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

//FONCTION AUXILIAIRE (hunt_kill_maze)
//fonction qui permet de trouver une cellule non visitée adjacente à une cellule visitée
//modifie les coordonnées px et py pour les coordonnées de la cellule non visitée
//retourne true si une cellule non visitée adjacente à une cellule visitée a été trouvée, false sinon
bool finding_hunt(const int width, const int height, const bool_tab visited, int *px, int *py){
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if(!get_bool(visited, j, i))
            {
                if(j + 1 < width && get_bool(visited, j + 1, i)){
                    *px = j;
                    *py = i;
                    return true;
                }
                if(i + 1 < height && get_bool(visited, j, i + 1)){
                    *px = j;
                    *py = i;
                    return true;
                }
                if(j > 1 && get_bool(visited, j - 1, i)){
                    *px = j;
                    *py = i;
                    return true;
                }
                if(i > 1 && get_bool(visited, j, i - 1)){
                    *px = j;
                    *py = i;
                    return true;
                }
            }
        }
    }
    return false;
}

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
            //chercher une cellule visitée adjacente à notre cellule non visitée
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
                        free_booltab(visited);
                        free_maze(maze);
                        exit(EXIT_FAILURE);
                    }
                }
            }
            set_true(visited, x, y);
        }
    }
    free_booltab(visited);
    return maze;
}


//Fonction Auxilliaire de lab_by_path
//fonctions de directions
bool lbp_can_move_left(maze_t *maze, int x, int  y, bool_tab tab_visited){
    if (x-1>=0 && !get_bool(tab_visited, x-1, y)) //si nous ne somme pas sur le bord gauche et que la case n'est pas visitée
            return true;
    return false;
}

bool lbp_can_move_right(maze_t *maze, int x, int y, bool_tab tab_visited){
    if (x+1<maze->width && !get_bool(tab_visited, x+1, y)) // si nous ne somme pas sur le bord droit et que la case n'est pas visitée
        return true;
    return false;
}

bool lbp_can_move_up(maze_t *maze, int x, int y, bool_tab tab_visited){
    if (y-1 >=0 && !get_bool(tab_visited, x, y-1)) // si ne nous somme pas sur le bord haut et que la case n'est pas visitée
            return true;
    return false;
}

bool lbp_can_move_down(maze_t *maze, int x, int y, bool_tab tab_visited){
    if (y+1<maze->height && !get_bool(tab_visited, x, y+1)) // si nous ne somme pas sur le bord bas  et que la case n'est pas visitée
        return true;
    return false;
}

//Fonction Auxilliaire de lab_by_path
//fonction qui applique une direction
bool lbp_path_move(maze_t *maze, int *x, int *y, bool_tab tab_visited){
    bool tab_dir[4] = {}; // créatrion du tableau de possibilité de direction
    int impossible_dir = 0; // compteur de direction en moins
    bool has_moved = false;
    while (impossible_dir <4){ // tant qu'il nous reste des directions
        int choice = rand() %(4-impossible_dir); // choix d'une direction
        while (tab_dir[choice]) // si notre direction n'est pas possible, on passe à la suivante
            choice ++;
        switch (choice){
            case 0:
                if (lbp_can_move_left(maze, *x, *y, tab_visited)){
                    (*x)--;
                    has_moved = true;
                }
                break;
            case 1:
                if (lbp_can_move_right(maze, *x, *y, tab_visited)){
                    (*x)++;
                    has_moved = true;
                }
                break;
            case 2:
                if (lbp_can_move_up(maze, *x, *y, tab_visited)){
                    (*y)--;
                    has_moved = true;
                }
                break;
            case 3:
                if (lbp_can_move_down(maze, *x, *y, tab_visited)){
                    (*y)++;
                    has_moved = true;
                }
                break;
            default:
                fprintf(stderr, "Problem in the switch case in lbp_path_move");
                exit(EXIT_FAILURE);
        }
        if (has_moved){
            set_true(tab_visited, *x, *y);
            return true;
        }
        tab_dir[choice] = true;
        impossible_dir++;
    }
    return false;
}
//Fonction Auxilliaire de lab_by_path
//fonction qui crée tout les chemins et ajoute les murs
void lbp_path(maze_t *maze, int *x, int *y, int *x_2, int *y_2, const bool_tab tab_visited){
    int width = maze -> width, height = maze -> height;
    set_true(tab_visited, *x, *y);

    if (*x==0 && *y == height -1){ // coin bas gauche
        if (*y-1 != *y_2 && get_bool(tab_visited, *x, (*y)-1))
            wall_up(*maze, *x, *y);
        if (*x+1 != *x_2 && get_bool(tab_visited, (*x)+1, *y))
            wall_right(*maze, *x, *y);
    }
    else if (*x==width -1 && *y == height -1){ // coin bas droit
        if ((*y) -1 != *y_2 && get_bool(tab_visited, *x, (*y)-1))
            wall_up(*maze, *x, *y);
        if (*x-1 != *x_2 && get_bool(tab_visited, (*x)-1, *y))
            wall_left(*maze, *x, *y);
    }
    else if (*x==0 && *y == height -1){ // coin haut gauche
        if ((*y) +1 != *y_2 && get_bool(tab_visited, *x, (*y)+1))
            wall_down(*maze, *x, *y);
        if (*x+1 != *x_2 && get_bool(tab_visited, (*x)+1, *y))
            wall_right(*maze, *x, *y);
    }
    else if (*x == width -1 && *y==0){ // coin du haut droit
        if ((*y)+1 != *y_2 && get_bool(tab_visited, *x, *y+1))
            wall_down(*maze, *x, *y);
        if (*x-1 != *x_2 && get_bool(tab_visited, *x-1, *y))
            wall_left(*maze, *x, *y);
    }
    else if (*x == 0){ // colonne gauche
        if (*y-1 != *y_2 && get_bool(tab_visited, *x, *y-1))
            wall_up(*maze, *x, *y);
        if (*y+1 != *y_2 && get_bool(tab_visited, *x, *y+1))
            wall_down(*maze, *x, *y);
        if (*x+1 != *x_2 && get_bool(tab_visited, *x+1, *y))
            wall_right(*maze, *x, *y);
    }
    else if (*x == width -1){ // colonne droite
        if (*y-1 != *y_2 && get_bool(tab_visited, *x, *y-1))
            wall_up(*maze, *x, *y);
        if (*y+1 != *y_2 && get_bool(tab_visited, *x, *y+1))
            wall_down(*maze, *x, *y);
        if (*x-1 != *x_2 && get_bool(tab_visited, *x-1, *y))
            wall_left(*maze, *x, *y);
    }
    else if (*y == 0){ // ligne haut
        if (*y+1 != *y_2 && get_bool(tab_visited, *x, *y+1))
            wall_down(*maze, *x, *y);
        if (*x-1 != *x_2 && get_bool(tab_visited, *x-1, *y))
            wall_left(*maze, *x, *y);
        if (*x+1 != *x_2 && get_bool(tab_visited, *x+1, *y))
            wall_right(*maze, *x, *y);
    }
    else if (*y == height -1){ // ligne bas
        if (*y-1 != *y_2 && get_bool(tab_visited, *x, *y-1))
            wall_up(*maze, *x, *y);
        if (*x-1 != *x_2 && get_bool(tab_visited, *x-1, *y))
            wall_left(*maze, *x, *y);
        if (*x+1 != *x_2 && get_bool(tab_visited, *x+1, *y))
            wall_right(*maze, *x, *y);
    }
    else{ // tout ce qui n'est pas sur le bord
        if (*y+1 != *y_2 && get_bool(tab_visited, *x, *y+1))
            wall_down(*maze, *x, *y);
        if (*y-1 != *y_2 && get_bool(tab_visited, *x, *y-1))
            wall_up(*maze, *x, *y);
        if (*x-1 != *x_2 && get_bool(tab_visited, *x-1, *y))
            wall_left(*maze, *x, *y);
        if (*x+1 != *x_2 && get_bool(tab_visited, *x+1, *y))
            wall_right(*maze, *x, *y);
    }
    *x_2 = *x, *y_2 = *y;
    if (lbp_path_move(maze, x, y, tab_visited)) // mouvement
        lbp_path(maze, x, y, x_2, y_2, tab_visited);
    else
        return;
}

maze_t by_path_maze(const int width, const int height){
    //const time_t t = time(NULL); //Création de la graine du random
    const time_t t = 50; // Création du graine fixe (pour les tests)
    srand(t);
    const bool_tab tab_visited = create_booltab(width, height);
    maze_t maze = create_basic_maze(width, height);

    if (width==1 || height ==1) //si le labyrinthe est une simple cellule, une ligne ou une collone, on la retourne
        return maze;

    int x_1=0, x_2=0, y_1=0, y_2=0; // création des coordonnées et coordonnées de la case précédente
    set_true(tab_visited, x_1, x_2); // on valide notre passage sur la case de départ
    if (rand()%2) // on choisit une direction aléatoire
        x_1++;
    else
        y_1++;
    set_true(tab_visited, x_1, y_1); //on valide notre passage sur la nouvelle case

    for (int y=0; y<height-1; y++){
        for (int x=0; x<width-1; x++){
            if(get_bool(tab_visited, x, y)){
                x_1 = x, y_1 = y, x_2 = x, y_2 = y;
                if (!get_bool(tab_visited, x+1, y)){ // si la case de droite est nouvelle, on y va depuis notre case actuelle
                    x_1++;
                    lbp_path(&maze, &x_1, &y_1, &x_2, &y_2, tab_visited);
                }
                else if (!get_bool(tab_visited, x, y+1)){// si la case du bas est nouvelle, on y va depuis notre case actuelle
                    y_1++;
                    lbp_path(&maze, &x_1, &y_1, &x_2, &y_2, tab_visited);
                }
            }

        }
    }
    if (!get_bool(tab_visited, width-1, height-1)){ // si on n'est jamais passé par la sortie, alors on passe
        if (rand()%2)
            wall_up(maze, width -1 ,height -1);
        else
           wall_left(maze, width -1 ,height -1);
    }
    free_booltab(tab_visited);
   return maze;
}

//fonction auxiliaire
//met à jour le tableau de connexité depuis la case (0, 0)
//retourne True si toutes les cases sont connexe, false sinon
//maze : labyrinthe
//is_connexe : tableau de connexité
//dx, dy : coordonnées de la case à traiter
//renderer : renderer pour l'affichage, TODO : à supprimer
bool static set_connexion(const maze_t maze, const bool_tab is_connexe, const int dx, const int dy, SDL_Renderer *renderer)
{
    const bool_tab visited = create_booltab(maze.width, maze.height); //tableau de booléens pour savoir si une case a été traitée
    stack *s = create_stack(); //cette pile contiendra les coordonnées des cases visitées
    queue *q = create_queue(); //cette file contiendra les coordonnées des cases pour lesquelles on doit vérifier la connexité
    push(dx, dy, s); //on commence par la case (0, 0)
    int x, y;
    bool connexion = false; //pour savoir si notre sous-graphe est lié à la case (0, 0)
    while(!isempty_stack(s))
    {
        pop(s, &x, &y);
        enqueue(x, y, q);
        set_true(visited, x, y);
        assert(x < maze.width); //TODO : à supprimer
        if(get_bool(is_connexe, x, y))
        {
            connexion = true;
        }
        if(y < maze.height - 1 && !has_wall_down(maze, x, y) && !get_bool(visited, x, y + 1)) //TODO ; supprimer la condition x < maze.width (incluse dans le has_wall_down)
        {
            push(x, y + 1, s);
        }
        if(x < maze.width - 1 && !has_wall_right(maze, x, y) && !get_bool(visited, x + 1, y)) //TODO ; supprimer la condition y < maze.height (incluse dans le has_wall_right)
        {
            push(x + 1, y, s);
        }
        if(y > 0 && !has_wall_up(maze, x, y) && !get_bool(visited, x, y - 1)) //TODO ; supprimer la condition x >= 0 (incluse dans le has_wall_up)
        {
            push(x, y - 1, s);
        }
        if(x > 0 && !has_wall_left(maze, x, y) && !get_bool(visited, x - 1, y)) //TODO ; supprimer la condition y >= 0 (incluse dans le has_wall_left)
        {
            push(x - 1, y, s);
        }
    }
    if(!connexion) //si notre sous-graphe n'est pas lié à la case (0, 0), c'est qu'il n'est pas connexe
    {
        free_stack(s);
        free_booltab(visited);
        free_queue(q);
        return false;
    }
    int c = 0; //compteur de cases visitées
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    while(!isempty_queue(q))
    {
        dequeue(q, &x, &y);
        set_true(is_connexe, x, y); //on marque la case comme connexe
        SDL_Rect rect = {x * 20, y * 20, 20, 20};
        SDL_RenderFillRect(renderer, &rect);
        c++;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Delay(1000);
    SDL_RenderPresent(renderer);
    free_stack(s);
    free_booltab(visited);
    free_queue(q);
    return c == maze.width * maze.height; //on retourne true si toutes les cases ont été visitées
}

maze_t cross_maze(const int width, const int height)
{
    const maze_t maze = create_wall_maze(width, height);
    const bool_tab annexe = create_booltab(width, height); //tableau de booléens pour savoir si une case a été traitée
    SDL_Renderer *renderer = NULL;
    SDL_Window *window = NULL;
    int dh, dw, d;
    initial_print_maze(maze, &renderer, &window, &dw, &dh);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    int t = width * height; //nombre de cases à traiter
    //on commence par crréer des chemins (en forme d'étoiles)
    while(t > 0)
    {
        const int r = rand() % t;
        d = r;
        for(int i = 0; i <= r; i++)
        {
            while(get_bool(annexe, d % width, d / width))
            {
                d++; //on saute les case déjà traitées
                assert(d % width < width); //TODO : à supprimer
            }
        }
        const int x = d % width;
        const int y = d / width;
        set_true(annexe, x, y);
        t--;
        //on ouvre les murs
        if(y > 0 && !get_bool(annexe, x, y - 1))
        {
            unwall_up(maze, x, y);
            set_true(annexe, x, y - 1);
            t--;
            SDL_RenderDrawLine(renderer, x * dw + 1, y * dh - 1, (x + 1) * dw - 2, y * dh - 1);
        }
        if(y < height - 1 && !get_bool(annexe, x, y + 1))
        {
            unwall_down(maze, x, y);
            set_true(annexe, x, y + 1);
            t--;
            SDL_RenderDrawLine(renderer, x * dw + 1, (y + 1) * dh - 1, (x + 1) * dw - 2, (y + 1) * dh - 1);
        }
        if(x > 0 && !get_bool(annexe, x - 1, y))
        {
            unwall_left(maze, x, y);
            set_true(annexe, x - 1, y);
            t--;
            SDL_RenderDrawLine(renderer, x * dw - 1, y * dh + 1, x * dw - 1, (y + 1) * dh - 2);
        }
        assert(x < width); //TODO : à supprimer
        if(x < width - 1 && !get_bool(annexe, x + 1, y))
        {
            unwall_right(maze, x, y);
            set_true(annexe, x + 1, y);
            t--;
            SDL_RenderDrawLine(renderer, (x + 1) * dw - 1, y * dh + 1, (x + 1) * dw - 1, (y + 1) * dh - 2);
        }
        SDL_Delay(100);
        SDL_RenderPresent(renderer);
    }
    for(int i = 0; i < maze.width; i++)
    {
        for(int j = 0; j < maze.height; j++)
        {
            set_false(annexe, i, j);
        }
    }
    //on va connexter les cases non connexes
    int x = rand() % width;
    int y = rand() % height;
    set_true(annexe, x, y); //on marque la case comme celle de départ
    bool b = set_connexion(maze, annexe, x, y, renderer);
    while(!b) //tant que toutes les cases ne sont pas connexes
    {
        d = rand() % (width * height);
        int fusible = width * height; //sécurité pendant l'implémentation
        assert(d % width < width); //TODO : à supprimer
        while(get_bool(annexe, d % width, d / width) && fusible > 0)
        {
            fusible--;
            d++;
            assert(d % width < width); //TODO : à supprimer
        }
        x = d % width;
        y = d / width;
        char dir[4] = {'R', 'D', 'L', 'U'}; //tableau des directions possibles
        //chercher une cellule visitée adjacente à notre cellule non visitée
        char c = '\0';
        int size = 4;
        while(c == '\0' && size > 0)
        {
            const int r = rand() % size;
            c = dir[r];
            if (c == 'R' && x + 1 < width && get_bool(annexe, x + 1, y))
            {
                unwall_right(maze, x, y);
                SDL_RenderDrawLine(renderer, (x + 1) * dw - 1, y * dh + 1, (x + 1) * dw - 1, (y + 1) * dh - 2);
                b = set_connexion(maze, annexe, x, y, renderer);
            }
            else if(c == 'L' && x > 0 && get_bool(annexe, x - 1, y))
            {
                unwall_left(maze, x, y);
                SDL_RenderDrawLine(renderer, x * dw - 1, y * dh + 1, x * dw - 1, (y + 1) * dh - 2);
                b = set_connexion(maze, annexe, x, y, renderer);
            }
            else if(c == 'D' && y + 1 < height && get_bool(annexe, x, y + 1))
            {
                unwall_down(maze, x, y);
                SDL_RenderDrawLine(renderer, x * dw + 1, (y + 1) * dh - 1, (x + 1) * dw - 2, (y + 1) * dh - 1);
                b = set_connexion(maze, annexe, x, y, renderer);
            }
            else if(c == 'U' && y > 0 && get_bool(annexe, x, y - 1))
            {
                unwall_up(maze, x, y);
                SDL_RenderDrawLine(renderer, x * dw + 1, y * dh - 1, (x + 1) * dw - 2, y * dh - 1);
                b = set_connexion(maze, annexe, x, y, renderer);
            }
            else
            {
                for (int i = r; i < size; i++){
                    dir[i] = dir[i+1];
                }
                size--;
                c = '\0';
            }
        }
    }
    free_booltab(annexe);
    SDL_Delay(100);
    SDL_RenderPresent(renderer);
    wait_and_destroy_print_maze(renderer, window);
    return maze;
}