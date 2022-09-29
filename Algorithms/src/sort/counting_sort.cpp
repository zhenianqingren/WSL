#include <stdio.h>
#include <vector>
#include <random>
using namespace std;
// 计数排序不是基于比较的排序算法，其核心在于将输入的数据值转化
// 为键存储在额外开辟的数组空间中。 作为一种线性时间复杂度的排
// 序，计数排序要求输入的数据必须是有确定范围的整数
void counting_sort(vector<int> &v)
{
    int max = 0xffffffff;
    for (int e : v)
        max = max > e ? max : e;
    vector<int> bucket(max + 1, 0);
    for (int e : v)
        bucket[e]++;
    for (int i = 1; i < max + 1; ++i)
        bucket[i] += bucket[i - 1];//bucket[i]代表小于等于i的值有多少个
    vector<int> temp(v.size());
    for (int i = v.size() - 1; i >= 0; i--)
    {
        temp[bucket[v[i]] - 1] = v[i];//按照还剩多少个小于等于它的值的原则一个个放入桶中
        --bucket[v[i]];
    }
    for (int i = 0; i < v.size(); i++)
        v[i] = temp[i];
}

int main(int argc, char const *argv[])
{
    vector<int> test_case;
    for (int i = 0; i < 15; i++)
        test_case.push_back(rand() % 60);
    for (int e : test_case)
        printf("%d ", e);
    printf("\n");
    counting_sort(test_case);
    for (int e : test_case)
        printf("%d ", e);
    printf("\n");
    return 0;
}
