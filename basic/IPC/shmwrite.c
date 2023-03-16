#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <string.h>

#include <sys/shm.h>
#include "shmdata.h"

int main(int argc, char *argv[]) {
    int shmid = 0;
    void *shmptr = NULL;    // 共享内存原首地址
    shmdata_t *shared = NULL;
    char msg[MESSAGE_MAX_SIZE];

    // 创建共享内存
    shmid = shmget((key_t)1234, sizeof(shmdata_t), 0666 | IPC_CREAT);
    if (shmid == -1) {
        perror(shmget);
        exit(EXIT_FAILURE);
    }
    printf("shmid: %d\n", shmid);

    // 将共享内存连接到当前进程地址空间
    shmptr = shmat(shmid, 0, 0);
    if (shmptr == (void *)-1) {
        perror(shmat);
        exit(EXIT_FAILURE);
    }
    printf("shared memory attached at %X\n", (int)shmptr);

    // 设置共享内存
    shared = (shmdata_t *)shmptr;

    while (1)
    {
        // 未读
        while (shared->flag == FLAG_WRITE) {
            sleep(2);
            printf("waiting to write...\n");
        }

        printf("    enter some to write:\n");
        fgets(msg, MESSAGE_MAX_SIZE, stdin);
        strncpy(shared->message, msg, MESSAGE_MAX_SIZE);

        shared->flag = FLAG_READ;

        if (strncmp(msg, "end", 3) == 0) {
            
        }
    }

    // 将共享内存从当前进程中分离
    if (shmdt(shmptr) == -1) {
        perror(shmdt);
        exit(EXIT_FAILURE);
    }

    sleep(2);
    exit(EXIT_SUCCESS);
}