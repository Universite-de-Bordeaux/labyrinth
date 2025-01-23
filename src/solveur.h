#ifndef SOLVEUR_H
#define SOLVEUR_H
#include "struct.h"

// --- solveurs en profondeur ---

// return true if the maze is solvable, false otherwise
// maze : the maze
// deep search (more memory efficient and faster than breadth search in average )
bool has_exit_deep_seeker(maze_t maze);

// return true if the maze is connexe, false otherwise
// maze : the maze
// deep search
bool is_connexe_deep_inspector(maze_t maze);

// return true if the maze is perfect, false otherwise
// maze : the maze
bool is_perfect_deep_inspector(maze_t maze);

// return the shortest way to exit the maze
// maze : the maze
// return an empty way if the maze is not solvable
// deep search (less memory efficient and slower than breadth search)
way* best_exit_deep_seeker(const maze_t maze);


// --- solveurs en largeur ---


// return true if the maze is solvable, false otherwise
// maze : the maze
// breadth search (less memory efficient and slower than deep search in average)
bool has_exit_breadth_seeker(maze_t maze);

// return true if the maze is connexe, false otherwise
// maze : the maze
// breadth search
bool is_connexe_breadth_inspector(maze_t maze);

// return true if the maze is perfect, false otherwise
// maze : the maze
// breadth search
bool is_perfect_breadth_inspector(maze_t maze);

// return the shortest way to exit the maze
// maze : the maze
// return an empty way if the maze is not solvable
// breadth search (more memory efficient and faster than deep search in average)
way* best_exit_breadth_seeker(const maze_t maze);


// --- visualisation ---

// function to visualize the maze and the progression of the solver
// return -1 in case of error, 1 otherwise
// maze : the maze
int show_has_exit_deep_seeker(maze_t maze);

// function to visualize the maze and the progression of the solver
// return -1 in case of error, 1 otherwise
// maze : the maze
int show_is_connexe_deep_inspector(maze_t maze);

// function to visualize the maze and the progression of the solver
// return -1 in case of error, 1 otherwise
// maze : the maze
int show_is_perfect_deep_inspector(maze_t maze);

// function to visualize the maze and the progression of the solver
// return -1 in case of error, 1 otherwise
// maze : the maze
int show_best_exit_deep_seeker(maze_t maze);

// function to visualize the maze and the progression of the solver
// return -1 in case of error, 1 otherwise
// maze : the maze
int show_has_exit_breadth_seeker(maze_t maze);

// function to visualize the maze and the progression of the solver
// return -1 in case of error, 1 otherwise
// maze : the maze
int show_is_connexe_breadth_inspector(maze_t maze);

// function to visualize the maze and the progression of the solver
// return -1 in case of error, 1 otherwise
// maze : the maze
int show_is_perfect_breadth_inspector(maze_t maze);

// function to visualize the maze and the progression of the solver
// return -1 in case of error, 1 otherwise
// maze : the maze
int show_best_exit_breadth_seeker(maze_t maze);

// function to visualize the maze and the progression of the solver
// return -1 in case of error, 1 otherwise
// maze : the maze in which we want to show the way
// w : the way to show
int show_the_way(const maze_t maze, const way* w);

//------------------------- escape from the maze -------------------------

//standard function to find the exit of the maze
//visualize the progression of the solver
//stop when the exit is found or the corresponding command is entered
//maze : the maze
//x, y : the coordinates of the starting point
void ESCAPE_TYPE(maze_t maze, int x, int y);

//function to find the exit of the maze from an random case
//visualize the progression of the solver
//stop when the exit is found or the corresponding command is entered
//maze : the maze
//x, y : the coordinates of the starting point
void random_escape(maze_t maze, int x, int y);

//function to find the exit of the maze by following directions
//visualize the progression of the solver
//stop when the exit is found or the corresponding command is entered
//maze : the maze
//x, y : the coordinates of the starting point
void try_direction(maze_t maze, int x, int y);

//function to find the exit of the maze like a ghost
//visualize the progression of the solver
//stop when the exit is found or the corresponding command is entered
//maze : the maze
//x, y : the coordinates of the starting point
void cheat_escape(maze_t maze, int x, int y);

#endif // SOLVEUR_H
