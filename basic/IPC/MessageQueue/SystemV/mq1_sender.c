#include <stdio.h>
#include <stdlib.h>

#include <sys/msg.h>
#include <string.h>

#define IPC_KEY 5000

typedef struct {
    long msgtype;
    char content[2048];
}mq_message_t;

int main(int argc, char *argv[]) {
    int msgid;
    int msgtype;
    char msg_content[2048];
    int retval;
    mq_message_t *mq_msg;

    msgid = msgget((key_t)IPC_KEY, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    while (1) {
        printf("input message type(int), [-9,9]: ");
        scanf("%d", msgtype);
        if (msgtype == 0) break;

        printf("input message to send: ");
        scanf("%s", msg_content);

        mq_msg->msgtype = msgtype;
        strcpy(mq_msg->content, msg_content);

        retval = msgsnd(msgid, (void *)mq_msg, sizeof(mq_msg), IPC_NOWAIT);
        if (retval < 0) {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }
    }

    msgctl(msgid, IPC_RMID, 0);

    return 0;
}