#include <bits/stdc++.h>
using namespace std;

#define N 128
int a[N];

int n;
int cnt = 0;

void backtrace(int h)
{
    if (h == n + 1)
    {
        ++cnt;
        return;
    }

    int i = h == 1 ? 1 : 0;
    int tmp;
    for (; i <= 9; ++i)
    {
        tmp = 0;
        a[h] = i;
        for (int j = 1; j <= h; ++j)
        {
            tmp = tmp * 10 + a[j];
            tmp = tmp % h;
        }
        if (tmp == 0)
            backtrace(h + 1);
    }

    return;
}

int main()
{
    cin >> n;
    backtrace(1);
    cout << cnt;
    
    return 0;
}