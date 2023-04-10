#include <termios.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define LINE 64

static void pr_winsiz(int fd)
{
    struct winsize siz;
    if (ioctl(fd, TIOCGWINSZ, (char *)&siz) < 0)
        write(STDERR_FILENO, "TIOCGWINSZ error!\n", LINE);

    char buf[LINE];
    sprintf(buf, "%d rows, %d columns\n", siz.ws_row, siz.ws_col);
    write(STDOUT_FILENO, buf, strlen(buf));
}

static void sig_winch(int signo)
{
    write(STDOUT_FILENO, "SIGWINCH received!\n", LINE);
    pr_winsiz(STDIN_FILENO);
    if (signal(SIGWINCH, sig_winch) == SIG_ERR)
        write(STDOUT_FILENO, "signal error\n", LINE);
}

int main()
{
    if (isatty(STDIN_FILENO) == 0)
        return -1;
    if (signal(SIGWINCH, sig_winch) == SIG_ERR)
        write(STDOUT_FILENO, "signal error\n", LINE);
    pr_winsiz(STDOUT_FILENO);
    for (;;)
        pause();
    return 0;
}