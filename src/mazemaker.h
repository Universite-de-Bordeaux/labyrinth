#ifndef MAZEMAKER_H
#define MAZEMAKER_H
#include "struct.h"

//a perfect maze is a maze where there is only exactly path between any pair of cells
//a maze is perfect if and only if connexe and doesn't contain any cycle
//a maze is connexe if and only if there is a path between any pair of cells
//a maze is solvable if and only if there is a path between the entrance (0, 0) and the exit (width - 1, height - 1)
//any of our maze must be solvable, ideally connex

//creates a perfect maze
//every wall is horizontal
//width : width of the maze
//height : height of the maze
maze_t line_maze(const int width, const int height);

//creates a perfect maze
//every wall is vertical
//width : width of the maze
//height : height of the maze
maze_t column_maze(const int width, const int height);

//Creates a solvable maze
//width : width of the maze
//height : height of the maze
maze_t one_way_maze(const int width, const int height);

//Creates a perfect maze
//width : width of the maze
//height : height of the maze
//every way only go to the right or down
maze_t comb_maze(const int width, const int height);

maze_t proto_comb_maze(const int width, const int height);

//Creates a perfect maze (universal generator)
//width : width of the maze
//height : height of the maze
//with the hunt and kill algorithm
maze_t hunt_kill_maze(const int width, const int height);

//Creates a perfect maze
//width : width of the maze
//height : height of the maze
maze_t by_path_maze(int width, int height);

//Creates perfect maze
//width : width of the maze
//height : height of the maze
maze_t cross_maze(const int width, const int height);

//"tears" some walls of the maze
//maze : the maze to modify
//prop : the proportion of cells with a wall to remove
void tear(maze_t maze, unsigned int prop);

#endif //MAZEMAKER_H
