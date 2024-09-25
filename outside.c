#include "outside.h"
#include <string.h>

maze_t maze_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: could not open file %s\n", filename);
        exit(1);
    }
    char *line = malloc(6 * sizeof(char)); //allouer une ligne de 6 caractères
    fgets(line, 6, file); //récupérer la première ligne
    int width, height;
    width = atoi(strtok(line, " ")); //récupérer la largeur
    printf("width = %d\n", width); //pour debbuguer au cas où
    height = atoi(strtok(NULL, " ")); //récupérer la hauteur
    printf("height = %d\n", height); //pour le debbug au cas où
    maze_t maze = create_basic_maze(width, height); //créer un labyrinthe de base

    fclose(file); //désallouer le fichier
    return maze;
}