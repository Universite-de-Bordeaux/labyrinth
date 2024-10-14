#include "outside.h"
#include "case.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// crée et renvoie un labyrinthe selon les configurations spécifiées dans le fichier
// filename : le nom du fichier
maze_t maze_from_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: impossible d'ouvrir le fichier %s\n", filename);
        exit(1);
    }
    int c = fgetc(file); //lire le premier caractère
    int width = 0;
    while(c != EOF && c != ' ') //tant que le caractère n'est pas la fin du fichier ou un espace
    {
        width *= 10;
        width += c - '0'; //convertir le caractère en entier
        c = fgetc(file); //lire le caractère suivant
    }
    int height = 0;
    c = fgetc(file); //lire le caractère suivant
    while(c != EOF && c != '\n') //tant que le caractère n'est pas la fin du fichier ou un retour à la ligne
    {
        height *= 10;
        height += c - '0'; //convertir le caractère en entier
        c = fgetc(file); //lire le caractère suivant
    }
    const maze_t maze = create_basic_maze(width, height); //créer un labyrinthe de la taille spécifiée

    c = fgetc(file); //lire le caractère suivant
    int x = 0, y = 0;
    while(c != EOF)
    {
        x = 0;
        y = 0;
        while(c != ' ')
        {
            x *= 10; //transformer les unités en dizaines
            x += c - '0'; //convertir le caractère en entier
            c = fgetc(file); //lire le caractère suivant
        }
        if(x > width - 1) //vérifier que x est dans les limites du labyrinthe
        {
            fprintf(stderr, "Erreur: x a été calculé à %d, mais la largeur du labyrinthe est de %d\n", x, width);
            free_maze(maze);
            fclose(file);
            exit(1);
        }
        c = fgetc(file); //lire le caractère suivant
        while(c != ' ')
        {
            y *= 10; //transformer les unités en dizaines
            y += c - '0'; //convertir le caractère en entier
            c = fgetc(file); //lire le caractère suivant
        }
        if(y > height - 1) //vérifier que y est dans les limites du labyrinthe
        {
            fprintf(stderr, "Errer: y a été calculé à %d, mais la dimension du labyrinthe est de %d\n", y, height);
            free_maze(maze);
            fclose(file);
            exit(1);
        }
        c = fgetc(file); //lire le caractère suivant
        if(c == 'h') //mur horizontal
        {
            wall_down(maze, x, y);
        }
        else if(c == 'v') //mur vertical
        {
            wall_right(maze, x, y);
        }
        else //caractère invalide ou bug du programme
        {
            fprintf(stderr, "Erreur: caractère invalide ou inconnu : '%c'. Attendu : 'v' ou 'h'\n", c);
            free_maze(maze);
            fclose(file);
            exit(1);
        }
        fgetc(file); //on passe le '\n'
        c = fgetc(file); //lire le caractère suivant
    }
    fclose(file); //désallouer le fichier
    return maze;
}

// écrit le labyrinthe dans un fichier texte
// maze : le labyrinthe
// filename : le nom du fichier où écrire le labyrinthe, sera écrasé s'il existe déjà
void maze_to_file(maze_t maze, const char *filename)
{
    FILE *fp = fopen (filename, "w");
    fprintf(fp, "%d %d\n", maze.width, maze.height);
    for (int y=0; y<maze.height; y++){
        for (int x=0; x<maze.width; x++){
            if (x!=maze.width-1 && has_wall_right(maze, x, y)){
                fprintf(fp, "%d %d V\n", x ,y);
            }
            if (y!=maze.height-1 && has_wall_down(maze, x, y)){
                fprintf(fp, "%d %d H\n", x ,y);
            }
        }
    }
    fclose(fp);
    return;
}
