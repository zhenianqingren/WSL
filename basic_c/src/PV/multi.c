#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void *father(void *sem_set) {
  sem_t *apple = ((sem_t **)sem_set)[0];
  sem_t *plate = ((sem_t **)sem_set)[2];

  while (1) {
    if (sem_wait(plate) != 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }
    printf("father put an apple\n");
    if (sem_post(apple) != 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }
    sleep(1);
  }

  return (void *)0;
}

void *mother(void *sem_set) {

  sem_t *orange = ((sem_t **)sem_set)[1];
  sem_t *plate = ((sem_t **)sem_set)[2];

  while (1) {
    if (sem_wait(plate) != 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }
    printf("mother put an orange\n");
    if (sem_post(orange) != 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }
    sleep(1);
  }

  return (void *)0;
}

void *son(void *sem_set) {
  sem_t *apple = ((sem_t **)sem_set)[0];
  sem_t *plate = ((sem_t **)sem_set)[2];

  while (1) {
    if (sem_wait(apple) != 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }
    printf("son get an apple\n");
    if (sem_post(plate) != 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }
    sleep(1);
  }

  return (void *)0;
}

void *daughter(void *sem_set) {
  sem_t *orange = ((sem_t **)sem_set)[1];
  sem_t *plate = ((sem_t **)sem_set)[2];

  while (1) {
    if (sem_wait(orange) != 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }
    printf("daughter get an orange\n");
    if (sem_post(plate) != 0) {
      fprintf(stderr, "error: %s\n", strerror(errno));
      break;
    }
    sleep(1);
  }

  return (void *)0;
}

int main() {

  sem_t *sem_set[3];
  sem_set[0] = sem_open("apple", O_CREAT | O_EXCL, S_IRWXU, 0);
  sem_set[1] = sem_open("orange", O_CREAT | O_EXCL, S_IRWXU, 0);
  sem_set[2] = sem_open("plate", O_CREAT | O_EXCL, S_IRWXU, 1);

  sem_unlink("apple");
  sem_unlink("orange");
  sem_unlink("plate");

  // 此处不需要mutex来保护互斥访问
  // 但是缓冲区大小大于1时如何？
  // 此时就需要mutex以免覆盖数据的问题
  // 判断需不需要mutex，关键是看生产者内部或消费者内部会不会有数据覆盖问题

  //我们应该先同步再互斥以避免死锁

  pthread_t tid[4];
  pthread_create(&tid[0], NULL, father, (void *)sem_set);
  pthread_create(&tid[1], NULL, mother, (void *)sem_set);
  pthread_create(&tid[2], NULL, son, (void *)sem_set);
  pthread_create(&tid[3], NULL, daughter, (void *)sem_set);

  void *res;
  for (int i = 0; i < 4; ++i)
    pthread_join(tid[i], &res);

  return 0;
}