#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/msg.h>
#include <string.h>

#define IPC_KEY 5000

typedef struct {
    long msgtype;
    char content[2048];
}mq_message_t;

void receive() {
    int msgid;
    int msgtype;
    char msg_content[2048];
    int retval;
    mq_message_t *mq_msg;

    while (1)
    {
        msgid = msgget((key_t)IPC_KEY, IPC_EXCL);
        if (msgid == -1) {
            perror("msgget");

            sleep(3);
            continue;
        }

        retval = msgrcv(msgid, mq_msg, sizeof(mq_msg), 0, 0);
        printf("pid[%d] recv data[%s]\n", getpid(), mq_msg->content);
    }
}

int main(int argc, char *argv[]) {
    int i;
    int pid;

    for (i = 0; i < 5; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork");
        } else if (pid == 0) {
            receive();
        }
    }

    return 0;
}