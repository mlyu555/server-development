#include <stdlib.h>
#include <stdio.h>


#include <sys/sem.h>

int main(int argc, char *argv[]) {

    int semid = 0;

    semid = semget((key_t)12345, 1, 0666 | IPC_CREAT);

    if (argc > 1) {
        
    }

}