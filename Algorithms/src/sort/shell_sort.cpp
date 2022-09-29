#include <stdio.h>
#include <vector>
#include <random>
using namespace std;

void shell_sort(vector<int> &v)
{
    int gap = v.size() >> 1;
    while (gap > 0)
    {
        for (int i = gap; i < v.size(); i++)
        {
            int j = i;
            int cur = v[j];
            while (j - gap >= 0 && cur < v[j - gap])
            {
                v[j] = v[j - gap];
                j -= gap;
            }
            v[j] = cur;
        } //希尔排序是划分一个个组分别排序
        //但代码实际执行的过程中是交错对每组进行排序
        gap = gap >> 1;
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
    shell_sort(test_case);
    for (int e : test_case)
        printf("%d ", e);
    printf("\n");
    return 0;
}
