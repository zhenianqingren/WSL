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
#define MAXADDRLEN 256

int initserver(int type, const struct sockaddr *addr, socklen_t alen, int qlen)
{
    int fd;
    int err = 0;

    if ((fd = socket(addr->sa_family, type, 0)) < 0)
        return -1;

    if (bind(fd, addr, alen) < 0)
        goto errout;

    if (type == SOCK_STREAM || type == SOCK_SEQPACKET)
        if (listen(fd, qlen) < 0)
            goto errout;

    return fd;

errout:
    err = errno;
    close(fd); // this place error may happen , so save error status about net
    errno = err;
    return -1;
}

void serve(int sockfd)
{
    int n;
    socklen_t alen;
    FILE *fp;
    char buf[BUFLEN];
    char abuf[MAXADDRLEN];
    struct sockaddr *addr = (struct sockaddr *)abuf;
    struct sockaddr_in *tmp;

    char ip[BUFLEN];
    int port;
    for (;;)
    {
        alen = MAXADDRLEN;
        if ((n = recvfrom(sockfd, buf, BUFLEN, 0, addr, &alen)) < 0)
        {
            fprintf(stderr, "recvfrom client error\n");
            exit(1);
        }

        tmp = (struct sockaddr_in *)addr;
        inet_ntop(tmp->sin_family, &tmp->sin_addr, ip, INET_ADDRSTRLEN);
        port = ntohs(tmp->sin_port);

        printf("message from ip: %s , port: %d\n", ip, port);

        if ((fp = popen("/usr/bin/uptime", "r")) == NULL)
        {
            sprintf(buf, "error: %s\n", strerror(errno));
            sendto(sockfd, buf, strlen(buf), 0, addr, alen);
        }
        else
        {
            if (fgets(buf, BUFLEN, fp) != NULL)
                sendto(sockfd, buf, strlen(buf), 0, addr, alen);
            pclose(fp);
        }
    }
}

int main(int argc, char const *argv[])
{
    struct addrinfo *ailist, *aip;
    struct addrinfo hints;
    int err, sockfd;

    bzero(&hints, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;

    if (getaddrinfo("192.168.80.128", "openvpn", &hints, &ailist) != 0)
    {
        fprintf(stderr, "getaddrinfo error\n");
        exit(1);
    }

    for (aip = ailist; aip != NULL; aip = aip->ai_next)
    {
        struct sockaddr_in *tmp = (struct sockaddr_in *)aip->ai_addr;
        tmp->sin_port = htons(8888);

        if ((sockfd = initserver(SOCK_DGRAM, aip->ai_addr, aip->ai_addrlen, 0)) >= 0)
        {
            serve(sockfd);
            exit(0);
        }
        else
            err = errno;
    }

    fprintf(stderr, "error: %s\n", strerror(err));
    return 1;
}
