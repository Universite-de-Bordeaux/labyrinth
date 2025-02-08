# Labyrinth

Le projet Labyrinth est organisé dans les fichiers suivants :

struct.c : définition des structures de données et de leurs primitives.

mazemaker.c : fonctions de génération de labyrinthes.

test.c : fonctions d'évaluation de labyrinthes.

solveur.c : fonctions de résolution de labyrinthes.

outside.c : fonctions d'écritures et de lectures de nos structures de données dans des fichiers.

cmd.c : fonctions de gestion de la ligne de commande de l'exécutable principal.

testing.c : fonctions de gestion de la ligne de commande de l'exécutable de tests.

le CMakeLists.txt est utilisé pour compiler les fichiers sources en deux exécutables :
- maze : exécutable principal (creation, résolution, affichage, écriture et lecture de labyrinthes)
- test_interne : exécutable de tests (à usage interne, pour tester les fonctions de génération de labyrinthes)

## Fichier " struct.c " :

Ce fichier contient la définition de l'ensemble des structures de données utilisées ainsi que leurs primitives.

### Structures de données
1. `cell` : structure représentant une case du labyrinthe, elle contient les informations sur ses murs.
2. `maze_t` : structure représentant un labyrinthe, elle contient les informations sur ses dimensions et les cases qui le composent.
3. `bool_tab` : tableau de booléens, utilisé par de nombreux algorithmes, généralement pour marquer les cases d'un labyrinthes.
4. `way` : structure représentant un chemin depuis l'origine jusqu'à une case donnée, elle contient les coordonnées de la case, le chemin parcouru et sa longueur.
5. `waytab` : tableau de chemins, utilisée pour stocker les chemins parcourus par un algorithme de recherche de chemin.
6. `queue` : structure de file de coordonnées, elle stock les valeurs par paires de coordonnées.
7. `stack` : structure de pile de coordonnées, elle stock les valeurs par paires de coordonnées.

### Primitives labyrinthes
#### **maze_t create_basic_maze(int width, int height)**
Fonction qui crée et renvoie un labyrinthe avec uniquement les murs extérieurs.

width : largeur du labyrinthe

height : hauteur du labyrinthe

#### **maze_t create_wall_maze(int width, int height)**
Fonction qui crée et renvoie un labyrinthe avec tous les murs internes et externes.

width : largeur du labyrinthe

height : hauteur du labyrinthe

#### **void free_maze(maze_t maze)**
Fonction qui libère la mémoire allouée pour un labyrinthe.

maze : labyrinthe à libérer

#### **void wall_up(maze_t maze, int x, int y)**
Fonction qui ajoute un mur en haut de la case `(x, y)`.

maze : labyrinthe

x, y : les coordonnées de la case

#### **void wall_down(maze_t maze, int x, int y)**
Fonction qui ajoute un mur en bas de la case `(x, y)`.

maze : labyrinthe

x, y : les coordonnées de la case

#### **void wall_left(maze_t maze, int x, int y)**
Fonction qui ajoute un mur à gauche de la case `(x, y)`.

maze : labyrinthe

x, y : les coordonnées de la case

#### **void wall_right(maze_t maze, int x, int y)**
Fonction qui ajoute un mur à droite de la case `(x, y)`.

maze : labyrinthe

x, y : les coordonnées de la case

#### **void unwall_up(maze_t maze, int x, int y)**
Fonction qui retire un mur en haut de la case `(x, y)`.

maze : labyrinthe

x, y : les coordonnées de la case

#### **void unwall_down(maze_t maze, int x, int y)**
Fonction qui retire un mur en bas de la case `(x, y)`.

maze : labyrinthe

x, y : les coordonnées de la case

#### **void unwall_left(maze_t maze, int x, int y)**
Fonction qui retire un mur à gauche de la case `(x, y)`.

maze : labyrinthe

x, y : les coordonnées de la case

#### **void unwall_right(maze_t maze, int x, int y)**
Fonction qui retire un mur à droite de la case `(x, y)`.

maze : labyrinthe

x, y : les coordonnées de la case

#### **bool has_wall_up(const maze_t maze, int x, int y)**
Fonction qui renvoie `true` si la case `(x, y)` a un mur en haut, `false` sinon.

maze : labyrinthe

x, y : les coordonnées de la case

#### **bool has_wall_down(const maze_t maze, int x, int y)**
Fonction qui renvoie `true` si la case `(x, y)` a un mur en bas, `false` sinon.

maze : labyrinthe

x, y : les coordonnées de la case

#### **bool has_wall_left(const maze_t maze, int x, int y)**
Fonction qui renvoie `true` si la case `(x, y)` a un mur à gauche, `false` sinon.

maze : labyrinthe

x, y : les coordonnées de la case

#### **bool has_wall_right(const maze_t maze, int x, int y)**
Fonction qui renvoie `true` si la case `(x, y)` a un mur à droite, `false` sinon.

maze : labyrinthe

x, y : les coordonnées de la case

### Primitives affichage
#### **int print_maze(maze_t maze)**
Fonction qui affiche le labyrinthe. Retourne 1 si l'affichage a réussi, -1 sinon.
Les murs de l'entrée sont bleus, les murs de la sortie sont verts.
La taille de chaque cellule est automatiquement ajustée en fonction de la taille de la fenêtre, si la taille du labyrinthe est trop grande, l'affichage peut ne pas fonctionner correctement.
Le programme s'arrête quand quand la fenêtre est fermée (en pressant `ESCAPE` ou `ENTER`).

maze : labyrinthe à afficher

#### **int initial_print_maze(const maze_t maze, SDL_Renderer\*\* renderer, SDL_Window\*\* window, int\* dw, int\* dh)**
Fonction pour afficher le labyrinth et ses murs dans une fenêtre SDL.
Initialise les données nécessaires pour l'affichage et sa modification.
Renvoie 1 en cas de succès, un nombre négatif en cas d'échec.

maze : labyrinthe à afficher

renderer : pointeur sur le renderer à initialiser

window : pointeur sur la fenêtre à initialiser

dw : pointeur sur la largeur d'une cellule

dh : pointeur sur la hauteur d'une cellule

#### **void destroy_print_maze(SDL_Renderer\* renderer, SDL_Window\* window)**
Fonction qui libère la mémoire allouée pour l'affichage du labyrinthe et ferme la fenêtre.

renderer : pointeur sur le renderer à libérer

window : pointeur sur la fenêtre à libérer

#### **void wait_and_destroy_print_maze(SDL_Renderer\* renderer, SDL_Window\* window)**
Fontion qui attend que l'utilisateur ferme la fenêtre avant de libérer la mémoire allouée pour l'affichage du labyrinthe et fermer la fenêtre.

renderer : pointeur sur le renderer à libérer

window : pointeur sur la fenêtre à libérer

### Primitives `bool_tab`
#### **bool_tab create_bool_tab(int width, int height)**
Fonction qui crée et renvoie un tableau de booléens de dimensions `width` par `height`.

width : largeur du tableau

height : hauteur du tableau

#### **void free_bool_tab(bool_tab tab)**
Fonction qui libère la mémoire allouée pour un tableau de booléens.

tab : tableau de booléens à libérer

#### **void set_true(bool_tab tab, int x, int y)**
Fonction qui met à `true` la case `(x, y)` du tableau `tab`.

tab : tableau de booléens

x, y : les coordonnées de la case

#### **void set_false(bool_tab tab, int x, int y)**
Fonction qui met à `false` la case `(x, y)` du tableau `tab`.

tab : tableau de booléens

x, y : les coordonnées de la case

#### **bool get_bool(bool_tab tab, int x, int y)**
Fonction qui renvoie la valeur de la case `(x, y)` du tableau `tab`.

tab : tableau de booléens

x, y : les coordonnées de la case

### Primitives `waytab`
#### **waytab create_waytab(int width, int height)**
Fonction qui crée et renvoie un tableau de `way` de dimensions `width` par `height`.

width : largeur du tableau

height : hauteur du tableau

#### **void free_waytab(waytab tab)**
Fonction qui libère la mémoire allouée pour un tableau de `way`.

tab : tableau de `way` à libérer

#### **bool has_way(waytab tab, int x, int y);**
Fonction qui renvoie `true` si la case `(x, y)` du tableau `tab` contient un chemin vers la case `(0, 0)`, `false` sinon.

tab : tableau de `way`

x, y : les coordonnées de la case

#### **way\* get_way(waytab tab, int x, int y)**
Fonction qui renvoie le chemin de la case `(x, y)` du tableau `tab`.

tab : tableau de `way`

x, y : les coordonnées de la case

#### **void connected_way(waytab tab, int x, int y, int dad_x, int dad_y)**
Fonction qui connecte le chemin de la case `(x, y)` du tableau `tab` à la case `(dad_x, dad_y)`.

tab : tableau de `way`

x, y : les coordonnées de la case

dad_x, dad_y : les coordonnées de la case à connecter

#### **unsigned int length_waytab(waytab tab, int x, int y)**
Fonction qui renvoie la longueur du chemin de la case `(x, y)` du tableau `tab`.

tab : tableau de `way`

x, y : les coordonnées de la case

### Primitives `way`

A completer

### Primitives `queue`

A completer

### Primitives `stack`

A completer

## Fichier " mazemaker.c "
Ce fichier contient les fonctions de génération de labyrinthes, ainsi que les fonctions auxilliaires nécessaires à leur création.

### Fonctions auxilliaires

#### int set_connexion(const maze_t maze, const bool_tab is_connexe, const int dx, const int dy)
Fonction qui met à jour le tableau `is_connexe` en ajourant les cases connexes à la case `(dx, dy)`.
Retourne le nombre de cases mise à jour.

maze : labyrinthe

is_connexe : tableau de booléens

dx : abscisse de la case

dy : ordonnée de la case

#### bool lbp_path_move(const maze_t* maze, int* x, int* y, const bool_tab tab_visited)
@micky

#### void lbp_path(maze_t* maze, int* x, int* y, int* x_2, int* y_2, const bool_tab tab_visited)
@micky

### Fonctions principales
Toutes les fonctions de générations de labyrinthes suivent la déclarations suivante :

```maze_t *nom_de_la_fonction(const int width, const int height)```
où `width` et `height` sont les dimensions du labyrinthe à générer.

#### column_maze
Fonction qui crée un labyrinthe parfait en supprimant tous les murs internes horizontaux et un mur vertical par colonne.

#### line_maze
Fonction qui crée un labyrinthe parfait en supprimant tous les murs internes verticaux et un mur horizontal par ligne.

#### one_way_maze
Fonction qui crée un labyrinthe parfait à partir d'un chemin principal. Semblable à un hunt-and-kill

#### octopus_maze
Fonction qui crée un labyrinthe parfait en partant d'un point central et en créant des branches.
Les branches sont crées en partant du centre et en allant vers les bords linéairement, ce qui donne un labyrinthes prévisible et intéressant.
N'est pas compilé dans le programme final.

#### my_octopus_maze
Version améliorée de `octopus_maze` qui impose un choix aléatoire pour la direction des branches, permettant de créer des labyrinthes plus variés.
est compilé sous le nom `octopus_maze` dans le programme final.

#### comb_maze
Fonction qui crée un labyrinthe parfait en ne cassant des murs que vers le bas ou la droite.
Toute cases de coordonnées `(x, y)` est donc accessible depuis la case `(0, 0)` via un chemin de x + y cases. (pire cas possible d'un parcours en largeur).

####  proto_comb_maze
Fonction destinée à remplacer `comb_maze` dans le programme final, elle n'a jamais été terminée et n'est pas compilée.


#### hunt_kill_maze
Fonction qui crée un labyrinthe parfait en partant d'une case aléatoire et en cassant des murs aléatoirement jusqu'à ce que le labyrinthe soit parfait.


#### by_path_maze
Fonction qui crée un labyrinthe parfait à partir de la case 0 0 en murant des couloirs puis sélectionne la prochaine case de haut en bas qui créera un couloir.

#### cross_maze
Fonction qui crée un labyrinthe parfait en deux étapes :
1. Perforation de murs autours de cases aléatoires sans créer de cycle (les perforations sont en forme de croix, d'où le nom)
2. perforation de murs pour connecter les cases entre elles.

### Autres fonctions

#### void tear(const maze_t maze, const unsigned int prop)
Fonction perforant des murs aléatoirement dans le labyrinthe `maze` en fonction de la proportion `prop` (en pourcentage) afin de créer des boucles.

`maze` : labyrinthe

`prop` : probabilité qu'un des murs d'une case soit perforé (en pourcentage), répété pour chaque case.

## Fichier " test.c "
Ce fichiers contient des fonctions d'évaluation de labyrinthes, ainsi que des fonctions auxilliaires nécessaires à leur évaluation.
Il est à usage interne.

### Fonctions

do_made_solvable_maze(const func_ptr f, const int x, const int y)
Fonction indiquant si un labyrinthe généré par la fonction `f` de dimension inférieure ou égale à `x` par `y` est solvable.

do_made_connected_maze(const func_ptr f, const int x, const int y)
Fonction indiquant si un labyrinthe généré par la fonction `f` de dimension inférieure ou égale à `x` par `y` est connexe.

do_made_perfect_maze(const func_ptr f, const int x, const int y)
Fonction indiquant si un labyrinthe généré par la fonction `f` de dimension inférieure ou égale à `x` par `y` est parfait.

evaluate_mazemaker(const func_ptr f)
Fonction évaluant les labyrinthes générés par la fonction `f` et renvoyant si ils sont solvables, connexes ou parfaits.

evaluate_time(const func_ptr f, char* name)
Fonction évaluant le temps de génération de labyrinthes par la fonction `f` et renvoyant un score.


## Fichier " solveur.c "
Ce fichier contient les fonctions de résolution de labyrinthes.

#### bool do_made_solvable_maze(const func_ptr f, const int x, const int y)
Fonction qui analyse si un labyrinthe généré par la fonction `f` de dimension inférieure ou égale à `x` par `y` est solvable.

#### bool do_made_perfect_maze(const func_ptr f, const int x, const int y)
Fonction qui analyse si un labyrinthe généré par la fonction `f` de dimension inférieure ou égale à `x` par `y` est parfait.

#### int evaluate_mazemaker(const func_ptr f)
Fonction qui évalue les labyrinthes générés par la fonction `f` de dimension inférieure ou égale à `x` par `y`.
la fonction renvoie :
- 3 si le labyrinthe est parfait
- 2 si le labyrinthe est connexe
- 1 si le labyrinthe est solvable
- 0 si le labyrinthe n'est pas solvable

Cette fonction est utilisée par la macro `EVALUATE_MAZEMAKER` afin de rendre un résultat plus lisible.

#### int evaluate_time(const func_ptr f, char* name)
Fonction qui renvoie un score en fonction du temps de génération d'un labyrinthe par la fonction `f`.
Les quatres critères de notation sont :
- la durée de création de labyrinthes "standard" de dimension k x t avec k et t de 1 à 100. (/50)
- la durée de création de labyrinthes "lignes" de dimension 1 x k ou k x 1 avec k de 1 à 100. (/10)
- la durée de création de labyrinthes "petits" de dimension 4 x 4. (/15)
- la durée de création de labyrinthes "moyens" de dimension 100 x 100. (/25)

les critères sont évidemment ajustables et sujet à contextualisation.

### Fonctions auxilliaires

--- aucune pour l'instant ---

### Fonctions de résolution
Toutes les fonctions de résolution de labyrinthes suivent la déclarations suivante :

```*nom_de_la_fonction(const maze_t maze)```
où `maze` est le labyrinthe à résoudre.

#### bool has_exit_deep_seeker
Fonction qui cherche une sortie en profondeur, elle ne retourne pas le chemin mais indique si une sortie existe.
Généralement plus rapide que la recherche en largeur.

#### bool is_connexe_deep_inspector
Fonction qui vérifie si le labyrinthe est connexe en profondeur. Elle ne donne pas d'indication en cas d'absence de connexité.

#### bool is_perfect_deep_inspector
Fonction qui vérifie si le labyrinthe est parfait en profondeur. Elle ne donne pas d'indication en cas d'imperfection.

#### *way best_exit_deep_seeker
Fonction qui cherche le meilleur chemin de sortie en profondeur. Elle retourne un pointeur sur le chemin trouvé, ou NULL si aucun chemin n'a été trouvé.

#### *way has_exit_breadth_seeker
Fonction qui cherche une sortie en largeur, elle ne retourne pas le chemin mais indique si une sortie existe.

#### bool is_connexe_breadth_inspector
Fonction qui vérifie si le labyrinthe est connexe en largeur. Elle ne donne pas d'indication en cas d'absence de connexité.

#### bool is_perfect_breadth_inspector
Fonction qui vérifie si le labyrinthe est parfait en largeur. Elle ne donne pas d'indication en cas d'imperfection.

#### *way best_exit_breadth_seeker
Fonction qui cherche le meilleur chemin de sortie en largeur. Elle retourne un pointeur sur le chemin trouvé, ou NULL si aucun chemin n'a été trouvé.
Généralement plus rapide que la recherche en profondeur.

### Fonctions de visualisation
La majorité des fonctions de visualisation suivent la déclarations suivante :

```int nom_de_la_fonction(const maze_t maze)```
où `maze` est le labyrinthe à visualiser.
la sortie est un entier relatif au type d'erreur rencontré si la visualisation a échouée.

#### show_has_exit_deep_seeker
Fonction qui illustre la fonction `has_exit_deep_seeker`.

#### show_is_connexe_deep_inspector
Fonction qui illustre la fonction `is_connexe_deep_inspector`.

#### show_is_perfect_deep_inspector
Fonction qui illustre la fonction `is_perfect_deep_inspector`.

#### show_best_exit_deep_seeker
Fonction qui illustre la fonction `best_exit_deep_seeker`.
N'enregistre pas le chemin trouvé.

#### show_has_exit_breadth_seeker
Fonction qui illustre la fonction `has_exit_breadth_seeker`.

#### show_is_connexe_breadth_inspector
Fonction qui illustre la fonction `is_connexe_breadth_inspector`.

#### show_is_perfect_breadth_inspector
Fonction qui illustre la fonction `is_perfect_breadth_inspector`.

#### show_best_exit_breadth_seeker
Fonction qui illustre la fonction `best_exit_breadth_seeker`.

#### int show_the_way(const maze_t maze, const way* w)
La seule fonction de visualisation à ne pas se baser sur un algorithme de résolution, elle illustre le chemin `way` dans le labyrinthe `maze`.
Attention, cette fonction ne vérifie pas si le chemin est bien issu du labyrinthe donné.

## Fichier " outside.c "
Ce fichiers contient les fonctions d'écritures et de lectures de nos structures de données dans des fichiers.

### Fonctions auxilliaires

#### void way_to_file_aux(const way* w, FILE* file)
Permets d'inverser l'ordre des cases d'un chemin pour l'écriture dans un fichier.
(les données de la structures sont écrites dans l'ordre inverse de leur lecture)

### Fonctions d'écriture

#### void maze_to_file(const maze_t maze, const char* filename)
Fonction qui écrit un labyrinthe dans un fichier.
Ecrase le fichier si il existe déjà.

`maze` : labyrinthe à écrire.

`filename` : nom du fichier à écrire.

#### void way_to_file(const way* w, const char* filename)
Fonction qui écrit un chemin dans un fichier.

`w` : chemin à écrire.

`filename` : nom du fichier à écrire.

### Fonctions de lecture

#### maze_t maze_from_file(const char* filename)
Fonction qui lit un labyrinthe depuis un fichier et le renvoie.

`filename` : nom du fichier à lire.

#### way* way_from_file(const char* filename)
Fonction qui lit un chemin depuis un fichier et le renvoie.

`filename` : nom du fichier à lire.

## Fichier " cmd.c " :
Fichier contenant les fonctions de gestion de la ligne de commande.

### Fonctions auxilliaires

#### bool safe_atoi(const char* str, int* out)
Fonction pour convertir en entier une chaine de caractères.
La chaines de caractères, si elle est valide, est convertie puis stockée dans `out`.
La fonction renvoie `true` si la conversion a réussie, `false` sinon.

`str` : chaine de caractères à convertir.

`out` : pointeur sur l'entier de stockage.

#### static void print_cmd_help(char* namefile)
Fonction qui affiche l'aide de la ligne de commande.

`namefile` : nom du fichier exécutable.

### Fonction principale

#### void cmd(char* argv[], const int argc)
Interprète les commandes passées en argument du programme et appelle les fonctions correspondantes.

`argv` : tableau des arguments.

`argc` : nombre d'arguments.

## Fichier " testing.c " :
Interprète les commandes passées en argument du programme et appelle les fonctions correspondantes pour l'exécutable de tests.
