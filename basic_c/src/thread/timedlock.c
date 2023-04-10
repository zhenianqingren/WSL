#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// 当线程试图获取一个已加锁的互斥量时，pthread_mutex_timedlock互斥量原语允许绑定线程阻塞时间。
// 超时，pthread_mutex_timedlock不会对互斥量进行加锁，而是返回错误码ETIMEDOUT

int main() {
  int err;
  struct timespec tout;
  struct tm *tmp;

  char buf[64];
  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

  pthread_mutex_lock(&lock);
  write(STDOUT_FILENO, "mutex is locked!\n", 32);
  clock_gettime(CLOCK_REALTIME, &tout);

  tmp = localtime(&tout.tv_sec);
  sprintf(buf, "current time is \0");
  strftime(buf + strlen(buf), 64, "%r", tmp);
  write(STDOUT_FILENO, buf, 128);
  write(STDOUT_FILENO, "\n", 1);

  tout.tv_sec += 10;
  err = pthread_mutex_timedlock(&lock, &tout);
  clock_gettime(CLOCK_REALTIME, &tout);
  tmp = localtime(&tout.tv_sec);
  sprintf(buf, "current time is \0");
  strftime(buf + strlen(buf), 64, "%r", tmp);
  write(STDOUT_FILENO, buf, 128);
  write(STDOUT_FILENO, "\n", 1);

  if (err == 0)
    write(STDOUT_FILENO, "mutex locked again!\n", 32);
  else
    printf("can't lock mutex again: %s\n", strerror(err));
  return 0;
}