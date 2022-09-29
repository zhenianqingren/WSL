#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

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

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);
    // connect主动与服务端建立连接  sockfd由系统调用返回
    // 一旦连接成功sockfd就唯一的标识这个连接
    // 连接失败返回-1并置位errno
    // ECONNREFUSED 目标不存在 连接被拒绝
    // ETIMEDOUT 连接超时
    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0)
        fprintf(stderr, "connection failed!\n");

    else
    {
        int i = 100;
        const char *oob_data = "abc";
        const char *normal_data = "123";
        send(sockfd, normal_data, strlen(normal_data), 0);
        // send往套接字写入数据 normal_data和strlen分别指定缓冲区大小 send成功返回实际写入长度 写入失败返回-1并置位errno
        // send(sockfd, oob_data, strlen(oob_data), MSG_OOB); // MSG_OOB 发送或接受紧急数据
        send(sockfd, normal_data, strlen(normal_data), 0);
        while (--i > 0)
            send(sockfd, normal_data, strlen(normal_data), 0);
        printf("\n%d\n", i);
    }

    close(sockfd);
    // close只是使该连接的引用计数减1 父进程fork一个子进程后要在两个进程中都调用close
    // 想要直接强行关闭调用shutdown
    return 0;
}
