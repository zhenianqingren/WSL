#include <iostream>
#include <math.h>
#include <iomanip>
using namespace std;

int main(int argc, char const *argv[])
{
    cout << "default bool values: " << true << "  "
         << false << "\nalpha bool values: " << boolalpha << true << " " << false << endl;
    cout << true << endl;
    cout << noboolalpha << true << endl;
    cout << "default:  " << 20 << "  " << 1024 << endl;
    cout << "octal: " << oct << 20 << " " << 1024 << endl;
    cout << "hex: " << hex << 20 << " " << 1024 << endl;
    cout << "decimal: " << dec << 20 << " " << 1024 << endl;
    //这些操纵符不只是对当前输出有影响 若不取消还会影响后续输出
    cout << uppercase << showbase << hex << 20 << " " << 1024 << nouppercase << noshowbase << dec << endl;

    cout << "Precision: " << cout.precision() << "  Value: " << sqrt(2.0) << endl;
    cout.precision(12);
    //设置精度
    cout << "Precision: " << cout.precision() << "  Value: " << sqrt(2.0) << endl;
    cout << setprecision(3); // iomanip
    cout << "Precision: " << cout.precision() << "  Value: " << sqrt(2.0) << endl;
    //均指有效数字位数

    cout << "default format: " << 100 * sqrt(2.0) << "\n"
         << "Scientific: " << scientific << 100 * sqrt(2.0) << "\n"
         << "Fixed: " << fixed << 100 * sqrt(2.0)
         << "\n"
         << "Hexfloat: " << hexfloat << 100 * sqrt(2.0) << defaultfloat << endl;
    //指的是小数点后的有效位数

    cout << 10.00 << " " << showpoint << 10.00 << noshowpoint << endl;
    //强制打印小数点

    cout << setprecision(8);
    int i = -16;
    double d = 3.14159;
    //补白第一列 使用输出中最小12个位置
    cout << "i: " << setw(12) << i << "next col"
         << "\n"
         << "d: " << d << setw(12) << "next col\n";
    //补白第一列 左对齐所有列
    cout << left << "i: " << setw(12) << i << "next col\n"
         << "d: " << setw(12) << d << "next col\n"
         << right; //恢复正常对齐
    //补白第一列 右对齐所有列
    cout << right << "i: " << setw(12) << i << "next col\n"
         << "d: " << setw(12) << d << "next col\n";
    //补白第一列 但补在域的内部
    cout << internal << "i: " << setw(12) << i << "next col\n"
         << "d: " << setw(12) << d << "next col\n";
    //补白第一列 用#作为补充字符
    cout << setfill('#') << "i: " << setw(12) << i << "next col\n"
         << "d: " << setw(12) << d << "next col\n"
         << setfill(' ') << endl;
    //恢复正常补充字符
    return 0;
}
