#include <iostream>
#include <vector>
#include "input.h"
#define swap(x, y) \
    {              \
        x = x ^ y; \
        y = y ^ x; \
        x = x ^ y; \
    }
using namespace std;

void heapify(vector<int> &v, int root, int size)
{
    int temp = v[root];
    int p = root;
    int k;
    while ((p << 1) + 1 < size)
    {
        k = (p << 1) + 1;
        int maxIndex = k + 1 < size && v[k] < v[k + 1] ? k + 1 : k;
        if (temp > v[maxIndex])
            break;
        v[p] = v[maxIndex];
        p = maxIndex;
    }
    v[p] = temp;
}

void heap_sort(vector<int> &v)
{
    for (int i = (v.size() >> 1) - 1; i >= 0; --i)
        heapify(v, i, v.size());
    for (int i = v.size() - 1; i > 0; --i)
    {
        swap(v[0], v[i]);
        heapify(v, 0, i);
    }
}

int main()
{
    // int N;
    // cin >> N;
    // vector<int> temp(N);
    // for (int i = 0; i < N; ++i)
    //     cin >> temp[i];
    // heap_sort(temp);
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

    heap_sort(v);

    auto end = chrono::steady_clock::now();
    chrono::duration<double, micro> span = end - start;
    cout << "time consumed: " << span.count() << " microseconds" << endl;
    return 0;
}