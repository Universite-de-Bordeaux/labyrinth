#include "test.h"
//comparable à cmd.c, ce fichier va uniquement gérer les arguments de la ligne de commande

static void usage(const char* name)
{
    printf("usage : %s\n", name);
    printf(".\\%s --evaluate <generator>\n", name);
    printf("-e can be used instead of --evaluate\n");
    printf(".\\%s --timed <generator>\n", name);
    printf("-t can be used instead of --timed\n");
    printf(".\\%s --evaluate_solvable <generator> <nb>\n", name);
    printf("-es can be used instead of --evaluate_solvable\n");
    printf(".\\%s --evaluate_connected <generator> <nb>\n", name);
    printf("-ec can be used instead of --evaluate_connected\n");
    printf(".\\%s --evaluate_perfect <generator> <nb>\n", name);
    printf("-ep can be used instead of --evaluate_perfect\n");
    printf("\n");
    printf("<nb> must be an integer, its default value is 100\n");
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

int main(const int argc, char* argv[])
{
}

