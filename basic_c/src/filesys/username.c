#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>

int main(int argc, char **argv)
{

    if (argc < 2)
        return 1;

    int uid = atoi(argv[1]);
    struct passwd *info = getpwuid(uid);

    printf("username: %s\tpassword: %s\n", info->pw_name, info->pw_passwd);

    return 0;
}