#ifndef ESCAPE_H
#define ESCAPE_H
#include "struct.h"

#define SIZE 11
extern int (*escape[SIZE])(maze_t maze, int x, int y);
extern const char *escape_name[SIZE];

// function to find the exit of the maze from an random case
// visualize the progression of the solver
// stop when the exit is found or the corresponding command is entered
// maze : the maze
// x, y : the coordinates of the starting point
// return the number of steps
int random_escape(maze_t maze, int x, int y);

// function to find the exit of the maze like a ghost
// visualize the progression of the solver
// stop when the exit is found or the corresponding command is entered
// maze : the maze
// x, y : the coordinates of the starting point
// return the number of steps
int cheat_escape(maze_t maze, int x, int y);

// function to find the exit of the maze in a hunt and kill like function
// visualize the progression of the solver
// stop when the exit is found or the corresponding command is entered
// maze : the maze
// x, y : the coordinates of the starting point
// return the number of steps
int hunt_kill_escape(maze_t maze, int x, int y);

// function to find the exit of the maze using the right hand
// visualize the progression of the solver
// stop when the exit is found or the corresponding command is entered
// maze : the maze
// x, y : the coordinates of the starting point
// return the number of steps
int right_hand(maze_t maze, int x, int y);

// function to find the exit of the maze using the right hand and sometimes random function
// visualize the progression of the solver
// stop when the exit is found or the corresponding command is entered
// maze : the maze
// x, y : the coordinates of the starting point
// return the number of steps
int right_hand_r(maze_t maze, int x, int y);


int right_hand_p(maze_t maze, int x, int y);
int right_hand_de(const maze_t maze, int x, int y);
int right_hand_p_de(const maze_t maze, int x, int y);

int random_p(const maze_t maze, int x, int y);
int random_de(const maze_t maze, int x, int y);
int random_p_de(const maze_t maze, int x, int y);
#endif // ESCAPE_H
