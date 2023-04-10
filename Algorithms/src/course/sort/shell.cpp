#include <iostream>
#include <vector>
#include "input.h"
using namespace std;

void shell_sort(vector<int> &v)
{
    int gap = 5;
    int count = 0;
    while (gap > 0)
    {
        for (int i = gap; i < v.size(); ++i)
        {
            int j = i;
            int cur = v[j];
            while (j - gap >= 0 && cur < v[j - gap])
            {
                v[j] = v[j - gap];
                j -= gap;
            }
            v[j] = cur;
        }
        gap >>= 1;
        // if (count < 3)
        // {
        //     for (int k = 0; k < v.size(); ++k)
        //     {
        //         cout << v[k];
        //         if (k != v.size() - 1)
        //             cout << " ";
        //     }
        //     cout << "\n";
        //     ++count;
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
    // shell_sort(test_case);
    // for (int k = 0; k < test_case.size(); ++k)
    // {
    //     cout << test_case[k];
    //     if (k != test_case.size() - 1)
    //         cout << " ";
    // }

    vector<int> v;
    init(v);

    auto start = chrono::steady_clock::now();

    shell_sort(v);

    auto end = chrono::steady_clock::now();
    chrono::duration<double, micro> span = end - start;
    cout << "time consumed: " << span.count() << " microseconds" << endl;
    return 0;
}