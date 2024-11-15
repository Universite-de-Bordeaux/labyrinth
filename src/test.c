#include "test.h"
#include "solveur.h"
#include <time.h>

bool do_made_solvable_maze(const func_ptr f, const int x, const int y)
{
    for(int i = 1; i < x; i++)
    {
        for(int j = 1; j < y; j++)
        {
            const maze_t maze = f(i, j);
            if(!has_exit_deep_seeker(maze))
            {
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
    for(int i = 1; i < x; i++)
    {
        for(int j = 1; j < y; j++)
        {
            const maze_t maze = f(i, j);
            if(!is_perfect_deep_inspector(maze))
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
    int t = 3;
    for(int i = 1; i < 100; i++)
    {
        for(int j = 1; j < 100; j++)
        {
            const maze_t maze = f(i, j);
            if(t == 3 && !is_perfect_breadth_inspector(maze))
            {
                t = 2;
            }
            if(t == 2 && !is_connexe_breadth_inspector(maze))
            {
                t = 1;
            }
            if(t == 1 && !has_exit_deep_seeker(maze))
            {
                free_maze(maze);
                return 0;
            }
            free_maze(maze);
        }
    }
    return t;
}

int evaluate_time(func_ptr f, char* name)
{
    int score1 = 50;
    int score2 = 10;
    int score3 = 15;
    int score4 = 25;
    clock_t start = clock();
    for(int i = 1; i < 100; i++)
    {
        for(int j = 1; j < 100; j++)
        {
            const maze_t maze = f(i, j);
            free_maze(maze);
        }
        if(clock() - start > 200000000)
        {
            score1 = 0;
            break;
        }
    }
    score1 -= (clock() - start) / 4000000;
    clock_t inter = clock();
    for(int k = 1; k < 1000; k++)
    {
        for(int i = 1; i < 100; i++)
        {
            const maze_t maze = f(i, 1);
            free_maze(maze);
        }
        const maze_t maze = f(1, k);
        free_maze(maze);
    }
    score2 -= (clock() - inter) / 35000;
    clock_t inter2 = clock();
    for(int k = 0; k < 1000; k++)
    {
        const maze_t maze = f(4, 4);
        free_maze(maze);
    }
    score3 -= (clock() - inter2) / 400;
    for(int k = 0; k < 10; k++)
    {
        const maze_t maze = f(100, 100);
        free_maze(maze);
    }
    score4 -= (clock() - inter2) / 100000;
    if(score1 < 0)
    {
        score1 = 0;
    }
    if(score2 < 0)
    {
        score2 = 0;
    }
    if(score3 < 0)
    {
        score3 = 0;
    }
    if(score4 < 0)
    {
        score4 = 0;
    }
    printf("Le générateur %s a un score de %d (%d %d %d %d)\n", name, score1 + score2 + score3 + score4, score1, score2, score3, score4);
    return score1 + score2 + score3 + score4;
}