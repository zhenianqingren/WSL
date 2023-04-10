#include <bits/posix1_lim.h>
#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct lock {
  sem_t *semp;
  char name[_POSIX_NAME_MAX];
};

struct lock *lalloc() {
  struct lock *sp;
  static int cnt;

  if ((sp = malloc(sizeof(struct lock))) == NULL)
    return NULL;

  do {
    snprintf(sp->name, sizeof(sp->name), "/%ld.%d", (long)getpid(), cnt++);
    sp->semp = sem_open(sp->name, O_CREAT | O_EXCL, S_IRWXU, 1);
    /*
    使用一个现有的信号量时，仅指定两个参数：信号量参数名字和第二个oflag参数的0值
    当oflag参数带有O_CREAT标志集，如果命名信号量不存在，则创建新的

    当制定了O_CREAT标志，需要提供两个额外参数。mode指定谁可以访问信号量，
    value指定信号量的初始值，通常为0~SEM_VALUE_MAX

    设置oflag = O_CREAT | O_EXCL，如果信号量已经存在，会导致sem_open失败
    */
  } while ((sp->semp == SEM_FAILED) && (errno == EEXIST));

  if (sp->semp == SEM_FAILED) {
    free(sp);
    return NULL;
  }

  sem_unlink(sp->name); /*删除信号量的名字
                           如果没有打开的信号量引用，则该信号量会被销毁*/

  return sp;
}

void lfree(struct lock *sp) {
  sem_close(sp->semp);
  free(sp);
}

int lock(struct lock *sp) {
  return (sem_wait(sp->semp)); /*信号量计数为0时发生阻塞
                                    直到信号量成功-1或者被信号中断时返回
                                */
}

int trylock(struct lock *sp) { return (sem_trywait(sp->semp)); }

int unlock(struct lock *sp) { return (sem_post(sp->semp)); }
