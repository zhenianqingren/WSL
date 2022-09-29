#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/syslog.h>
#include <errno.h>
#include <error.h>
#include <string.h>

#define FNAME "/tmp/out"

int daemonize()
{
    pid_t pid = fork();
    if (pid < 0)
        return -1;
    else if (pid > 0)
        exit(0);

    int fd = open("/dev/null", O_RDWR);
    if (fd < 0)
        return -1;
    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);
    if (fd > 2)
        close(fd);

    setsid();
    chdir("/");
    return 0;
}

int main(int argc, char const *argv[])
{
    FILE *fp;
    openlog("daemon", LOG_PID, LOG_DAEMON);
    if (daemonize() < 0)
    {
        syslog(LOG_EMERG, "daemonize() failed");
        exit(1);
    }
    else
        syslog(LOG_INFO, "daemonize success");

    fp = fopen(FNAME, "w");
    if (fp == NULL)
    {
        syslog(LOG_ERR, "fopen:%s", strerror(errno));
        exit(1);
    }
    for (int i = 0;; ++i)
    {
        fprintf(fp, "%d\n", i);
        fflush(fp);
        sleep(1);
    }

    fclose(fp);
    closelog();
    return 0;
}
