#include <stdio.h>
#include <vector>
#include <random>
using namespace std;

void bubble_sort(vector<int> &v)
{
    for (int i = 0; i < v.size() - 1; ++i)
        for (int j = 0; j < v.size() - 1 - i; ++j)
            if (v[j] > v[j + 1])
            {
                v[j] = v[j] ^ v[j + 1];
                v[j + 1] = v[j] ^ v[j + 1];
                v[j] = v[j] ^ v[j + 1];
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
    bubble_sort(test_case);
    for (int e : test_case)
        printf("%d ", e);
    printf("\n");
    return 0;
}
