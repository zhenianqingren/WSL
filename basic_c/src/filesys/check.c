#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <shadow.h>
#include <string.h>

int main(int argc, char *argv[])
{
    if (argc < 2)
        return 1;

    char *input;
    struct spwd *shadow;
    char *crypted;
    input = getpass("Please Input PassWord:");

    shadow = getspnam(argv[1]);
    crypted = crypt(input, shadow->sp_pwdp);

    if (!strcmp(shadow->sp_pwdp, crypted))
        printf("Ok\n");
    else
        printf("Fail\n");

    printf("%s\n", shadow->sp_pwdp);
    printf("%s\n", crypted);
    return 0;
}