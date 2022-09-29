#include <sys/socket.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <unistd.h>

int main(int argc, char *const argv)
{
    if (argc <= 2)
    {
        printf("usage: %s id_address port_numer backlog\n", basename(argv[0]));
        return 1;
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);

    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    int ret = bind(sock, (struct sockaddr *)&address, sizeof(address));
    assert(ret != -1);

    ret = listen(sock, 5);
    assert(ret != -1);

    sleep(20);
    struct sockaddr_in client;
    socklen_t client_addrlen = sizeof(client);
    int connfd = accept(sock, (struct sockaddr *)&client, &client_addrlen);
    /*阻塞 sock是之前监听的套接字 client接受连接的客户端
    服务器接受到相应监听的连接后返回一个新的socket，它唯一的表示的二者之间的连接
    二者之间数据的传输都可以通过这个返回的套接字进行
    */
    if (connfd < 0)
        printf("errno is:%d\n", errno);

    else
    {
        char remote[INET_ADDRSTRLEN];
        printf("connected with ip: %s and port: %d\n", inet_ntop(AF_INET, &client.sin_addr, remote, INET_ADDRSTRLEN), ntohs(client.sin_port));
        close(connfd);
    }
    close(sock);

    /*启动telnet后立刻断开连接或者关掉客户端 accept对此毫不知情
    由此可见accept只是从监听队列中取出连接而不论连接处于何种状态 更不关心任何网络情况的变化
    */
    return 0;
}