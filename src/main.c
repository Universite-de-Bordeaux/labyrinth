#include <stdlib.h>
#include <time.h>
#include "cmd.h"

int main(const int argc, char *argv[]) {
    srand(time(NULL));
    cmd(argv, argc);
    return 0;
}
