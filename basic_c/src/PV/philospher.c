#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define COUNT 5

sem_t *chopsticks[COUNT];
int philosphers[COUNT] = {0, 1, 2, 3, 4};
pthread_t tids[COUNT];

pthread_mutex_t lkcnt = PTHREAD_MUTEX_INITIALIZER;
int count = 0;

pthread_mutex_t get = PTHREAD_MUTEX_INITIALIZER;

void *only4(void *arg) {
  int pos = *(int *)arg;
  while (1) {

    pthread_mutex_lock(&lkcnt);
    if (count < 4) {
      ++count;
      if (sem_wait(chopsticks[pos]) < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        break;
      }
      if (sem_wait(chopsticks[(pos + 1) % COUNT]) < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        break;
      }
      pthread_mutex_unlock(&lkcnt);
    } else {
      pthread_mutex_unlock(&lkcnt);
      continue;
    }

    printf("NO.%d philospher eat\n", pos);
    if (sem_post(chopsticks[pos]) < 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }
    if (sem_post(chopsticks[(pos + 1) % COUNT]) < 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }
    pthread_mutex_lock(&lkcnt);
    --count;
    pthread_mutex_unlock(&lkcnt);
    sleep(1);
  }

  return (void *)0;
}

void *oddeven(void *arg) {
  int pos = *(int *)arg;
  while (1) {
    if (pos % 2 == 0) {
      if (sem_wait(chopsticks[pos]) < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        break;
      }
      if (sem_wait(chopsticks[(pos + 1) % COUNT]) < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        break;
      }
    } else {
      if (sem_wait(chopsticks[(pos + 1) % COUNT]) < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        break;
      }
      if (sem_wait(chopsticks[pos]) < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        break;
      }
    }

    printf("NO.%d philospher eat\n", pos);

    if (pos % 2 == 0) {
      if (sem_post(chopsticks[pos]) < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        break;
      }
      if (sem_post(chopsticks[(pos + 1) % COUNT]) < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        break;
      }
    } else {
      if (sem_post(chopsticks[(pos + 1) % COUNT]) < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        break;
      }
      if (sem_post(chopsticks[pos]) < 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        break;
      }
    }

    sleep(1);
  }

  return (void *)0;
}

void *mutexget(void *arg) {
  int pos = *(int *)arg;
  while (1) {

    pthread_mutex_lock(&get);
    if (sem_wait(chopsticks[pos]) < 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }
    if (sem_wait(chopsticks[(pos + 1) % COUNT]) < 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }
    pthread_mutex_unlock(&get);

    printf("NO.%d philospher eat\n", pos);

    if (sem_post(chopsticks[pos]) < 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }
    if (sem_post(chopsticks[(pos + 1) % COUNT]) < 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }

    sleep(1);
  }

  return (void *)0;
}

const char *name[COUNT] = {"0", "1", "2", "3", "4"};

int main() {
  for (int i = 0; i < COUNT; ++i)
    chopsticks[i] = sem_open(name[i], O_CREAT | O_EXCL, S_IRWXU, 1);

  for (int i = 0; i < COUNT; ++i)
    sem_unlink(name[i]);

  for (int i = 0; i < COUNT; ++i)
    // pthread_create(&tids[i], NULL, only4, &philosphers[i]);
    // pthread_create(&tids[i], NULL, oddeven, (void *)&(philosphers[i]));
    pthread_create(&tids[i], NULL, mutexget, (void *)&(philosphers[i]));

  sleep(15);
  return 0;
}