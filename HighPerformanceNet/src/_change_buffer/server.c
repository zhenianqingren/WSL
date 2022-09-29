#include <sys/socket.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>
#include <libgen.h>

#define SIZE 512

int main(int argc, char const *argv[])
{
    if (argc <= 2)
    {
        fprintf(stderr, "useage:%s ip_address port_number\n", (char *)(basename(argv[0])));
        return 1;
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    int recvbuf = atoi(argv[3]);
    int len = sizeof(recvbuf);
    /*先设置接收缓冲区的大小，然后立即读取之*/
    setsockopt(sock, SOL_SOCKET, SO_RCVBUF, &recvbuf, sizeof(recvbuf));
    getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &recvbuf, &len);
    printf("the tcp receive buffer size after setting is:%d\n", recvbuf);

    int ret = bind(sock, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(sock, 5);
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_addrlength = sizeof(client);

    int connfd;
    if ((connfd = accept(sock, (struct sockaddr *)&client, &client_addrlength)) < 0)
        printf("error:%s\n", strerror(errno));

    else
    {
        char buffer[SIZE];
        memset(buffer, '\0', SIZE);
        int num = 0;
        while (recv(connfd, buffer, SIZE - 1, 0) > 0)
        {
            printf("%d\t%s\n", ++num, buffer);
            printf("\n%d\n", strlen(buffer));
            memset(buffer, '\0', SIZE);
        }
        close(connfd);
    }

    return 0;
}
