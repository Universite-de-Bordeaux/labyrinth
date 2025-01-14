link_directories(${CMAKE_SOURCE_DIR})

cmake_minimum_required(VERSION 3.3)
project(main C)

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

add_library(maze src/struct.c src/mazemaker.c src/solveur.c src/outside.c src/cmd.c src/main.c)
#add_library(test_interne testing.c)

add_executable(main src/main.c)
#add_executable(test_interne testing.c)

target_link_libraries(main maze)
#target_link_libraries(test_interne maze test_interne)