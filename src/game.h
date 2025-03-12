#ifndef GAME_H
#define GAME_H

#include "struct.h"

enum direction
{
    NORD = 0,
    SUD,
    EST,
    OUEST
};

// --- GAME FUNCTIONS ---
/*
 * Function to play the game
 * maze : the maze
 * x, y : the starting point
 * the player can't see the maze
 */
void game_blind(maze_t maze, int x, int y);

/*
 * Function to play the game
 * maze : the maze
 * x, y : the starting point
 * the player can see the maze
 */
void game_show(maze_t, int, int);

/*
 * Function to play the game
 * maze : the maze
 * x, y : the starting point
 * the player can see the visited cells
 */
void game(maze_t, int, int);

/*
 * Function to play the game
 * maze : the maze
 * x, y : the starting point
 * the player can see the twenty last visited cells
 */
void game_half_blind(maze_t, int, int);

/*
 * Function to play the game
 * maze : the maze
 * x, y : the starting point
 * the player can see 4 cells around him
 */
void game_quarter_blind(maze_t, int, int);

/*
 * Function to play the game
 * maze : the maze
 * x, y : the starting point
 * the player can see the front cell
 */
void game_front_blind(maze_t, int, int);

#endif // GAME_H
