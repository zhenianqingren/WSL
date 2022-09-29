#include <iostream>
#include <vector>
using namespace std;

vector<int> v({1, 2, 3, 4, 5, 6});
int &ReturnVal(int i)
{
    return v[i];
}

int main(int argc, char const *argv[])
{
    ReturnVal(2) = 0;
    cout << v[2];
    return 0;
}
