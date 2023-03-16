#include <stdio.h>
#include <stdlib.h>

struct Time {
    unsigned int year;
    unsigned int month;
    unsigned int day;
};

// 位域 Bit Fields
// 位可表示的最大值为: 2^x
struct BFTime {
    unsigned int year:14,       // 最多表示0-9999，14bits即可
            month:4,    // 一年12个月，4bits即可
        day:5;      // 一月最多31天，5bits即可
};

// test: 必须指明有无符号
struct BFTime2 {
    int year:14,
        month:4,
        day:5;
};


struct  Test {
    char x;
    int y;
    long long z;
};


struct BFTest {
    char x:2;
    int y:6;
    long long z:10;
};

int main(int argc, char *argv[]) {
    struct Time now = {2021, 8, 2};
    struct BFTime now2 = {2021, 8, 2};
    struct BFTime2 now3 = {2021, 8, 2};

    printf("%d %d %d\tsizeof(Time): %lu\n", now.year, now.month, now.day, sizeof(struct Time));
    // 2021 8 2        sizeof(Time): 12
    printf("%d %d %d\tsizeof(BFTime): %lu\n", now2.year, now2.month, now2.day, sizeof(struct BFTime));
    // 2021 8 2        sizeof(BFTime): 4

    // warning: implicit truncation from 'int' to bit-field changes value from 8 to -8 [-Wbitfield-constant-conversion]
    printf("%d %d %d\tsizeof(BFTime2): %lu\n", now3.year, now3.month, now3.day, sizeof(struct BFTime2));
    // 2021 -8 2       sizeof(BFTime2): 4


    // 内存对齐
    printf("sizeof(Test): %lu\n", sizeof(struct Test));
    printf("sizeof(BFTest): %lu\n", sizeof(struct BFTest));


    return 0;
}
