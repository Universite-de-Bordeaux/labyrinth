# Define the minimum CMake version and the project
cmake_minimum_required(VERSION 3.3)
project(Labyrinth C)

# Set the compilation flags
set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

# Find and include SDL2
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Define source files for libraries
set(MAZE_SOURCES src/struct.c src/mazemaker.c src/solveur.c src/outside.c src/escape.c)
set(TEST_SOURCES src/struct.c src/mazemaker.c src/solveur.c src/test.c)

# Create libraries
add_library(maze ${MAZE_SOURCES})
add_library(test_interne ${TEST_SOURCES})

# Define executables
add_executable(main src/cmd.c)
add_executable(test src/testing.c)

# Link libraries to executables
target_link_libraries(main maze ${SDL2_LIBRARIES})
target_link_libraries(test test_interne ${SDL2_LIBRARIES})
