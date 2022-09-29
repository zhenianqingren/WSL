#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#define BEGIN 123456
#define END 123654

void prime_verify(int num)
{
    char jmp_sym = '0';
    for (int i = 2; i < (int)(sqrt(num)) + 1; ++i)
        if (num % i == 0)
        {
            jmp_sym = '1';
            break;
        }
    if (jmp_sym == '0')
        printf("%d:No Prime Number!\n", num);
    else
        printf("%d:Prime Number\n", num);
    exit(0);
}

int main()
{
    for (int i = BEGIN; i <= END; ++i)
    {
        __pid_t cur_pid = fork();
        if (cur_pid == 0)
            prime_verify(i);
        if (cur_pid == -1)
        {
            fprintf(stderr, "error!");
            return -1;
        }
    }

    return 0;
}