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

#define BUFFERSIZE 1024

int main(int argc, char const *argv[])
{
    if (argc <= 2)
    {
        fprintf(stderr, "useage:%s ip_address port_number\n", (char *)(basename(argv[0])));
        return 1;
    }
    
    const char *ip = argv[1];
    int port = atoi(argv[2]);
    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &server_address.sin_addr);
    server_address.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    int sendbuf = atoi(argv[3]);
    int len = sizeof(sendbuf);
    /*先设置TCP发送缓冲区的大小，然后立刻读取之*/
    /*setsockopt与getsockopt是专门用来读取和设置socket描述符属性*/
    /*第一个参数指定被操作的目标socket*/
    /*第二个参数指定要操作哪个协议的选项:IPv4 IPv6 TCP等*/
    /*第三个参数则是指定选项的名字(设置或读取关于该socket的哪个属性)*/
    /*第四个和第五个则是选项的值和长度*/
    setsockopt(sock, SOL_SOCKET, SO_SNDBUF, &sendbuf, sizeof(sendbuf));
    getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &sendbuf, (socklen_t *)&len);
    printf("the tcp send buffer size after setting is:%d\n", len);

    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) != -1)
    {
        char buffer[BUFFERSIZE];
        memset(buffer, 'a', BUFFERSIZE);
        send(sock, buffer, BUFFERSIZE, 0);
    }
    close(sock);
    return 0;
}
