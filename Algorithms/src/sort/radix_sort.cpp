#include <stdio.h>
#include <vector>
#include <random>
using namespace std;

int maxbits(vector<int> &v)
{
    int max = 0xffffffff;
    for (int i = 0; i < v.size(); ++i)
        max = max < v[i] ? v[i] : max;
    int res = 0;
    while (max != 0)
    {
        res++;
        max /= 10;
    }
    return res;
}

int getDigit(int num, int bit)
{
    while (--bit != 0)
        num /= 10;
    return num % 10;
}

void radix_sort(vector<int> &v)
{
    int digit = maxbits(v);
    const int radix = 10;
    //不同进制的基底
    int *bucket = new int[v.size()];
    for (int d = 1; d <= digit; ++d) //有多少位就进出几次
    {
        // count[i]当前位是0~i的数字有多少个
        vector<int> count(radix, 0);
        for (int i = 0; i < v.size(); ++i)
            ++count[getDigit(v[i], d)];
        for (int i = 1; i < radix; ++i)
            count[i] += count[i - 1];
        //小于等于当前位的数字有多少个
        for (int i = v.size() - 1; i >= 0; --i)
        {
            int j = getDigit(v[i], d);
            bucket[count[j] - 1] = v[i];//此处类似于计数排序 逆序遍历是尽量使大的值放在后面 如果不这样做本来桶中的32 37会变成37 32
            --count[j]; //当前减一即可 因为它一定会放在所有小于它的数字之前
        }

        //逆序倒入桶内
        for (int i = 0; i < v.size(); ++i)
            v[i] = bucket[i];
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
    radix_sort(test_case);
    for (int e : test_case)
        printf("%d ", e);
    printf("\n");
    return 0;
}
