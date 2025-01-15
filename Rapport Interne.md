# je sais plus ce qu'est le nom de ce que je fais

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

### Primitives


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
Fonction qui crée un labyrinthe parfait @micky

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
Il est à usage interne et n'est pas compilé dans le programme final.

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

## Fichier " main.c " :

ne fait actuellement rien d'autre qu'appeler cmd...

## Fichier " testing.c " :
Interprète les commandes passées en argument du programme et appelle les fonctions correspondantes pour l'exécutable de tests.

Ne sera pas compilé dans le programme final.