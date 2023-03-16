#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

// 管道测试
// #define DEBUG_PIPE
#ifdef DEBUG_PIPE

#endif //DEBUG_PIPE



// 信号量测试
#define DEBUG_SEM

#ifdef DEBUG_SEM
#include <sys/sem.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *arr;
    struct seminfo *_buf;
};

// op -1 P 0 V
void pv(int semid, int op) {
    struct sembuf semb;
    semb.sem_num = 0;
    semb.sem_op = op;
    semb.sem_flg = SEM_UNDO;
    semop(semid, &semb, 1);
}

#endif //DEBUG_SEM


int main(int argc, char *argv[]) {
    pid_t pid;
    int ret;
    char buf[1024];

#ifdef DEBUG_PIPE
    // pipe
    int pipefd[2];
    ret = pipe(pipefd);
    if (ret == -1) {
        perror("pipe");
        return -1;
    }
#endif //DEBUG_PIPE

#ifdef DEBUG_SEM

    int semid;
    semid = semget(IPC_PRIVATE, 1, 0666);
    union semun semu;
    semu.val = 1;
    semctl(semid, 0, SETVAL, semu);

#endif //DEBUG_SEM

    pid = fork();
    if (pid == -1) {
        perror("fork");
        return -1;
    }

    // parent process
    if (pid > 0) {
        printf("prarent process, pid: %d, ppid: %d\n", getpid(), getppid());
        printf("----child process, pid: %d\n", pid);
        printf("\n");

#ifdef DEBUG_PIPE
        // pipe write
        close(pipefd[0]);
        const char msg[] = "parent send to child hello!";
        ret = write(pipefd[1], msg, sizeof(msg));
        printf("%d\t, send: %s\n", ret, msg);
        close(pipefd[1]);

        printf("-----------------------------------------\n");
        waitpid(pid, NULL, 0);
    }
#endif //DEBUG_PIPE

    // child process
    if (pid == 0) {
        printf("prarent process, pid: %d, ppid: %d\n", getpid(), getppid());
        printf("\n");

#ifdef DEBUG_PIPE
        // pipe read
        close(pipefd[1]);
        ret = read(pipefd[0], buf, 1023);
        printf("%d\t, recv: %s\n", ret, buf);
        close(pipefd[0]);
#endif //DEBUG_PIPE

        printf("-----------------------------------------\n");
    }

    return 0;
}