#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

#define MAX_BUFFER_LENGTH 1024

int main( int argc, char *argv[] ) {

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


    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if ( sockfd == -1) {
        perror("socket");
        return EXIT_FAILURE;
    }

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr = (argc == 3)? inet_addr(ip) : INADDR_ANY;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = (argc == 3)? ntohs(port): ntohs(8888);

    int connfd = connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    if (connfd == -1) {
        perror("connect");
        return EXIT_FAILURE;
    }

    char send_buf[MAX_BUFFER_LENGTH] = {0};
    memcpy(send_buf, "this is test data!", MAX_BUFFER_LENGTH);
    ssize_t ret = send(connfd, send_buf, sizeof(send_buf), 0);
    if ( ret == -1 ) {
        perror("send");
        return EXIT_FAILURE;
    }
    printf("send: %ldbytes|%s|\n", ret, send_buf);

    char recv_buf[MAX_BUFFER_LENGTH] = {0};
    ret = recv(connfd, recv_buf, MAX_BUFFER_LENGTH, 0);
    if ( ret == -1 ) {
        perror("recv");
        return EXIT_FAILURE;
    }
    printf("recv: %ldbytes|%s|\n", ret, recv_buf);

    close(connfd);
    close(sockfd);

    return 0;
}