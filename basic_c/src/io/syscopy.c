#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define BUFFERSIZE 64

int main(int argv, char *argc[])
{
    int sour, dst;
    if (argv < 3)
    {
        fprintf(stderr, "Error Arguments");
        return 1;
    }

    do
    {
        sour = open(argc[1], O_RDONLY);
        if (sour < 0 && errno != EINTR)
        {
            fprintf(stderr, "Fail Open");
            return 1;
        }
    } while (sour < 0);

    do
    {
        dst = open(argc[2], O_WRONLY | O_CREAT | O_TRUNC, 0600);
        if (dst < 0 && errno != EINTR)
        {
            fprintf(stderr, "Fail Open");
            close(sour);
            return 1;
        }
    } while (dst < 0);

    int len;
    char buffer[BUFFERSIZE];
    int pos, ret;
    while ((len = read(sour, buffer, BUFFERSIZE)) > 0)
    {
        if (len < 0 && errno != EINTR)
        {
            fprintf(stderr, "Write Interrupt\n");
            close(sour);
            close(dst);
            return 1;
        }
        pos = 0;
        while (len > 0)
        {
            if ((ret = write(dst, buffer + pos, len)) < 0 && errno != EINTR)
            {
                fprintf(stderr, "Write Interrupt\n");
                close(sour);
                close(dst);
                return 1;
            }
            pos += ret;
            len -= ret;
        }
    }

    close(sour);
    close(dst);

    return 0;
}