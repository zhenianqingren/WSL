#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argv, char *argc[])
{

    printf("BEGIN\n");
    fflush(NULL);
    pid_t pid;
    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "ERROR:%s\n", strerror(errno));
        exit(1);
    }
    else if (pid == 0) // child
    {
        execl("/bin/date", "+%s", NULL);
        fprintf(stderr, "ERROR");
        exit(1);
    }

    pid = wait(NULL);
    printf("In %s PROCESS:%d Returned\n", __FUNCTION__, pid);
    return 0;
}