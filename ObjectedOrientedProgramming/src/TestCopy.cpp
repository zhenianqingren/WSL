#include <iostream>
using namespace std;

class A
{
public:
    int a;
    int b;
    int c;
    int v[3] = {0, 1, 2};
};

class B
{
public:
    A B_A;
    double b;
};
// 编译器的默认拷贝构造是开辟一个新的空间 然后将数据如实的一位一位的全部复制 这就造成了两个不同对象的指针指向相同
int main(int argc, char const *argv[])
{
    B b1;
    b1.b = 1.234;
    b1.B_A.a = 0;
    b1.B_A.b = 0;
    b1.B_A.c = 0;
    B b2 = b1;
    b2.b = 3.14159;
    b2.B_A.a = -1;
    b2.B_A.v[0] = 100;
    cout << b1.b << "  " << b1.B_A.a << "  " << b1.B_A.v[0] << endl;
    return 0;
}
