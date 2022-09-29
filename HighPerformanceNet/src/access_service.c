#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

// access target host service : date

int main(int argc, char const *argv[])
{
    assert(argc == 2);
    char *host = argv[1];
    struct hostent *hostinfo = gethostbyname(host);
    // gethostbyname 根据主机名称获取主机完整信息
    // hostent结构体指针是返回值
    // 包括主机名 主机别名列表 地址类型 地址长度 按网络字节序列出的主机IP地址列表

    if (!hostinfo)
    {
        switch (h_errno)
        {
        case HOST_NOT_FOUND:
            printf("HOST_NOT_FOUNT\n");
            break;
        case NO_DATA:
            printf("The requested name is valid but does not have an IP address.\n");
            break;
        case NO_RECOVERY:
            printf("A nonrecoverable name server error occurred.\n");
            break;
        case TRY_AGAIN:
            printf("A temporary error occurred on an authoritative name server.  Try again later.\n");
            break;
        default:
            printf("unknown error\n");
            break;
        }
        return 1;
    }
    struct servent *serviceinfo = getservbyname("http", "tcp");
    //  根据名称获取某个服务的完整信息 第二个参数是服务类型(tcp udp NULL(获取所有类型服务))
    //  返回值servent结构体指针
    //  包含服务名称 服务的别名列表 端口号 服务类型
    if (!serviceinfo)
    {
        fprintf(stderr, "%s\n", strerror(errno));
        return 1;
    }
    assert(serviceinfo);
    printf("date port is %d\n", ntohs(serviceinfo->s_port));

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = serviceinfo->s_port;
    // h_addr_list本身是使用网络字节序的地址列表 无需再转换IP字节序
    address.sin_addr = *(struct in_addr *)*hostinfo->h_addr_list;

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    int res = connect(sockfd, (struct sockaddr *)&address, sizeof(address));
    assert(res != -1);
    char buffer[128];
    res = read(sockfd, buffer, 128);
    assert(res > 0);
    buffer[res] = '\0';
    printf("%s\n", buffer);
    close(sockfd);
    return 0;
}
