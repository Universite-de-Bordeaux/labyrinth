#include "outside.h"

#include <inttypes.h>
#include <string.h>

maze_t maze_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: could not open file %s\n", filename);
        exit(1);
    }
    char *line = malloc(6 * sizeof(char)); //allouer une ligne de 6 caractères
    fgets(line, 6, file); //récupérer la première ligne
    const int width = (int)strtol(strtok(line, " "), NULL, 10); //récupérer la largeur
    printf("width = %d\n", width); //pour debbuguer au cas où
    const int height = (int)strtol(strtok(line, " ")+3, NULL, 10); //récupérer la hauteur)
    printf("height = %d\n", height); //pour le debbug au cas où
    const maze_t maze = create_basic_maze(width, height); //créer un labyrinthe de base

    while(fgets(line, 6, file) != NULL && line != NULL)
    {
        int x = (int)strtol(strtok(line, " "), NULL, 10); //récupérer la coordonnée x
        int y = (int)strtol(strtok(line, " ")+2, NULL, 10); //récupérer la coordonnée y
        char* wall = strtok(line, " ")+4; //récupérer la lettre correspondant au mur
        printf("x = %d, y = %d, wall = %s\n", x, y, wall); //pour le debbug au cas où
        if(wall == NULL) //si le mur n'est pas spécifié
        {
            fprintf(stderr, "Error: invalid file format\n");
            free(line);
            fclose(file);
            free_maze(maze);
            exit(1);
        }
        else if(strcmp(wall, "v") == 0) //si le mur est vertical
        {
            wall_up(maze, x, y);
        }
        else if(strcmp(wall, "h") == 0) //si le mur est horizontal
        {
            wall_right(maze, x, y);
        }
    }

    free(line);
    fclose(file); //désallouer le fichier
    return maze;
}