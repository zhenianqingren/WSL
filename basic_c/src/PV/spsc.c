#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#define BUFFSIZE 8

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *producer(void *arg) {
  sem_t *full = ((sem_t **)arg)[0];
  sem_t *empty = ((sem_t **)arg)[1];
  while (1) {
    if (sem_wait(empty) != 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }

    pthread_mutex_lock(&lock); // can omit
    printf("producer produces product\n");
    pthread_mutex_unlock(&lock);

    if (sem_post(full) != 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }
    sleep(1);
  }
  pthread_exit((void *)(0));
}

void *consumer(void *arg) {
  sem_t *full = ((sem_t **)arg)[0];
  sem_t *empty = ((sem_t **)arg)[1];

  while (1) {
    if (sem_wait(full) != 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }

    pthread_mutex_lock(&lock); // can omit
    printf("consumer consumes product\n");
    sleep(1);
    pthread_mutex_unlock(&lock);

    if (sem_post(empty) != 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }
    sleep(1);
  }
  pthread_exit((void *)(0));
}

int main() {
  pthread_t prod;
  pthread_t cons;
  sem_t *sem_set[2];
  sem_set[0] = sem_open("full", O_CREAT | O_EXCL, S_IRWXU, 0);
  // mutex with producer
  sem_set[1] = sem_open("empty", O_CREAT | O_EXCL, S_IRWXU, BUFFSIZE);
  // mutex with consumer

  sem_unlink("full");
  sem_unlink("empty");

  // protect the buff
  //   in fact, for single p/c, doesn't need it

  pthread_create(&prod, NULL, producer, (void *)sem_set);
  pthread_create(&cons, NULL, consumer, (void *)sem_set);

  void *res;
  pthread_join(prod, &res);
  pthread_join(cons, &res);
  return 0;
}