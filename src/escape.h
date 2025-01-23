#ifndef ESCAPE_H
#define ESCAPE_H
#include "struct.h"

// standard function to find the exit of the maze
// visualize the progression of the solver
// stop when the exit is found or the corresponding command is entered
// maze : the maze
// x, y : the coordinates of the starting point
void ESCAPE_TYPE(maze_t maze, int x, int y);

// function to find the exit of the maze from an random case
// visualize the progression of the solver
// stop when the exit is found or the corresponding command is entered
// maze : the maze
// x, y : the coordinates of the starting point
void random_escape(maze_t maze, int x, int y);

// function to find the exit of the maze by following directions
// visualize the progression of the solver
// stop when the exit is found or the corresponding command is entered
// maze : the maze
// x, y : the coordinates of the starting point
void try_direction(maze_t maze, int x, int y);

// function to find the exit of the maze like a ghost
// visualize the progression of the solver
// stop when the exit is found or the corresponding command is entered
// maze : the maze
// x, y : the coordinates of the starting point
void cheat_escape(maze_t maze, int x, int y);


#endif // ESCAPE_H
