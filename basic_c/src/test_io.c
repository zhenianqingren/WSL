#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, const char *argv[])
{
    int fd = open(argv[1], O_RDONLY);
    char buff[5];
    int len;
    while ((len = read(fd, buff, 5)) != 0)
        printf("%d\n", len);

    printf("%d\n", len);
    close(fd);
    return 0;
}