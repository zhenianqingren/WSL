#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
int main(int argv, char *argc[])
{

    FILE *reader = fopen(argc[1], "r");
    FILE *writer = fopen(argc[1], "r+");
    char buffer[16];
    char *line = NULL;
    int overwrite = 10;
    int width;
    size_t len = 0;
    while (overwrite-- != 0)
        width = getline(&line, &len, reader);
    printf("%s", line);
    printf("\n\n%lu\n", len);

    // overwrite = 9;
    // while (overwrite-- != 0)
    //     getline(&line, &len, writer);

    // while ((len = fread(buffer, 1, 16, reader)) > 0)
    //     fwrite(buffer, 1, len, writer);

    fclose(reader);
    fclose(writer);
    // truncate(argc[1], 14);
    return 0;
}