#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <error.h>

int main(int argc, char **argv)
{
    FILE *sour, *dst;
    int ch;
    if (argc < 3)
    {
        fprintf(stderr, "Usage:%s <source> <destination>\n", argv[0]);
        exit(1);
    }
    sour = fopen(argv[1], "r");
    dst = fopen(argv[2], "w");
    if (sour == NULL || dst == NULL)
    {
        fprintf(stderr, "fopen:%s\n", strerror(errno));
        exit(1);
    }
    while ((ch = fgetc(sour)) != EOF /*fgets(buffer,buffersize,sour)!=NULL  (n=fread(buffer,1,buffersize,sour))>0*/)
        fputc(ch, dst); // fputs(buffer,dst); fwrite(buffer,1,n,dst); buffersize*1

    fclose(sour);
    fclose(dst);
}