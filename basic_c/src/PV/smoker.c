#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

sem_t *desk;
sem_t *offer[3];

void *smoker(void *arg) {
  sem_t *slfoff = (sem_t *)arg;
  while (1) {
    if (sem_wait(slfoff) != 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }
    printf("%lu smoke\n", pthread_self());
    if (sem_post(desk) != 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }
    sleep(1);
  }
  return (void *)0;
}

void *supplier(void *arg) {
  while (1) {
    for (int i = 0; i < 3; ++i) {
      if (sem_wait(desk) != 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        break;
      }
      printf("supply %d\n", i);
      if (sem_post(offer[i]) != 0) {
        fprintf(stderr, "error: %s\n", strerror(errno));
        break;
      }
    }
  }
  return (void *)0;
}

int main() {
  offer[0] = sem_open("0", O_CREAT | O_EXCL, S_IRWXU, 0);
  offer[1] = sem_open("1", O_CREAT | O_EXCL, S_IRWXU, 0);
  offer[2] = sem_open("2", O_CREAT | O_EXCL, S_IRWXU, 0);

  desk = sem_open("desk", O_CREAT | O_EXCL, S_IRWXU, 1);
  sem_unlink("0");
  sem_unlink("1");
  sem_unlink("2");
  sem_unlink("desk");

  pthread_t tid[3];
  for (int i = 0; i < 3; ++i)
    pthread_create(&tid[i], NULL, smoker, (void *)offer[i]);

  pthread_t stid;
  pthread_create(&stid, NULL, supplier, NULL);

  void *res;
  for (int i = 0; i < 3; ++i)
    pthread_join(tid[0], &res);
  return 0;
}