#include <iostream>
#include <vector>
#include <random>
#include "input.h"
using namespace std;

void bubble_sort(vector<int> &v)
{
    for (int i = 0; i < v.size() - 1; ++i)
    {
        for (int j = 0; j < v.size() - 1 - i; ++j)
            if (v[j] > v[j + 1])
            {
                v[j] = v[j] ^ v[j + 1];
                v[j + 1] = v[j] ^ v[j + 1];
                v[j] = v[j] ^ v[j + 1];
            }
        // if (i <= 2)
        // {
        //     for (int k = 0; k < v.size(); ++k)
        //     {
        //         cout << v[k];
        //         if (k != v.size() - 1)
        //             cout << " ";
        //     }
        //     cout << "\n";
        // }
    }
}

int main()
{
    // int n;
    // cin >> n;
    // vector<int> test_case(n);
    // for (int i = 0; i < n; ++i)
    //     cin >> test_case[i];
    // bubble_sort(test_case);
    // for (int k = 0; k < test_case.size(); ++k)
    // {
    //     cout << test_case[k];
    //     if (k != test_case.size() - 1)
    //         cout << " ";
    // }

    vector<int> v;
    init(v);

    auto start = chrono::steady_clock::now();

    bubble_sort(v);

    auto end = chrono::steady_clock::now();
    chrono::duration<double, micro> span = end - start;
    cout << "time consumed: " << span.count() << " microseconds" << endl;
    return 0;
}