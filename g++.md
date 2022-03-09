预处理-Pre-Processing               //.i文件

1# -E 选项指示编译器仅对输入文件进行预处理
2g++  -E  test.cpp  -o  test.i    //.i文件


编译-Compiling                        // .s文件

1# -S 编译选项告诉 g++ 在为 C++ 代码产生了汇编语言文件后停止编译
2#  g++ 产生的汇编语言文件的缺省扩展名是 .s 
3g++  -S  test.i  -o   test.s


汇编-Assembling                   // .o文件

1# -c 选项告诉 g++ 仅把源代码编译为机器语言的目标代码
2# 缺省时 g++ 建立的目标代码文件有一个 .o 的扩展名。
3g++  -c  test.s  -o  test.o


链接-Linking                       // bin文件

1# -o 编译选项来为将产生的可执行文件用指定的文件名
2g++  test.o  -o  test
3.2 g++重要编译参数
-g     编译带调试信息的可执行文件

1# -g 选项告诉 GCC 产生能被 GNU 调试器GDB使用的调试信息，以调试程序。
2
3# 产生带调试信息的可执行文件test
4g++ -g test.cpp


-O[n]    优化源代码

 1## 所谓优化，例如省略掉代码中从未使用过的变量、直接将常量表达式用结果值代替等等，这些操作会缩减目标文件所包含的代码量，提高最终生成的可执行文件的运行效率。
 2
 3# -O 选项告诉 g++ 对源代码进行基本优化。这些优化在大多数情况下都会使程序执行的更快。 -O2 选项告诉 g++ 产生尽可能小和尽可能快的代码。 如-O2，-O3，-On（n 常为0–3）
 4# -O 同时减小代码的长度和执行时间，其效果等价于-O1
 5# -O0 表示不做优化
 6# -O1 为默认优化
 7# -O2 除了完成-O1的优化之外，还进行一些额外的调整工作，如指令调整等。
 8# -O3 则包括循环展开和其他一些与处理特性相关的优化工作。
 9# 选项将使编译的速度比使用 -O 时慢， 但通常产生的代码执行速度会更快。
10
11# 使用 -O2优化源代码，并输出可执行文件
12g++ -O2 test.cpp


-l  和  -L     指定库文件  |  指定库文件路径

 1# -l参数(小写)就是用来指定程序要链接的库，-l参数紧接着就是库名
 2# 在/lib和/usr/lib和/usr/local/lib里的库直接用-l参数就能链接
 3
 4# 链接glog库
 5g++ -lglog test.cpp
 6
 7# 如果库文件没放在上面三个目录里，需要使用-L参数(大写)指定库文件所在目录
 8# -L参数跟着的是库文件所在的目录名
 9
10# 链接mytest库，libmytest.so在/home/bing/mytestlibfolder目录下
11g++ -L/home/bing/mytestlibfolder -lmytest test.cpp


-I    指定头文件搜索目录

1# -I 
2# /usr/include目录一般是不用指定的，gcc知道去那里找，但 是如果头文件不在/usr/icnclude里我们就要用-I参数指定了，比如头文件放在/myinclude目录里，那编译命令行就要加上-I/myinclude 参数了，如果不加你会得到一个”xxxx.h: No such file or directory”的错误。-I参数可以用相对路径，比如头文件在当前 目录，可以用-I.来指定。上面我们提到的–cflags参数就是用来生成-I参数的。
3
4g++ -I/myinclude test.cpp


-Wall    打印警告信息

1# 打印出gcc提供的警告信息
2g++ -Wall test.cpp


-w    关闭警告信息

1# 关闭所有警告信息
2g++ -w test.cpp


-std=c++11    设置编译标准

1# 使用 c++11 标准编译 test.cpp
2g++ -std=c++11 test.cpp


-o     指定输出文件名

1# 指定即将产生的文件名
2
3# 指定输出可执行文件名为test
4g++ test.cpp -o test


-D     定义宏

1# 在使用gcc/g++编译的时候定义宏
2
3# 常用场景：
4# -DDEBUG 定义DEBUG宏，可能文件中有DEBUG宏部分的相关信息，用个DDEBUG来选择开启或关闭DEBUG
示例代码：

 1// -Dname 定义宏name,默认定义内容为字符串“1”
 2
 3#include <stdio.h>
 4
 5int main()
 6{
 7    #ifdef DEBUG
 8        printf("DEBUG LOG\n");
 9    #endif
10        printf("in\n");
11}
12
13// 1. 在编译的时候，使用gcc -DDEBUG main.cpp
14// 2. 第七行代码可以被执行
