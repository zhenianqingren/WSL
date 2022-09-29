#include "../include/complex.h"

// complex &_doapl(complex *ths, const complex &r)
// {
//     ths->re += r.re;
//     ths->im += r.im;
//     return *ths;
// }

complex &complex::operator+=(const complex &r)
{
    return _doapl(this, r);
}

double imag(const complex &c)
{
    return c.imag();
}
double real(const complex &c)
{
    return c.real();
}
// 此处不用引用 返回时局部变量会被销毁
complex operator+(const complex &x, const complex &y)
{
    return complex(real(x) + real(y), imag(x) + imag(y)); //临时对象在下一行就会被销毁
}

complex operator+(const complex &c, double d)
{
    return complex(real(c) + d, imag(c));
}

complex operator+(double d, const complex &c)
{
    return complex(real(c) + d, imag(c));
}

complex &operator+(complex &c)
{
    return c;
}

complex operator-(const complex &c)
{
    return complex(-real(c), -imag(c));
}

bool operator==(const complex &x, const complex &y)
{
    return real(x) == real(y) && imag(x) == imag(y);
}

bool operator==(const complex &c, double d)
{
    return real(c) == d && imag(c) == 0;
}

bool operator==(double d, const complex &c)
{
    return real(c) == d && imag(c) == 0;
}

bool operator!=(const complex &x, const complex &y)
{
    return real(x) != real(y) || imag(x) != imag(y);
}

bool operator!=(const complex &c, double d)
{
    return real(c) != d || imag(c) != 0;
}

bool operator!=(double d, const complex &c)
{
    return real(c) != d || imag(c) != 0;
}

complex conj(const complex &c)
{
    return complex(real(c), -imag(c));
}

ostream &operator<<(ostream &os, const complex &c)
{
    return os << "( " << real(c) << " , " << imag(c) << " )";
}
