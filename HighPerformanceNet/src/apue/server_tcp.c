#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <error.h>
#include <arpa/inet.h>

#define BUFLEN 64

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
    int clfd;
    FILE *fp;
    char buf[BUFLEN];

    for (;;)
    {
        if ((clfd = accept(sockfd, NULL, NULL)) < 0)
        {
            fprintf(stderr, "accept connect to http fail\n");
            return;
        }

        if ((fp = popen("/usr/bin/uptime", "r")) == NULL)
        {
            sprintf(buf, "error: %s\n", strerror(errno));
            send(clfd, buf, BUFLEN, 0);
        }
        else
        {
            while (fgets(buf, BUFLEN, fp) != NULL)
                send(clfd, buf, BUFLEN, 0);
            pclose(fp);
        }

        close(clfd);
    }
}

int main(int argc, char const *argv[])
{

    struct addrinfo *ailist, *aip;
    struct addrinfo hints;
    int sockfd;

    bzero(&hints, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;

    if (getaddrinfo("192.168.80.128", "http", &hints, &ailist) != 0)
    {
        fprintf(stderr, "getaddrinfo error\n");
        return -1;
    }

    for (aip = ailist; aip != NULL; aip = aip->ai_next)
    {
        struct sockaddr_in *tmp = (struct sockaddr_in *)(aip->ai_addr);
        tmp->sin_port = htons(8888);
        if ((sockfd = initserver(SOCK_STREAM, aip->ai_addr, aip->ai_addrlen, 3)) >= 0)
        {
            serve(sockfd);
            exit(0);
        }
    }
    return 1;
}
