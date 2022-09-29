#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char const *argv[])
{
    char ch;
    //未格式化IO
    //会读取而不是忽略空白字符
    // istream.get(ch)
    // ostream.put(ch)
    // istream.get() 将is的下一个字节作为int返回
    // istream.putback(ch) 将字符ch放回is 返回is
    // is.unget() 将is往返移动一个字节 返回is
    // is.peek() 将下一个字节作为int返回 但不从流中删除它
    int count = 0;
    ifstream ifs("/home/mice/cpp_learn/src/IO/file/test3");
    /*     cout << ifs.peek() << " ";
        while (ifs.get(ch))
        {
            // cout.put(ch);
            cout << ifs.peek() << " ";
            ++count;
            if (count == 8)
                ifs.unget();
        } */
    cout << ifs.peek() << endl;
    ifs.get(ch);
    ifs.get(ch);
    ifs.get(ch);
    cout << ifs.peek() << endl;
    ifs.unget();
    cout << ifs.peek() << endl;
    ifs.seekg(0, ifstream::end);
    cout << ifs.peek() << endl;
    return 0;
}
