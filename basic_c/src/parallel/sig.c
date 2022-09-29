#include <stdio.h>
#include <signal.h>
#include <unistd.h>
static void interrupt(int signo)
{
    write(1, "!", 1);
}
int main()
{
    // signal(SIGINT, SIG_IGN);
    signal(SIGINT, interrupt);
    for (int i = 0; i < 10; ++i)
    {
        write(1, "*", 1);
        sleep(1);
    }
    return 0;
}