#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static unsigned long long count = 0;
static volatile int loop = 1; //如果开大O优化的话，必须加volatile

void exit_handle(int signo)
{
    loop = 0;
}

int main()
{
    signal(SIGALRM, exit_handle);
    alarm(5);
    while (loop)
        ++count;
    printf("%llu\n", count);

    return 0;
}