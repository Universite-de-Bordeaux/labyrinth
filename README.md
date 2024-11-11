# Project Labyrinth

Le Projet Labyrinth a pour but de pouvoir créer et résoudre des labyrinthes parfaits en utilisant différents algorithmes afin de pouvoir les comparer

## Dependences pour compiler
Pour compiler le projet il est nécessaire d'avoir
- gcc, pour compiler pour le projet
- SDL, la librairie graphique que nous utilisons

## Installer les packages sur Debian
Utiliser cette commande pour télécharger et installer les dépendances
````sudo apt install gcc && sudo apt-get install libsdl2-dev````

## Utilisation

Les différents noms de labyrinthes sont abrégés comme ceci :
- owm : one way maze
- cbm : comb maze
- hkm : hunt and kill maze
- bpm : by path maze
- sm : straight maze (column maze or line maze)
- cm : cross maze

Les différents types d'analyse seront abrégés comme ceci :
- isp : is perfect (est parfait)
- isc : is connected (est connexe)
- he : has exit (possède une sortie)
- she : shortest exit (enregistre en mémoire le chemin de sortie le plus court)

Il est impossible de traiter plusieurs labyrinthes ou chemins simultanément.
Si un argument apparaît plusieurs fois, seul le dernier est utilisé
If an argument occures multiple times, the last one will be taken
Si un argument est inconnu, il est ignoré
If an argument is unknown, it will be ignored
### Afficher l'aide en cmd
Afficher l'aide
-h : help

### Générer un labyrinthe

Générer un labyrinthe :
-g <type>

On peut préciser les dimensions "nb1" x "nb2" :
-g <type> <nb1> <nb2>

type : est un générateur de labyrinthe parmis ceux de cette liste [owm, cbm, hkm, bpm, sm, cm]
nb1, nb2 : un couple de valeurs strictement positives, la valeur par défaut, ou en cas d'erreur, est 10 10

### Lire un labyrinthe à partir d'un fichier

Extraire un labyrinthe depuis un fichier :
-r <filename>
filename : le nom du fichier

### Arguments suivant la génération d'un labyrinthe

#### Créer des cycles

Supprimer une partie des murs du labyrinthe :
-t

On peut préciser la proportion de murs éliminés en pourcentage "percent" :
-t <percent>

nb : valeur strictement positive, la valeur par défaut, ou en cas d'erreur, est 4

#### Lire un chemin à partir d'un fichier

Extraire un chemin depuis un fichier :
-rw <filename>

filename : le nom du fichier

#### Analyse

Analyser un labyrinthe :
-slv <inspection> <solver>

inspection : le type de recherche [isp, isc, he, she]
solver : un type d'algorithme parmis ceux de la liste [deep, breadth]

### Ecrire dans un fichier

Ecrire un labyrinthe dans un fichier :
-w <filename>

filename : le nom du fichier


Ecrire un chemin dans un fichier :
-ww <filename>

filename : le nom du fichier

#### Affichage

Afficher un labyrinthe :
-sh : show maze


Afficher le déroulement d'une analyse sans renvoyer le résultat :
-sh <inspection> <solver>

inspection : le type de recherche [isp, isc, he, she]
solver : un type d'algorithme parmis ceux de la liste [deep, breadth]

## Exemples d'utilisation
