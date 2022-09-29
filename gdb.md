1## 以下命令后括号内为命令的简化使用，比如run（r），直接输入命令 r 就代表命令run
 2
 3$(gdb)help(h)        # 查看命令帮助，具体命令查询在gdb中输入help + 命令 
 4
 5$(gdb)run(r)         # 重新开始运行文件（run-text：加载文本文件，run-bin：加载二进制文件）
 6
 7$(gdb)start          # 单步执行，运行程序，停在第一行执行语句
 8
 9$(gdb)list(l)        # 查看原代码（list-n,从第n行开始查看代码。list+ 函数名：查看具体函数）
10
11$(gdb)set            # 设置变量的值
12
13$(gdb)next(n)        # 单步调试（逐过程，函数直接执行）
14
15$(gdb)step(s)        # 单步调试（逐语句：跳入自定义函数内部执行）
16
17$(gdb)backtrace(bt)  # 查看函数的调用的栈帧和层级关系
18
19$(gdb)frame(f)       # 切换函数的栈帧
20
21$(gdb)info(i)        # 查看函数内部局部变量的数值
22
23$(gdb)finish         # 结束当前函数，返回到函数调用点
24
25$(gdb)continue(c)    # 继续运行
26
27$(gdb)print(p)       # 打印值及地址
28
29$(gdb)quit(q)        # 退出gdb
30
31$(gdb)break+num(b)                 # 在第num行设置断点
32
33$(gdb)info breakpoints             # 查看当前设置的所有断点
34
35$(gdb)delete breakpoints num(d)    # 删除第num个断点
36
37$(gdb)display                      # 追踪查看具体变量值
38
39$(gdb)undisplay                    # 取消追踪观察变量
40
41$(gdb)watch                        # 被设置观察点的变量发生修改时，打印显示
42
43$(gdb)i watch                      # 显示观察点
44
45$(gdb)enable breakpoints           # 启用断点
46
47$(gdb)disable breakpoints          # 禁用断点
48
49$(gdb)x                            # 查看内存x/20xw 显示20个单元，16进制，4字节每单元
50
51$(gdb)run argv[1] argv[2]          # 调试时命令行传参
52
53$(gdb)set follow-fork-mode child   # Makefile项目管理：选择跟踪父子进程（fork()）
54$(gdb)layout next #切换到下一个布局模式
55$(gdb)layout prev #切换到上一个布局模式
56$(gdb)layout src #只显示源码
57$(gdb)layout asm #只显示汇编代码
58$(gdb)layout split #显示汇编代码和源代码
