#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE 128

int main()
{

    FILE *fp = fopen("/tmp/out", "a+");
    if (fp == NULL)
        return EXIT_FAILURE;
    char *buffer = NULL;
    size_t n = 0;

    time_t cur_t;
    struct tm *cur_tm = NULL;

    while (getline(&buffer, &n, fp) != -1)
        ;

    if (buffer != NULL)
        free(buffer);

    buffer = (char *)malloc(SIZE);
    while (1)
    {
        cur_t = time(NULL);
        cur_tm = localtime(&cur_t);

        if (cur_tm == NULL)
        {
            printf("tm obtain error!\n");
            return EXIT_FAILURE;
        }

        fprintf(fp, "%lu-%lu-%lu   %lu-%lu-%lu\n", cur_tm->tm_year + 1900, cur_tm->tm_mon + 1, cur_tm->tm_mday, cur_tm->tm_hour + 1,
                cur_tm->tm_min, cur_tm->tm_sec);
        fflush(fp);
        sleep(1);
    }

    fclose(fp);

    return 0;
}