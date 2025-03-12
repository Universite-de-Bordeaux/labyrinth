#ifndef GAME_H
#define GAME_H

#include "struct.h"

// --- GAME FUNCTIONS ---
// function to play the game
// maze : the maze
// x, y : the starting point
// the player can't see the maze
void game_blind(maze_t maze, int x, int y);

// function to play the game
// maze : the maze
// x, y : the starting point
// the maze is always shown
void game_show(maze_t, int, int);

// function to play the game
// maze : the maze
// x, y : the starting point
// the player can see all the cellules visited
void game(maze_t, int, int);

// function to play the game
// maze : the maze
// x, y : the starting point
// the player can see the thirteen cellules visited
void game_half_blind(maze_t, int, int);

// function to play the game
// maze : the maze
// x, y : the starting point
// the player can see 4 cellules around him
void game_quarter_blind(maze_t, int, int);

#endif //GAME_H
