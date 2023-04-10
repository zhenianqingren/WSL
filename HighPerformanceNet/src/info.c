#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define size 64

void pfamily(struct addrinfo *aip)
{
    printf(" family : ");
    switch (aip->ai_family)
    {
    case AF_INET:
        printf(" inet ");
        break;
    case AF_INET6:
        printf(" inet6 ");
        break;
    case AF_UNIX:
        printf(" unix ");
        break;
    case AF_UNSPEC:
        printf(" unspecified ");
        break;
    default:
        printf(" unknown ");
    }
}

void pprotocol(struct addrinfo *aip)
{
    printf(" protocol : ");
    switch (aip->ai_protocol)
    {
    case 0:
        printf("default");
        break;
    case IPPROTO_TCP:
        printf(" TCP ");
        break;
    case IPPROTO_UDP:
        printf(" UDP ");
        break;
    case IPPROTO_RAW:
        printf(" RAW ");
        break;
    default:
        printf(" unknown (%d) ", aip->ai_protocol);
    }
}

int main(int argc, char const *argv[])
{
    struct addrinfo hint;
    struct addrinfo *ailist, *aip;

    struct sockaddr_in *sinp;
    const char *addr;

    char abuf[INET_ADDRSTRLEN];

    bzero(&hint, sizeof(hint));
    hint.ai_socktype = SOCK_DGRAM;
    int ret = getaddrinfo("mice", "openvpn", &hint, &ailist);
    // 此函数返回的ip地址为127.0.1.1 是一个回环地址 到了传输层不再往下发
    // 根本无法监听对应的端口 192.168.80.128才是面向外部连接的ip地址
    if (ret != 0)
    {
        switch (ret)
        {
        // case EAI_ADDRFAMILY:
        //     printf("The specified network host does not have any network addresses in the requested address family.\n");
        //     break;
        case EAI_AGAIN:
            printf("The name server returned a temporary failure indication.  Try again later.\n");
            break;
        case EAI_BADFLAGS:
            printf("hints.ai_flags contains invalid flags; or, hints.ai_flags included AI_CANONNAME and name was NULL.\n");
            break;
        case EAI_FAIL:
            printf("The name server returned a permanent failure indication.\n");
            break;
        case EAI_FAMILY:
            printf("The requested address family is not supported.\n");
            break;
        // case EAI_NODATA:
        //     printf("The specified network host exists, but does not have any network addresses defined.\n");
        //     break;
        case EAI_NONAME:
            printf("The node or service is not known; or both node and service are NULL; or AI_NUMERICSERV was specified in hints.ai_flags and service was not a numeric port-number string.\n");
            break;
        case EAI_SERVICE:
            printf("EAI_SERVICE.\n");
            break;
        case EAI_SOCKTYPE:
            printf("The requested socket type is not supported.  This could occur, for example, if hints.ai_socktype and hints.ai_protocol are inconsistent (e.g., SOCK_DGRAM and IPPROTO_TCP, respectively)..\n");
            break;
        case EAI_SYSTEM:
            printf("Other system error, check errno for details.\n");
            break;
        default:
            break;
        }
        return 1;
    }

    struct servent *sername;

    for (aip = ailist; aip != NULL; aip = aip->ai_next)
    {
        pfamily(aip);
        pprotocol(aip);
        printf("\n\thost %s", aip->ai_canonname ? aip->ai_canonname : "-");

        if (aip->ai_family == AF_INET)
        {
            sinp = (struct sockaddr_in *)aip->ai_addr;
            addr = inet_ntop(AF_INET, &sinp->sin_addr, abuf, INET_ADDRSTRLEN);
            printf(" address %s", addr ? addr : "unknown");
            printf(" port %d", ntohs(sinp->sin_port));
            printf("\n\n");

            sername = getservbyport(ntohs(sinp->sin_port), NULL);
            if (sername != NULL)
                printf("service name: %s\n", sername->s_name);
        }
    }
    printf("%s\n", strerror(0));

    return 0;
}
