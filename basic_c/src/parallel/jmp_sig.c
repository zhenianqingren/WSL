#include <stdio.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>

static int count = 1;
static jmp_buf verify;
static void handle(int signo)
{
    printf("in handle:%d\n", count);
    alarm(1);
    longjmp(verify, count);
    return;
}

int main(int argc, char const *argv[])
{

    signal(SIGALRM, handle);
    alarm(1);
    int res = setjmp(verify);
    if (res != 0)
        printf("back from handle\n");

    for (;;)
        ;
    return 0;
}
