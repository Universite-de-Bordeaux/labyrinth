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
- <span style="color:red">owm</span> : one way maze
- <span style="color:red">cbm</span> : comb maze
- <span style="color:red">hkm</span> : hunt and kill maze
- <span style="color:red">bpm</span> : by path maze
- <span style="color:red">sm</span> : straight maze (column maze or line maze)
- <span style="color:red">cm</span> : cross maze

Les différents types d'analyse seront abrégés comme ceci :
- <span style="color:red">isp</span> : is perfect
- <span style="color:red">isc</span> : is connected
- <span style="color:red">he</span> : has exit
- <span style="color:red">she</span> : shortest exit (enregistre en mémoire le chemin de sortie le plus court)

Il est impossible de traiter plusieurs labyrinthes ou chemins simultanément.
Seule la dernière occurernce de chaque argument est prise en compte.
Tout argument est inconnu est ignoré.
### Afficher l'aide en cmd
Afficher l'aide
<span style="color:green">-h</span> : help

### Générer un labyrinthe

Générer un labyrinthe :
<span style="color:green">-g</span> <span style="color:red">type</span>

On peut préciser les dimensions "nb1" x "nb2" :
<span style="color:green">-g</span> <span style="color:red">type</span> <span style="color:blue">nb1</span> <span style="color:blue">nb2</span>


<span style="color:red">type</span> : est un générateur de labyrinthe parmis ceux de cette liste [<span style="color:red">owm</span>, <span style="color:red">cbm</span>, <span style="color:red">hkm</span>, <span style="color:red">bpm</span>, <span style="color:red">sm</span>, <span style="color:red">cm</span>]
<span style="color:blue">nb1</span>, <span style="color:blue">nb2</span> : un couple de valeurs strictement positives, la valeur par défaut, ou en cas d'erreur, est 10 10

### Lire un labyrinthe à partir d'un fichier

Extraire un labyrinthe depuis un fichier :
<span style="color:green">-r</span> <span style="color:blue">filename
filename</span> : le nom du fichier

### Arguments suivant la génération d'un labyrinthe

#### Créer des cycles

Supprimer une partie des murs du labyrinthe :
<span style="color:green">-t</span>

On peut préciser la proportion de murs éliminés en pourcentage "percent" :
<span style="color:green">-t</span> <span style="color:blue">percent</span>

<span style="color:blue">percent</span> : valeur strictement positive, la valeur par défaut, ou en cas d'erreur, est 4

#### Lire un chemin à partir d'un fichier

Extraire un chemin depuis un fichier :
<span style="color:green">-rw</span> <span style="color:blue">filename</span>

<span style="color:blue">filename</span> : le nom du fichier

#### Analyse

Analyser un labyrinthe :
<span style="color:green">-slv</span> <span style="color:red">inspection</span> <span style="color:red">solver</span>

<span style="color:red">inspection</span> : le type de recherche [<span style="color:red">isp</span>, <span style="color:red">isc</span>, <span style="color:red">he</span>, <span style="color:red">she</span>]
<span style="color:red">solver</span> : un type d'algorithme parmis ceux de la liste [<span style="color:red">deep</span>, <span style="color:red">breath</span>]

### Ecrire dans un fichier

Ecrire un labyrinthe dans un fichier :
<span style="color:green">-w</span> <span style="color:blue">filename</span>

<span style="color:blue">filename</span> : le nom du fichier

Ecrire un chemin dans un fichier :
<span style="color:green">-ww</span> <span style="color:blue">filename</span>

<span style="color:blue">filename</span> : le nom du fichier

#### Affichage

Afficher un labyrinthe :
<span style="color:green">-sh</span>


Afficher le déroulement d'une analyse (sans en renvoyer le résultat) :
<span style="color:red">inspection</span> <span style="color:red">solver</span> <span style="color:green">-sh</span>

<span style="color:red">inspection</span> : le type de recherche [<span style="color:red">isp</span>, <span style="color:red">isc</span>, <span style="color:red">he</span>, <span style="color:red">she</span>]
<span style="color:red">solver</span> : un type d'algorithme parmis ceux de la liste [<span style="color:red">deep</span>, <span style="color:red">breath</span>]


Afficher un chemin :
-shw
(Attention, nécessite un labyrinthe ET un chemin)

## Exemples d'utilisation

Générer un labyrinthe et l'enregistrer dans un fichier
-g cm -w temp


Récupérer le labyrinthe avant de l'afficher
-r temp -sh


Afficher l'analyse du labyrinthe
-r temp -sh isp deep


Sans passer par un fichier temporaire
-g cm -w temp -sh isp deep


Enregistrer un labyrinthe et écrire le chemin le plus court pour le résoudre et les enregistrer dans des fichiers
-g bpm -w temp -ww temp_c -slv she breath


Afficher le chemin dans le labyrinthe enregistré
-r temp -rw temp_c -shw
