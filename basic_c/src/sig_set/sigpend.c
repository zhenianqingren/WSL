#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

// 返回一信号集 对调用进程而言 其中的各个信号是阻塞而不能递送的 因此当前也
//  一定是未决的

static void sig_quit(int signo)
{
    printf("caught SIGQUIT\n");
    if (signal(SIGQUIT, SIG_DFL) == SIG_ERR) //恢复默认 接收到SIGQUIT时退出
        fprintf(stderr, "can't reset SIGQUIT\n");
}

int main()
{
    sigset_t newmask, oldmask, pendmask;
    if (signal(SIGQUIT, sig_quit) == SIG_ERR)
        fprintf(stderr, "can't reset SIGQUIT\n");

    sigemptyset(&newmask);
    sigaddset(&newmask, SIGQUIT);

    if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
        fprintf(stderr, "SIG_BLOCK error\n");

    if (sigpending(&pendmask) < 0)
        fprintf(stderr, "sigpending error\n");
    if (sigismember(&pendmask, SIGQUIT))
        fprintf(stdout, "SIGQUIT pending\n");

    sleep(10);

    if (sigpending(&pendmask) < 0)
        fprintf(stderr, "sigpending error\n");
    if (sigismember(&pendmask, SIGQUIT))
        fprintf(stdout, "SIGQUIT pending\n");

    if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
        fprintf(stderr, "SIG_SETMASK error\n");
    printf("SIGQUIT unblocked\n");

    sleep(10);
    return 0;
}