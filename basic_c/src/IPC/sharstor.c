// 共享存储允许两个或多个进程共享一个给定的存储区。

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>

#define ARRAY_SIZE 40000
#define MALLOC_SIZE 100000
#define SHM_SIZE 100000
#define SHM_MODE 0600 /*user read/write*/

char array[ARRAY_SIZE];

int main() {
  int shmid;
  char *ptr, *shmptr;

  printf("array from %p to %p\n", (void *)&array[0],
         (void *)&array[ARRAY_SIZE]);

  printf("stack around %p\n", (void *)&shmid);

  if ((ptr = malloc(MALLOC_SIZE)) == NULL)
    fprintf(stderr, "malloc error\n");
  printf("malloced from %p to %p\n", (char *)ptr, (char *)ptr + MALLOC_SIZE);
  if ((shmid = shmget(IPC_PRIVATE, SHM_SIZE, SHM_MODE)) < 0)
    fprintf(stderr, "shmget error\n");

  if ((shmptr = shmat(shmid, 0, 0)) == (void *)-1)
    fprintf(stderr, "shmat error\n");

  printf("shared memory attached from %p to %p\n", (char *)shmptr,
         (char *)shmptr + MALLOC_SIZE);

  if (shmctl(shmid, IPC_RMID, 0) < 0)
    fprintf(stderr, "shmctl error\n");
  return 0;
}