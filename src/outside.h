#ifndef OUTSIDE_H
#define OUTSIDE_H
#include "struct.h"

// Create a maze according to the information in the file
// filename : the name of the file
maze_t maze_from_file(const char *filename);

// Write the maze in a text file
// maze : the maze
// filename : the name of the file where to write the maze, will be overwritten if it already exists
void maze_to_file(maze_t maze, const char *filename);

// Create a way according to the information in the file
// filename : the name of the file
// returns the way or stops the program if an error occurs
way *way_from_file(const char *filename);

// Write the way in a text file
// w : the way
// filename : the name of the file where to write the way, will be overwritten if it already exists
void way_to_file(const way* w, const char *filename);

#endif //OUTSIDE_H
