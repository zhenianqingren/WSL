#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>



int main(int argv, char *argc[])
{
    struct stat status;

    stat(argc[1], &status);
    printf("%llu\n", status.st_size);

    return 0;
}