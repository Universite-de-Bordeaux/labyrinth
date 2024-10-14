#include "mazemaker.h"
#include "test.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "outside.h"
#include <string.h>

int main(const int argc, char *argv[]) {
    if(argv[1] == NULL)
    {
        printf("Usage : %s arg1 arg2 ... \n", argv[0]);
        return 1;
    }
    if(!strcmp(argv[1], "-g") || !strcmp(argv[1], "--generate"))
    {
        if(argv[2] == NULL || argv[3] == NULL || argv[4] == NULL || argv[5] == NULL)
        {
            printf("Usage : %s -g <generator> <width> <height> <namefile>\n", argv[0]);
            return 1;
        }
        maze_t maze;
        if(!strcmp(argv[2], "powm"))
        {
            maze = perfect_one_way_maze(atoi(argv[3]), atoi(argv[4]));
        }
        else if(!strcmp(argv[2], "iowm"))
        {
            maze = imperfect_one_way_maze(atoi(argv[3]), atoi(argv[4]));
        }
        else if(!strcmp(argv[2], "hkm"))
        {
            maze = hunt_kill_maze(atoi(argv[3]), atoi(argv[4]));
        }
        else if(!strcmp(argv[2], "bpm"))
        {
            maze = by_path_maze(atoi(argv[3]), atoi(argv[4]));
        }
        else if(!strcmp(argv[2], "lm"))
        {
            maze = line_maze(atoi(argv[3]), atoi(argv[4]));
        }
        else if(!strcmp(argv[2], "cm"))
        {
            maze = column_maze(atoi(argv[3]), atoi(argv[4]));
        }
        else
        {
            printf("Usage : %s -g <generator> <width> <height> <namefile>\n", argv[0]);
            printf("<generator> : powm, iowm, hkm, bpm, lm, cm\n");
            return 1;
        }
        if(argv[5] == NULL)
        {
            printf("Usage : %s -g <generator> <width> <height> <namefile>\n", argv[0]);
            return 1;
        }
        maze_to_file(maze, argv[5]);
        printf("Maze saved in %s\n", argv[5]);
        free_maze(maze);
        return 0;
    }
    if(!strcmp(argv[1], "-e") || !strcmp(argv[1], "--evaluate"))
    {
        if(argv[2] == NULL || argv[3] == NULL || argv[4] == NULL || argv[5] == NULL)
        {
            printf("Usage : %s -e <generator> <type eval> <max_x> <max_y>\n", argv[0]);
            return 1;
        }
        func_ptr f;
        char *name;
        if(!strcmp(argv[2], "powm"))
        {
            f = &perfect_one_way_maze;
            name = "perfect_one_way_maze";
        }
        else if(!strcmp(argv[2], "iowm"))
        {
            f = &imperfect_one_way_maze;
            name = "imperfect_one_way_maze";
        }
        else if(!strcmp(argv[2], "hkm"))
        {
            f = &hunt_kill_maze;
            name = "hunt_kill_maze";
        }
        else if(!strcmp(argv[2], "bpm"))
        {
            f = &by_path_maze;
            name = "by_path_maze";
        }
        else if(!strcmp(argv[2], "lm"))
        {
            f = &line_maze;
            name = "line_maze";
        }
        else if(!strcmp(argv[2], "cm"))
        {
            f = &column_maze;
            name = "column_maze";
        }
        else
        {
            printf("Usage : %s -e <generator> <type eval> <max_x> <max_y>\n", argv[0]);
            printf("<generator> : powm, iowm, hkm, bpm, lm, cm\n");
            return 1;
        }
        if(!strcmp(argv[3], "time"))
        {
            EVALUATE_TIME(f, name, atoi(argv[4]), atoi(argv[5]));
            return 0;
        }
        if(!strcmp(argv[3], "type"))
        {
            EVALUATE_MAZEMAKERN(f, name, atoi(argv[4]), atoi(argv[5]));
            return 0;
        }
        printf("Usage : %s -e <generator> <type eval> <max_x> <max_y>\n", argv[0]);
        printf("<generator> : powm, iowm, hkm, bpm, lm, cm\n");
        printf("<type eval> : time, type\n");
        return 1;
    }
    if(!strcmp(argv[1], "-s") || !strcmp(argv[1], "--show"))
    {
        if(argv[2] == NULL)
        {
            printf("Usage : %s -s <filename>\n", argv[0]);
            return 1;
        }
        const maze_t maze = maze_from_file(argv[2]);
        print_maze(maze);
        free_maze(maze);
        return 0;
    }
    if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
    {
        printf("<generator> : powm, iowm, hkm, bpm, lm, cm\n");
        printf("<type eval> : time, type\n");
        printf("Usage : %s -g <generator> <width> <height> <namefile>\n", argv[0]);
        printf("Usage : %s -s <namefile>\n", argv[0]);
        printf("Usage : %s -e <generator> <type eval> <max_x> <max_y>\n", argv[0]);
        printf("Usage : %s -h\n", argv[0]);
        return 0;
    }
    printf("Usage : %s arg1 arg2 ... \n", argv[0]);
    return 0;
}
