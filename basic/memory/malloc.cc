#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>


void test_malloc() {
    void *p = malloc(1024);
    printf("address of p: %p\n", p);
    free(p);
}


void test_align() {
    void *p = malloc(1024);
    void *p8 = aligned_alloc(8, 1024);
    void *p16 = aligned_alloc(16, 1024);
    void *p32 = aligned_alloc(32, 1024);
    void *p64 = aligned_alloc(64, 1024);

    printf("address of p: %p\n", p);
    printf("address of p8: %p\n", p8);
    printf("address of p16: %p\n", p16);
    printf("address of p32: %p\n", p32);
    printf("address of p64: %p\n", p64);

    free(p);
    free(p8);
    free(p16);
    free(p32);
    free(p64);
}


void test_malloc_usable_size() {
    int *arr, nbytes;

    arr = (int *)malloc(sizeof(int) * 3);
    nbytes = malloc_usable_size(arr);

    printf("sizeof(int)  = %lu\n", sizeof(int));
    printf("malloc_usable_size = %d\n", nbytes);
    printf("malloc             = %lu\n", sizeof(arr));
}

int main(int argc, char *argv[]) {

#if 0

    test_malloc();

#endif

#if 0

    test_malloc_usable_size();

#endif

#if 1

    test_align();

#endif

    return 0;
}