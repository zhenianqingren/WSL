#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <libgen.h>

#define BUFLEN 128
#define TIMEOUT 20

/*
struct addrinfo{
    int ai_flags;       customize behavior
    int ai_family;      address family
    int ai_socktype;    socket type
    int ai_protocol;    protocol
    socklen_t ai_addrlen;       length in bytes of address
    struct sockaddr *ai_addr;   address   
    char *ai_canonname;         ai_cannonname
    struct addrinfo *ai_next;   next in list

int getaddrinfo(const char*restrict host,const char*restrict service
                const struct addrinfo *restrict hint,
                struct addrinfo **restrict res);

    允许将一个主机名和一个服务器名映射到一个地址
    提供主机名、服务器名或者两者都提供
};
*/


void sigalarm(int signo)
{
}

void print_uptime(int sockfd, struct addrinfo *aip)
{
    int n;
    
}

int main(int argc, char const *argv[])
{

    return 0;
}
