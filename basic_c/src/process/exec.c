#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
int main(int argc, char *argv[])
{
    puts("BEGIN");
    fflush(NULL); // if annotation and redirect to no stdio?

    // execl("/bin/date", "+%s", NULL);
    execl("./bin/out_index", NULL);

    // puts("ERROR");
    fprintf(stderr, "%s\n", strerror(errno));
    exit(1);
}
