#include <iostream>
#include <vector>
#include "input.h"
using namespace std;

int maxbits(vector<int> &v)
{
    int max = 0xffffffff;
    for (int i = 0; i < v.size(); ++i)
        max = max < v[i] ? v[i] : max;
    int res = 0;
    while (max != 0)
    {
        ++res;
        max /= 10;
    }
    return res;
}

int getDigit(int num, int bit)
{
    while (--bit != 0)
        num /= 10;
    return num % 10;
}

void radix_sort(vector<int> &v)
{
    int digit = maxbits(v);
    const int radix = 10;
    vector<int> bucket(v.size());

    for (int d = 1; d <= digit; ++d)
    {
        vector<int> count(radix, 0);
        for (int e : v)
            ++count[getDigit(e, d)];
        for (int i = 1; i < count.size(); ++i)
            count[i] += count[i - 1];

        for (int i = v.size() - 1; i >= 0; --i)
        {
            int j = getDigit(v[i], d);
            bucket[count[j] - 1] = v[i];
            --count[j];
        }
        for (int i = 0; i < v.size(); ++i)
            v[i] = bucket[i];
    }
}

int main()
{
    // int N;
    // cin >> N;
    // vector<int> temp(N);
    // for (int i = 0; i < N; ++i)
    //     cin >> temp[i];
    // radix_sort(temp);
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

    radix_sort(v);

    auto end = chrono::steady_clock::now();
    chrono::duration<double, micro> span = end - start;
    cout << "time consumed: " << span.count() << " microseconds" << endl;
    return 0;
}