#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int *arr, nbytes;

    arr = (int *)malloc(sizeof(int) * 3);
    nbytes = malloc_usable_size(arr);

    printf("sizeof(int)  = %lu\n", sizeof(int));
    printf("malloc_usable_size = %d\n", nbytes);
    printf("malloc             = %lu\n", sizeof(arr));

    return 0;
}