#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

int main(int argc, char const *argv[])
{
    FILE *reader = fopen("./src/iofunc.c", "r");
    if (reader == NULL)
    {
        fprintf(stderr, "Error Open");
        return -1;
    }

    fseek(reader, 0, SEEK_END); // SEEK_END+0
    printf("%ld\n", ftell(reader));
    fclose(reader);

    char ch;
    short sel = 0;
    for (;;)
    {
        ch = sel ? '=' : '-';
        for (int i = 0; i < 12; ++i)
        {
            printf("%c", ch);
            fflush(NULL);
            usleep(100000);
        }
        for (int i = 0; i < 12; ++i)
        {
            printf("\b");
            fflush(NULL);
        }
        sel = sel ? 0 : 1;
    }

    return 0;
}
