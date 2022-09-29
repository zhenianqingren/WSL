#include <stdio.h>
#include <stdlib.h>
#include <glob.h>
#include <string.h>
#include <error.h>
#include <errno.h>
#include <sys/types.h>
#include <dirent.h>

#define PAT "/etc/.*"
#define DIRNAME "/etc"
//glob match all filename into pglob
// opendir address dir
int error_f(const char *epath, int eerrno)
{
    printf("ERROR PATH:%s\nERROR:%s\n", epath, strerror(eerrno));
}

int main()
{
    glob_t pglob;
    int (*pf)(const char *, int) = error_f;
    int res = glob(PAT, 0, pf, &pglob);
    if (res != 0)
    {
        *pf;
        return 1;
    }

    for (size_t i = 0; i < pglob.gl_pathc; ++i)
        puts(pglob.gl_pathv[i]);
    globfree(&pglob);

    printf("\n\n\n");

    DIR *dir = opendir(DIRNAME);
    struct dirent *dir_i;

    while ((dir_i = readdir(dir)) != NULL)
        puts(dir_i->d_name);

    closedir(dir);
    return 0;
}