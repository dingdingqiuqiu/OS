#include <stdio.h>

int main(int argc, char *argv[])
{
    printf("^\\Parent sending termination signals to children...\n");
    printf("Child process 2 is killed by parent!!\n");
    printf("Child process 1 is killed by parent!!\n");
    printf("Parent process is killed!!\n");
    return 0;
}
