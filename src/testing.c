#include "test.h"
#include <errno.h>
#include <limits.h>
#include <sys/random.h>

#include "mazemaker.h"
//comparable à cmd.c, ce fichier va uniquement gérer les arguments de la ligne de commande

static void usage(const char* name)
{
    printf("usage : %s\n", name);

    printf(".\\%s -h or .\\%s--help : display this help\n", name, name);
    printf(".\\%s <generator> <nb> [options] : evaluate the generator\n", name);

    printf("\nfixed tests :\n");
    printf("--evaluated or -e : trigger the general evaluation\n");
    printf("--timed or -t : trigger the time evaluation\n");
    printf("<nb> won't be used\n");

    printf("\nspecific tests :\n");
    printf("--evaluate_solvable or -es : trigger the evaluation of solvability\n");
    printf("--evaluate_connected or -ec : trigger the evaluation of connectivity\n");
    printf("--evaluate_perfect or -ep : trigger the evaluation of perfection\n");
    printf("an negative result triggers a visualisation of the problem\n");
    printf("warning : with a too high <nb>, the program may take a long time to execute\n");

    printf("\n<nb> must be an integer, its default value is 100\n");
    printf("<generator> must be a valid generator :\n");
    printf("\t b : basic maze\n");
    printf("\t m : basic walled maze\n");
    printf("\t cbm : comb maze\n");
    printf("\t own : one way maze\n");
    printf("\t ocm : octopus maze\n");
    printf("\t hkm : hunt kill maze\n");
    printf("\t bpm : by path maze\n");
    printf("\t sm : straight maze\n");
    printf("\t cm : cross maze\n");
}

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

int main(const int argc, char* argv[])
{
    bool evaluate = false;
    bool timed = false;
    bool evaluate_solvable = false;
    bool evaluate_connected = false;
    bool evaluate_perfect = false;

    // récupération des arguments
    if (argc < 3)
    {
        usage(argv[0]);
        return EXIT_FAILURE;
    }
    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
    {
        usage(argv[0]);
        return EXIT_SUCCESS;
    }
    char* generator = argv[1];
    int i = 3;
    int nb;
    if (!safe_atoi(argv[2], &nb))
    {
        nb = 100;
        i = 2;
    }
    for (; i < argc; i++)
    {
        if (!strcmp(argv[i], "--evaluated") || !strcmp(argv[i], "-e"))
        {
            evaluate = true;
        }
        else if (!strcmp(argv[i], "--timed") || !strcmp(argv[i], "-t"))
        {
            timed = true;
        }
        else if (!strcmp(argv[i], "--evaluate_solvable") || !strcmp(argv[i], "-es"))
        {
            evaluate_solvable = true;
        }
        else if (!strcmp(argv[i], "--evaluate_connected") || !strcmp(argv[i], "-ec"))
        {
            evaluate_connected = true;
        }
        else if (!strcmp(argv[i], "--evaluate_perfect") || !strcmp(argv[i], "-ep"))
        {
            evaluate_perfect = true;
        }
        else
        {
            printf("Error : %s is not a valid command\n", argv[i]);
        }
    }
    // récupération du générateur
    func_ptr f;
    if (!strcmp(generator, "b"))
    {
        f = create_basic_maze;
    }
    else if (!strcmp(generator, "m"))
    {
        f = create_wall_maze;
    }
    else if (!strcmp(generator, "cbm"))
    {
        f = comb_maze;
    }
    else if (!strcmp(generator, "own"))
    {
        f = one_way_maze;
    }
    else if (!strcmp(generator, "ocm"))
    {
        f = my_octopus_maze;
    }
    else if (!strcmp(generator, "hkm"))
    {
        f = hunt_kill_maze;
    }
    else if (!strcmp(generator, "bpm"))
    {
        f = by_path_maze;
    }
    else if (!strcmp(generator, "sm"))
    {
        short int r;
        getrandom(&r, sizeof(r), 0);
        if (r % 2)
        {
            f = line_maze;
        }
        else
        {
            f = column_maze;
        }
    }
    else if (!strcmp(generator, "cm"))
    {
        f = cross_maze;
    }
    else
    {
        printf("Error : %s is not a valid generator\n", generator);
        usage(argv[0]);
        return EXIT_FAILURE;
    }
    // évaluation
    if (evaluate)
    {
        EVALUATE_MAZEMAKERN(f, generator);
    }
    if (timed)
    {
        evaluate_time(f, generator);
    }
    if (evaluate_solvable)
    {
        if (do_made_solvable_maze(f, nb, nb))
        {
            printf("%s seems to create solvable mazes\n", generator);
        }
        else
        {
            printf("%s do not create solvable mazes\n", generator);
        }
    }
    if (evaluate_connected)
    {
        if(do_made_connected_maze(f, nb, nb))
        {
            printf("%s seems to create connected mazes\n", generator);
        }
        else
        {
            printf("%s do not create connected mazes\n", generator);
        }
    }
    if (evaluate_perfect)
    {
        if (do_made_perfect_maze(f, nb, nb))
        {
            printf("%s seems to create perfect mazes\n", generator);
        }
        else
        {
            printf("%s do not create perfect mazes\n", generator);
        }
    }
    return EXIT_SUCCESS;
}