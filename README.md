# Project Labyrinth

Le Projet Labyrinth a pour but de pouvoir créer et résoudre des labyrinthes parfaits en utilisant différents algorithmes afin de pouvoir les comparer.

## Dependences pour compiler
Pour compiler le projet il est nécessaire d'avoir
- gcc, pour compiler pour le projet
- SDL, la librairie graphique que nous utilisons

### Sur Linux 
#### Debian

Vous pouvez utiliser cette commande
````sudo apt install gcc && sudo apt-get install libsdl2-dev````

### Sur Windows (10 et 11)

#### Installer Mingw
Il vous faut installer [mingw-w64]([https://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/installer/mingw-w64-install.exe/download](https://github.com/brechtsanders/winlibs_mingw/releases/download/14.2.0mcf-12.0.0-ucrt-r1/winlibs-x86_64-mcf-seh-gcc-14.2.0-mingw-w64ucrt-12.0.0-r1.7z)) ou [mingw-w32](https://github.com/brechtsanders/winlibs_mingw/releases/download/14.2.0posix-18.1.8-12.0.0-ucrt-r1/winlibs-i686-posix-dwarf-gcc-14.2.0-llvm-18.1.8-mingw-w64ucrt-12.0.0-r1.7z) selon votre système.
Extraire l'archive 7zip, puis ajouter le chemin absolu de votre configuration mingw à la variable d'envrionnment PATH, le chemin sera du type suivant : 
VotreInstallation\wingw64\bin 
ou 
VotreInstallation\wingw32\bin
