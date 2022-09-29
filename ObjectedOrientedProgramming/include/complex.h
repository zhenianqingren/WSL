#ifndef _COMPLEX_
#define _COMPLEX_

#include <iostream>
#include <cmath>
using namespace std;

class complex; //前置声明

complex &_doapl(complex *ths, const complex &r);
double imag(const complex &c);
double real(const complex &c);
complex operator+(const complex &x, const complex &y);
complex operator+(const complex &c, double d);
complex operator+(double d, const complex &c);
complex &operator+(const complex &c);
complex operator-(const complex &c);
bool operator==(const complex &x, const complex &y);
bool operator==(const complex &c, double d);
bool operator==(double d, const complex &c);
bool operator!=(const complex &x, const complex &y);
bool operator!=(const complex &c, double d);
bool operator!=(double d, const complex &c);

complex conj(const complex &x);

ostream &operator<<(ostream &os, const complex &c);
//自定义类型作为参数或者返回值最好传递引用(但根据具体情况而定)
class complex
{
public:
    complex(double r = 0, double i = 0) : re(r), im(i) {}
    // 列表初始化效率比在函数体内初始化要高
    // complex(){} 若已经有构造函数的全部参数都指明了默认值 那么再写这种默认构造函数就是错误的
    complex &operator+=(const complex &);
    double real() const { return re; }
    // 当一个成员函数不会修改成员数据时要加const(习惯) 因为可能要建立一个常量对象
    double imag() const { return im; }
    //在类内部实现定义的成员函数会展开为inline
    // 但实际上编译器会根据函数的复杂度决定最终要不要inline
    // 在测试中发现显示inline编译不会通过
private:
    double re, im;
    friend complex &_doapl(complex *, const complex &); //同一个类的多个实例互为友元
    // 成员中没有指针的类一般不需要析构
};

#endif