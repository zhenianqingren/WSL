#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <libgen.h>

#define BUFSIZE 1024

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

    int ret = bind(sock, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(sock, 5);
    assert(ret != -1);

    struct sockaddr_in client;
    socklen_t client_addrlen = sizeof(client);
    int connfd = accept(sock, (struct sockaddr *)&client, &client_addrlen);

    if (connfd < 0)
        printf("error occur:%s\n", strerror(errno));
    else
    {
        char buffer[BUFSIZE];
        memset(buffer, '\0', BUFSIZE);

        ret = recv(connfd, buffer, BUFSIZE - 1, 0);
        // recv读取sockfd上面的数据 buf和len分别指定缓冲区的地址和大小
        // recv成功时返回读取到的数据的长度，它可能小于缓冲区的大小 因此可能需要多次读取
        // recv可能返回0 意味着连接已经关闭
        // 出错时返回-1并设置errno
        printf("go %d bytes of normal data '%s'\n", ret, buffer);
        // memset(buffer, '\0', BUFSIZE);

        // ret = recv(connfd, buffer, BUFSIZE - 1, MSG_OOB);
        // printf("go %d bytes of oob data '%s'\n", ret, buffer);
        // memset(buffer, '\0', BUFSIZE);

        // ret = recv(connfd, buffer, BUFSIZE - 1, 0);
        // printf("go %d bytes of normal data '%s'\n", ret, buffer);
        int c = 0;
        printf("\n");
        for (int i = 0; i < 1024 && buffer[i] != '\0'; ++i)
            if (buffer[i] == '1')
                ++c;
        printf("\n%d\n", c);
        close(connfd);
    }
    close(sock);
    return 0;
}
