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

int epfd;

struct sockitem {
    int sockfd;
    int (*callback)(int sockfd, int events, void *arg);
};


int accept_cb(int sockfd, int events, void *arg);
int recv_cb(int sockfd, int events, void *arg);
int send_cb(int sockfd, int events, void *arg);


int main(int argc, char *argv[]) {

    int listenfd, clientfd;
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

    // 解决进程退出后端口短时间占用问题，实现端口重用
    int reuse = 0;
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt");
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

    struct sockitem *si = (struct sockitem *)malloc(sizeof(struct sockitem));
    si->sockfd = listenfd;
    si->callback = accept_cb;

    ev.data.ptr = si;
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

        int sockfd;
        for (i = 0; i < nready; i++) {
            if (events[i].events & EPOLLIN) {
                si = (struct sockitem *)events[i].data.ptr;
                si->callback(si->sockfd, events[i].events, si);
            }

            if (events[i].events & EPOLLOUT) {
                si = (struct sockitem *)events[i].data.ptr;
                si->callback(si->sockfd, events[i].events, si);
            }
        }
    }

    return 0;
}

int accept_cb(int sockfd, int events, void *arg) {
    struct sockaddr_in client_addr;
    int client_addr_len;
    int clientfd;
    char ip[INET_ADDRSTRLEN] = {0};
    struct epoll_event ev;
    int ret;

    memset(&client_addr, 0, sizeof(struct sockaddr_in));
    client_addr_len = sizeof(client_addr);
    clientfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (clientfd < 0) {
        perror("accept");
    }

    printf("new connection from %s:%d, fd: %d\n", inet_ntop(AF_INET, &client_addr.sin_addr, ip, sizeof(ip)), ntohs(client_addr.sin_port), clientfd);

    struct sockitem *si = (struct sockitem *)malloc(sizeof(struct sockitem));
    si->sockfd = clientfd;
    si->callback = recv_cb;
    ev.data.ptr = si;
    ev.events = EPOLLIN;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, clientfd, &ev);
    if (ret < 0) {
        perror("epoll_ctl");
    }

    return 0;
}

int recv_cb(int sockfd, int events, void *arg) {
    char buffer[1024] = {0};
    int ret;
    struct epoll_event ev;
    struct sockitem *si = arg;

    ret = recv(sockfd, buffer, 1024, 0);
    if (ret < 0) {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return -1;
        }
    } else if (ret == 0) {
        printf("disconnect from %d\n", sockfd);

        close(sockfd);
        ev.data.ptr = si;
        ev.events = EPOLLIN;
        ret = epoll_ctl(epfd, EPOLL_CTL_DEL, sockfd, &ev);
        if (ret < 0) {
            perror("epoll_ctl");
        }
        free(si);
    } else {
        printf("recv %dbytes: %s\n", ret, buffer);

        // 添加响应
        si->callback = send_cb;
        ev.data.ptr = si;
        ev.events = EPOLLOUT;
        ret = epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
        if (ret < 0) {
            perror("epoll_ctl");
        }
    }
    return 0;
}

int send_cb(int sockfd, int events, void *arg) {
    char buffer[1024] = "hello!";
    int ret;
    struct epoll_event ev;
    struct sockitem *si = (struct sockitem *)arg;

    ret = send(sockfd, buffer, sizeof(buffer), 0);

    si->sockfd = sockfd;
    si->callback = recv_cb;
    ev.events = EPOLLIN;
    ev.data.ptr = si;

    ret = epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
    if (ret < 0) {
        perror("epoll_ctl");
    }

    return 0;
}