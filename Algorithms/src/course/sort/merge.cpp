#include <iostream>
#include <vector>
#include <algorithm>
#include "input.h"

using namespace std;

void merge(vector<int> &v, int l, int mid, int r)
{
    int left = l, right = mid + 1, k = 0;
    vector<int> temp(r - l + 1);
    while (left <= mid && right <= r)
        if (v[left] < v[right])
            temp[k++] = v[left++];
        else
            temp[k++] = v[right++];

    while (left <= mid)
        temp[k++] = v[left++];
    while (right <= r)
        temp[k++] = v[right++];
    transform(temp.begin(), temp.end(), v.begin() + l, [](int e) -> int
              { return e; });
}

void merge_sort(vector<int> &v, int l, int r)
{
    if (l >= r)
        return;
    int mid = l + ((r - l) >> 1);
    merge_sort(v, l, mid);
    merge_sort(v, mid + 1, r);
    merge(v, l, mid, r);
}

int main()
{
    // int N;
    // cin >> N;
    // vector<int> temp(N);
    // for (int i = 0; i < N; ++i)
    //     cin >> temp[i];

    // merge_sort(temp, 0, temp.size() - 1);
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

    merge_sort(v, 0, SIZE - 1);

    auto end = chrono::steady_clock::now();
    chrono::duration<double, micro> span = end - start;
    cout << "time consumed: " << span.count() << " microseconds" << endl;
    return 0;
}