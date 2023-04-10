#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

void info_exit(int status)
{
    if (WIFEXITED(status))
        printf("normal exit, status= %d\n", WEXITSTATUS(status));
    else if (WIFSIGNALED(status))
        printf("abnormal termination, signal number=%d\n", WTERMSIG(status));
    else if (WIFSTOPPED(status))
        printf("child stopped , number=%d\n", WSTOPSIG(status));
#ifdef WCOREDUMP
    WCOREDUMP(status) ? "core file generated\n" : "";
#else

#endif
}

int main()
{
    pid_t pid;
    int status;
    
    if ((pid = fork()) < 0)
        printf("error fork\n");
    else if (pid == 0)
        return 5;

    if (wait(&status) != pid)
        fprintf(stderr, "error wait\n");
    info_exit(status);

    if ((pid = fork()) < 0)
        printf("error fork\n");
    else if (pid == 0)
        abort();

    if (wait(&status) != pid)
        fprintf(stderr, "error wait\n");
    info_exit(status);

    if ((pid = fork()) < 0)
        printf("error fork\n");
    else if (pid == 0)
        status /= 0;

    if (wait(&status) != pid)
        fprintf(stderr, "error wait\n");
    info_exit(status);

    return 0;
}