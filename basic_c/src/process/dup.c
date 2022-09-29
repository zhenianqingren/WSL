#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
    close(1);

    int fd = open("/tmp/out", O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (fd < 0)
    {
        fprintf(stdout, "%s\n", strerror(errno));
        return 1;
    }

    puts("TEST\n");
    fflush(stdout);
    close(fd);
    return 0;
}