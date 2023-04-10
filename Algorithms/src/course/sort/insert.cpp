#include <iostream>
#include <vector>
#include <random>
#include "input.h"
using namespace std;

void insert_sort(vector<int> &v)
{
    for (int i = 1; i < v.size(); i++)
    {
        int j;
        int cur = v[i];
        for (j = i; j > 0 && cur < v[j - 1]; --j)
            v[j] = v[j - 1];
        v[j] = cur;

        // if (i <= 3)
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

int main(int argc, char const *argv[])
{
    // int n;
    // cin >> n;
    // vector<int> test_case(n);
    // for (int i = 0; i < n; ++i)
    //     cin >> test_case[i];
    // insert_sort(test_case);
    // for (int k = 0; k < test_case.size(); ++k)
    // {
    //     cout << test_case[k];
    //     if (k != test_case.size() - 1)
    //         cout << " ";
    // }
    vector<int> v;
    init(v);

    auto start = chrono::steady_clock::now();

    insert_sort(v);

    auto end = chrono::steady_clock::now();
    chrono::duration<double, micro> span = end - start;
    cout << "time consumed: " << span.count() << " microseconds" << endl;
    return 0;
}
