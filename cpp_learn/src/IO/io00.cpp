#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char const *argv[])
{
    char ch;
    //默认情况下 输入运算符会忽略空白符
    //空格符 制表符 换行符 换纸符 回车符
    cin >> noskipws; //命令输入运算符读入空白符 而不是跳过它们

    while (cin >> ch)
        cout << ch;
    cin >> skipws; //恢复

    ifstream ifs("/home/mice/cpp_learn/src/IO/file/test3");
    ifs >> noskipws;
    while (ifs >> ch)
        cout << ch;
    cout << endl;
    return 0;
}