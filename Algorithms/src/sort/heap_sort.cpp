#include <stdio.h>
#include <vector>
#include <random>
using namespace std;

void heapify(vector<int> &v, int root, int size)
{
    int temp = v[root];
    int p = root;
    while (p * 2 + 1 < size)
    {
        int maxIndex = v[(p << 1) + 1] < v[(p << 1) + 2] && ((p << 1) + 2) < size ? ((p << 1) + 2) : ((p << 1) + 1);
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
        v[0] = v[0] ^ v[i];
        v[i] = v[0] ^ v[i];
        v[0] = v[0] ^ v[i];
        heapify(v, 0, i);
    }
}


int main(int argc, char const *argv[])
{
    vector<int> test_case;
    // for (int i = 0; i < 16; i++)
    //     test_case.push_back(rand() % 60);
    for (int i = 4; i >= 0; --i)
        test_case.push_back(i);
    for (int e : test_case)
        printf("%d ", e);
    printf("\n");
    heap_sort(test_case);
    for (int e : test_case)
        printf("%d ", e);
    printf("\n");
    return 0;
}
