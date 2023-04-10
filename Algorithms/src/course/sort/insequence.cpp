#include <bits/stdc++.h>

// #define DP
#define BS
using namespace std;

#define N 2000

int dp[N];
int id[N];
int arr[N];

int d[N];

vector<int> seq[N];

int main()
{

    int n;
    cin >> n;
    for (int i = 0; i < n; ++i)
        arr[i] = rand() % 1000;

    // for (int i = 0; i < n; ++i)
    //     cin >> arr[i];

    // for (int i = 0; i < N; ++i)
    //     seq[i].reserve(N);
    auto start = chrono::steady_clock::now();
#ifdef DP
    int maxi = n - 1;
    for (int i = 0; i < n; ++i)
    {
        id[i] = n;
        dp[i] = 1;
    }

    for (int i = n - 2; i >= 0; --i)
    {
        for (int j = i + 1; j < n; ++j)
            if (arr[j] > arr[i] && (dp[j] + 1) > dp[i])
            {
                dp[i] = dp[j] + 1;
                id[i] = j;
            }

        if (dp[i] > dp[maxi])
            maxi = i;
    }

    // for (int i = maxi; i < n; i = id[i])
    //     if (id[i] < n)
    //         cout << arr[i] << " ";
    //     else
    //         cout << arr[i];
    // cout << endl;

    cout << dp[maxi] << endl;
#endif

#ifdef BS
    int len = 1;
    d[1] = arr[0];
    // seq[1].emplace_back(arr[0]);

    for (int i = 1; i < n; ++i)
    {
        if (arr[i] > d[len])
        {
            d[++len] = arr[i];
            // seq[len].resize(len);
            // transform(seq[len - 1].begin(), seq[len - 1].end(), seq[len].begin(), [](int e) -> int
            //           { return e; });
            // seq[len][len - 1] = arr[i];
            continue;
        }
        int left = 0;
        int right = len - 1;
        int mid;
        int rep;
        while (left <= right)
        {
            mid = (left + right) >> 1;
            if (arr[i] > d[mid])
            {
                rep = mid + 1;
                left = mid + 1;
            }
            else
                right = mid - 1;
        }
        d[rep] = arr[i];
        // seq[rep][rep - 1] = arr[i];
    }

    // for (int i = 0; i < len; ++i)
    //     cout << seq[len][i] << " ";
    // cout << endl;
    cout << len << endl;
#endif

    auto end = chrono::steady_clock::now();
    chrono::duration<double, nano> span = end - start;
    cout << "time: " << span.count() << endl;

    return 0;
}