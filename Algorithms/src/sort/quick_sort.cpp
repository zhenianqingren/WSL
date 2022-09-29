#include <stdio.h>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

void quick_sort(vector<int> &v, int l, int r)
{
    if (l >= r)
        return;
    int base = v[l];
    int left = l, right = r;
    while (left < right)
    {
        while (left < right && v[right] >= base)
            --right;
        while (left < right && v[left] <= base)
            ++left;
        if (left != right)
        {
            v[left] = v[left] ^ v[right];
            v[right] = v[left] ^ v[right];
            v[left] = v[left] ^ v[right];
        }
    }
    if (left != l)
    {
        v[left] = v[left] ^ v[l];
        v[l] = v[left] ^ v[l];
        v[left] = v[left] ^ v[l];
    }
    quick_sort(v, l, left - 1);
    quick_sort(v, left + 1, r);
}

int main(int argc, char const *argv[])
{
    vector<int> test_case;
    for (int i = 0; i < 15; i++)
        test_case.push_back(rand() % 60);
    for (int e : test_case)
        printf("%d ", e);
    printf("\n");
    quick_sort(test_case, 0, test_case.size() - 1);
    for (int e : test_case)
        printf("%d ", e);
    printf("\n");
    return 0;
}
