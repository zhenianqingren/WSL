#include <iostream>
#include <vector>
#include "input.h"
#define swap(x, y) \
    {              \
        x = x ^ y; \
        y = x ^ y; \
        x = x ^ y; \
    }
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

int main()
{
    // int N;
    // cin >> N;
    // vector<int> temp(N);
    // for (int i = 0; i < N; ++i)
    //     cin >> temp[i];
    // quick_sort(temp, 0, temp.size() - 1);
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

    quick_sort(v, 0, SIZE - 1);

    auto end = chrono::steady_clock::now();
    chrono::duration<double, micro> span = end - start;
    cout << "time consumed: " << span.count() << " microseconds" << endl;
    return 0;
}