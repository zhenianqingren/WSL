#include <stdio.h>
#include <stdlib.h>

/*
行缓冲：换行时刷新
全缓冲：缓冲区满时强制刷新
无缓冲：stderr
*/


int main()
{

    // printf("Before while\t");
    // printf("Before while\n");
    printf("Before while\t");
    fflush(stdout);
    for (;;)
        ;
    printf("After while\n");

    return 0;
}