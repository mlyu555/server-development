#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>

static void daemonize(void);

int main(int argc, char *argv[]) {
    daemonize();

    return 0;
}

static void daemonize(void) {
    pid_t pid;
    int fd, maxfd;

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    if (setsid() == -1) {
        perror("setsid");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    chdir("/");

    // maxfd = sysconf(_SC_OPEN_MAX);
    // if (maxfd == -1) {
    //     maxfd = BD_MAX_CLOSE;
    // }
    // for (fd = 0; fd < maxfd; fd++) {
    //     close(fd);
    // }

    fd = open("/dev/null", O_RDWR);
    if (fd == -1) {
        perror("open('/dev/null')");
        exit(EXIT_FAILURE);
    }

    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);
}