#include <stdio.h>
#include <signal.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>

static void handle(int signo)
{
    struct passwd *tt1;
    printf("in signal handler\n");
    if ((tt1 = getpwnam("tt1")) == NULL)
        fprintf(stderr, "getpwnam(tt1) error\n");

    alarm(1);
}

int main(int argc, char const *argv[])
{
    struct passwd *mice;
    signal(SIGALRM, handle);
    alarm(1);
    for (;;)
    {
        if ((mice = getpwnam("mice")) == NULL)
            fprintf(stderr, "getpwnam(mice) error\n");
        if (strcmp(mice->pw_name, "mice") != 0)
            printf("return value corrupted:%s\n", mice->pw_name);
    }
// 在信号处理程序中不能判断当前进程执行到何处
// getpwnam在一块静态内存分配数据时被打断 信号处理程序又重新在此分配
    return 0;
}
