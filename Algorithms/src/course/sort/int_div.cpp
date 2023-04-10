// #include <iostream>
// using namespace std;

// #define N 100000
// int dp[N];

// int main()
// {
//     dp[1] = 0;
//     dp[2] = 0;
//     dp[3] = 0;

//     int n;
//     cin >> n;
//     if (n <= 3)
//         return dp[n];

//     for (int i = 4; i <= n; ++i)
//     {
//         dp[i] = 0;
//         int half_ = (i >> 1) + 1;
//         for (int j = half_; j > 1; --j)
//         {
//             if (i % j != 0)
//                 continue;
//             dp[i] += 1 + dp[j] + dp[i / j];
//         }
//     }

//     return 0;
// }

#include <bits/stdc++.h>

using namespace std;

const int N = 100000;
int ff[N];

int f(int n)
{
    int sum = 1;
    if (n < N && ff[n] != 0)
        return ff[n];
    for (int i = 2; i <= sqrt(n); i++)
    {
        if (n % i == 0)
        {
            if (i * i == n)
                sum += f(i);
            else
                sum += f(i) + f(n / i);
        }
    }
    if (n < N)
        ff[n] = sum;
    return sum;
}

int main()
{
    int n;
    scanf("%d", &n);
    memset(ff, 0, sizeof(ff));
    printf("%d", f(n));
    return 0;
}