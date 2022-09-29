#include <iostream>
using namespace std;
class Test
{
public:
    void operator()(int&x, int&y)
    {
        if (x != y)
        {
            x = x ^ y;
            y = x ^ y;
            x = x ^ y;
        }
    }
};

int main(int argc, char const *argv[])
{
    int i = 1, j = 2;
    Test()(i, j);
    cout << i << "  " << j << endl;
    return 0;
}
