# Labyrinth

Le projet Labyrinth est organisé dans les fichiers suivants :

struct.c : définition des structures de données et de leurs primitives.

mazemaker.c : fonctions de génération de labyrinthes.

test.c : fonctions d'évaluation de labyrinthes.

solveur.c : fonctions de résolution de labyrinthes.

escape.c : fonctions de visite de labyrinthes.

game.c : fonctions de jeu.

outside.c : fonctions d'écritures et de lectures de nos structures de données dans des fichiers.

cmd.c : fonctions de gestion de la ligne de commande de l'exécutable principal.

testing.c : fonctions de gestion de la ligne de commande de l'exécutable de tests.

le CMakeLists.txt est utilisé pour compiler les fichiers sources en deux exécutables :
- maze : exécutable principal (creation, résolution, visite, affichage, écriture et lecture de labyrinthes)
- test_interne : exécutable de tests (pour tester les fonctions de génération de labyrinthes)

# Fichier " struct.c "
Ce fichier contient la définition de l'ensemble des structures de données utilisées ainsi que leurs primitives.

## Structures de données
1. `cell` : structure représentant une case du labyrinthe, elle contient les informations sur ses murs.
2. `maze_t` : structure représentant un labyrinthe, elle contient les informations sur ses dimensions et les cases qui le composent.
3. `bool_tab` : tableau de booléens, utilisé par de nombreux algorithmes, généralement pour marquer les cases d'un labyrinthes.
4. `way` : structure représentant un chemin depuis l'origine jusqu'à une case donnée, elle contient les coordonnées de la case, le chemin parcouru et sa longueur.
5. `waytab` : tableau de chemins, utilisée pour stocker les chemins parcourus par un algorithme de recherche de chemin.
6. `queue` : structure de file de coordonnées, elle stock les valeurs par paires de coordonnées.
7. `stack` : structure de pile de coordonnées, elle stock les valeurs par paires de coordonnées.

## Primitives labyrinthes
paramètres :

width : largeur du labyrinthe

height : hauteur du labyrinthe
### **maze_t create_basic_maze(int width, int height)**
Fonction qui crée et renvoie un labyrinthe de taille `width` par `heigth` avec uniquement les murs extérieurs.

### **maze_t create_wall_maze(int width, int height)**
Fonction qui crée et renvoie un labyrinthe de taille `width` par `heigth` avec tous les murs internes et externes.

### **void free_maze(maze_t maze)**
Fonction qui libère la mémoire allouée pour le labyrinth `maze`.

## Primitives pour la gestion des murs
paramètres :

maze : labyrinthe

x, y : les coordonnées de la case
### **void wall_up(down/left/right)(maze_t maze, int x, int y)**
Fonction qui ajoute un mur dans la direction donnée de la case `(x, y)`.

### **void unwall_up(down/left/right)(maze_t maze, int x, int y)**
Fonction qui retire un mur dans la direction donnée de la case `(x, y)`.

### **bool has_wall_up(down/left/right)(maze_t maze, int x, int y)**
Fonction qui renvoie `true` si la case `(x, y)` a un mur dans la direction donnée, `false` sinon.

## Primitives affichage
paramètres :

maze : labyrinthe à afficher

renderer : pointeur sur le renderer à initialiser

window : pointeur sur la fenêtre à initialiser

dw : pointeur sur la largeur d'une cellule

dh : pointeur sur la hauteur d'une cellule
### **int print_maze(maze_t maze)**
Fonction qui affiche le labyrinthe. Retourne 1 si l'affichage a réussi, -1 sinon.
Les murs de l'entrée sont bleus, les murs de la sortie sont verts.
La taille de chaque cellule est automatiquement ajustée en fonction de la taille de la fenêtre, si la taille du labyrinthe est trop grande, l'affichage peut ne pas fonctionner correctement.
Le programme s'arrête quand la fenêtre est fermée (en pressant `ESCAPE` ou `ENTER`).

### **int pre_print_maze(maze_t maze, SDL_Renderer\*\* renderer, SDL_Window\*\* window, int\* dw, int\* dh)**
Fonction pour afficher le labyrinthe dans une fenêtre SDL.
Initialise les données nécessaires pour l'affichage et sa modification.
Renvoie 1 en cas de succès, un nombre négatif en cas d'échec.

### **int initial_print_maze(maze_t maze, SDL_Renderer\*\* renderer, SDL_Window\*\* window, int\* dw, int\* dh)**
Fonction pour afficher le labyrinthe et ses murs dans une fenêtre SDL.
Initialise les données nécessaires pour l'affichage et sa modification.
Renvoie 1 en cas de succès, un nombre négatif en cas d'échec.

### **void destroy_print_maze(SDL_Renderer\* renderer, SDL_Window\* window)**
Fonction qui libère la mémoire allouée pour l'affichage du labyrinthe et ferme la fenêtre.

### **void wait_and_destroy_print_maze(SDL_Renderer\* renderer, SDL_Window\* window)**
Fontion qui attend que l'utilisateur ferme la fenêtre avant de libérer la mémoire allouée pour l'affichage du labyrinthe et fermer la fenêtre.

## Primitives `bool_tab`
### **bool_tab create_bool_tab(int width, int height)**
Fonction qui crée et renvoie un tableau de booléens de dimensions `width` par `height`.

`width` : largeur du tableau

`height` : hauteur du tableau

### **void free_bool_tab(bool_tab tab)**
Fonction qui libère la mémoire allouée pour un tableau de booléens.

`tab` : tableau de booléens à libérer

### **void set_true/false(bool_tab tab, int x, int y)**
Fonction qui met à `true` (respectivement à `false`) la case `(x, y)` du tableau de booléens `tab`.

### **bool get_bool(bool_tab tab, int x, int y)**
Fonction qui renvoie la valeur de la case `(x, y)` du tableau de booléens `tab`.

## Primitives `waytab`
### **waytab create_waytab(int width, int height)**
Fonction qui crée et renvoie un tableau de `way` de dimensions `width` par `height`.

`width` : largeur du tableau

`height` : hauteur du tableau

### **void free_waytab(waytab tab)**
Fonction qui libère la mémoire allouée pour un tableau de `way`.

`tab` : tableau de `way` à libérer

### **bool has_way(waytab tab, int x, int y);**
Fonction qui renvoie `true` si la case `(x, y)` du tableau de way `tab` contient un chemin vers la case `(0, 0)`, `false` sinon.

### **way\* get_way(waytab tab, int x, int y)**
Fonction qui renvoie le chemin de la case `(x, y)` du tableau de way `tab`.

### **void connected_way(waytab tab, int x, int y, int dad_x, int dad_y)**
Fonction qui connecte le chemin de la case `(x, y)` du tableau de way `tab` à la case `(dad_x, dad_y)`.

### **unsigned int length_waytab(waytab tab, int x, int y)**
Fonction qui renvoie la longueur du chemin de la case `(x, y)` du tableau de way `tab`.

## Primitives `way`

### **way\* create_way()**
Fonction qui crée et renvoie un chemin vide.

### **void print_way(way\* w)**
Fonction qui affiche le chemin `w`.

### **unsigned int length_way(way\* w)**
Fonction qui renvoie la longueur du chemin `w`.
Si le chemin n'est pas défini, la fonction renvoie L_TP (actuellement 3000000000).

### **void new_dad(way\* son, way\* dad)**
Fonction qui connecte le chemin `son` à un nouveau père `dad`.
Elle ne vérifie pas si le chemin est valide.

### **way\* copy_way(way\* w)**
Fonction qui copie un chemin `w` et renvoie la copie.

### **void free_way(way\* w)**
Fonction qui libère la mémoire allouée pour un chemin `w`.
Cette fonction ne doit jamais être utilisée sur un chemin issu d'un tableau de way.

### **bool is_empty(way\* w)**
Fonction qui renvoie `true` si le chemin `w` est vide, `false` sinon.

### **way\* get_dad(way\* w)**
Fonction qui renvoie le père du chemin `w` ou `NULL` si le chemin n'a pas de père.

### **int get_x(y)(way\* w)**
Fontion qui renvoie les coordonnées x (respectivement y) du chemin `w`.

### **bool is_origin(way\* w)**
Fonction qui renvoie `true` si le chemin `w` est issu de l'origine (0, 0), `false` sinon.

### **void fix_size(way\* w)**
Fonction qui corrige la longueur du chemin `w` et de ses ancêtres.

## Primitives `queue`

### **queue\* create_queue(void)**
Fonction qui crée et renvoie une file vide.
Des allocations de mémoire sont effectuées pour la file.

### **void free_queue(queue\* q)**
Fonction qui libère la mémoire allouée pour une file `q`.

### **int size_queue(queue\* q)**
Fonction qui renvoie le nombre d'éléments (paires de coordonnées \* 2) dans la file `q`.
note : coordonnée\*2 car chaque coordonnée est un couple de deux entiers.

### **bool isempty_queue(queue\* q)**
Fonction qui renvoie `true` si la file `q` est vide, `false` sinon.

### **void enqueue(int x, int y, queue\* q)**
Fonction qui ajoute les coordonnées `(x, y)` à la file `q`.

### **void dequeue(queue\* q, int\* x, int\* y)**
Fonction qui renvoie les coordonnées en tête de la file `q` et les retire de la liste.
Les coordonnées sont stockées dans `x` et `y`.

### **void print_queue(queue\* q)**
Fonction qui affiche la file `q`.

## Primitives `stack`

### **stack\* create_stack(void)**
Fonction qui crée et renvoie une pile vide.

### **void free_stack(stack\* s)**
Fonction qui libère la mémoire allouée pour une pile `s`.

### **bool isempty_stack(stack\* s)**
Fonction qui renvoie `true` si la pile `s` est vide, `false` sinon.

### **int size_stack(stack\* s)**
Fonction qui renvoie le nombre d'éléments (paires de coordonnées \* 2) dans la pile `s`.
note : coordonnées\*2 car chaque coordonnée est un couple de deux entiers.

### **void pop(stack\* s, int\* x, int\* y)**
Fonction qui renvoie les coordonnées en tête de la pile `s` et les retire de la liste.
Les coordonnées sont stockées dans `x` et `y`.

### **void rpop(stack\* s, int\* x, int\* y)**
Fonction qui renvoie les coordonnées d'une case aléatoire de la pile `s` et les retire de la liste.
Les coordonnées sont stockées dans `x` et `y`.

### **void push(int x, int y, stack\* s)**
Fonction qui ajoute les coordonnées `(x, y)` à la pile `s`.

# Fichier " mazemaker.c "
Ce fichier contient les fonctions de génération de labyrinthe, ainsi que les fonctions auxilliaires nécessaires à leur création.

## Macros
### **CAN_MOVE_LEFT (\*x > 0 && !get_bool(tab_visited, \*x - 1, \*y))**
Macro qui nous permet de savoir si on peut se déplacer à gauche : si nous ne somme pas sur le bord gauche et que la case n'est pas visitée.

### **CAN_MOVE_RIGHT (\*x + 1 < maze->width && !get_bool(tab_visited, \*x + 1, \*y))**
Macro qui nous permet de savoir si on peut se déplacer à droite : si nous ne somme pas sur le bord droit et que la case n'est pas visitée.

### **CAN_MOVE_UP (\*y > 0 && !get_bool(tab_visited, \*x, \*y - 1))**
Macro qui nous permet de savoir si on peut se déplacer en haut : si ne nous somme pas sur le bord haut et que la case n'est pas visitée.

### **CAN_MOVE_DOWN (\*y + 1 < maze->height && !get_bool(tab_visited, \*x, \*y + 1))**
Macro qui nous permet de savoir si on peut se déplacer en bas : si nous ne somme pas sur le bord bas et que la case n'est pas visitée.

## Fonctions auxilliaires

### **int set_connexion(maze_t maze, bool_tab is_connexe, int dx, int dy)**
Fonction qui met à jour le tableau de booléen `is_connexe` en ajourant les cases connexes à la case `(dx, dy)`.
Retourne le nombre de cases mise à jour (≠ nombre de cases connexes).

`maze` : labyrinthe

### **int can_move_dir(maze_t\* maze, int\* x, int\* y, bool_tab tab_visited, int dir)**
Fonction qui renvoie le résultat de la macro `CAN_MOVE_dir` où `dir` est la direction de déplacement.

### **bool lbp_path_move(const maze_t\* maze, int\* x, int\* y, const bool_tab tab_visited)**
Fonction qui modifie `x` et `y` pour arriver sur une cellule adjacente non visitée si possible.

### **void lbp_path(maze_t\* maze, int\* x, int\* y, int\* x_2, int\* y_2, const bool_tab tab_visited)**
Fonction récursive qui ajoute les murs entres les cases visitées qui ne sont pas notre précédente (il y a un appel à lbp_path_move pour se déplacer).


## Fonctions principales
Toutes les fonctions de générations de labyrinthes suivent la déclaration suivante :

```maze_t *nom_de_la_fonction(int width, int height)```
où `width` et `height` sont les dimensions du labyrinthe à générer.

### column_maze
Fonction qui crée un labyrinthe parfait en supprimant tous les murs internes horizontaux et un mur vertical par colonne.

### line_maze
Fonction qui crée un labyrinthe parfait en supprimant tous les murs internes verticaux et un mur horizontal par ligne.

### one_way_maze
Fonction qui crée un labyrinthe parfait à partir d'un chemin principal. Semblable à un hunt-and-kill.

### octopus_maze
Fonction qui crée un labyrinthe parfait en partant d'un point central et en créant des branches.
Les branches sont crées en partant du centre et en allant vers les bords linéairement, ce qui donne un labyrinthe prévisible et inintéressant.
N'est pas compilé dans le programme final.

### my_octopus_maze
Version améliorée de `octopus_maze` qui impose un choix aléatoire pour la direction des branches, permettant de créer des labyrinthes plus variés.
Elle est compilé sous le nom `octopus_maze` dans le programme final.

### comb_maze
Fonction qui crée un labyrinthe parfait en ne cassant des murs que vers le bas ou la droite.
Toute case de coordonnées `(x, y)` est donc accessible depuis la case `(0, 0)` via un chemin de x + y cases (pire cas possible d'un parcours en largeur).

###  proto_comb_maze
Fonction destinée à remplacer `comb_maze` dans le programme final, elle n'a jamais été terminée et n'est pas compilée.

Il a été ajouté parmis les easter eggs malgré ses lacunes. Il s'agit du seul générateur de labyrinthe qui ne soit pas parfait.
### hunt_kill_maze
Fonction qui crée un labyrinthe parfait en partant d'une case aléatoire et en cassant des murs aléatoirement jusqu'à ce que le labyrinthe soit parfait.

### by_path_maze
Fonction qui crée un labyrinthe parfait à partir de la case `(0, 0)` en murant des couloirs puis sélectionne la prochaine case de en partant de la première ligne de gauche à droite puis en changeant de ligne.

### cross_maze
Fonction qui crée un labyrinthe parfait en deux étapes :
1. Perforation de murs autour de cases aléatoires sans créer de cycle (les perforations sont en forme de croix, d'où le nom)
2. Perforation de murs pour connecter les cases entre elles.

## Fonctions principales innutilisées
Ces fonctions ne sont accessible à l'utilisateur que via l'easter egg `goose`

Elles ne sont pas vérifiées et peuvent causer des erreurs, mais devraient être valides et générer des labyrinthes parfaits.

### snail_maze
Fonction qui crée un labyrinthe parfait en partant d'une case aléatoire et en cassant des murs en spirale.

Si la cellule de départ touche un mur extérieur, génère une seule spirale, sinon, en génère deux.

### snake_maze
Fonction qui crée un labyrinthe parfait en partant d'une case aléatoire et en cassant des murs en zigzag.

Forme particulière d'un line_maze ou column_maze.

### reverse_comb_maze
Fonction qui crée un labyrinthe parfait en ne cassant des murs que vers le haut ou la gauche.

### reccursive_maze
Fonction qui crée un labyrinthe parfait en réduisant le problème à une taille plus petite à chaque étape. Quand le problème est réduit à une taille de 1x1, le labyrinthe est parfait.
Il suffit de briser un murs entre deux labyrinthe parfaits voisins pour obtenir un labyrinthe parfait.

### weeping_willow_maze
Fonction qui crée un labyrinthe parfait sensé avoir la forme d'un saule pleureur (un tronc au centre, des racines en bas et des branches en haut).

Le résultat est déterminsite (et ne ressemble pas vraiment à un saule pleureur).

Les dimensions d'entrée doivent être d'au moins 4 par 10.

### four_maze
Fonction qui demande à quatre autres fonctions de lui créer un labyrinthe parfait d'un quart de la taille du labyrinthe final, puis les connecte entre eux.

Les dimensions d'entrée doivent être d'au moins 4 par 4.

### golden_maze
Fonction qui demande à d'autre fonction de lui créer des labyrinthe parfait avant de les connecter en une forme rappellant une coquille de nautile.

Les dimensions d'entrée doivent être d'au moins 4 par 4.

## Autres fonctions

### **void tear(maze_t maze, unsigned int prop)**
Fonction perforant des murs aléatoirement dans le labyrinthe `maze` en fonction de la proportion `prop` (en pourcentage) afin de créer des boucles.

# Fichier " test.c "
Ce fichier contient des fonctions d'évaluation de labyrinthe, ainsi que des fonctions auxilliaires nécessaires à leur évaluation.

## Fonctions

### **bool do_made_solvable_maze(func_ptr f, int x, int y)**
Fonction indiquant si un labyrinthe généré par la fonction `f` de dimension inférieure ou égale à `x` par `y` est solvable.

### **bool do_made_connected_maze(func_ptr f, int x, int y)**
Fonction indiquant si un labyrinthe généré par la fonction `f` de dimension inférieure ou égale à `x` par `y` est connexe.

### **bool do_made_perfect_maze(func_ptr f, int x, int y)**
Fonction indiquant si un labyrinthe généré par la fonction `f` de dimension inférieure ou égale à `x` par `y` est parfait.

### **int evaluate_mazemaker(func_ptr f)**
Fonction qui évalue les labyrinthes générés par la fonction `f` de taille maximale 100 x 100.
la fonction renvoie :
- 3 si le labyrinthe est parfait
- 2 si le labyrinthe est connexe
- 1 si le labyrinthe est solvable
- 0 si le labyrinthe n'est pas solvable


Cette fonction est utilisée par la macro `EVALUATE_MAZEMAKER` afin de rendre un résultat plus lisible.

### **int evaluate_time(func_ptr f, char\* name)**
Fonction qui renvoie un score en fonction du temps de génération d'un labyrinthe par la fonction `f`.
Les quatres critères de notation sont :
- la durée de création de labyrinthes "standard" de dimension k x t avec k et t de 1 à 100. (/50)
- la durée de création de labyrinthes "lignes" de dimension 1 x k ou k x 1 avec k de 1 à 100. (/10)
- la durée de création de labyrinthes "petits" de dimension 4 x 4. (/15)
- la durée de création de labyrinthes "moyens" de dimension 100 x 100. (/25)

les critères sont évidemment ajustables et sujet à contextualisation.


# Fichier " solveur.c "
Ce fichier contient les fonctions de résolution de labyrinthe.

Il y a 3 types de parcours pour la résolution de labyrinthe : en profondeur, en largeur et en tirage.

Pour les parcours en profondeur, on utilise `deep` ; pour les parcours en largeur, on utilise `breadth` ; pour les parcours en tirage, on utilise `draw`.

Pour chacun de ses trois types de parcours on a 4 fonctions de résolution et 4 fonctions de visualisation.

## Fonctions de résolution
Toutes les fonctions de résolution de labyrinthes suivent la déclarations suivante :

```*nom_de_la_fonction(maze_t maze)```
où `maze` est le labyrinthe à résoudre.

### bool has_exit_`*type_de_parcours`_seeker
Fonction qui cherche une sortie en fonction du type de parcours, elle ne retourne pas le chemin mais indique si une sortie existe.

La recherche en `profondeur` est généralement plus rapide que la recherche en `largeur`.

### bool is_connexe_`*type_de_parcours`_inspector
Fonction qui vérifie si le labyrinthe est connexe en fonction du type de parcours.
Elle ne donne pas d'indication en cas d'absence de connexité.

### bool is_perfect_`*type_de_parcours`_inspector
Fonction qui vérifie si le labyrinthe est parfait en fonction du type de parcours.
Elle ne donne pas d'indication en cas d'imperfection.

### way\* best_exit_`*type_de_parcours`_seeker
Fonction qui cherche le meilleur chemin de sortie en fonction du type de parcours.
Elle retourne un pointeur sur le chemin trouvé, ou NULL si aucun chemin n'a été trouvé.

La recherche en `largeur` est généralement plus rapide que la recherche en `profondeur`.

## Fonctions de visualisation
La majorité des fonctions de visualisation suivent la déclaration suivante :

```int nom_de_la_fonction(maze_t maze)```
où `maze` est le labyrinthe à visualiser.
La sortie est un entier relatif au type d'erreur rencontré si la visualisation a échouée.

Pour le parcours en `profondeur` et en `tirage`, le nombre de cellules actualisées à la fois est proportionnel à la taille du labyrinthe.

Pour le parcours en `largeur`, les cellules sont actualisées par génération.

### show_has_exit_`*type_de_parcours`_seeker
Fonction qui illustre la fonction `has_exit_*type_de_parcours_seeker`.

### show_is_connexe_`*type_de_parcours`_inspector
Fonction qui illustre la fonction `is_connexe_*type_de_parcours_inspector`.

### show_is_perfect_`*type_de_parcours`_inspector
Fonction qui illustre la fonction `is_perfect_*type_de_parcours_inspector`.

### show_best_exit_`*type_de_parcours`_seeker
Fonction qui illustre la fonction `best_exit_*type_de_parcours_seeker`.
N'enregistre pas le chemin trouvé.

### **int show_the_way(maze_t maze, way\* w)**
La seule fonction de visualisation à ne pas se baser sur un algorithme de résolution, elle illustre le chemin `way` dans le labyrinthe `maze`.
Attention, cette fonction ne vérifie pas si le chemin est bien issu du labyrinthe donné.

# Fichier " outside.c "
Ce fichier contient les fonctions d'écritures et de lectures de nos structures de données dans des fichiers.

## Fonctions auxilliaires

### **void way_to_file_aux(way\* w, FILE\* file)**
Fonction permettant d'inverser l'ordre des cases d'un chemin pour l'écriture dans un fichier.
(les données de la structures sont écrites dans l'ordre inverse de leur lecture)

## Fonctions d'écriture

### **void maze_to_file(maze_t maze, char\* filename)**
Fonction qui écrit un labyrinthe dans un fichier.
Ecrase le fichier si il existe déjà.

`maze` : labyrinthe à écrire.

`filename` : nom du fichier à écrire.

### **void way_to_file(way\* w, char\* filename)**
Fonction qui écrit un chemin dans un fichier.

`w` : chemin à écrire.

`filename` : nom du fichier à écrire.

## Fonctions de lecture

### **maze_t maze_from_file(char\* filename)**
Fonction qui lit un labyrinthe depuis un fichier et le renvoie.

`filename` : nom du fichier à lire.

### **way\* way_from_file(char\* filename)**
Fonction qui lit un chemin depuis un fichier et le renvoie.

`filename` : nom du fichier à lire.

# Fichier " cmd.c " :
Fichier contenant les fonctions de gestion de la ligne de commande.

## Fonctions auxilliaires

### **bool safe_atoi(char\* str, int\* out)**
Fonction pour convertir en entier une chaine de caractères.
La chaines de caractères `str`, si elle est valide, est convertie puis stockée dans `out`.
La fonction renvoie `true` si la conversion a réussie, `false` sinon.

Les phrases d'explications des commandes sont stockées dans des macros pour faciliter la modification.

### **void print_cmd_help(char\* namefile)**
Fonction qui affiche l'aide de la ligne de commande.

`namefile` : nom du fichier exécutable.

## Fonction principale

### **int main (char\* argv[], int argc)**
Interprète les commandes passées en argument du programme et appelle les fonctions correspondantes.

`argv` : tableau des arguments.

`argc` : nombre d'arguments.

# Fichier " testing.c " :
Interprète les commandes passées en argument du programme et appelle les fonctions correspondantes pour l'exécutable de test.

## Fonctions auxilliaires

### ** void usage(char* name)**
Affiche l'aide de la ligne de commande.

`name` : nom de l'exécutable.

### **bool safe_atoi(char* str, int* out)**
cf `cmd.c`

## Fonction principale

### **int main (char* argv[], int argc)**
Interprète les commandes passées en argument du programme et appelle les fonctions correspondantes.

# Fichier " escape.c " :
Ce fichier contient les algorithmes de visite de labyrinthe.

## Fonctions auxilliaires

### **bool has_accessible_cells(int x, int y, bool_tab visited, maze_t maze)**
Fonction qui renvoie `true` si la case `(x, y)` a des cases accessibles non visitées, `false` sinon.

maze : le labyrinthe

visited : tableau de booléens

### **bool get_adj(int x, int y, int\* x_next, int\* y_next, maze_t maze, char dir)**
Fonction qui renvoie true si la case `(x, y)` a une case accessible dans la direction `dir`, `false` sinon.
Elle renvoie également les coordonnées de la case accessible dans `x_next` et `y_next`.

### **bool can_go(int x, int y, maze_t maze, char dir)**
Fonction qui renvoie `true` si la case `(x, y)` a une case accessible dans la direction `dir`, `false` sinon.

### **void go(int\* x, int\* y, int dir)**
Fonction qui déplace les coordonnées `(x, y)` dans la direction `dir`.

### **int visited_value(int\*\* visited, int x, int y, int dir)**
Fonction qui envoie la valeur du tableau visited du voisin de la cellule x y dans la direction dir

## Fonctions principales

Toutes les fonctions de visites de labyrinthe suivent la déclaration suivante :

```int *nom_de_la_fonction(maze_t maze, int x, int y)```
où `maze` est le labyrinthe à visiter, `x` et `y` sont les coordonnées de départ.

Elles renvoient le nombre d'étapes nécessaires pour trouver la sortie.

Elles permettent toutes de visualiser le parcours dans le labyrinthe.
Elles s'arrêtent quand la sortie est trouvée ou quand l'utilisateur arrête le programme avec la touche `ESCAPE` ou `ENTER`.

### random_escape
Fonction qui visite un labyrinthe en choisissant aléatoirement la direction à suivre à chaque intersection.

### cheat_escape
Fonction qui visite un labyrinthe en passant à travers les murs.

### hunt_kill_escape
Fonction qui visite un labyrinthe en ayant une approche de type hunt-and-kill.
Durant la visite, on connait les cases qu'on a visité et on cherche à visiter les cases non visitées.

### right_hand
Fonction qui visite un labyrinthe en suivant le mur à sa droite.

### right_hand_p_de
Fonction qui visite un labyrinthe en respectant l'algorithme main droite
en considérant uniquement les cases les moins parcourues qui ne sont pas des culs de sacs déjà détectés

### random_p_de
Fonction qui visite un labyrinthe en se déplaçant aléatoirement parmis les cases les moins parcourues qui ne sont pas des culs de sacs déjà détectés

# Fichier " game.c " :
Ce fichier contient les fonctions de gestion de la fenêtre de jeu.

## Fonctions auxilliaires

### void reprint_maze(const maze_t maze, SDL_Renderer *renderer, const int dh, const int dw)
Fonction qui affiche les murs du labyrinthe dans la fenêtre de jeu.

### way *local_best_exit_breadth_seeker(const maze_t maze, const int end_x, const int end_y)
Version modifié de best_exit_breadth_seeker qui donne le chemin entre l'entrée et la position actuelle du joueur.

### void local_show_the_way(const maze_t maze, const way *w, SDL_Renderer* renderer, const int dh, const int dw)
version modifié de show_the_way qui affiche le chemin dans la fenêtre de jeu plutôt que dans une nouvelle fenêtre.

### void draw_border(const maze_t maze, SDL_Renderer *renderer, const int x, const int y, const int dw, const int dh)
Fonction qui dessine les bordures d'une cellules sans dépasser sur les murs du labyrinthe.

### void draw_wall(const maze_t maze, SDL_Renderer *renderer, const int x, const int y, const int dw, const int dh)
Fonction qui dessine les murs d'une cellules.

### void clear(const maze_t maze, SDL_Renderer *renderer, const int dw, const int dh)
Fonction qui efface le contenu de la fenêtre de jeu à l'exception de la cellule de sortie.

## Fonctions principales
Toutes les fonction de jeu suivent la déclaration suivante :

```void *nom_de_la_fonction(maze_t maze, int x, int y)``` où `maze` est le labyrinthe à visiter, `x` et `y` sont les coordonnées de départ.

Le joueur est représneter par un carré bleu, la sortie est la cellule (0,0).  
Le joueur peut se déplacer avec les flèches directionnelles, escalader un mur en face de lui avec la touche `c` et quitter le jeu avec les touches `ESCAPE` ou `ENTER`.
Le joueur peut afficher le plus court chemin vers la sortie avec la touche `s`.

### game_show
Lance un jeu dans lequel le joueur voit tout le labyrinthe.

### game
Lance un jeu dans lequel le joueur ne voit que les murs des cellules déjà visitées.

### game_front_blind
Lance un jeu dans lequel le joueur ne voit que les murs des cellules situées devant lui.

### game_half_blind
Lance un jeu dans lequel le joueur ne voit que les murs des vingt dernières cellules visitées.

### game_quarter_blind
Lance un jeu dans lequel le joueur ne voit que les murs de la cellule actuelle et des quatres cellules adjacentes.

### game_blind
Lance un jeu dans lequel le joueur ne voit que les murs de la cellule actuelle.