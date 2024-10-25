#ifndef TEST_H
#define TEST_H
#include "struct.h"

//Définition du type de pointeur de fonction (avec 2 entiers en paramètres et un maze_t en retour)
typedef maze_t (*func_ptr)(int, int);

//Fonction permettant d'évaluer un générateur de labyrinthe
//f : pointeur de fonction de génération de labyrinthe
//x : largeur maximale du labyrinthe
//y : hauteur maximale du labyrinthe
//retourne true si le générateur semble correct, false sinon
//risque de faux positifs
bool do_made_solvable_maze(const func_ptr f, const int x, const int y);

//Fonction permettant d'évaluer un générateur de labyrinthe
//f : pointeur de fonction de génération de labyrinthe
//x : largeur maximale du labyrinthe
//y : hauteur maximale du labyrinthe
//retourne true si le générateur semble correct, false sinon
//risque de faux positifs
//exponentiel en temps
bool do_made_perfect_maze(const func_ptr f, const int x, const int y);

//Fonction permettant d'évaluer un générateur de labyrinthe
//f : pointeur de fonction de génération de labyrinthe
//retourne 0 si le générateur est incorrect, 1 si le générateur semble créer des labyrinthes solvables, 2 si le générateur semble créer des labyrinthes parfaits
//risque de faux positifs
//exponentiel en temps
int evaluate_mazemaker(func_ptr f, int x, int y);

//Macro permettant d'évaluer un générateur de labyrinthe
//générateur : pointeur sur la fonction de génération de labyrinthe
//name : nom de la fonction de génération de labyrinthe
//risque de faux positifs
//exponentiel en temps
#define EVALUATE_MAZEMAKERN(générateur, name, x, y) \
    do { \
        const int t = evaluate_mazemaker(f, x, y); \
        if(t == 2) \
        { \
            printf("%s semble créer des labyrinthes parfaits\n", name); \
        } \
        else if(t == 1) \
        { \
            printf("%s semble créer des labyrinthes solvables\n", name); \
        } \
        else \
        { \
            printf("%s n'est pas conforme au cahier des charges\n", name); \
        } \
    } while(0)

#define EVALUATE_TIME(générateur, name, x, y) \
    do { \
        clock_t start = clock(); \
        for(int i = 1; i < x; i++) \
        { \
            for(int j = 1; j < y; j++) \
            { \
                const maze_t maze = générateur(i, j); \
                free_maze(maze); \
            } \
        } \
        clock_t end = clock(); \
        printf("Temps de génération moyen par %s : \t%f secondes\n", name, (double)(end - start) / CLOCKS_PER_SEC /(x * y)); \
    } while(0)
#endif //TEST_H
