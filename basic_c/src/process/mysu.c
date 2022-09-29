#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[])
{
    pid_t pid;
    if (argc < 3)
        return 1;

    if ((pid = fork()) < 0)
        return 1;
    else if (pid == 0)
    {
        setuid(atoi(argv[1]));
        execvp(argv[2], argv + 2);
        printf("ERROR FORK\n");
        return 1;
    }

    wait(NULL);
    return 0;
}