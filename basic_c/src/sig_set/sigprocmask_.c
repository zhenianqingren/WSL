#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

// int sigprocmask(int how,const sigset_t*restrict set,sigset_t*restrict oset)
// if oset!=NULL 信号屏蔽字通过oset返回
// if set!=NULL how指示如何修改当前屏蔽字 SIG_BLOCK SIG_UNBLOCK SIG_SETMASK

void pr_mask(const char *str)
{
    sigset_t sigset;
    int errno_save;

    errno_save = errno;
    printf("before errno:%d\n", errno_save);
    if (sigprocmask(0, NULL, &sigset) < 0)
        fprintf(stderr, "error sigprocmask\n");
    else
    {
        printf("%s\n", str);
        if (sigismember(&sigset, SIGINT))
            printf("SIGINT\n");
        if (sigismember(&sigset, SIGQUIT))
            printf("SIGQUIT\n");
        if (sigismember(&sigset, SIGUSR1))
            printf("SIGUSR1\n");
        if (sigismember(&sigset, SIGUSR2))
            printf("SIGUSR2\n");
        if (sigismember(&sigset, SIGALRM))
            printf("SIGALARM\n");
        if (sigismember(&sigset, SIGBUS))
            printf("SIGBUS\n");
        if (sigismember(&sigset, SIGIO))
            printf("SIGIO\n");
        printf("after errno:%d\n", errno_save);
    }
    errno = errno_save;
}

void test()
{
    sigset_t set, oset;
    int how = SIG_BLOCK;
    sigaddset(&set, SIGINT);
    if ((sigprocmask(how, &set, &oset)) < 0)
        fprintf(stderr, "error sigprocmask\n");
    else
    {
        if (sigismember(&oset, SIGINT))
            printf("SIGINT in oset\n"); // not executed
        if (sigismember(&set, SIGINT))
            printf("SIGINT in set\n");
    }
}

int main(int argc, const char *argv[])
{
    // pr_mask("test\n");
    printf("%s\n", argv[0]);
    test();
    printf("after test\n");
    pause();
    return 0;
}