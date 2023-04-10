#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>

#define BUFLEN 64
#define TIMEOUT 16

void sigalarm(int signo)
{
    /*
        only to interrupt
        ----------
        nothing to do
    */
}

void puptime(int sockfd, struct addrinfo *aip)
{
    int n;
    char buf[BUFLEN];

    if (sendto(sockfd, "\0", 1, 0, aip->ai_addr, aip->ai_addrlen) < 0)
    {
        fprintf(stderr, "initial sendto error\n");
        return;
    }

    alarm(TIMEOUT);
    if ((n = recvfrom(sockfd, buf, BUFLEN, 0, aip->ai_addr, &aip->ai_addrlen)) < 0)
    {
        fprintf(stderr, "recvfrom error\n");
        return;
    }
    alarm(0);

    write(STDOUT_FILENO, buf, BUFLEN);
}

int main(int argc, char const *argv[])
{
    struct addrinfo *aip, *ailist;
    struct addrinfo hints;
    int err, sockfd;

    signal(SIGALRM, sigalarm);

    bzero(&hints, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo("192.168.80.128", "openvpn", &hints, &ailist) != 0)
    {
        fprintf(stderr, "getaddrinfo error\n");
        return 1;
    }

    for (aip = ailist; aip != NULL; ++aip)
    {
        struct sockaddr_in *tmp = (struct sockaddr_in *)aip->ai_addr;
        tmp->sin_port = htons(8888);

        if ((sockfd = socket(aip->ai_family, SOCK_DGRAM, 0)) < 0)
            err = errno;
        else
        {
            puptime(sockfd, aip);
            exit(0);
        }
    }

    fprintf(stderr, "error: %s\n", strerror(err));
    return 1;
}
