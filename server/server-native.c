#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_IP "0.0.0.0"
#define DEFAULT_PORT 8888
#define BACKLOG     5
#define RETURN_FAILUTRE (-1)
#define MAX_BUFFER_LENGTH 1024


int main(int argc, char *argv[]) {

    printf("usage: %s [ip] [port]\n", argv[0]);
    printf("    - ip default: 0.0.0.0\n");
    printf("    - port default: 8888\n");
    printf("    - otherwise: ip and port required!!!\n");

    char ip[16] = {0};
    unsigned short port = 0;
    if (argc == 3) {
        // memcpy(ip, argv[1], 16);
        strcpy(ip, argv[1]);
        port = atoi(argv[2]);
    }

    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == RETURN_FAILUTRE) {
        perror("socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = (argc == 3)? inet_addr(ip) : INADDR_ANY;
    // serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // char *ip_string;
    // inet_aton(ip_string, &serv_addr.sin_addr);
    // ip_string = inet_ntoa(serv_addr.sin_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = (argc == 3)? ntohs(port): ntohs(8888);

    int ret = bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if ( ret == RETURN_FAILUTRE) {
        perror("bind");
        return EXIT_FAILURE;
    }

    ret = listen(listenfd, BACKLOG);
    if ( ret == RETURN_FAILUTRE) {
        perror("listen");
        return EXIT_FAILURE;
    }

    while (1) {
        struct sockaddr_in cli_addr;
        socklen_t cli_addr_len;
        int connfd = accept(listenfd, (struct sockaddr *)&cli_addr, &cli_addr_len);
        if ( connfd == RETURN_FAILUTRE ) {
            perror("accept");
            return EXIT_FAILURE;
        }

        char recv_buf[MAX_BUFFER_LENGTH] = {0};
        ssize_t n = recv(connfd, recv_buf, MAX_BUFFER_LENGTH, 0);
        if ( ret == RETURN_FAILUTRE ) {
            perror("recv");
            return EXIT_FAILURE;
        }
        printf("recv: %ldbytes|%s|\n", n, recv_buf);

        char send_buf[MAX_BUFFER_LENGTH] = {0};
        memcpy(send_buf, recv_buf, sizeof(recv_buf));
        n = send(connfd, send_buf, sizeof(send_buf), 0);
        if ( ret == RETURN_FAILUTRE ) {
            perror("send");
            return EXIT_FAILURE;
        }
        printf("send: %ldbytes|%s|\n", n, send_buf);

        close(connfd);
    }

    close(listenfd);

    return 0;
}