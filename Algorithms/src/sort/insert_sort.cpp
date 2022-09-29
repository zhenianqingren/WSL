#include <stdio.h>
#include <vector>
#include <random>
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
    insert_sort(test_case);
    for (int e : test_case)
        printf("%d ", e);
    printf("\n");
    return 0;
}
