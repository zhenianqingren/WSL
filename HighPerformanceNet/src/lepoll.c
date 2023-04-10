#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <pthread.h>

#define MAX_EVENT_NUM 1024
#define TCP_BUF 512
#define UDP_BUF 1024

int setnonblock(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void addfd(int epollfd, int fd)
{
    struct epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblock(fd);
}

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        printf("usage: %s ipaddress portnumber\n", basename(argv[0]));
        exit(1);
    }
    const char *ip = argv[1];
    int port = argv[2];

    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    int udpfd = socket(PF_INET, SOCK_DGRAM, 0);
    assert(udpfd >= 0);

    ret = bind(udpfd, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    struct epoll_event events[MAX_EVENT_NUM];
    int epollfd = epoll_create(5);
    assert(epollfd != -1);

    addfd(epollfd, listenfd);
    addfd(epollfd, udpfd);

    while (1)
    {
        int number;
        if ((number = epoll_wait(epollfd, events, MAX_EVENT_NUM, -1)) < 0)
        {
            printf("epoll failure\n");
            break;
        }
        for (int i = 0; i < number; ++i)
        {
            int sockfd = events[i].data.fd;
            if (sockfd == listenfd)
            {
                struct sockaddr_in client;
                socklen_t client_len = sizeof(client);
                int connectfd = accept(sockfd, (struct sockaddr *)&client, &client_len);
                addfd(sockfd, connectfd);
            }
            else if (sockfd == udpfd)
            {
                char buf[UDP_BUF];
                memset(buf, 0, UDP_BUF);
                struct sockaddr_in client;
                socklen_t client_len = sizeof(client);
                ret = recvfrom(udpfd, buf, UDP_BUF - 1, 0, (struct sockaddr *)&client, &client_len);
                if (ret > 0)
                    sendto(udpfd, buf, UDP_BUF - 1, 0, (struct sockaddr *)&client, &client_len);
            }
            else if (events[i].events & EPOLLIN)
            {
                char buf[TCP_BUF];
                while (1)
                {
                    memset(buf, 0, TCP_BUF);
                    ret = recv(sockfd, buf, TCP_BUF, 0);
                    if (ret < 0)
                    {
                        if ((errno == EAGAIN) || (errno == EWOULDBLOCK))
                            break;
                        close(sockfd);
                        break;
                    }
                    else if (ret == 0)
                        close(sockfd);
                    else
                        send(sockfd, buf, ret, 0);
                }
            }
            else
                printf("something wrong\n");
        }
        close(listenfd);
        return 0;
    }
}