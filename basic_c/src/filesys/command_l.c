#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
/*
-y year
-m month
-M minute
-d day
-H hour
-S second

getopt tackle command line
*/
#define TSIZE 128
#define FMTSIZE 128
extern char *optarg;
extern int optind;
int main(int argc, char *argv[])
{
    FILE *fp = stdout;
    char t_buffer[TSIZE];
    char fmt[FMTSIZE];

    t_buffer[0] = '\0';
    time_t cur_t;

    int c;

    while ((c = getopt(argc, argv, "-y:mMdH:S")) != -1)
    {
        switch (c)
        {
        case 1:
            fp = fopen(argv[optind - 1], "w");
            if (fp == NULL)
                fprintf(stderr, "Fail Open %s\n", strerror(errno));
            break;
        case 'y':
            if (strcmp(optarg, "2") == 0)
                strcat(t_buffer, "%y  ");
            else if (strcmp(optarg, "4") == 0)
                strcat(t_buffer, "%Y  ");
            else
            {
                fprintf(stdout, "Invalid Argument For Year\n");
                return 1;
            }
            break;
        case 'm':
            strcat(t_buffer, "%m  ");
            break;
        case 'M':
            strcat(t_buffer, "%M  ");
            break;
        case 'd':
            strcat(t_buffer, "%d  ");
            break;
        case 'H':
            if (strcmp(optarg, "12") == 0)
                strcat(t_buffer, "%I(%p)  ");
            else if (strcmp(optarg, "24") == 0)
                strcat(t_buffer, "%H  ");
            else
            {
                fprintf(stdout, "Invalid Argument For Hour\n");
                return 1;
            }
            break;
        case 'S':
            strcat(t_buffer, "%S  ");
            break;
        default:
            fprintf(stdout, "Invalid Argument\n");
            return 1;
        }
    }
    strcat(t_buffer, "\n");
    cur_t = time(NULL);
    if (strftime(fmt, FMTSIZE, t_buffer, localtime(&cur_t)) == 0)
        fmt[0] = '\0';

    fputs(fmt, fp);
    if (fp != stdout)
        fclose(fp);
    return 0;
}
