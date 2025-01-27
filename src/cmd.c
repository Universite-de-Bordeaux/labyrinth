#include "cmd.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mazemaker.h"
#include "outside.h"
#include "escape.h"

#include <sys/random.h>

#include "solveur.h"

// convert a string to an integer safely
// str : string to convert
// out : the integer
// return true if the conversion is successful, false otherwise
// if the conversion is not successful, out is set to 0
static bool safe_atoi(const char* str, int* out)
{
    char* endptr;
    const long val = strtol(str, &endptr, 10);
    if (errno == ERANGE || val > INT_MAX || val < INT_MIN || *endptr != '\0' || str == endptr)
    {
        *out = 0;
        return false;
    }
    *out = (int)val;
    return true;
}
#define GENERATE_TYPE_NB "-g <type> <nb> <nb> : generate maze (type : cbm, owm, ocm, hkm, bpm, crm, sym) of size nb x nb"
#define GENERATE_TYPE "-g <type> : generate maze (type : cbm, owm, ocm, hkm, bpm, crm, sym) of size 10x10"
#define READ_MAZE "-r <filename> : read maze from file"
#define READ_WAY "-rw <filename> : read way from file"
#define TEAR "-t <nb> : (if maze) tear the maze by removing nb%% of the walls"
#define TEAR_DEFAULT "-t : (if maze) tear the maze by removing 4%% of the walls"
#define SOLVEUR_DEFAULT "-slv <inspection> : said if the maze is perfect (inspection : isp, isc, he, she) with the best algorithm we have"
#define SOLVEUR                                                                                                                                                                    \
    "-slv <inspection> <solver> : said if the maze (inspection : isp, isc, he, she) with the (solver : deep, "                                                                     \
    "breadth) algorithm"
#define WRITE_MAZE "-w <filename> : write maze in file (if maze)"
#define WRITE_WAY "-ww <filename> : write way in file (if way)"
#define SHOW_DEFAULT "-sh : show maze (if maze)"
#define SHOW_ARG                                                                                                                                                                   \
    "-sh <inspection> <solveur> : show if the maze (inspection : isp, isc, he, she) with the (solver : deep, "                                                                     \
    "breadth) algorithm"
#define SHOW_ARG_DEFAULT "-sh <inspection> : show if the maze (inspection : isp, isc, he, she) with the best algorithm we have"
#define SHOW_WAY "-shw : show way (if way & maze)"
#define HELP "-h : help (will override any other command)"

// print the help of the command
static void print_cmd_help(char* namefile)
{
    printf("\nUsage %s: \n", namefile);
    printf("\nTo make a maze : \n");
    printf("\t%s\n", GENERATE_TYPE_NB);
    printf("\t%s\n", GENERATE_TYPE);
    printf("\t%s\n", READ_MAZE);
    printf("\t%s\n", TEAR);
    printf("\t%s\n", TEAR_DEFAULT);

    printf("\nTo make a way : \n");
    printf("\t%s\n", READ_WAY);
    printf("The she solver (ex : -slv she deep will also register the way)\n");

    printf("\n\tTo analyse a maze : (a maze must be initilazed)\n");
    printf("\t%s\n", SOLVEUR);
    printf("\t%s\n", SOLVEUR_DEFAULT);

    printf("\nTo write: \n");
    printf("\t%s\n", WRITE_MAZE);
    printf("\t%s\n", WRITE_WAY);

    printf("\nTo show (a maze must be initilazed) : \n");
    printf("\t%s\n", SHOW_WAY);
    printf("\t%s\n", SHOW_DEFAULT);
    printf("\t%s\n", SHOW_ARG);
    printf("\t%s\n", SHOW_ARG_DEFAULT);

    printf("\n\t%s\n", HELP);
    printf("\nArguments order doesn't matter, but only one maze and one way are allowed\n");
    printf("If an argument occures multiple times, the last one will be taken\n");
    printf("If an argument is unknown, it will be ignored\n");

    printf("See the README for more informations\n");
}

void cmd(char* argv[], const int argc)
{
    if (argc < 2)
    {
        if (argv != NULL)
        {
            print_cmd_help(argv[0]);
        }
        return;
    }
    if (argv == NULL)
    {
        fprintf(stderr, "Error : argv is NULL\n");
        return;
    }
    //-g <type> -> generate maze (if type) powm, iowm, hkm, bpm, lm, cmr
    //-r <filename> -> read maze
    //-slv <inspection> <solver> : said if the maze (inspection) isp, isc, he, she with the (solver)
    //-w <filename> -> write maze in file (if maze)
    //-sh -> show maze
    //-sh <inspection> <SOLVEUR> : show ifthe maze (inspection) isp, isc, he, she with the (solver)
    //-ww <filename> -> write way in file (if way)
    //-rw <filename> -> read way in file (if way)
    //-shw -> show way (if way & maze)
    //-h -> help (only if no other command)
    //-t <nb> -> tear the maze by removing nb% of the walls (if maze)

    // initialisation des variables
    bool generate = false; // -g
    char* generator = '\0'; // <type>
    int width = 0; // <nb>
    int height = 0; // <nb>

    bool must_tear = false; // -t
    int tear_prop = 4; // <nb>

    bool read = false; // -r
    const char* filename = '\0'; // <filename>

    bool is_maze = false; // internal
    maze_t maze; // maze (internal)

    bool solve = false; // -slv
    int solve_type = 0; // <nb>

    bool read_way = false; // -rw
    const char* filename_read_way = '\0'; // <filename>

    bool is_way = false; // internal
    way* w; // way (internal)

    bool write = false; // -w
    const char* filename_write = '\0'; // <filename>

    bool write_way = false; // -ww
    const char* filename_write_way = '\0'; // <filename>

    bool show = false; // -sh
    int type_show = 0; // <nb>

    bool show_way = false; // -shw

    bool exit = false; // -ex
    int exit_type = 0; // <type>


    // tri des arguments
    int i = 1;
    while (i < argc)
    {
        if (!strcmp(argv[i], "-g"))
        {
            generate = true;
            if (i < argc - 1)
            {
                generator = argv[i + 1];
                i++;
            }
            else
            {
                fprintf(stderr, "Error : -g misused, <type> missing\n");
                printf("usage : %s\n", GENERATE_TYPE);
                printf("usage : %s\n", GENERATE_TYPE_NB);
                return;
            }

            if (i < argc - 1 && safe_atoi(argv[i + 1], &width))
            {
                if (i < argc - 2 && safe_atoi(argv[i + 2], &height))
                {
                    i += 2;
                }
                else
                {
                    fprintf(stderr, "Error : -g <type> <nb1> <nb2> : <nb2> hasn't been found or is not an integer\n");
                    printf("Default height applied\n");
                    height = 10;
                }
            }
            else
            {
                width = 10;
                height = 10;
            }
        }
        else if (!strcmp(argv[i], "-r"))
        {
            read = true;
            if (i < argc - 1)
            {
                filename = argv[i + 1];
                i++;
            }
            else
            {
                fprintf(stderr, "Error : -r misused, <filename> missing\n");
                printf("usage : %s\n", READ_MAZE);
                return;
            }
        }
        else if (!strcmp(argv[i], "-t"))
        {
            must_tear = true;
            if (i < argc - 1 && safe_atoi(argv[i + 1], &tear_prop))
            {
                i++;
            }
            else
            {
                tear_prop = 4;
            }
        }
        else if (!strcmp(argv[i], "-slv"))
        {
            solve = true;
            if (i < argc - 1)
            {
                if (!strcmp(argv[i + 1], "isp"))
                {
                    solve_type = 1;
                }
                else if (!strcmp(argv[i + 1], "isc"))
                {
                    solve_type = 2;
                }
                else if (!strcmp(argv[i + 1], "he"))
                {
                    solve_type = 3;
                }
                else if (!strcmp(argv[i + 1], "she"))
                {
                    solve_type = 4;
                }
                else
                {
                    fprintf(stderr, "Error : -slv <inspection> : %s is not a valid inspection\n", argv[i + 1]);
                    printf("usage : %s\n", SOLVEUR);
                    printf("usage : %s\n", SOLVEUR_DEFAULT);
                    return;
                }
            }
            if (i < argc - 2)
            {
                printf("i < argc - 2 : %d %d\n", i, argc - 2);
                if (!strcmp(argv[i + 2], "deep"))
                {
                    i += 2;
                }
                else if (!strcmp(argv[i + 2], "breadth"))
                {
                    solve_type += 4;
                    i += 2;
                }
                else
                {
                    solve_type = -solve_type;
                    i++;
                }
            }
            else
            {
                solve_type = -solve_type;
                i++;
            }
        }
        else if (!strcmp(argv[i], "-w"))
        {
            write = true;
            if (i < argc - 1)
            {
                filename_write = argv[i + 1];
                i++;
            }
            else
            {
                fprintf(stderr, "Error : -w misused, <filename> missing\n");
                printf("usage : %s\n", WRITE_MAZE);
                return;
            }
        }
        else if (!strcmp(argv[i], "-sh"))
        {
            show = true;
            if (i < argc - 1)
            {
                if (!strcmp(argv[i + 1], "isp"))
                {
                    type_show = 1;
                }
                else if (!strcmp(argv[i + 1], "isc"))
                {
                    type_show = 2;
                }
                else if (!strcmp(argv[i + 1], "he"))
                {
                    type_show = 3;
                }
                else if (!strcmp(argv[i + 1], "she"))
                {
                    type_show = 4;
                }
                else
                {
                    type_show = 0;
                }
            }
            if (type_show != 0)
            {
                if (i < argc - 2)
                {
                    if (!strcmp(argv[i + 2], "deep"))
                    {
                        i += 2;
                    }
                    else if (!strcmp(argv[i + 2], "breadth"))
                    {
                        type_show += 4;
                        i += 2;
                    }
                    else
                    {
                        type_show = -type_show;
                        i++;
                    }
                }
                else
                {
                    type_show = -type_show;
                    i++;
                }
            }
        }
        else if (!strcmp(argv[i], "-ww"))
        {
            write_way = true;
            if (i < argc - 1)
            {
                filename_write_way = argv[i + 1];
                i++;
            }
            else
            {
                fprintf(stderr, "Error : -ww misused, <filename> missing\n");
                printf("usage : %s\n", WRITE_WAY);
                return;
            }
        }
        else if (!strcmp(argv[i], "-rw"))
        {
            read_way = true;
            if (i < argc - 1)
            {
                filename_read_way = argv[i + 1];
                i++;
            }
            else
            {
                fprintf(stderr, "Error : -rw misused, <filename> missing\n");
                printf("usage : %s\n", READ_WAY);
                return;
            }
        }
        else if (!strcmp(argv[i], "-shw"))
        {
            show_way = true;
        }
        else if (!strcmp(argv[i], "-h"))
        {
            if (argc != 2)
            {
                fprintf(stderr, "Warning : -h will override any other command\n");
            }
            print_cmd_help(argv[0]);
            return;
        }
        else if (!strcmp(argv[i], "-ex"))
        {
            exit = true;
            i++;
            if (i >= argc)
            {
                continue;
            }
            if (!strcmp(argv[i], "random"))
            {
                exit_type = 0;
            }
            else if (!strcmp(argv[i], "try_direction"))
            {
                exit_type = 1;
            }
            else if (!strcmp(argv[i], "cheat"))
            {
                exit_type = 2;
            }
            else if(!strcmp(argv[i], "hunt_kill"))
            {
                exit_type = 3;
            }
        }
        else
        {
            fprintf(stderr, "Error : %s is not a valid command\n", argv[i]);
        }
        i++;
    }

    // traitements des arguments
    if (generate)
    {
        if (!strcmp(generator, "cbm"))
        {
            maze = comb_maze(width, height);
        }
        else if (!strcmp(generator, "owm"))
        {
            maze = one_way_maze(width, height);
        }
        else if (!strcmp(generator, "ocm"))
        {
            maze = my_octopus_maze(width, height);
        }
        else if (!strcmp(generator, "hkm"))
        {
            maze = hunt_kill_maze(width, height);
        }
        else if (!strcmp(generator, "bpm"))
        {
            maze = by_path_maze(width, height);
        }
        else if (!strcmp(generator, "stm"))
        {
            unsigned short int r;
            getrandom(&r, sizeof(r), 0);
            if (r % 2)
            {
                maze = line_maze(width, height);
            }
            else
            {
                maze = column_maze(width, height);
            }
        }
        else if (!strcmp(generator, "crm"))
        {
            maze = cross_maze(width, height);
        }
        else
        {
            fprintf(stderr, "Error : -g <type> : %s is not a valid type\n", generator);
            printf("usage : %s\n", GENERATE_TYPE);
            printf("usage : %s\n", GENERATE_TYPE_NB);
            return;
        }
        is_maze = true;
        printf("Maze generated\n");
    }
    if (read)
    {
        if (is_maze)
        {
            fprintf(stderr, "Error : -r <filename> & -g <type> superposition : only one maze can be generated or read\n");
            return;
        }
        maze = maze_from_file(filename);
        is_maze = true;
        printf("Maze extract from %s\n", filename);
    }
    if (must_tear)
    {
        if (is_maze)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            tear(maze, tear_prop);
            printf("Maze teared\n");
        }
        else
        {
            fprintf(stderr, "Error : -t <nb> : no maze to tear\n");
            return;
        }
    }
    if (read_way)
    {
        w = way_from_file(filename_read_way);
        is_way = true;
        printf("Way extract from %s\n", filename_read_way);
    }
    if (solve)
    {
        if (!is_maze)
        {
            fprintf(stderr, "Error : -slv <nb> : no maze to solve\n");
            return;
        }
        if (solve_type == 1 || solve_type == -1)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            if (is_perfect_deep_inspector(maze))
            {
                printf("The maze is perfect\n");
            }
            else
            {
                printf("The maze is not perfect\n");
            }
        }
        else if (solve_type == 2 || solve_type == -2)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            if (is_connexe_deep_inspector(maze))
            {
                printf("The maze is connected\n");
            }
            else
            {
                printf("The maze is not connected\n");
            }
        }
        else if (solve_type == 3 || solve_type == -3)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            if (has_exit_deep_seeker(maze))
            {
                printf("The maze has an exit\n");
            }
            else
            {
                printf("The maze has no exit\n");
            }
        }
        else if (solve_type == 4)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            w = best_exit_deep_seeker(maze);
            if (is_empty(w))
            {
                printf("The maze has no exit\n");
            }
            else
            {
                is_way = true;
                printf("way found and saved\n");
            }
        }
        else if (solve_type == 5)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            if (is_perfect_breadth_inspector(maze))
            {
                printf("The maze is perfect\n");
            }
            else
            {
                printf("The maze is not perfect\n");
            }
        }
        else if (solve_type == 6)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            if (is_connexe_breadth_inspector(maze))
            {
                printf("The maze is connected\n");
            }
            else
            {
                printf("The maze is not connected\n");
            }
        }
        else if (solve_type == 7)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            if (has_exit_breadth_seeker(maze))
            {
                printf("The maze has an exit\n");
            }
            else
            {
                printf("The maze has no exit\n");
            }
        }
        else if (solve_type == 8 || solve_type == -4)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            w = best_exit_breadth_seeker(maze);
            if (is_empty(w))
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
            // théoriquement impossible
            fprintf(stderr, "Error : -slv misused, <inspection> <solver> : %d is not a valid type\n", solve_type);
            printf("usage : %s\n", SOLVEUR);
            printf("usage : %s\n", SOLVEUR_DEFAULT);
            return;
        }
    }
    if (write)
    {
        if (!is_maze)
        {
            fprintf(stderr, "Error : -w <filename> : no maze to write\n");
            return;
        }
        // ReSharper disable once CppLocalVariableMightNotBeInitialized
        maze_to_file(maze, filename_write);
        printf("Maze written in %s\n", filename_write);
    }
    if (show)
    {
        if (!is_maze)
        {
            fprintf(stderr, "Error : -sh <nb> : no maze to show\n");
            return;
        }
        if (type_show == 0)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            print_maze(maze);
        }
        else if (type_show == 1 || type_show == -1)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            show_is_perfect_deep_inspector(maze);
        }
        else if (type_show == 2 || type_show == -2)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            show_is_connexe_deep_inspector(maze);
        }
        else if (type_show == 3 || type_show == -3)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            show_has_exit_deep_seeker(maze);
        }
        else if (type_show == 4)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            show_best_exit_deep_seeker(maze);
        }
        else if (type_show == 5)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            show_is_perfect_breadth_inspector(maze);
        }
        else if (type_show == 6)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            show_is_connexe_breadth_inspector(maze);
        }
        else if (type_show == 7)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            show_has_exit_breadth_seeker(maze);
        }
        else if (type_show == 8 || type_show == -4)
        {
            // ReSharper disable once CppLocalVariableMightNotBeInitialized
            show_best_exit_breadth_seeker(maze);
        }
        else
        {
            // théoriquement impossible
            fprintf(stderr, "Error : -sh <nb> : %d is no reconized as type\n", type_show);
            printf("usage : %s\n", SHOW_DEFAULT);
            printf("usage : %s\n", SHOW_ARG);
            printf("usage : %s\n", SHOW_ARG_DEFAULT);
            return;
        }
    }
    if (write_way)
    {
        if (!is_way)
        {
            fprintf(stderr, "Error : -ww <filename> : no way to write\n");
            return;
        }
        // ReSharper disable once CppLocalVariableMightNotBeInitialized
        way_to_file(w, filename_write_way);
        printf("Way written in %s\n", filename_write_way);
    }
    if (show_way)
    {
        if (!is_maze)
        {
            fprintf(stderr, "Error : -shw : no maze to show way\n");
            return;
        }
        if (!is_way)
        {
            fprintf(stderr, "Error : -shw : no way to show\n");
            return;
        }
        // ReSharper disable twice CppLocalVariableMightNotBeInitialized
        show_the_way(maze, w);
    }
    if (exit)
    {
        if (!is_maze)
        {
            fprintf(stderr, "Error : -ex : no maze to exit\n");
            return;
        }

        int x, y;
        getrandom(&x, sizeof(x), 0);
        x = abs(x) % maze.width;
        getrandom(&y, sizeof(y), 0);
        y = abs(y) % maze.height;

        if (exit_type == 0)
        {
            random_escape(maze, x, y);
        }
        else if (exit_type == 1)
        {
            try_direction(maze, x, y);
        }
        else if (exit_type == 2)
        {
            cheat_escape(maze, x, y);
        }
        else if (exit_type == 3){
            hunt_kill_escape(maze, x, y);
        }
        else // théoriquement impossible
        {
            printf("Error : -ex <type> : %d is not a valid type\n", exit_type);
        }
    }
    if (is_maze)
    {
        free_maze(maze);
    }
    if (is_way)
    {
        free_way(w);
    }
}