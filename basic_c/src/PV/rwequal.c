#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

sem_t *rw;     // mutex access to file
int count = 0; // count reader, only first need to wait
pthread_mutex_t lkcnt = PTHREAD_MUTEX_INITIALIZER;

sem_t *w; // ensure equality

void *reader(void *arg) {

  while (1) {
    if (sem_wait(w) < 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }
    pthread_mutex_lock(&lkcnt);
    if (count == 0) {
      if (sem_wait(rw) < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        break;
      }
    }
    ++count;
    pthread_mutex_unlock(&lkcnt);

    if (sem_post(w) < 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }

    printf("%lu reader access to file\n", pthread_self());
    pthread_mutex_lock(&lkcnt);
    --count;
    if (count == 0) {
      // if readers arrive consecutively, it's not equal to writers, count won't
      // be zero
      if (sem_post(rw) < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        break;
      }
    }
    pthread_mutex_unlock(&lkcnt);
    sleep(1);
  }
  return (void *)0;
}

void *writer(void *arg) {
  while (1) {
    if (sem_wait(w) < 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }
    if (sem_wait(rw) < 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }

    printf("%lu writer access to file\n", pthread_self());

    if (sem_post(rw) < 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }
    if (sem_post(w) < 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }
    sleep(1);
  }
  return (void *)0;
}

int main() {

  pthread_t readers[4];
  pthread_t writers[2];

  rw = sem_open("rw", O_CREAT | O_EXCL, S_IRWXU, 1);
  w = sem_open("w", O_CREAT | O_EXCL, S_IRWXU, 1);

  for (int i = 0; i < 4; ++i)
    pthread_create(&readers[i], NULL, reader, NULL);

  for (int i = 0; i < 2; ++i)
    pthread_create(&writers[i], NULL, writer, NULL);

  sleep(15);
  return 0;
}