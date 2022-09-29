#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char const *argv[])
{
    //输入流
    // tellg() 返回标记的当前位置
    // seekg(pos) 标记重定位到给定的绝对地址pos
    // seekg(off,from) 将标记定位到from之前或之后off个字符
    // from可以是 beg:偏移量相对于流开始位置
    //           cur:偏移量相对于流当前位置
    //           end:偏移量相对于流结尾位置

    // tellp()
    // seekp(pos)
    // seekp(off,from)
    fstream inOut("/home/mice/cpp_learn/src/IO/file/copyOut", fstream::ate | fstream::in | fstream::out);
    // cout << inOut.peek() << endl;
    // inOut.unget();
    // cout << inOut.peek() << endl;
    inOut << "\n";
    auto end_mark = inOut.tellg();
    inOut.seekg(0, fstream::beg);
    size_t cnt = 0;
    string line;
    while (inOut && inOut.tellg() != end_mark && getline(inOut, line))
    {
        cnt += line.size() + 1; //加1表示换行符
        auto mark = inOut.tellg();
        inOut.seekp(0, fstream::end);
        inOut << cnt;
        //输出累计的长度
        if (mark != end_mark)
            inOut << " ";
        inOut.seekg(mark);
    }
    inOut.seekp(0, fstream::end);
    inOut << "\n";
    return 0;
}
