#include <stdio.h>
#include <setjmp.h>
static jmp_buf verify;
void back_trace(int depth)
{
    if (depth == 16)
    {
        longjmp(verify, depth);
        return;
    }

    printf("CURRENT: %d\n%s\n", depth, __FUNCTION__);
    back_trace(depth + 1);
}

int main()
{
    int res = setjmp(verify);
    if (res == 0)
    {
        printf("BEGIN\n");
        back_trace(1);
    }
    else
        printf("END DEPTH: %d\n%s\n", res, __FUNCTION__);
    return 0;
}