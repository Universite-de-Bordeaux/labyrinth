#include "test.h"
#include "solveur.h"

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