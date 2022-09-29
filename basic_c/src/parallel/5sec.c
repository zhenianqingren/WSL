#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    time_t end;
    unsigned long long count = 0;
    end = time(NULL) + 5;
    while (time(NULL) <= end)
        ++count;

    printf("%llu\n", count);
    return 0;
}
