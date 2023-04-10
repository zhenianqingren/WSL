#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

//
int main(int argc, char const *argv[])
{
    int fd;
    char ch;
    if (fork() == 0)
    {
        fd = open("./note.md", O_RDONLY);
        printf("child process fd: %d\n", fd);
        sleep(3);
        read(fd, &ch, 1);
        printf("child process char: %c\n", ch);
        exit(0);
    }

    else
    {
        fd = open("./note.md", O_RDWR);
        printf("parent process fd: %d\n", fd);
        lseek(fd, 8, SEEK_SET);
        sleep(6);
        read(fd, &ch, 1);
        printf("parent process char: %c\n", ch);
    }

    return 0;
}
