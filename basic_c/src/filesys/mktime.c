#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE 128

int main()
{
    char buffer[SIZE];

    time_t _time = time(NULL);

    struct tm *_tm;
    _tm = localtime(&_time);

    if (strftime(buffer, SIZE, "NOW:%Y-%m-%d", _tm) == 0)
        return EXIT_FAILURE;

    printf("%s\n", buffer);

    _tm->tm_mday += 100;
    if (mktime(_tm) == (time_t)-1)
        return EXIT_FAILURE;

    if (strftime(buffer, SIZE, "100 DAYS LATER:%Y-%m-%d", _tm) == 0)
        return EXIT_FAILURE;

    puts(buffer);

    return 0;
}