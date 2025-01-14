#ifndef TEST_H
#define TEST_H
#include "struct.h"

// Definition of a function pointer
typedef maze_t (*func_ptr)(int, int);

// function to analysed if a maze is solvable
// f : pointer to the maze generator function
// x : the range of the width of the maze
// y : the range of the height of the maze
// return true if generator looks making solvable maze, false otherwise
// exponential in time (do not use for large values of x and y)
bool do_made_solvable_maze(const func_ptr f, const int x, const int y);

// function to analysed if a maze is connected
// f : pointer to the maze generator function
// x : the range of the width of the maze
// y : the range of the height of the maze
// return true if generator looks making connected maze, false otherwise
// exponential in time (do not use for large values of x and y)
bool do_made_connected_maze(const func_ptr f, const int x, const int y);

// function to analysed if a maze is perfect
// f : pointer to the maze generator function
// x : the range of the width of the maze
// y : the range of the height of the maze
// return true if generator looks making perfect maze, false otherwise
// exponential in time (do not use for large values of x and y)
bool do_made_perfect_maze(const func_ptr f, const int x, const int y);

// function to evaluate a maze generator
// f : pointer to the maze generator function
// return 3 if generator looks making perfect maze, 2 if generator looks making connected maze, 1 if generator looks
// making solvable maze, 0 otherwise may return false positive (but it's unlikely)
int evaluate_mazemaker(func_ptr f);

// MACRO to evaluate a maze generator
// generator : pointer to the maze generator function
// name : name of the generator
// print the result of the evaluation
// may return false positive (but it's unlikely)
#define EVALUATE_MAZEMAKERN(generator, name)                                                                                                                                       \
    do                                                                                                                                                                             \
    {                                                                                                                                                                              \
        const int t = evaluate_mazemaker(generator);                                                                                                                               \
        if (t == 3)                                                                                                                                                                \
        {                                                                                                                                                                          \
            printf("The %s seem to create perfect mazes\n", name);                                                                                                                 \
        }                                                                                                                                                                          \
        else if (t == 2)                                                                                                                                                           \
        {                                                                                                                                                                          \
            printf("The %s seem to create connected mazes\n", name);                                                                                                               \
        }                                                                                                                                                                          \
        else if (t == 1)                                                                                                                                                           \
        {                                                                                                                                                                          \
            printf("The %s seem to create solvable mazes\n", name);                                                                                                                \
        }                                                                                                                                                                          \
        else                                                                                                                                                                       \
        {                                                                                                                                                                          \
            printf("The %s do not seem to create solvable mazes\n", name);                                                                                                         \
        }                                                                                                                                                                          \
    }                                                                                                                                                                              \
    while (0)

// function to evaluate the time of a maze generator
// générateur : pointer to the maze generator function
// name : name of the generator
// print a score between 0 (bad) and 100 (good) of the generator
int evaluate_time(func_ptr f, char* name);
#endif // TEST_H
