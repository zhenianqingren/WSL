#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{

    system("date");

    // execl("/home/mice/basic_c/bin/test", (char *)0);
    // execlp("ls","ls","-al","/home/mice",(char *)0);
    pid_t pid;
    if ((pid = fork()) < 0)
    {
        fprintf(stderr, "ERROR:%s\n", strerror(errno));
        exit(1);
    }
    else if (pid == 0) // child
    {
        execl("/bin/sh", "sh", "-c", "date", NULL);
        fprintf(stderr, "ERROR");
        exit(1);
    }

    pid = wait(NULL);
    return 0;
}