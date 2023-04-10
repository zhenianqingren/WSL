#include <stdio.h>
#include <unistd.h>
int addcnt;

int main()
{
    ++addcnt;
    printf("%d\n", addcnt);
    sleep(16);
    return 0;
}