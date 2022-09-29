#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int fp = open("/home/mice/basic_c/emptyf", O_CREAT | O_WRONLY | O_TRUNC, 0600);
    lseek(fp, 5LL * 1024LL * 1024LL * 1024LL - 1LL, SEEK_SET);
    close(fp);
    return 0;
}