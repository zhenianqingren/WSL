#include <stdio.h>

void multstore(long, long, long *);

int main()
{
    long d;
    multstore(2, 3, &d);
    printf("2 * 3 --> %ld\n", d);
    return 0;
}

long mult2(long a, long b)
{
    long s = a * b;
    return s;
}

// 生成实际可执行的代码需要对一组目标文件代码运行链接器
// 而这一组目标文件代码中必须要有main函数

// gcc -Og -o prog main.c multstore.c

// 所有以'.'开头的行都是指导汇编器和链接器工作的伪指令


// C程序和汇编代码结合
// 编写完整函数放入独立汇编代码文件中，让汇编器和链接器把它和用C语言书写的代码结合
// 用asm伪指令