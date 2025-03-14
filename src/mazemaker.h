#ifndef MAZEMAKER_H
#define MAZEMAKER_H
#include "struct.h"

// a perfect maze is a maze where there is only exactly path between any pair of cells
// a maze is perfect if and only if connexe and doesn't contain any cycle
// a maze is connexe if and only if there is a path between any pair of cells
// a maze is solvable if and only if there is a path between the entrance (0, 0) and the exit (width - 1, height - 1)
// any of our maze must be solvable, ideally connex

// creates a perfect maze
// every wall is horizontal
// width : width of the maze
// height : height of the maze
maze_t line_maze(const int width, const int height);

// creates a perfect maze
// every wall is vertical
// width : width of the maze
// height : height of the maze
maze_t column_maze(const int width, const int height);

// Creates a perfect maze
// width : width of the maze
// height : height of the maze
// create a main path with a branch in every possible direction
//  (deep creation)
maze_t one_way_maze(const int width, const int height);

// Creates a perfect maze
// width : width of the maze
// height : height of the maze
// every way try to go the most far possible
//  (breadth creation)
maze_t octopus_maze(const int width, const int height);

// Creates a perfect maze
// width : width of the maze
// height : height of the maze
// every way lead to the same cell
//  (draw creation)
maze_t my_octopus_maze(const int width, const int height);

// Creates a perfect maze
// width : width of the maze
// height : height of the maze
// every way only go to the right or down
maze_t comb_maze(const int width, const int height);

// Creates a perfect maze
// width : width of the maze
// height : height of the maze
// a better version of comb_maze withoun the right or down restriction
// (prototype unfinished)
maze_t proto_comb_maze(const int width, const int height);

// Creates a perfect maze (universal generator)
// width : width of the maze
// height : height of the maze
// with the hunt and kill algorithm
maze_t hunt_kill_maze(const int width, const int height);

// Creates a perfect maze
// width : width of the maze
// height : height of the maze
maze_t by_path_maze(int width, int height);

// Creates perfect maze
// width : width of the maze
// height : height of the maze
maze_t cross_maze(const int width, const int height);

// --- special mazes (only for fun, may cause error) ---

// Creates a perfect maze in shape of a snail's shell
// width : width of the maze
// height : height of the maze
maze_t snail_maze(const int width, const int height);

// Creates a perfect maze in shape of a snake
// width : width of the maze
// height : height of the maze
maze_t snake_maze(const int width, const int height);

// Creates a perfect maze reflecting a comb one
// width : width of the maze
// height : height of the maze
maze_t reverse_comb_maze(const int width, const int height);

// Creates a perfect maze reccursiveley
// width : width of the maze
// height : height of the maze
maze_t reccursive_maze(const int width, const int height);

// Creates a perfect maze in shape of weeping willow
// width : width of the maze, must be at least 4
// height : height of the maze, must be at least 10
maze_t weeping_willow_maze(int width, int height);

// Creates a perfect maze by merge four mazes (randomly)
// width : width of the maze, must be at least 4
// height : height of the maze, must be at least 4
maze_t four_maze(int width, int height);

// Creates a perfect maze by merge some maze (randomly)
// width : width of the maze, must be at least 4
// height : height of the maze, must be at least 4
maze_t golden_maze(int width, int height);

//"tears" some walls of the maze
// maze : the maze to modify
// prop : the proportion of cells with a wall to remove
void tear(maze_t maze, unsigned int prop);

#endif // MAZEMAKER_H
