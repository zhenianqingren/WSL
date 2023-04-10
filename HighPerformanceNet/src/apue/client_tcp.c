#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>

#define MAXSLEEP 4
#define BUFLEN 64
#define TNUM 8

int connect_retry(int domain, int type, int protocol, const struct sockaddr *addr, socklen_t alen)
{
    int numsec, fd;

    for (numsec = 1; numsec <= MAXSLEEP; numsec <<= 1)
    {
        if ((fd = socket(domain, type, protocol)) < 0)
            return -1;

        if (connect(fd, addr, alen) == 0)
            return fd;
        close(fd);
        sleep(numsec);
    }

    return -1;
}

void pmsg(int sockfd)
{
    int n;
    char buf[BUFLEN];

    while ((n = recv(sockfd, buf, BUFLEN, 0)) > 0)
        write(STDOUT_FILENO, buf, n);

    if (n < 0)
        fprintf(stderr, "recv error\n");
}

void transport(struct sockaddr *addr)
{
    struct sockaddr_in *tmp = (struct sockaddr_in *)addr;
    tmp->sin_port = htons(8888);
}

void *th_fn(void *arg)
{
    printf("thread: %lu started\n", (unsigned long)pthread_self());
    struct addrinfo *aip = (struct addrinfo *)arg;
    int sockfd, err;
    if ((sockfd = connect_retry(aip->ai_family, SOCK_STREAM, 0, aip->ai_addr, aip->ai_addrlen)) < 0)
        err = errno;
    else
        pmsg(sockfd);

    if (err != 0)
        fprintf(stderr, "thread: %lu error :%s\n", (unsigned long)pthread_self(), strerror(err));
    close(sockfd);
    pthread_exit((void *)0);
}

int main(int argc, char const *argv[])
{
    struct addrinfo *ailist, *aip;
    struct addrinfo hint;

    bzero(&hint, sizeof(hint));
    hint.ai_socktype = SOCK_STREAM;

    const char *n0cmp = NULL == 0 ? "Yes" : "No";
    printf("NULL == 0 ? %s\n\n", n0cmp);

    if (getaddrinfo("192.168.80.128", "http", &hint, &ailist) != 0)
    {
        fprintf(stderr, "getaddrinfo error\n");
        return -1;
    }

    struct sockaddr_in *sinp;
    char *addr;
    char abuf[INET_ADDRSTRLEN];

    int err;
    pthread_t tarr[TNUM];

    for (aip = ailist; aip != NULL; aip = aip->ai_next)
    {
        transport(aip->ai_addr);

        if (aip->ai_family == AF_INET)
        {
            sinp = (struct sockaddr_in *)aip->ai_addr;
            addr = inet_ntop(AF_INET, &sinp->sin_addr, abuf, INET_ADDRSTRLEN);
            printf(" address %s", addr ? addr : "unknown");
            printf(" port %d", ntohs(sinp->sin_port));
            printf("\n\n");
        }
        for (int i = 0; i < TNUM; ++i)
        {
            if ((err = pthread_create(&tarr[i], NULL, th_fn, (void *)aip)) != 0)
                fprintf(stderr, "NO.%d thread create fail\n", i);
        }

        for (int i = 0; i < TNUM; ++i)
        {
            if ((err = pthread_join(tarr[i], NULL)) != 0)
                fprintf(stderr, "NO.%d thread join fail\n", i);
        }
    }

    return 0;
}
