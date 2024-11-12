#ifndef TEST_H
#define TEST_H
#include "struct.h"

//Definition of a function pointer
typedef maze_t (*func_ptr)(int, int);

// function to analysed if a maze is solvable
//f : pointer to the maze generator function
//x : the range of the width of the maze
//y : the range of the height of the maze
//return true if generator looks making solvable maze, false otherwise
//exponential in time (do not use for large values of x and y)
bool do_made_solvable_maze(const func_ptr f, const int x, const int y);

// function to analysed if a maze is perfect
//f : pointer to the maze generator function
//x : the range of the width of the maze
//y : the range of the height of the maze
//return true if generator looks making perfect maze, false otherwise
//exponential in time (do not use for large values of x and y)
bool do_made_perfect_maze(const func_ptr f, const int x, const int y);

// function to evaluate a maze generator
//f : pointer to the maze generator function
//return 3 if generator looks making perfect maze, 2 if generator looks making connected maze, 1 if generator looks making solvable maze, 0 otherwise
//may return false positive (but it's unlikely)
int evaluate_mazemaker(func_ptr f);

// MACRO to evaluate a maze generator
//generator : pointer to the maze generator function
//name : name of the generator
//print the result of the evaluation
//may return false positive (but it's unlikely)
#define EVALUATE_MAZEMAKERN(generator, name) \
    do { \
        const int t = evaluate_mazemaker(generator); \
        if(t == 3) \
        { \
            printf("%s semble créer des labyrinthes parfaits\n", name); \
        } \
        else if(t == 2) \
        { \
            printf("%s semble créer des labyrinthes connexes\n", name); \
        } \
        else if(t == 1) \
        { \
            printf("%s semble créer des labyrinthes solvables\n", name); \
        } \
        else \
        { \
            printf("%s n'est pas conforme au cahier des charges\n", name); \
        } \
    } while(0)

// MACRO to evaluate the time of a maze generator
//générateur : pointer to the maze generator function
//name : name of the generator
//print a score between 0 (bad) and 100 (good) of the generator
#define EVALUATE_TIME(générateur, name) \
    do { \
        int score1 = 25; \
        int score2 = 25; \
        int score3 = 25; \
        int score4 = 25; \
        clock_t start = clock(); \
        for(int i = 1; i < 100; i++) \
        { \
            for(int j = 1; j < 100; j++) \
            { \
                const maze_t maze = générateur(i, j); \
                free_maze(maze); \
            } \
            if(clock() - start > 100000000) \
            { \
            score1 = 0; \
            break; \
            } \
        } \
        score1 -= (clock() - start) / 4000000; \
        clock_t inter = clock(); \
        for(int k = 0; k < 1000000; k++) \
        { \
            const maze_t maze = générateur(1, 1); \
            free_maze(maze); \
        } \
        score2 -= (clock() - inter) / 35000; \
        clock_t inter2 = clock(); \
        for(int k = 0; k < 1000; k++) \
        { \
            const maze_t maze = générateur(4, 4); \
            free_maze(maze); \
        } \
        score3 -= (clock() - inter2) / 400; \
        for(int k = 0; k < 10; k++) \
        { \
            const maze_t maze = générateur(100, 100); \
            free_maze(maze); \
        } \
        score4 -= (clock() - inter2) / 100000; \
        if(score1 < 0) \
        { \
            score1 = 0; \
        } \
        if(score2 < 0) \
        { \
            score2 = 0; \
        } \
        if(score3 < 0) \
        { \
            score3 = 0; \
        } \
        if(score4 < 0) \
        { \
            score4 = 0; \
        } \
        printf("Le générateur %s a un score de %d (%d %d %d %d)\n", name, score1 + score2 + score3 + score4, score1, score2, score3, score4); \
    } while(0)
#endif //TEST_H
