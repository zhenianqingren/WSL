#include <stdio.h>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

void merge(vector<int> &v, int l, int mid, int r)
{
    int left = l, right = mid + 1;
    vector<int> temp;
    while (left <= mid && right <= r)
    {
        if (v[left] < v[right])
        {
            temp.push_back(v[left]);
            ++left;
        }
        else
        {
            temp.push_back(v[right]);
            ++right;
        }
    }
    while (left <= mid)
        temp.push_back(v[left++]);
    while (right <= r)
        temp.push_back(v[right++]);
    transform(temp.begin(), temp.end(), v.begin()+l, [](int e) -> int
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

int main(int argc, char const *argv[])
{
    vector<int> test_case;
    for (int i = 0; i < 15; i++)
        test_case.push_back(rand() % 60);
    for (int e : test_case)
        printf("%d ", e);
    printf("\n");
    merge_sort(test_case, 0, test_case.size() - 1);
    for (int e : test_case)
        printf("%d ", e);
    printf("\n");
    return 0;
}
