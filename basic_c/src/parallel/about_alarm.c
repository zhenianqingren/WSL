#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <setjmp.h>

int how_much = 0;

static void sig_alarm1(int signo)
{
    /*nothing to do, just wake up the pause*/
    printf("%d\n", how_much++);
}

unsigned int sleep1(unsigned int seconds)
{
    if (signal(SIGALRM, sig_alarm1) == SIG_ERR)
        fprintf(stderr, "Error Signal\n");

    alarm(seconds);
    for (int i = 0; i < 50000; ++i)
        for (int j = 0; j < 50000; ++j)
            ;
    // pause之前触发alarm 将永远陷入阻塞
    printf("before pause\n");
    pause();
    return (alarm(0)); //测试一下alarm(0) 是否又一次得到调用 (没有)
}

unsigned int test_alarm(unsigned int seconds)
{
    alarm(seconds);
    alarm(100);
    if (signal(SIGALRM, sig_alarm1) == SIG_ERR)
        fprintf(stderr, "Error Signal\n");
    return alarm(0);
}

static jmp_buf env_alarm;
static void sig_alarm2(int signo)
{
    longjmp(env_alarm, 1);
}

unsigned int sleep2(unsigned int seconds)
{
    if (signal(SIGALRM, sig_alarm2) == SIG_ERR)
        fprintf(stderr, "Error Signal\n");

    if (setjmp(env_alarm) == 0)
    {
        alarm(seconds);
        for (int i = 0; i < 50000; ++i)
            for (int j = 0; j < 50000; ++j)
                ;
        printf("before pause\n");
        pause();
    }
    // 解决了pause永远阻塞的问题
    // 但是涉及到与其他信号的交互 如果alarm中断了某个信号处理程序
    // 那么调用longjmp将过早的结束
    return alarm(0);
}

static void sig_int(int signo)
{
    printf("sig_int started\n");
    int i, j;
    volatile int k;
    for (int i = 0; i < 500000; ++i)
        for (int j = 0; j < 500000; ++j)
            k += i * j;
    printf("sig_int finished\n");
}

int main()
{
    // printf("%u\n", sleep1(2));
    // printf("%u\n", test_alarm(2));

    // printf("%u\n", sleep2(1));

    unsigned int unslept;
    if (signal(SIGINT, sig_int) == SIG_ERR)
        fprintf(stderr, "Error Signal\n");

    unslept = sleep2(4);
    printf("sleep2 returned: %u\n", unslept);
    return 0;
}