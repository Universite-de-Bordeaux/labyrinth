#include "cmd.h"
#include "struct.h"

int main(const int argc, char *argv[]) {
    queue *q = create_queue();
    for(int i = 0; i < 12; i++)
    {
        enqueue(i, i, q);
        printf("size : %d\n", size_queue(q));
    }
    print_queue(q);
    int x, y;
    for(int i = 0; i < 9; i++)
    {
        dequeue(q, &x, &y);
        printf("dequeue \nx : %d y : %d\n", x, y);
        printf("size : %d\n", size_queue(q));
    }
    print_queue(q);
    free_queue(q);
    //cmd(argv, argc);
    return 0;
}
