#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
    int i = 5;
    auto old_state = cin.rdstate(); //记住cin当前的状态
    cin.clear();                    //将流置为有效
    cin >> i;
    cin.setstate(old_state);

    //复位failbit和badbit 保持其他标志位不变
    cin.clear(cin.rdstate() & ~cin.failbit & ~cin.badbit);
    cout << "hi" << endl;  //输出hi和一个换行 并刷新缓冲区
    cout << "hi" << ends;  //输出hi和一个空字 并换行
    cout << "hi" << flush; //输出hi和一个空字符 然后刷新缓冲区

    cout << unitbuf;   //所有输出操作后立即刷新缓冲区
    cout << nounitbuf; //回到正常的缓冲模式

    //当一个输入流被关联到一个输出流时 任何试图从输入流读取数据的操作
    //都会先刷新关联的输出流

    // tie()返回指向关联流的指针
    ostream *old_tie = cin.tie(nullptr);
    // cin不再与其他流关联
    cin.tie(old_tie);
    //重新与cout关联
    
    return 0;
}
