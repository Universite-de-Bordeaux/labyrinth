#include "test.h"
#include <errno.h>
#include <limits.h>
#include <sys/random.h>
#include <stdbool.h>

#include "mazemaker.h"
//comparable à cmd.c, ce fichier va uniquement gérer les arguments de la ligne de commande

static void usage(const char* name)
{
    printf("usage : %s\n", name);
    printf("-h or --help : display this help\n");
    printf("<generator> <nb> [options] : evaluate the generator\n");

    printf("\n<nb> must be an integer, its default value is 100, so it can be omitted\n");
    printf("<generator> must be one of the following :\n");
    printf("\t b : basic maze\n");
    printf("\t m : basic walled maze\n");
    printf("\t cbm : comb maze\n");
    printf("\t owm : one way maze\n");
    printf("\t ocm : octopus maze\n");
    printf("\t hkm : hunt kill maze\n");
    printf("\t bpm : by path maze\n");
    printf("\t sm : straight maze\n");
    printf("\t cm : cross maze\n");

    printf("\noptions :\n");
    printf("\nfixed tests :\n");
    printf("--evaluated or -e : trigger the general evaluation\n");
    printf("--timed or -t : trigger the time evaluation\n");
    printf("<nb> won't be used here\n");

    printf("\nspecific tests :\n");
    printf("--evaluate_solvable or -es : trigger the evaluation of solvability\n");
    printf("--evaluate_connected or -ec : trigger the evaluation of connectivity\n");
    printf("--evaluate_perfect or -ep : trigger the evaluation of perfection\n");
    printf("an negative result triggers a visualisation of the problem\n");
    printf("nb is used to determine the size of the maze and the number of tests\n");
    printf("warning : the evaluation is exponential in time\n");

}

// convert a string to an integer safely
// str : string to convert
// out : the integer
// return true if the conversion is successful, false otherwise
// if the conversion is not successful, out is set to 0
static bool safe_atoi(const char* str, int* out)
{
    char* endptr;
    errno = 0;
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
    char* generator_name;
    if (!strcmp(generator, "b"))
    {
        f = create_basic_maze;
        generator_name = "basic generator";
    }
    else if (!strcmp(generator, "m"))
    {
        f = create_wall_maze;
        generator_name = "basic wall generator";
    }
    else if (!strcmp(generator, "cbm"))
    {
        f = comb_maze;
        generator_name = "comb generator";
    }
    else if (!strcmp(generator, "owm"))
    {
        f = one_way_maze;
        generator_name = "one way generator";
    }
    else if (!strcmp(generator, "ocm"))
    {
        f = my_octopus_maze;
        generator_name = "octopus generator";
    }
    else if (!strcmp(generator, "hkm"))
    {
        f = hunt_kill_maze;
        generator_name = "hunt kill generator";
    }
    else if (!strcmp(generator, "bpm"))
    {
        f = by_path_maze;
        generator_name = "by path generator";
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
        generator_name = "straight generator";
    }
    else if (!strcmp(generator, "cm"))
    {
        f = cross_maze;
        generator_name = "cross generator";
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
        printf("Evaluation of the %s, criteria : general\n", generator_name);
        EVALUATE_MAZEMAKERN(f, generator_name);
    }
    if (timed)
    {
        printf("Evaluation of the %s, criteria : time\n", generator_name);
        evaluate_time(f, generator_name);
    }
    if (evaluate_solvable)
    {
        printf("Evaluation of the %s, criteria : solvable\n", generator_name);
        if (do_made_solvable_maze(f, nb, nb))
        {
            printf("The %s seems to create solvable mazes\n", generator_name);
        }
        else
        {
            printf("The %s do not create solvable mazes\n", generator_name);
        }
    }
    if (evaluate_connected)
    {
        printf("Evaluation of the %s, criteria : connected\n", generator_name);
        if(do_made_connected_maze(f, nb, nb))
        {
            printf("The %s seems to create connected mazes\n", generator_name);
        }
        else
        {
            printf("The %s do not create connected mazes\n", generator_name);
        }
    }
    if (evaluate_perfect)
    {
        printf("Evaluation of the %s, criteria : perfect\n", generator_name);
        if (do_made_perfect_maze(f, nb, nb))
        {
            printf("The %s seems to create perfect mazes\n", generator_name);
        }
        else
        {
            printf("The %s do not create perfect mazes\n", generator_name);
        }
    }
    printf("End of the evaluation\n");
    return EXIT_SUCCESS;
}