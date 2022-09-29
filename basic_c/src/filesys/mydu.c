#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <glob.h>

#define PATHSIZE 256
static char suffix[3] = {'.', '*', '\0'};

int is_noloop(const char *str)
{
    const char *pos = strrchr(str, '/');

    if (!strcmp(pos + 1, ".") || !strcmp(pos + 1, ".."))
        return 0;

    return 1;
}

int64_t cal(const char *path)
{
    static struct stat status;
    int sym = lstat(path, &status);
    if (sym != 0)
    {
        printf("Error Occur!\n");
        exit(1);
    }

    if (!S_ISDIR(status.st_mode))
        return status.st_blocks;

    int64_t total = status.st_blocks;
    glob_t pglob;
    static char nextpath[PATHSIZE];
    strncpy(nextpath, path, PATHSIZE);
    strncat(nextpath, "/*", PATHSIZE);
    glob(nextpath, 0, NULL, &pglob);

    for (size_t i = 0; i < 3; ++i)
        nextpath[i + strlen(nextpath) - 1] = suffix[i];

    glob(nextpath, GLOB_APPEND, NULL, &pglob);
    for (size_t i = 0; i < pglob.gl_pathc; ++i)
        if (is_noloop(pglob.gl_pathv[i]))
            total += cal(pglob.gl_pathv[i]);

    globfree(&pglob);
    return total;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Error Arguments\n");
        return 1;
    }

    printf("%ld\t%s\n", cal(argv[1]) / 2, argv[1]);
    // printf("%ld\n", cal("./.vscode"));

    return 0;
}