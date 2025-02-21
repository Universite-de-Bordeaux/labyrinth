#ifndef CASE_H
#define CASE_H
#include <SDL2/SDL.h>
#include <stdbool.h>
#define L_TP 3000000000 // length of a teleportation

// Structure - typedef - fonction (dans l'ordre)

// --- Structures ---

struct maze
{
    int width;
    int height;
    char** walls; // wall up: 1, down: 2, left: 4, right: 8
};

struct bool_tab
{
    int width;
    int height;
    char** tab;
};

struct way
{
    struct way* dad;
    int x;
    int y;
    unsigned int length;
};

struct waytab
{
    struct way** tab;
    int width;
    int height;
};

struct queue
{
    int* array; // Array of values.
    int size_array; // Size of the array allocated in memory.
    int left; // Index of the value to the left of the file (if non-empty).
    int right; // Index following that of the value to the right of the file (if it is non-empty).
    bool empty; // Boolean indicating if the file is empty.
};

struct stack
{
    int* array; // Array containing the values of the represented stack.
    int size_array; // Size of the array allocated in memory.
    int size_stack; // Number of elements in the stack represented.
};

// --- Typedef ---

// Structure representing a maze
// width : width of the maze
// height : height of the maze
// walls : sort of table of booleans representing the walls of the maze
// malloc used, remember to free_maze to free memory
typedef struct maze maze_t;

// Structure representing a boolean table
// width : width of the table
// height : height of the table
// tab : table of table of booleans
// malloc used, remember to free_booltab to free memory
typedef struct bool_tab bool_tab;

// Structure representing a way
// dad : pointer to the previous cell in the way
// x, y : coordinates of the cell
// length : length of the way from the start (0, 0). Can teleport to a cost of L_TP
// malloc used, remember to free_way to free memory
// never use a way function in a waytab, it may cause lots of errors
typedef struct way way;

// Structure representing a way table
// width : width of the table
// height : height of the table
// tab : table of table of ways
// malloc used, remember to free_waytab to free memory
typedef struct waytab waytab;

// Structure representing a queue
// array : array of values
// size_array : size of the array allocated in memory
// left : index of the value to the left of the file (if non-empty)
// right : index following that of the value to the right of the file (if it is non-empty)
// empty : boolean indicating if the file is empty
// malloc used, remember to free_queue to free memory
typedef struct queue queue;

// Structure representing a stack
// size_array : size of the array allocated in memory
// size_stack : number of elements in the stack represented
// array : array containing the values of the represented stack
// malloc used, remember to free_stack to free memory
typedef struct stack stack;


// --- Fonctions ---
// --- MAZE ---

// Create a maze without walls (except the outer walls)
// width : width of the maze
// height : height of the maze
// returns the maze
// display an error message and stop the program if an error occurs
maze_t create_basic_maze(int width, int height);

// Create a maze with all walls closed
// width : width of the maze
// height : height of the maze
// returns the maze
// display an error message and stop the program if an error occurs
maze_t create_wall_maze(int width, int height);

// free the memory allocated for the maze
// maze : the maze to free
void free_maze(maze_t maze);

// add a wall at the top of the cell x, y
// maze : the maze
// x, y : coordinates of the cell
// display an error message withoun stopping the program if an error occurs
void wall_up(maze_t maze, int x, int y);

// add a wall at the bottom of the cell x, y
// maze : the maze
// x, y : coordinates of the cell
// display an error message withoun stopping the program if an error occurs
void wall_down(maze_t maze, int x, int y);

// add a wall to the left of the cell x, y
// maze : the maze
// x, y : coordinates of the cell
// display an error message withoun stopping the program if an error occurs
void wall_left(maze_t maze, int x, int y);

// add a wall to the right of the cell x, y
// maze : the maze
// x, y : coordinates of the cell
// display an error message withoun stopping the program if an error occurs
void wall_right(maze_t maze, int x, int y);

// remove the wall at the top of the cell x, y
// maze : the maze
// x, y : coordinates of the cell
// display an error message withoun stopping the program if an error occurs
void unwall_up(maze_t maze, int x, int y);

// remove the wall at the bottom of the cell x, y
// maze : the maze
// x, y : coordinates of the cell
// display an error message withoun stopping the program if an error occurs
void unwall_down(maze_t maze, int x, int y);

// remove the wall to the left of the cell x, y
// maze : the maze
// x, y : coordinates of the cell
// display an error message withoun stopping the program if an error occurs
void unwall_left(maze_t maze, int x, int y);

// remove the wall to the right of the cell x, y
// maze : the maze
// x, y : coordinates of the cell
// display an error message withoun stopping the program if an error occurs
void unwall_right(maze_t maze, int x, int y);

// returns if the wall at the top of the cell x, y is present
// maze : the maze
// x, y : coordinates of the cell
// display an error message withoun stopping the program if an error occurs
bool has_wall_up(maze_t maze, int x, int y);

// returns if the wall at the bottom of the cell x, y is present
// maze : the maze
// x, y : coordinates of the cell
// display an error message withoun stopping the program if an error occurs
bool has_wall_down(maze_t maze, int x, int y);

// returns if the wall to the left of the cell x, y is present
// maze : the maze
// x, y : coordinates of the cell
// display an error message withoun stopping the program if an error occurs
bool has_wall_left(maze_t maze, int x, int y);

// returns if the wall to the right of the cell x, y is present
// maze : the maze
// x, y : coordinates of the cell
// display an error message withoun stopping the program if an error occurs
bool has_wall_right(maze_t maze, int x, int y);

// --- VISUALISATION ---


// display the maze, returns -1 in case of error, 1 otherwise
// maze : the maze to display
// the walls of the entrance are in blue, the walls of the exit in green
// the size of each cell is automatically adjusted to fill the window if possible (may not be possible if the maze is
// too large) the program will stop when the user closes the window, press ESCAPE or ENTER
int print_maze(maze_t maze);

// display the maze, returns -1 in case of error, 1 otherwise
// assigns the renderer and window to the pointers renderer and window
// maze : the maze to display
// renderer : the renderer to use
// window : the window to use
// dw : the width of each cell
// dh : the height of each cell
// the walls of the entrance are in blue, the walls of the exit in green
// the size of each cell is automatically adjusted to fill the window if possible (may not be possible if the maze is
// too large)
// do not print interieur of the maze
int pre_print_maze(maze_t maze, SDL_Renderer** renderer, SDL_Window** window, int* dw, int* dh);

// display the maze, returns -1 in case of error, 1 otherwise
// assigns the renderer and window to the pointers renderer and window
// maze : the maze to display
// renderer : the renderer to use
// window : the window to use
// dw : the width of each cell
// dh : the height of each cell
// the walls of the entrance are in blue, the walls of the exit in green
// the size of each cell is automatically adjusted to fill the window if possible (may not be possible if the maze is
// too large)
int initial_print_maze(const maze_t maze, SDL_Renderer** renderer, SDL_Window** window, int* dw, int* dh);

// free the memory allocated for the maze display and close the window
// renderer : the renderer to free
// window : the window to free
void destroy_print_maze(SDL_Renderer* renderer, SDL_Window* window);

// free the memory allocated for the maze display and wait for the user to close the window
// the user can also press ESCAPE or ENTER to close the window
// renderer : the renderer to free
// window : the window to free
void wait_and_destroy_print_maze(SDL_Renderer* renderer, SDL_Window* window);

// --- BOOL_TAB ---

// create a boolean table with all values set to false
// width : width of the table
// height : height of the table
// returns the boolean table
bool_tab create_booltab(int width, int height);

// free the memory allocated for the boolean table
// tab : the boolean table to free
void free_booltab(bool_tab tab);

// set the value of the cell x, y of the boolean table to true
// tab : the boolean table
// x, y : the coordinates of the cell
// display an error message without stopping the program if an error occurs
void set_true(bool_tab tab, int x, int y);

// set the value of the cell x, y of the boolean table to false
// tab : the boolean table
// x, y : the coordinates of the cell
// display an error message without stopping the program if an error occurs
void set_false(bool_tab tab, int x, int y);

// get the value of the cell x, y of the boolean table
// tab : the boolean table
// x, y : the coordinates of the cell
// display an error message and stop the program if an error occurs
bool get_bool(bool_tab tab, int x, int y);


// --- WAYTAB ---

// create a table of way
// width : width of the table
// height : height of the table
// returns the table of way
waytab create_waytab(int width, int height);

// free the memory allocated for the table of way
// tab : the table of way to free
void free_waytab(waytab tab);

// returns true if the cell x, y of the table has a way to the start (0, 0), false otherwise
// tab : the table of way
// x, y : the coordinates of the cell
// display an error message and stop the program if an error occurs
bool has_way(waytab tab, int x, int y);

// returns the way of the cell x, y of the table
// tab : the table of way
// x, y : the coordinates of the cell
// display an error message and stop the program if an error occurs
way* get_way(waytab tab, int x, int y);

// connect the cell x, y of the table to the new father
// tab : the table of way
// x, y : the coordinates of the cell
// dad_x, dad_y : the coordinates of the new father
// does not check if the path is valid
// display an error message without stopping the program if an error occurs
void connected_way(waytab tab, int x, int y, int dad_x, int dad_y);

// returns the length of the way of the cell x, y of the table
// tab : the table of way
// x, y : the coordinates of the cell
// display an error message and stop the program if an error occurs
unsigned int length_waytab(waytab tab, int x, int y);


// --- WAY ---

// create a way
// returns the way
way* create_way();

// print the way
// w : the way
void print_way(const way* w);

// returns the length of the way
// w : the way
// the way can teleport to a cost of L_TP
unsigned int length_way(const way* w);

// connect the way to a new father
// son : the way
// dad : the new father
// does not check if the path is valid
void new_dad(way* son, way* dad);

// copy the way
// w : the way to copy
// returns the copy of the way
way* copy_way(const way* w);

// free the memory allocated for the way (and its ancestors)
// w : the way to free
// DO NOT USE THIS FUNCTION IN A WAYTAB
void free_way(way* w);

// returns true if the way is empty, false otherwise
// w : the way
bool is_empty(const way* w);

// returns the father of the way
// w : the way
// may return NULL
way* get_dad(const way* w);

// returns the x coordinate of the way
// w : the way
int get_x(const way* w);

// returns the y coordinate of the way
// w : the way
int get_y(const way* w);

// returns true if the way is the origin (0, 0), false otherwise
// w : the way
bool is_origin(const way* w);

// set the correct length of the way (and its ancestors)
// w : the way
void fix_size(way* w);

// --- QUEUE ---

// Create an empty queue.
// Returns a pointer to the queue.
// Malloc used, remember to free_queue to free memory.
queue* create_queue(void);

// Free the memory allocated for the queue.
// q : address of the queue.
void free_queue(queue* q);

// Returns the number of elements (coordinates*2) in the queue.
// q : address of the queue.
int size_queue(const queue* q);

// Returns true if the queue is empty, false otherwise.
// q : address of the queue.
bool isempty_queue(const queue* q);

// add a coordinate to the queue.
// x, y : coordinates to add.
// q : address of the queue.
void enqueue(int x, int y, queue* q);

// Returns the coordinates at the front of the queue and removes them.
// x, y : addresses of the variables that will receive the coordinates.
// q : address of the queue.
// Displays an error message and stops the program in case of error.
void dequeue(queue* q, int* x, int* y);

// Print the queue.
// q : address of the queue.
void print_queue(const queue* q);

// --- STACK ---

// Create an empty stack.
// Returns a pointer to the stack.
// Malloc used, remember to free_stack to free memory.
stack* create_stack(void);

// Free the memory allocated for the stack.
// s : address of the stack.
void free_stack(stack* s);

// Returns true if the stack is empty, false otherwise.
// s : address of the stack.
bool isempty_stack(const stack* s);

// Returns the number of elements (coordinates*2) in the stack.
// s : address of the stack.
int size_stack(const stack* s);

// Returns the coordinates at the top of the stack and removes them.
// x, y : addresses of the variables that will receive the coordinates.
// s : address of the stack.
// Displays an error message and stops the program in case of error.
void pop(stack* s, int* x, int* y);

// Returns a coordinate from somewhere (random) in the stack and removes it.
// x, y : addresses of the variables that will receive the coordinates.
// s : address of the stack.
// display an error message and stop the program in case of error.
// this function sucks, it's just here for fun (and facilitate the maze solving)
void rpop(stack* s, int* x, int* y);

// Add a coordinate to the stack.
// x, y : coordinates to add.
// s : address of the stack.
void push(int x, int y, stack* s);


#endif // CASE_H
