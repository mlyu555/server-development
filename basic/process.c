#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    printf("[before fork] 回车\n");                         // 打印一次
    printf("[before fork] 无回车, pid\t%d\t", getpid());    // 打印两次，且pid是父进程的

    pid = fork();
    // 子进程从fork()之后运行，即当前行开始执行

    // fork() >0 父进程；=0 子进程
    if (pid == 0)
        printf("\n[after fork], child pid\t%d\n", getpid());
    else
        printf("\n[after fork], parent pid\t%d\n", getpid());
}   