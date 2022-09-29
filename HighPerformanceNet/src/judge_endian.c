#include <stdio.h>

void _byte_order()
{
    union
    {
        short value;
        char union_bytes[sizeof(short)];
    } test;
    test.value = 0x0102;
    if ((test.union_bytes[0] == 1) && (test.union_bytes[1] == 2))
        printf("big endian\n");
    else if ((test.union_bytes[0] == 2) && (test.union_bytes[1] == 1))
        printf("little endian\n");
    else
        printf("unknown...\n");
}

int main()
{
    _byte_order();
    return 0;
}
// 现代PC大多采用小端字节序 因此小端字节序又被称为主机字节序
// 格式化的数据在不同字节序的机器之间传输必然出错
// 因此每次发送方都将发送的数据转化为大端格式 接收方根据自身字节序进行转换
// 因此大端字节序又被称为网络字节序
// 四个函数完成转换
// htonl htons ntohl ntohs