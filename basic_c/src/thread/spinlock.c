#include <pthread.h>
#include <sched.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

// 自旋锁不是通过休眠使进程阻塞，而是在获取锁之前一直处于忙等
// 适用于锁被持有时间短，线程不希望在重新调度上花费太多成本

const char msg[] = "critical resource\n";

void *work(void *arg)
{
  int err;
  pthread_spinlock_t *slock = (pthread_spinlock_t *)arg;
  if ((err = pthread_spin_lock(slock)) != 0)
  {
    printf("spin lock obtain fail\n");
    pthread_exit((void *)-1);
  }
  sleep(2);
  printf("thread : %lu access : %s\n", pthread_self(), msg);

  pthread_spin_unlock(slock);
  pthread_exit((void *)0);
}

int main()
{
  pthread_spinlock_t slock;
  int err;

  if (pthread_spin_init(&slock, PTHREAD_PROCESS_PRIVATE) != 0)
  {
    printf("error init spin lock: %s\n", strerror(err));
    return -1;
  }

  pthread_t tid[3];
  for (int i = 0; i < 3; ++i)
  {
    if ((err = pthread_create(&tid[i], NULL, work, (void *)&slock)) != 0)
    {
      printf("error create thread\n");
      goto err;
    }
  }

  void *rtp;
err:

  for (int i = 0; i < 3; ++i)
  {
    if ((err = pthread_join(tid[i], &rtp)) != 0)
      printf("error join thread : %s\n", strerror(err));
  }

  if (pthread_spin_destroy(&slock) != 0)
  {
    printf("error destroy spin lock: %s\n", strerror(err));
    return -1;
  }
  return 0;
}