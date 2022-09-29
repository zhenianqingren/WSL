#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define CPS 10
#define BUFFSIZE CPS

// static int volatile loop = 1;
static int volatile token = 0;

void handler(int signo)
{
    alarm(1);
    ++token;
    // you'd better set a upper bound
}

int main(int argc, char *argv[])
{
    int sfd, dfd = 1;
    char buf[BUFFSIZE];
    int len, ret, pos;

    if (argc < 2)
    {
        fprintf(stderr, "error argument......\n");
        return 1;
    }
    signal(SIGALRM, handler);
    do
    {
        sfd = open(argv[1], O_RDONLY);
        // sfd = open("./src/parallel/slow_cat.c", O_RDONLY);
        if (sfd < 0 && errno != EINTR)
        {
            fprintf(stderr, "error:%s\n", strerror(errno));
            return 1;
        }
    } while (sfd < 0);
    alarm(1);
    while (1)
    {
        while ((len = read(sfd, buf, BUFFSIZE)) < 0)
        {
            if (errno == EINTR)
                continue;
            fprintf(stderr, "\nerror:%s\n", strerror(errno));
            return 1;
        }
        if (len == 0)
            return 0;
        while (token <= 0)
            pause();
        --token;
        // pause();
        pos = 0;
        while (len > 0)
        {
            ret = write(dfd, buf + pos, len);
            if (ret < 0)
            {
                if (errno == EINTR)
                    continue;
                fprintf(stderr, "error:%s\n", strerror(errno));
                return 1;
            }
            len -= ret;
            pos += ret;
        }
    }

    close(sfd);
    printf("\n");
    return 0;
}