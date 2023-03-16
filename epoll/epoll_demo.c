// .epolldemo 8888
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define PORT 8888

int main(int argc, char *argv[]) {

    int listenfd, clientfd, epfd;
    int i, ret, nready, client_addr_len;
    struct sockaddr_in serv_addr, client_addr;
    struct epoll_event ev, events[1024] = {0};
    char ip[INET_ADDRSTRLEN] = {0};
    char buffer[1024] = {0};

    // socket流程
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0) {
        perror("socket");
        return EXIT_FAILURE;
    }

    memset(&serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    serv_addr.sin_addr.s_addr = htons(INADDR_ANY);

    ret = bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr_in));
    if (ret < 0) {
        perror("bind");
        return EXIT_FAILURE;
    }

    ret = listen(listenfd, 5);
    if (ret < 0) {
        perror("listen");
        return EXIT_FAILURE;
    }

    // epoll流程
    epfd = epoll_create(1);
    if (epfd < 0) {
        perror("epoll_create");
        return EXIT_FAILURE;
    }

    ev.data.fd = listenfd;
    ev.events = EPOLLIN;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);
    if (ret < 0) {
        perror("epoll_ctl");
        return EXIT_FAILURE;
    }

    while (1) {
        nready = epoll_wait(epfd, events, 1024, -1);
        if (nready < 0) {
            break;
        }

        for (i = 0; i < nready; i++) {
            if (events[i].data.fd == listenfd) {
                memset(&client_addr, 0, sizeof(struct sockaddr_in));
                client_addr_len = sizeof(client_addr);
                clientfd = accept(listenfd, (struct sockaddr *)&client_addr, &client_addr_len);
                if (clientfd < 0) {
                    perror("accept");
                    continue;
                }

                printf("new connection from %s:%d, fd: %d\n", inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip)), ntohs(client_addr.sin_port), clientfd);

                ev.data.fd = clientfd;
                ev.events = EPOLLIN;
                ret = epoll_ctl(epfd, EPOLL_CTL_ADD, clientfd, &ev);
                if (ret < 0) {
                    perror("epoll_ctl");
                    continue;
                }
            } else {
                clientfd = events[i].data.fd;

                ret = recv(clientfd, buffer, 1024, 0);
                if (ret < 0) {
                    if (errno == EAGAIN || errno == EWOULDBLOCK) {
                        continue;
                    }
                } else if (ret == 0) {
                    printf("disconnect from %d\n", clientfd);

                    close(clientfd);
                    ev.data.fd = clientfd;
                    ev.events = EPOLLIN;
                    ret = epoll_ctl(epfd, EPOLL_CTL_DEL, clientfd, &ev);
                    if (ret < 0) {
                        perror("epoll_ctl");
                        continue;
                    }
                } else {
                    printf("recv %dbytes: %s\n", ret, buffer);
                }
            }
        }
    }

    return 0;
}