#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

static bool stop = false;
/*SIGTERM信号函数 触发时结束主程序循环*/
static void handle_term(int signo)
{
    stop = true;
}
int main(int argc, char const *argv[])
{
    signal(SIGTERM, handle_term);
    /*kill(1)命令发送的系统默认终止信号*/
    if (argc <= 3)
    {
        printf("usage: %s id_address port_numer backlog\n", basename(argv[0]));
        return 1;
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int backlog = atoi(argv[3]);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    /*创捷一个TCP/IP协议族的ipv4的TCP套接字*/
    assert(sock >= 0);

    /*创建IPv4 socket地址*/

    /*socket网络编程中表示socket的是结构体sockaddr 它有两个成员sa_family(地址族)与sa_data(地址)*/
    /*协议族和地址族关系
    PF_UNIX AF_UNIX UNIX本地域协议族
    PF_INET AF_INET TCP/IPv4协议族
    PF_INET6 AF_INET6   TCP/IPv6协议族
    */

    /*专用socket
    UNIX本地协议族 sockaddr_un
    TCP/IP协议族 sockaddr_in sockaddr_in6
    */
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;              /*sin_family地址族 每个结构体有各自的固定值*/
    inet_pton(AF_INET, ip, &address.sin_addr); /*sin_addr ip地址结构体*/
    /*inet_pton函数将用字符串表示的IP地址(点分十进制)转换成用网络字节序整数表示的ip地址 并将结果存储于指定内存 失败时返回INADDR_NONE
        inet_ntop则相反
    */

    address.sin_port = htons(port); /*端口号 要用网络字节序表示 u_int16_t*/

    int ret = bind(sock, (struct sockaddr *)&address, sizeof(address)); /*将socket与相应地址进行绑定*/
    /*所有专用socket使用时要转为通用socket 直接强制转换即可*/

    assert(ret != -1);

    ret = listen(sock, backlog); /*backlog提示内核监听队列的最大长度*/
    assert(ret != -1);

    /*循环等待连接 直至有SIGTERM信号将它中断*/
    while (!stop)
    {
        sleep(1);
    }
    /*关闭socket*/
    close(sock);
    /*close调用并非总是立即关闭一个连接 而是将fd的引用计数减1 只有当fd的引用计数为0时 才真正关闭连接*/
    return 0;
}