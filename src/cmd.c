#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/random.h>
#include "escape.h"
#include "mazemaker.h"
#include "outside.h"
#include "solveur.h"

// --- FONCTIONS AUXILIAIRES ---

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

#define GENERATE_TYPE_NB "-g <type> <width> <height> : generate maze (type : cbm, owm, ocm, hkm, bpm, crm, stm) of size width x height"
#define GENERATE_TYPE "-g <type> : generate maze (type : cbm, owm, ocm, hkm, bpm, crm, stm) of size 10x10"
#define READ_MAZE "-r <filename> : read maze from file"
#define READ_WAY "-rw <filename> : read way from file"
#define TEAR "-t <nb> : (if maze) tear the maze by removing nb%% of the walls"
#define TEAR_DEFAULT "-t : (if maze) tear the maze by removing 4%% of the walls"
#define SOLVEUR_DEFAULT "-slv <inspection> : said if the maze is perfect (inspection : isp, isc, he, she) with the best algorithm we have"
#define SOLVEUR                                                                                                                                                                    \
    "-slv <inspection> <solver> : said if the maze (inspection : isp, isc, he, she) with the (solver : deep, "                                                                     \
    "breadth, draw) algorithm"
#define ESCAPE                                                                                                                                                                     \
    "-ex <type> : escape the maze (type : random, cheat, right_hand, hunt_kill, "                                                                                                  \
    "right_hand_p_de, random_p_de) from the random position"
#define ESCAPE_DEFAULT "-ex : escape the maze with the random algorithm from a random position"
#define ESCAPE_POSITION                                                                                                                                                            \
    "-ex <type> <x> <y> : escape the maze (type : random, cheat, right_hand, hunt_kill, "                                                                                          \
    "right_hand_p_de, random_p_de) from the position x y"
#define ESCAPE_WARNING                                                                                                                                                             \
    "Warning, the escape function are experimental and may not work as expected \nThe fonction will "                                                                              \
    "always use visualisation, use the space key to disable it"
#define WRITE_MAZE "-w <filename> : write maze in file (if maze)"
#define WRITE_WAY "-ww <filename> : write way in file (if way)"
#define SHOW_DEFAULT "-sh : show maze (if maze)"
#define SHOW_ARG                                                                                                                                                                   \
    "-sh <inspection> <solveur> : show if the maze (inspection : isp, isc, he, she) with the (solver : deep, "                                                                     \
    "breadth, draw) algorithm"
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

    printf("\nTo escape a maze : (a maze must be initilazed)\n");
    printf("\t%s\n", ESCAPE);
    printf("\t%s\n", ESCAPE_DEFAULT);
    printf("\t%s\n", ESCAPE_POSITION);
    printf("\t%s\n", ESCAPE_WARNING);

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

// --- MAIN ---

int main(const int argc, char* argv[])
{
    if (argc < 2)
    {
        if (argv != NULL)
        {
            print_cmd_help(argv[0]);
        }
        return EXIT_SUCCESS;
    }
    if (argv == NULL)
    {
        fprintf(stderr, "Error : argv is NULL\n");
        return EXIT_FAILURE;
    }

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
    int x = -1;
    int y = -1;


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
                return EXIT_FAILURE;
            }

            if (i < argc - 1 && safe_atoi(argv[i + 1], &width))
            {
                if (i < argc - 2 && safe_atoi(argv[i + 2], &height))
                {
                    i += 2;
                }
                else
                {
                    fprintf(stderr, "Error : -g <type> <width> <height> : <height> hasn't been found or is not an integer\n");
                    printf("Default height applied\n");
                    height = 10;
                    i++;
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
                return EXIT_FAILURE;
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
                    return EXIT_FAILURE;
                }
            }
            if (i < argc - 2)
            {
                if (!strcmp(argv[i + 2], "deep"))
                {
                    i += 2;
                }
                else if (!strcmp(argv[i + 2], "breadth"))
                {
                    solve_type += 4;
                    i += 2;
                }
                else if (!strcmp(argv[i + 2], "draw"))
                {
                    solve_type += 8;
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
                return EXIT_FAILURE;
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
                    else if (!strcmp(argv[i + 2], "draw"))
                    {
                        type_show += 8;
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
                return EXIT_FAILURE;
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
                return EXIT_FAILURE;
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
            return EXIT_SUCCESS;
        }
        else if (!strcmp(argv[i], "-ex"))
        {
            exit = true;
            i++;
            if (i >= argc)
            {
                continue;
            }
            for (int j = 0; j < SIZE; j++)
            {
                if (!strcmp(argv[i], escape_name[j]))
                {
                    exit_type = j;
                    break;
                }
            }
            if (i < argc - 1 && safe_atoi(argv[i + 1], &x))
            {
                if (i < argc - 2 && safe_atoi(argv[i + 2], &y))
                {
                    i += 2;
                }
                else
                {
                    fprintf(stderr, "Error : -ex <type> <x> <y> : <y> hasn't been found or is not an integer\n");
                    printf("Random y applied\n");
                    i++;
                }
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
            char r;
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
        else if (!strcmp(generator, "goose"))
        {
            printf("Easter egg ! Un générateur caché va être choisit au hasard : \t");
            char r;
            getrandom(&r, sizeof(r), 0);
            r %= 7;
            switch (r)
            {
            case 0:
                printf("Labyrinthe escargot\n");
                maze = snail_maze(width, height);
                break;
            case 1:
                printf("Labyrinthe serpent (on ne peut pas toujours gagner)\n");
                maze = snake_maze(width, height);
                break;
            case 2:
                printf("Labyrinthe Comb inversé\n");
                maze = reverse_comb_maze(width, height);
                break;
            case 3:
                printf("Labyrinthe récursif (vive les carrés)\n");
                maze = reccursive_maze(width, height);
                break;
            case 4:
                printf("Labyrinthe saule pleureur (si si, c'est très ressemblant)\n");
                maze =weeping_willow_maze(width, height);
                break;
            case 5:
                printf("Labyrinthe octopus (mais en plus raide)\n");
                maze = octopus_maze(width, height);
                break;
            default: // mon chouchou
                printf("Fusion !\n");
                maze = four_maze(width, height);
                break;
            }
        }
        else
        {
            fprintf(stderr, "Error : -g <type> : %s is not a valid type\n", generator);
            printf("usage : %s\n", GENERATE_TYPE);
            printf("usage : %s\n", GENERATE_TYPE_NB);
            return EXIT_FAILURE;
        }
        is_maze = true;
        printf("Maze generated\n");
    }
    if (read)
    {
        if (is_maze)
        {
            fprintf(stderr, "Error : -r <filename> & -g <type> superposition : only one maze can be generated or read\n");
            return EXIT_FAILURE;
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
            return EXIT_FAILURE;
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
            fprintf(stderr, "Error : -slv : no maze to solve\n");
            return EXIT_FAILURE;
        }
        switch (solve_type)
        {
        case 1:
        case -1:
            is_perfect_deep_inspector(maze) ? printf("The maze is perfect\n") : printf("The maze is not perfect\n");
            break;
        case 2:
        case -2:
            is_connexe_deep_inspector(maze) ? printf("The maze is connexe\n") : printf("The maze is not connexe\n");
            break;
        case 3:
        case -3:
            has_exit_deep_seeker(maze) ? printf("The maze has an exit\n") : printf("The maze has no exit\n");
            break;
        case 4:
            if (is_way)
            {
                fprintf(stderr, "Error : -rw <filename> & -slv she supperposition : only one way can be read or generated\n");
                return EXIT_FAILURE;
            }
            w = best_exit_deep_seeker(maze);
            if (is_empty(w))
            {
                printf("No way found\n");
            }
            else
            {
                printf("Way found in %d steps\n", length_way(w));
                is_way = true;
            }
            break;
        case 5:
            is_perfect_breadth_inspector(maze) ? printf("The maze is perfect\n") : printf("The maze is not perfect\n");
            break;
        case 6:
            is_connexe_breadth_inspector(maze) ? printf("The maze is connexe\n") : printf("The maze is not connexe\n");
            break;
        case 7:
            has_exit_breadth_seeker(maze) ? printf("The maze has an exit\n") : printf("The maze has no exit\n");
            break;
        case 8:
        case -4:
            if (is_way)
            {
                fprintf(stderr, "Error : -rw <filename> & -slv she supperposition : only one way can be read or generated\n");
                return EXIT_FAILURE;
            }
            w = best_exit_breadth_seeker(maze);
            if (is_empty(w))
            {
                printf("No way found\n");
            }
            else
            {
                printf("Way found in %d steps\n", length_way(w));
                is_way = true;
            }
            break;
        case 9:
            is_perfect_draw_inspector(maze) ? printf("The maze is perfect\n") : printf("The maze is not perfect\n");
            break;
        case 10:
            is_connexe_draw_inspector(maze) ? printf("The maze is connexe\n") : printf("The maze is not connexe\n");
            break;
        case 11:
            has_exit_draw_seeker(maze) ? printf("The maze has an exit\n") : printf("The maze has no exit\n");
        case 12:
            if (is_way)
            {
                fprintf(stderr, "Error : -rw <filename> & -slv she supperposition : only one way can be read or generated\n");
                return EXIT_FAILURE;
            }
            w = best_exit_draw_seeker(maze);
            if (is_empty(w))
            {
                printf("No way found\n");
            }
            else
            {
                printf("Way found in %d steps\n", length_way(w));
                is_way = true;
            }
            break;
        default:
            // théoriquement impossible
            fprintf(stderr, "Error : -slv misused, <inspection> <solver> : %d is not a valid type\n", solve_type);
            printf("usage : %s\n", SOLVEUR);
            printf("usage : %s\n", SOLVEUR_DEFAULT);
            return EXIT_FAILURE;
        }
    }
    if (write)
    {
        if (!is_maze)
        {
            fprintf(stderr, "Error : -w <filename> : no maze to write\n");
            return EXIT_FAILURE;
        }
        maze_to_file(maze, filename_write);
        printf("Maze written in %s\n", filename_write);
    }
    if (show)
    {
        if (!is_maze)
        {
            fprintf(stderr, "Error : -sh <type> : no maze to show\n");
            return EXIT_FAILURE;
        }
        switch (type_show)
        {
        case 0:
            print_maze(maze);
            break;
        case 1:
        case -1:
            show_is_perfect_deep_inspector(maze);
            break;
        case 2:
        case -2:
            show_is_connexe_deep_inspector(maze);
            break;
        case 3:
        case -3:
            show_has_exit_deep_seeker(maze);
            break;
        case 4:
            show_best_exit_deep_seeker(maze);
            break;
        case 5:
            show_is_perfect_breadth_inspector(maze);
            break;
        case 6:
            show_is_connexe_breadth_inspector(maze);
            break;
        case 7:
            show_has_exit_breadth_seeker(maze);
            break;
        case 8:
        case -4:
            show_best_exit_breadth_seeker(maze);
            break;
        case 9:
            show_is_perfect_draw_inspector(maze);
            break;
        case 10:
            show_is_connexe_draw_inspector(maze);
            break;
        case 11:
            show_has_exit_draw_seeker(maze);
            break;
        case 12:
            show_best_exit_draw_seeker(maze);
            break;
        default:
            // théoriquement impossible
            fprintf(stderr, "Error : -sh <type> : %d is no reconized as type\n", type_show);
            printf("usage : %s\n", SHOW_DEFAULT);
            printf("usage : %s\n", SHOW_ARG);
            printf("usage : %s\n", SHOW_ARG_DEFAULT);
            return EXIT_FAILURE;
        }
    }
    if (write_way)
    {
        if (!is_way)
        {
            fprintf(stderr, "Error : -ww <filename> : no way to write\n");
            return EXIT_FAILURE;
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
            return EXIT_FAILURE;
        }
        if (!is_way)
        {
            fprintf(stderr, "Error : -shw : no way to show\n");
            return EXIT_FAILURE;
        }
        show_the_way(maze, w);
    }
    if (exit)
    {
        if (!is_maze)
        {
            fprintf(stderr, "Error : -ex : no maze to exit\n");
            return EXIT_FAILURE;
        }
        if (x == -1)
        {
            getrandom(&x, sizeof(x), 0);
            x = abs(x) % maze.width;
        }
        if (y == -1)
        {
            getrandom(&y, sizeof(y), 0);
            y = abs(y) % maze.height;
        }
        if (x < 0 || x > maze.width)
        {
            fprintf(stdout, "Warning : x must be between 0 and %d, detected value : %d\n", maze.width - 1, x);
            printf("random x applied\n");
            getrandom(&x, sizeof(x), 0);
            x = abs(x) % maze.width;
        }
        if (y < 0 || y > maze.height)
        {
            fprintf(stdout, "Warning : y must be between 0 and %d, detected value : %d\n", maze.height - 1, y);
            printf("random y applied\n");
            getrandom(&y, sizeof(y), 0);
            y = abs(y) % maze.height;
        }
        int step = 0;
        step = (*escape[exit_type])(maze, x, y);
        if (step != -1)
        {
            printf("The exit has been found in %d steps\n", step);
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
    return EXIT_SUCCESS;
}
