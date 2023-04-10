#include <iostream>
#include <vector>
#include <algorithm>
#include "input.h"
using namespace std;

void count_sort(vector<int> &v, vector<int> &b)
{
    int max = 0xffffffff;
    for (int e : v)
        max = max > e ? max : e;
    vector<int> bucket(max + 1, 0);
    for (int e : v)
        ++bucket[e];

    b.resize(max + 1);
    transform(bucket.begin(), bucket.end(), b.begin(), [](int e) -> int
              { return e; });
    for (int i = 1; i < max + 1; ++i)
        bucket[i] += bucket[i - 1];
    vector<int> temp(v.size());
    for (int i = 0; i < v.size(); ++i)
    {
        temp[bucket[v[i]] - 1] = v[i];
        --bucket[v[i]];
    }
    transform(temp.begin(), temp.end(), v.begin(), [](int e) -> int
              { return e; });
}

int main()
{
    // int N;
    // cin >> N;
    // vector<int> temp(N);
    // vector<int> b;
    // for (int i = 0; i < N; ++i)
    //     cin >> temp[i];
    // count_sort(temp, b);
    // for (int i = 0; i < b.size(); ++i)
    //     if (b[i] != 0)
    //         cout << i << " " << b[i] << "\n";
    // for (int i = 0; i < N; ++i)
    // {
    //     if (i != N - 1)
    //         cout << temp[i] << " ";
    //     else
    //         cout << temp[i];
    // }
    vector<int> v, b;
    init(v);

    auto start = chrono::steady_clock::now();

    count_sort(v, b);

    auto end = chrono::steady_clock::now();
    chrono::duration<double, micro> span = end - start;
    cout << "time consumed: " << span.count() << " microseconds" << endl;
    return 0;
}