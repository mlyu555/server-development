#include <stdlib.h>
#include <stdio.h>


void func() {

    static int s_count = 0;

    printf("func() run: %d\n", ++s_count);
}


int main() {
    int count = 5;
    while (count--) {
        func();
    }

    return 0;
}