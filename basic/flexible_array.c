// 测试柔性数组的用法

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LENGTH 13

struct test_farr {
    int i;
    char d[0];
};

struct test_farr1 {
    int i;
    char d[];
};

struct test_arr {
    int i;
    char d[DATA_LENGTH];
};

struct test_ptr {
    int i;
    char *d;
};

int main(int argc, const char *argv[]) {
    const char *data = "flexible array";

    struct test_farr *fa = (struct test_farr *)malloc(sizeof(struct test_farr) + sizeof(char) * DATA_LENGTH);
    fa->i = DATA_LENGTH;
    strcpy(fa->d, data);

    struct test_farr1 *fa1 = (struct test_farr1 *)malloc(sizeof(struct test_farr1) + sizeof(char) * DATA_LENGTH);
    fa1->i = DATA_LENGTH;
    strcpy(fa1->d, data);

    struct test_arr *aa = (struct test_arr *)malloc(sizeof(struct test_arr));
    aa->i = DATA_LENGTH;
    strcpy(aa->d, data);

    struct test_ptr *pa = (struct test_ptr *)malloc(sizeof(struct test_ptr));
    char *p = (char *)malloc(sizeof(char) * DATA_LENGTH);
    pa->i = DATA_LENGTH;
    strcpy(p, data);
    pa->d = p;

    printf("sizeof(int): %ld\n", sizeof(int));
    printf("sizeof(char*): %ld\n\n", sizeof(char *));

    printf("%10s\t%8s\t%15s\t%10s\t%10s\t%10s\t%10s\n", "name", "sizeof", "address", "&i", "&d", "d->", "data");
    printf("%10s\t%8ld\t%p\t%p\t%p\t%p\t%s\n", "test_farr", sizeof(struct test_farr), fa, &fa->i, &fa->d, fa->d, fa->d);
    printf("%10s\t%8ld\t%p\t%p\t%p\t%p\t%s\n", "test_farr1", sizeof(struct test_farr1), fa1, &fa1->i, &fa1->d, fa1->d, fa1->d);
    printf("%10s\t%8ld\t%p\t%p\t%p\t%p\t%s\n", "test_arr", sizeof(struct test_arr), aa, &aa->i, &aa->d, aa->d, aa->d);
    printf("%10s\t%8ld\t%p\t%p\t%p\t%p\t%s\n", "test_ptr", sizeof(struct test_ptr), pa, &pa->i, &pa->d, pa->d, pa->d);


    return 0;
}