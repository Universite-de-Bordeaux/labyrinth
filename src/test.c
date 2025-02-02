#include "test.h"
#include <time.h>
#include "solveur.h"

bool do_made_solvable_maze(const func_ptr f, const int x, const int y)
{
    for (int i = 1; i < x; i++)
    {
        for (int j = 1; j < y; j++)
        {
            const maze_t maze = f(i, j);
            if (!has_exit_deep_seeker(maze))
            {
                show_best_exit_deep_seeker(maze);
                free_maze(maze);
                return false;
            }
            free_maze(maze);
        }
    }
    return true;
}

bool do_made_connected_maze(const func_ptr f, const int x, const int y)
{
    for (int i = 1; i < x; i++)
    {
        for (int j = 1; j < y; j++)
        {
            const maze_t maze = f(i, j);
            if (!is_connexe_breadth_inspector(maze))
            {
                show_is_connexe_breadth_inspector(maze);
                free_maze(maze);
                return false;
            }
            free_maze(maze);
        }
    }
    return true;
}

bool do_made_perfect_maze(const func_ptr f, const int x, const int y)
{
    for (int i = 1; i < x; i++)
    {
        for (int j = 1; j < y; j++)
        {
            const maze_t maze = f(i, j);
            if (!is_perfect_deep_inspector(maze))
            {
                show_is_perfect_deep_inspector(maze);
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
    int t = 3;
    for (int i = 1; i < 100; i++)
    {
        for (int j = 1; j < 100; j++)
        {
            const maze_t maze = f(i, j);
            if (t == 3 && !is_perfect_deep_inspector(maze))
            {
                t = 2;
            }
            if (t == 2 && !is_connexe_breadth_inspector(maze))
            {
                t = 1;
            }
            if (t == 1 && !has_exit_deep_seeker(maze))
            {
                free_maze(maze);
                return 0;
            }
            free_maze(maze);
        }
    }
    return t;
}

int evaluate_time(const func_ptr f, char* name)
{
    long score1 = 50;
    long int score2 = 10;
    long int score3 = 15;
    long int score4 = 25;
    clock_t start = clock();
    const maze_t t_maze = create_basic_maze(1000, 10);
    const clock_t temp = 2 * (clock() - start); // temps moyen pour générer un labyrinthe vide de 1000x10
    free_maze(t_maze);

    // evaluation de la génération standard
    start = clock();
    for (int i = 1; i < 100; i++)
    {
        for (int j = 1; j < 100; j++)
        {
            const maze_t maze = f(i, j);
            free_maze(maze);
        }
        if (clock() - start > 100000000)
        {
            break;
        }
    }
    score1 -= (clock() - start) / 4000000; // une valeur arbitraire pour avoir un score entre 0 et 50
    clock_t temp2;
    // evaluation de la génération de labyrinthes ligne/colonne
    for (int k = 1; k < 1000; k++)
    {
        for (int i = 1; i < 100; i++)
        {
            start = clock();
            const maze_t maze = f(i, 1);
            free_maze(maze);
            temp2 = clock() - start;
            if (temp2 > temp) // si on dépasse le double du temps moyen pour générer un labyrinthe vide
            {
                score2 -= 1;
                if (score2 == 0)
                {
                    break;
                }
            }
        }
        start = clock();
        const maze_t maze = f(1, k);
        free_maze(maze);
        temp2 = clock() - start;
        if (temp2 > temp) // si on dépasse le double du temps moyen pour générer un labyrinthe vide
        {
            score2 -= 1;
        }
        if (score2 <= 0)
        {
            break;
        }
    }

    // evaluation de la génération de labyrinthes 4x4 (petits labyrinthes)
    start = clock();
    for (int k = 0; k < 1000; k++)
    {
        const maze_t maze = f(4, 4);
        free_maze(maze);
    }
    score3 -= (clock() - start) / 400; // une valeur arbitraire pour avoir un score entre 0 et 15

    // evaluation de la génération de labyrinthes 100x100 (grands labyrinthes)
    start = clock();
    for (int k = 0; k < 10; k++)
    {
        const maze_t maze = f(100, 100);
        free_maze(maze);
    }
    score4 -= (clock() - start) / 100000; // une valeur arbitraire pour avoir un score entre 0 et 25
    if (score1 < 0)
    {
        score1 = 0;
    }
    if (score2 < 0)
    {
        score2 = 0;
    }
    if (score3 < 0)
    {
        score3 = 0;
    }
    if (score4 < 0)
    {
        score4 = 0;
    }
    printf("The %s has been evaluated\n", name);
    printf("\tStandard generation score : %ld/50\n", score1);
    printf("\tLine/Column generation score : %ld/10\n", score2);
    printf("\t4x4 maze generation score : %ld/15\n", score3);
    printf("\t100x100 maze generation score : %ld/25\n", score4);
    printf("\tTotal score : %ld/100\n", score1 + score2 + score3 + score4);
    const int score = (int)(score1 + score2 + score3 + score4);
    return score;
}