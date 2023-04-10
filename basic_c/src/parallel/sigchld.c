#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

void sigcld(int signo)
{
    if (signo == SIGCHLD)
        printf("SIGCLD=SIGCHLD\n");
    pid_t pid;
    int status;
    printf("SIGCLD received\n");
    if ((pid = wait(&status)) < 0)
        printf("wait error\n");
}

void interrupt(int signo)
{
    printf("interrupt\n");
}

int main()
{
    pid_t pid;
    if (signal(SIGCLD, sigcld) < 0)
        printf("signal error\n");
    if (signal(SIGALRM, interrupt) < 0)
        printf("signal error\n");

    if ((pid = fork()) < 0)
        printf("fork error\n");
    else if (pid == 0)
    {
        sleep(3);
        exit(0);
    }

    pause();
    alarm(3);
    pause();
    return 0;
}