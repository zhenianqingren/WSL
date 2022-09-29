#include <iostream>
#include "../include/complex.h"
#include "../include/String.h"
int main(int argc, char const *argv[])
{
    complex c(2, 2);
    cout << conj(c) << endl;
    cout << -c << endl;

    String init("abcd123");
    String str1 = init;     //实际上调用的不是重载=str的函数 而是拷贝构造函数
    String str2 = "456789"; //实际上调用的不是重载=char*的函数 而是构造函数
    cout << str1 << "  " << str2 << endl;
    return 0;
}
