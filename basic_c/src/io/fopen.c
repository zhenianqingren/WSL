#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <errno.h>
#include <string.h>

int main()
{

    FILE *fp = fopen("nothing", "r");
    if (fp == NULL)
    {
        fprintf(stderr, "fopen():%s\n", strerror(errno));
        exit(1);
    }

    fclose(fp);

    puts("OK!");

    char *test = "abcd";
    printf("%s\n", test);
    test[0] = 'x';
    printf("%s\n", test);
    test = "mnpo";
    printf("%s\n", test);
    exit(0);
}