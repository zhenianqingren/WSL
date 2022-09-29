#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Invalid Parameter\n");
        return -1;
    }
    FILE *file = fopen(argv[1], "r");
    char *line = NULL;
    size_t l_size = 0;//缓冲区大小 char**

    while ((getline(&line, &l_size, file)) > 0)
        printf("%s", line);
    free(line);
    return 0;
}
