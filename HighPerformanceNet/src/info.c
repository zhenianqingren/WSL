#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

#define size 64

int main(int argc, char const *argv[])
{
    struct addrinfo hints;
    struct addrinfo *res;

    bzero(&hints, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    int ret = getaddrinfo("mice", "ssh", &hints, &res);
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

    switch (hints.ai_family)
    {
    case AF_INET:
        printf("IPv4 family\n");
        break;
    case AF_INET6:
        printf("IPv6 family\n");
        break;
    case AF_UNSPEC:
        printf("Both IPV4 And IPv6\n");
    default:
        break;
    }

    printf("%s\n", hints.ai_canonname);

    struct sockaddr_in *address_pointer = (struct sockaddr_in *)hints.ai_addr;
    struct sockaddr_in address = *address_pointer;
    int port = ntohs(address.sin_port);
    char ip[size];
    inet_ntop(hints.ai_family, &address.sin_addr, ip, size);
    printf("%s\n", ip);
    freeaddrinfo(&hints);
    return 0;
}
