#include <stdio.h>
#include <stdlib.h>

int main(int argv, char *argc[])
{
    FILE *fp = fopen(argc[1], "r");

    while (fgetc(fp) != EOF)
        ;
    printf("size: %ld\n", ftell(fp));
    fclose(fp);
    return 0;
}