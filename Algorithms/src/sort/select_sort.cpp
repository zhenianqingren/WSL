#include <stdio.h>
#include <vector>
#include <random>
using namespace std;

void select_sort(vector<int> &v)
{
    for (int i = 0; i < v.size() - 1; i++)
    {
        int minIndex = i;
        for (int j = i + 1; j < v.size(); j++)
            if (v[j] < v[minIndex])
                minIndex = j;
        if (minIndex != i)
        {
            v[i] = v[i] ^ v[minIndex];
            v[minIndex] = v[i] ^ v[minIndex];
            v[i] = v[i] ^ v[minIndex];
        }
    }
}

int main(int argc, char const *argv[])
{
    vector<int> test_case;
    for (int i = 0; i < 15; i++)
        test_case.push_back(rand() % 60);
    for (int e : test_case)
        printf("%d ", e);
    printf("\n");
    select_sort(test_case);
    for (int e : test_case)
        printf("%d ", e);
    printf("\n");
    return 0;
}
