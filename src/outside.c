#include "outside.h"
#include <stdio.h>
#include <stdlib.h>

// --- FONCTIONS AUXILIAIRES ---

// fonction auxiliaire pour way_from_file
static void way_to_file_aux(const way* w, FILE* file)
{
    if (!is_origin(w))
    {
        way_to_file_aux(get_dad(w), file);
    }
    fprintf(file, "%d %d\n", get_x(w), get_y(w));
}

// --- FONCTIONS PRINCIPALES ---

// FONCTIONS DE LECTURE
maze_t maze_from_file(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: impossible d'ouvrir le fichier %s\n", filename);
        exit(1);
    }
    int c = fgetc(file); // lire le premier caractère
    int width = 0;
    while (c != EOF && c != ' ') // tant que le caractère n'est pas la fin du fichier ou un espace
    {
        width *= 10;
        width += c - '0'; // convertir le caractère en entier
        c = fgetc(file); // lire le caractère suivant
    }
    int height = 0;
    c = fgetc(file); // lire le caractère suivant
    while (c != EOF && c != '\n') // tant que le caractère n'est pas la fin du fichier ou un retour à la ligne
    {
        height *= 10;
        height += c - '0'; // convertir le caractère en entier
        c = fgetc(file); // lire le caractère suivant
    }
    const maze_t maze = create_basic_maze(width, height); // créer un labyrinthe de la taille spécifiée

    c = fgetc(file); // lire le caractère suivant
    int x = 0, y = 0;
    while (c != EOF)
    {
        x = 0;
        y = 0;
        while (c != ' ')
        {
            x *= 10; // transformer les unités en dizaines
            x += c - '0'; // convertir le caractère en entier
            c = fgetc(file); // lire le caractère suivant
        }
        if (x > width - 1) // vérifier que x est dans les limites du labyrinthe
        {
            fprintf(stderr, "Erreur: x a été calculé à %d, mais la largeur du labyrinthe est de %d\n", x, width);
            free_maze(maze);
            fclose(file);
            exit(1);
        }
        c = fgetc(file); // lire le caractère suivant
        while (c != ' ')
        {
            y *= 10; // transformer les unités en dizaines
            y += c - '0'; // convertir le caractère en entier
            c = fgetc(file); // lire le caractère suivant
        }
        if (y > height - 1) // vérifier que y est dans les limites du labyrinthe
        {
            fprintf(stderr, "Errer: y a été calculé à %d, mais la dimension du labyrinthe est de %d\n", y, height);
            free_maze(maze);
            fclose(file);
            exit(1);
        }
        c = fgetc(file); // lire le caractère suivant
        if (c == 'H') // mur horizontal
        {
            wall_down(maze, x, y);
        }
        else if (c == 'V') // mur vertical
        {
            wall_right(maze, x, y);
        }
        else // caractère invalide ou bug du programme
        {
            fprintf(stderr, "Erreur: caractère invalide ou inconnu : '%c'. Attendu : 'V' ou 'H'\n", c);
            free_maze(maze);
            fclose(file);
            exit(1);
        }
        fgetc(file); // on passe le '\n'
        c = fgetc(file); // lire le caractère suivant
    }
    fclose(file); // désallouer le fichier
    return maze;
}

void maze_to_file(const maze_t maze, const char* filename)
{
    FILE* fp = fopen(filename, "w"); // ouvre le fichier de nom filename en mode écriture
    fprintf(fp, "%d %d\n", maze.width, maze.height); // écrit les dimensions du labyrinthe
    for (int y = 0; y < maze.height; y++)
    {
        for (int x = 0; x < maze.width; x++)
        {
            if (x != maze.width - 1 && has_wall_right(maze, x, y))
            {
                fprintf(fp, "%d %d V\n", x, y); // écrit x y et V si il y a un mur à droite
            }
            if (y != maze.height - 1 && has_wall_down(maze, x, y))
            {
                fprintf(fp, "%d %d H\n", x, y); // écrit x y et H si il y a un mur en bas
            }
        }
    }
    fclose(fp); // fermeture du fichier
}

// FONCTIONS D'ECRITURE
void way_to_file(const way* w, const char* filename)
{
    if (w == NULL || is_empty(w))
    {
        fprintf(stderr, "Erreur: le chemin est NULL\n");
        exit(1);
    }
    FILE* file = fopen(filename, "w");
    if (file == NULL)
    {
        fprintf(stderr, "Error: impossible d'ouvrir le fichier %s\n", filename);
        exit(1);
    }
    way_to_file_aux(w, file);
    fclose(file);
}

way* way_from_file(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: impossible d'ouvrir le fichier %s\n", filename);
        exit(1);
    }
    int x = 0, y = 0;
    fscanf(file, "%d %d", &x, &y);
    way* w = create_way();
    w->x = x;
    w->y = y;
    while (!feof(file))
    {
        way* next = create_way();
        new_dad(next, w);
        w = next;
        w->x = x;
        w->y = y;
        fscanf(file, "%d %d", &x, &y);
    }
    fclose(file);
    return w;
}
