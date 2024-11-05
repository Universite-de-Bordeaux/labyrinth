#include "cmd.h"
#include "mazemaker.h"
#include "struct.h"
#include "solveur.h"
#include <time.h>

#include "outside.h"

int main(const int argc, char *argv[]) {
    srand(time(NULL));
    const maze_t maze = hunt_kill_maze(200, 3);
    for(int i = 0; i < 100; i++)
    {
        for(int j = 0; j < 100; j++)
        {
            const int t = rand() % 6;
            if(t == 0)
            {
                unwall_down(maze, j, i);
            }
            else if(t == 1)
            {
                unwall_right(maze, j, i);
            }
        }
    }
    show_best_exit_deep_seeker(maze);
    maze_to_file(maze, "test");
    free_maze(maze);
    //cmd(argv, argc);
    return 0;
}
