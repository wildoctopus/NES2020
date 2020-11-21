#include <stdio.h>
#include <stdlib.h>

#define RUNNING 0
#define DONE    1

int protothread()
{
    static int step = 1;
    switch(step)
    {
    case 1:
        printf("Executed Step 1\n");
        step = 2;
        return RUNNING;
    case 2:
        printf("Executed Step 2\n");
        step = 3;
        return RUNNING;
    case 3:
        printf("Executed Step 3\n");
        step = 4;
        return RUNNING;
    default:
        printf("Executed Step 4\n");
        step = 4;
        break;

    }
    return DONE;

}

int main(int argc, char *argv[])
{

    int i = 0;
    int ret;

    do
    {
        i++;
        ret = protothread();

    }
    while (ret != DONE);

    printf("thread stopped after %d calls\n", i);

    return 0;
}
