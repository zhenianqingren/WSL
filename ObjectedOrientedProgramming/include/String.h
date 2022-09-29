#ifndef _STRING_
#define _STRING_

#include <string.h>
#include <iostream>
using namespace std;
class String;

ostream &operator<<(ostream &os, const String &str);

class String
{
public:
    String(const char *cstr = nullptr);
    String(const String &str);
    // 编译器的默认拷贝构造是开辟一个新的空间 然后将数据如实的一位一位的全部复制 这就造成了两个不同对象的指针指向相同
    // 如果是数组的话没关系
    // 因此当类中的数据成员包含指针时要格外注意
    String &operator=(const String &str);
    //可能出现a=(b=c)的连等赋值情况 因此一般重载=要返回自身
    String &operator=(const char *cstr);
    ~String() { delete[] data; }
    char *get_c_str() const { return this->data; }
    // 类的静态成员只有一份 静态数据只被初始化一次

private:
    char *data;
};

#endif