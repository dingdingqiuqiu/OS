#include <stdio.h>
#include "utils.h"

int main() {
    printf("Welcome to my program!\n");

    int a = 5, b = 3;
    printf("The sum of %d and %d is %d.\n", a, b, add(a, b));

    return 0;
}
