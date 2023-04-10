#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// int pthread_equal(pthread tid1 , pthread tid2) compare two threads
// pthread_t pthread_self(void) return the thread itself

// int pthread_create(pthread_t*restrict tidp ,
//                      const pthread_attr_t *restrict attr
//                         void *(*start_rtn)(void*),void *restrict arg)

pthread_t ntid;

void showtids(const char *s) {
  pid_t pid;
  pthread_t tid;

  pid = getpid();
  tid = pthread_self();

  printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid,
         (unsigned long)tid, (unsigned long)tid);
}

void *thread_fn(void *arg) {
  showtids("new thread: ");
  return ((void *)0);
}

int main(int argc, const char *argv[]) {
  int err;
  pid_t pid;

  if ((pid = fork()) < 0)
    fprintf(stderr, "fork error\n");
  else if (pid == 0) {
    printf("child process:\n");
    if ((err = pthread_create(&ntid, NULL, thread_fn, NULL)) != 0)
      fprintf(stderr, "create thread failed\n");
    showtids("main thread: ");
    sleep(2);
  } else {
    if (wait(NULL) < 0)
      fprintf(stderr, "child process wait error\n");

    printf("parent process:\n");
    if ((err = pthread_create(&ntid, NULL, thread_fn, NULL)) != 0)
      fprintf(stderr, "create thread failed\n");
    showtids("main thread: ");
    sleep(2);
  }

  
  exit(0);
}