#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    alarm(5);
    for (;;)
        ;
    return 0;
}
