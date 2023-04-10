#include <bits/stdc++.h>
using namespace std;

#define N 1024
char s[N];
char stk[N];

int main()
{
    int input;
    int k;
    scanf("%s", s);
    scanf("%d", &k);

    int n = strlen(s);

    int i = 0;
    int top = -1;
    while (i < n)
    {
        while (top != -1 && stk[top] < s[i] && k > 0)
        {
            --k;
            --top;
        }
        stk[++top] = s[i++];
    }

    while (k-- > 0)
        --top;

    for (i = 0; stk[i] == '0' && i <= top;)
        ++i;

    if (i > top)
        cout << 0;
    else
    {
        for (; i <= top; ++i)
            cout << (int)(stk[i] - '0');
    }
    return 0;
}