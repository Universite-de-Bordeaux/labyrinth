# je sais plus ce qu'est le nom de ce que je fais

## Fichier " cmd.c " :

ne pas faire car future main...

## Fichier " mazemaker.c "

### Fonctions auxilliaires



#### maze_t line_maze(const int width, const int height)
Fonction qui crée un labyrinthe parfait en supprimant tous les murs internes verticaux et un mur horizontal par ligne.
width : largeur du labyrinthe
height : hauteur du labyrinthe

#### maze_t column_maze(const int width, const int height)
Fonction qui crée un labyrinthe parfait en supprimant tous les murs internes horizontaux et un mur vertical par colonne.
width : largeur du labyrinthe

height : hauteur du labyrinthe

#### maze_t one_way_maze(const int width, const int height)
Fonction qui crée un labyrinthe imparfait ou seuls les chemins de sortie possèdent des embranchements.
width : largeur du labyrinthe
height : hauteur du labyrinthe