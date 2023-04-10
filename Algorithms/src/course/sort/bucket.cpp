#include <iostream>
#include <vector>
#include "input.h"
#define BUCKETCOUNT 5
using namespace std;

void quick_sort(vector<int> &v, int l, int r)
{
    if (l >= r)
        return;
    int L = l - 1, R = r + 1;
    int key = v[l + ((r - l) >> 1)];
    for (int i = l; i < R; ++i)
    {
        if (v[i] < key)
        {
            ++L;
            if (L != i)
                swap(v[L], v[i]);
        }
        else if (v[i] > key)
        {
            --R;
            if (i != R)
                swap(v[i], v[R]);
            --i;
        }
    }
    quick_sort(v, l, L);
    quick_sort(v, R, r);
}

void bucket_sort(vector<int> &v)
{
    int max = 0xffffffff;
    int min = 0x7fffffff;
    for (int e : v)
    {
        max = max >= e ? max : e;
        min = min <= e ? min : e;
    }
    int interval = (max - min) / BUCKETCOUNT + 1;
    vector<vector<int>> bucket_set(BUCKETCOUNT);

    for (int e : v)
        bucket_set[(e - min) / interval].push_back(e);
    for (vector<int> &tmp : bucket_set)
        quick_sort(tmp, 0, tmp.size() - 1);

    int i = 0;
    for (vector<int> &tmp : bucket_set)
        for (int e : tmp)
            v[i++] = e;
}

int main()
{
    // int N;
    // cin >> N;
    // vector<int> temp(N);
    // for (int i = 0; i < N; ++i)
    //     cin >> temp[i];
    // bucket_sort(temp);
    // for (int i = 0; i < N; ++i)
    // {
    //     if (i != N - 1)
    //         cout << temp[i] << " ";
    //     else
    //         cout << temp[i];
    // }

    vector<int> v;
    init(v);

    auto start = chrono::steady_clock::now();

    bucket_sort(v);

    auto end = chrono::steady_clock::now();
    chrono::duration<double, micro> span = end - start;
    cout << "time consumed: " << span.count() << " microseconds" << endl;
    return 0;
}