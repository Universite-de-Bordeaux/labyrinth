#include <stdlib.h>
#include <time.h>
#include "cmd.h"

int main(const int argc, char* argv[])
{
    srand(time(NULL)); // NOLINT(*-msc51-cpp)
    cmd(argv, argc);
    return 0;
}
