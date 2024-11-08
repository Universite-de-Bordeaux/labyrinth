#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <stdio.h>
#include "mazemaker.h"
#include "outside.h"
#include "cmd.h"
#include "solveur.h"

//convertit une chaine de caractère en entier de manière sécurisée
//str : chaine de caractère
//return : booléen, true si la conversion a réussi, false sinon
//en cas d'erreur, reinitialise out à 0
bool safe_atoi(const char *str, int *out)
{
    int errno;
    char *endptr;
    const long val = strtol(str, &endptr, 10);
    if(errno == ERANGE || val > INT_MAX || val < INT_MIN || *endptr != '\0' || str == endptr)
    {
        *out = 0;
        return false;
    }
    *out = (int)val;
    return true;
}

//affiche l'aide
void print_cmd_help(char* namefile)
{
    printf("\nUsage %s: \n", namefile);
    printf("\nTo make a maze : \n");
    printf("\t-g <type> <nb> <nb> : generate maze (type) powm, iowm, hkm, bpm, lm, cm (if nb) width, height\n");
    printf("\t-g <type> : generate maze (if type) powm, iowm, hkm, bpm, lm, cm of size 10x10\n");
    printf("\t-r <filename> : read maze from file\n");

    printf("\nTo make a way : \n");
    printf("\t-rw <filename> : read way from file\n");
    printf("You can also use the solve command with parameter 3 or 6 to generate a way\n");

    printf("\nTo solve a maze : (a maze must be initiated)\n");
    printf("\t-slv <nb> : solve 1 : is perfect (deep inspector), 2 : has exit (deep seeker), 3 : shortest exit (deep seeker)(and save way), 4 : is perfect (breadth inspector), 5 : has exit (breadth seeker), 6 : shortest exit (breadth seeker)(and save way)\n");


    printf("\nTo write: \n");
    printf("\t-w <filename> : write maze in file (if maze)\n");
    printf("\t-ww <filename> : write way in file (if way)\n");

    printf("\nTo show (a maze must be initiated) : \n");
    printf("\t-sh : show maze\n");
    printf("\t-sh <nb> : show 0 : show maze, 1 : is perfect (deep inspector), 2 : has exit (deep seeker), 3 : shortest exit (deep seeker), 4 : is perfect (breadth inspector), 5 : has exit (breadth seeker), 6 : shortest exit (breadth seeker)\n");
    printf("\t-shw : show way (a way must be initiated)\n");

    printf("\n\t-h : help\n");
    printf("\nArguments order doesn't matter, but only one maze and one way are allowed\n");
    printf("If an argument occures multiple times, the last one will be taken\n");
    printf("If an argument is unknown, it will be ignored\n");
}

void cmd(char *argv[], const int argc)
{
    if(argc < 2)
    {
        return;
    }
    if(argv == NULL)
    {
        fprintf(stderr, "Error : argv is NULL\n");
        return;
    }
    //-g <type> -> generate maze (if type) powm, iowm, hkm, bpm, lm, cm
    //-r <filename> -> read maze
    //-slv <nb> -> solve (if maze) 1 : is perfect, 2 : has exit, 3 : shortest exit (and save way)
    //-w <filename> -> write maze in file (if maze)
    //-sh -> show maze
    //-sh <nb> -> show (if maze) 1 : is perfect, 2 : has exit, 3 : shortest exit
    //-ww <filename> -> write way in file (if way)
    //-rw <filename> -> read way in file (if way)
    //-shw -> show way (if way & maze)
    //-h -> help (only if no other command)

    //initialisation des variables
    bool generate = false;                  // -g
    char *generator = '\0';                 // <type>
    bool is_size = false;                   // internal
    int width = 0;                          // <nb>
    int height = 0;                         // <nb>

    bool read = false;                      // -r
    const char *filename = '\0';            // <filename>

    bool is_maze = false;                   // internal
    maze_t maze;                            // maze (internal)

    bool solve = false;                     // -slv
    int solve_type = 0;                     // <nb>

    bool read_way = false;                  // -rw
    const char *filename_read_way = '\0';   // <filename>

    bool is_way = false;                    // internal
    way *w;                                 // way (internal)

    bool write = false;                     // -w
    const char *filename_write = '\0';      // <filename>

    bool write_way = false;                 // -ww
    const char *filename_write_way = '\0';  // <filename>

    bool show = false;                      // -sh
    int type_show = 0;                      // <nb>

    bool show_way = false;                  // -shw


    //tri des arguments
    int i = 1;
    while(i < argc)
    {
        if(!strcmp(argv[i], "-g"))
        {
            generate = true;
            if(i < argc - 1)
            {
                generator = argv[i+1];
                i++;
            }
            else
            {
                fprintf(stderr, "Error : -g <type> : <type> must be specified\n type : powm, iowm, hkm, bpm, lm, cm\n");
                return;
            }

            if(i < argc - 1 && safe_atoi(argv[i+1], &width))
            {
                if(i < argc - 2 && safe_atoi(argv[i+2], &height))
                {
                    is_size = true;
                    i += 2;
                }
                else
                {
                    fprintf(stderr, "Error : -g <type> <nb> <nb2> : <nb2> hasn't been found or is not an integer\n");
                    return;
                }
            }
            else
            {
                is_size = false;
            }
        }
        else if(!strcmp(argv[i], "-r"))
        {
            read = true;
            if(i < argc - 1)
            {
                filename = argv[i+1];
                i++;
            }
            else
            {
                fprintf(stderr, "Error : -r <filename> : <filename> must be specified\n");
                return;
            }
        }
        else if(!strcmp(argv[i], "-slv"))
        {
            solve = true;
            if(i < argc - 1 && safe_atoi(argv[i+1], &solve_type))
            {
                i++;
            }
            else
            {
                fprintf(stderr, "Error: -slv <nb> : <nb> must be specified as an integer\n");
                return;
            }
        }
        else if(!strcmp(argv[i], "-w"))
        {
            write = true;
            if(i < argc - 1)
            {
                filename_write = argv[i+1];
                i++;
            }
            else
            {
                fprintf(stderr, "Error : -w <filename> : <filename> must be specified\n");
                return;
            }
        }
        else if(!strcmp(argv[i], "-sh"))
        {
            show = true;
            if(i < argc - 1 && safe_atoi(argv[i+1], &type_show))
            {
                i++;
            }
            // -sh admet un argument par defaut
        }
        else if(!strcmp(argv[i], "-ww"))
        {
            write_way = true;
            if(i < argc - 1)
            {
                filename_write_way = argv[i+1];
                i++;
            }
            else
            {
                fprintf(stderr, "Error : -ww <filename> : <filename> must be specified\n");
                return;
            }
        }
        else if(!strcmp(argv[i], "-rw"))
        {
            read_way = true;
            if(i < argc - 1)
            {
                filename_read_way = argv[i+1];
                i++;
            }
            else
            {
                fprintf(stderr, "Error : -rw <filename> : <filename> must be specified\n");
                return;
            }
        }
        else if(!strcmp(argv[i], "-shw"))
        {
            show_way = true;
        }
        else if(!strcmp(argv[i], "-h"))
        {
            if(argc != 2)
            {
                fprintf(stderr, "Warning : -h must be the only command\n");
            }
            print_cmd_help(argv[0]);
            return;
        }
        else
        {
            fprintf(stderr,"Error : %s is not a valid command\n", argv[i]);
        }
        i++;
    }

    //traitements des arguments
    if(generate)
    {
        if(!is_size)
        {
            width = 10;
            height = 10;
        }
        if(!strcmp(generator, "powm"))
        {
            maze = perfect_one_way_maze(width, height);
        }
        else if(!strcmp(generator, "iowm"))
        {
            maze = imperfect_one_way_maze(width, height);
        }
        else if(!strcmp(generator, "hkm"))
        {
            maze = hunt_kill_maze(width, height);
        }
        else if(!strcmp(generator, "bpm"))
        {
            maze = by_path_maze(width, height);
        }
        else if(!strcmp(generator, "lm"))
        {
            if(rand() % 2 == 0)
            {
                maze = line_maze(width, height);
            }
            else
            {
                maze = column_maze(width, height);
            }
        }
        else if(!strcmp(generator, "cm"))
        {
            maze = cross_maze(width, height);
        }
        else
        {
            fprintf(stderr, "Error : -g <type> : %s is no reconized as type\n type : powm, iowm, hkm, bpm, lm, cm\n", generator);
            return;
        }
        is_maze = true;
        printf("Maze generated\n");
    }

    if(read)
    {
        if(is_maze)
        {
            fprintf(stderr, "Error : -r <filename> & -g <type> superposition : only one maze can be generated or read\n");
            return;
        }
        maze = maze_from_file(filename);
        is_maze = true;
        printf("Maze extract from %s\n", filename);
    }
    if(read_way)
    {
        w = way_from_file(filename_read_way);
        is_way = true;
        printf("Way extract from %s\n", filename_read_way);
    }
    if(solve)
    {
        if(!is_maze)
        {
            fprintf(stderr, "Error : -slv <nb> : no maze to solve\n");
            return;
        }
        if(solve_type == 1)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            if(is_perfect_deep_inspector(maze))
            {
                printf("The maze is perfect\n");
            }
            else
            {
                printf("The maze is not perfect\n");
            }
        }
        else if(solve_type == 2)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            if(has_exit_deep_seeker(maze))
            {
                printf("The maze has an exit\n");
            }
            else
            {
                printf("The maze has no exit\n");
            }
        }
        else if(solve_type == 3)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            w = best_exit_deep_seeker(maze);
            if(is_empty(w))
            {
                printf("The maze has no exit\n");
            }
            else
            {
                is_way = true;
                printf("way found and saved\n");
            }
        }
        else if(solve_type == 4)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            if(is_perfect_breadth_inspector(maze))
            {
                printf("The maze is perfect\n");
            }
            else
            {
                printf("The maze is not perfect\n");
            }
        }
        else if(solve_type == 5)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            if(has_exit_breadth_seeker(maze))
            {
                printf("The maze has an exit\n");
            }
            else
            {
                printf("The maze has no exit\n");
            }
        }
        else if(solve_type == 6)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            w = best_exit_breadth_seeker(maze);
            if(is_empty(w))
            {
                printf("The maze has no exit\n");
            }
            else
            {
                is_way = true;
                printf("way found and saved\n");
            }
        }
        else
        {
            fprintf(stderr, "Error : -slv <nb> : %d is no reconized as type\n type : 1 (is perfect deep), 2 (has exit deep), 3 (shortest exit deep), 4 (is perfect breadth), 5 (has exit breadth), 6 (shortest exit breadth)\n", solve_type);
            return;
        }
    }
    if(write)
    {
        if(!is_maze)
        {
            fprintf(stderr, "Error : -w <filename> : no maze to write\n");
            return;
        }
        // ReSharper disable once CppLocalVariableMightNotBeInitialized
        maze_to_file(maze, filename_write);
        printf("Maze written in %s\n", filename_write);
    }
    if(show)
    {
        if(!is_maze)
        {
            fprintf(stderr, "Error : -sh <nb> : no maze to show\n");
            return;
        }
        if(type_show == 1)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            show_is_perfect_deep_inspector(maze);
        }
        else if(type_show == 2)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            show_has_exit_deep_seeker(maze);
        }
        else if(type_show == 3)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            show_best_exit_deep_seeker(maze);
        }
        else if(type_show == 0)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            print_maze(maze);
        }
        else if(type_show == 4)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            show_is_perfect_breadth_inspector(maze);
        }
        else if(type_show == 5)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            show_has_exit_breadth_seeker(maze);
        }
        else if(type_show == 6)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            show_best_exit_breadth_seeker(maze);
        }
        else
        {
            fprintf(stderr, "Error : -sh <nb> : %d is no reconized as type\n type : 0 (show maze), 1 (is perfect deep), 2 (has exit deep), 3 (shortest exit deep), 4 (is perfect breadth), 5 (has exit breadth), 6 (shortest exit breadth)\n", type_show);
            return;
        }
    }
    if(write_way)
    {
        if(!is_way)
        {
            fprintf(stderr, "Error : -ww <filename> : no way to write\n");
            return;
        }
        // ReSharper disable once CppLocalVariableMightNotBeInitialized
        way_to_file(w, filename_write_way);
        printf("Way written in %s\n", filename_write_way);
    }
    if(show_way)
    {
        if(!is_maze)
        {
            fprintf(stderr, "Error : -shw : no maze to show way\n");
            return;
        }
        if(!is_way)
        {
            fprintf(stderr, "Error : -shw : no way to show\n");
            return;
        }
        // ReSharper disable twice CppLocalVariableMightNotBeInitialized
        show_the_way(maze, w);
    }
    if(is_maze)
    {
        free_maze(maze);
    }
    if(is_way)
    {
        free_way(w);
    }
}