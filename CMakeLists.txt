# on défini le chemin
link_directories(${CMAKE_SOURCE_DIR})

# on défini le projet et ses prérequis
cmake_minimum_required(VERSION 3.3)
project(main)

# on défini les flags utilisés lors de la compilation (à modifier si besoin)
set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

# SDL2
find_package(SDL2 REQUIRED)
include_directories(SDL2Test ${SDL2_INCLUDE_DIRS})

# on crée les bibliothèques
#pour tout le projet
add_library(maze src/struct.c src/mazemaker.c src/solveur.c src/outside.c src/cmd.c)
#juste pour les tests
add_library(test_interne src/test.c)

# on ajoute les exécutables
add_executable(main src/main.c)
add_executable(test src/testing.c)

# on ajoute les dépendances
target_link_libraries(main maze ${SDL2_LIBRARIES})
target_link_libraries(test maze test_interne ${SDL2_LIBRARIES})