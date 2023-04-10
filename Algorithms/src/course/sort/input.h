#include <vector>
#include <chrono>
using namespace std;

#define SIZE 11000

void init(vector<int> &v)
{
    v.resize(SIZE);
    for (int i = SIZE; i >= 1; --i)
        v[SIZE - i] = i;
}
