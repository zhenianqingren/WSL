// 为了最佳的可移植性，决不应预先假定系统支持全双工管道
// 管道只能在具有公共祖先的两个进程之间使用
// fd[2] fd[0]为读而打开 fd[1]为写而打开 fd[1]的输出是fd[0]的输入

// fstat函数对管道的每一端都返回一个FIFO类型的文件描述符

// 当管道的一端被关闭，下面两条规则起作用
// 1. 读一个写端被关闭的管道时，所有数据都被读取后，read返回0。表示文件结束
// 2.
// 当写一个读端已经被关闭的管道，产生信号SIGPIPE，如果忽略该信号或者捕捉该信号并且从其处理
// 程序返回，则write返回-1，errno设置为EPIPE。

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
  int n;
  int fd[2];
  pid_t pid;
  char line[512];
  if (pipe(fd) < 0)
    fprintf(stderr, "pipe error\n");

  if ((pid = fork()) < 0)
    fprintf(stderr, "fork error\n");
  else if (pid > 0) {
    close(fd[0]);
    write(fd[1], "hello world\n", 12);
  } else {
    close(fd[1]);
    n = read(fd[0], line, 512);
    write(STDOUT_FILENO, line, n);
  }
  return 0;
}