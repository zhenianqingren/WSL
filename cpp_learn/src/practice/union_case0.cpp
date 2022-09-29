#include <iostream>
union UNION
{
    struct
    {
        int x;
        int y;
        int z;
    } u;
    int k;
} a;

int main(int argc, char const *argv[])
{
    a.u.x = 4;
    a.u.y = 5;
    a.u.z = 6;
    a.k = 0;
    std::cout << a.u.x << "  " << a.u.y << "  " << a.u.z << "  " << a.k << std::endl;
    //以最大成员所占大小为空间
    //空间用完后 后面的值将重新覆盖掉前面的值
    return 0;
}
// 默认访问控制符为 public
// 可以含有构造函数、析构函数
// 不能含有引用类型的成员
// 不能继承自其他类，不能作为基类
// 不能含有虚函数
// 匿名 union 在定义所在作用域可直接访问 union 成员
// 匿名 union 不能包含 protected 成员或 private 成员
// 全局匿名联合必须是静态（static）的
