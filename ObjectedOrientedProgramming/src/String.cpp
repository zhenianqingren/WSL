#include "../include/String.h"

ostream &operator<<(ostream &os, const String &str)
{
    return os << str.get_c_str();
}

String::String(const char *cstr)
{
    if (cstr)
    {
        data = new char[strlen(cstr) + 1];
        strcpy(data, cstr);
    }
    else
        data = new char('\0');
    cout << "parameter constructor" << endl;
}

String::String(const String &str)
{
    data = new char[strlen(str.data) + 1];
    strcpy(data, str.data);
    std::cout << "copy constructor" << std::endl;
}

String &String::operator=(const String &str)
{
    if (this == &str)
        return *this; //不写会发生错误
    //如果不添加自我判定
    delete[] data;                         //删除了自身的data
    data = new char[strlen(str.data) + 1]; //又再次访问了自身的data
    strcpy(data, str.data);
    return *this;
}

String &String::operator=(const char *cstr)
{
    if (data)
        delete[] data;
    data = new char[strlen(cstr) + 1];
    strcpy(data, cstr);
    return *this;
}