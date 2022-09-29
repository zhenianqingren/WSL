#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{

    struct stat status;
    if (stat("/home/mice/basic_c/practice1.txt", &status) < 0)
    {
        fprintf(stderr, "Fail\n");
        return -1;
    }
    char mode_set[3] = {'r', 'w', 'x'};
    char mode_p;
    // printf("%o\n", status.st_mode);
    printf("___");
    for (int i = 0; i < 9; ++i)
    {
        if ((status.st_mode & (S_IRUSR >> i)) != 0)
            mode_p = mode_set[i % 3];
        else
            mode_p = '-';
        printf("%c", mode_p);
        // printf("%o\n", status.st_mode & (S_IRUSR >> i));
    }
    printf(" 1 %d %d %d    practice1.txt\n", status.st_uid, status.st_gid, status.st_size);

    return 0;
}