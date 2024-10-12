#include "test.h"
#include "solveur.h"

//Fonction permettant d'évaluer un générateur de labyrinthe
//f : pointeur de fonction de génération de labyrinthe
//retourne true si le générateur semble correct, false sinon
//risque de faux positifs
bool create_solvable_maze(const func_ptr f)
{
    for(int i = 1; i < 100; i++)
    {
        for(int j = 1; j < 100; j++)
        {
            const maze_t maze = f(i, j);
            if(!has_exit_cascade(maze))
            {
                free_maze(maze);
                return false;
            }
            free_maze(maze);
        }
    }
    return true;
}

//Fonction permettant d'évaluer un générateur de labyrinthe
//f : pointeur de fonction de génération de labyrinthe
//retourne true si le générateur semble correct, false sinon
//risque de faux positifs
bool create_perfect_maze(const func_ptr f)
{
    for(int i = 1; i < 100; i++)
    {
        for(int j = 1; j < 100; j++)
        {
            const maze_t maze = f(i, j);
            if(!is_perfect_cascade(maze))
            {
                free_maze(maze);
                return false;
            }
            free_maze(maze);
        }
    }
    return true;
}

int evaluate_mazemaker(const func_ptr f)
{
    if(create_perfect_maze(f))
    {
        return 2;
    }
    return create_solvable_maze(f) ? 1 : 0;
}