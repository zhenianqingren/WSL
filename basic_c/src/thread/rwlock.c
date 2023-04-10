// 读写锁
// 三种状态: 读模式下加锁状态 写模式下加锁状态 不加锁状态
// 只有一个进程可以占用写模式锁
// 多个线程可以同时占有读模式锁

// 读写锁处于写加锁状态，被解锁前所有加锁进程会被阻塞
// 读写锁处于读加锁状态，被解锁前所有加写锁进程会被阻塞
// 读写锁处于读加锁状态，如果有一个线程试图写加锁，其后所有读加锁线程会被阻塞

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct job
{
  struct job *next;
  struct job *prev;
  pthread_t id; // tell which thread handles this job
};

struct job *jcreate(pthread_t pid)
{
  struct job *j = (struct job *)malloc(sizeof(struct job));
  j->next = NULL;
  j->prev = NULL;
  j->id = pid;
  return j;
};

struct queue
{
  struct job *head;
  struct job *tail;
  pthread_rwlock_t q_lock;
};

/*initialize a queue*/
int qinit(struct queue *q)
{
  q->head = NULL;
  q->tail = NULL;
  int err = pthread_rwlock_init(&q->q_lock, NULL);
  if (err != 0)
  {
    printf("error when init lock : %s\n", strerror(err));
    return -1;
  }

  else
    return 0;
}

/*queue push*/
void job_push(struct queue *q, struct job *j)
{
  if (j == NULL || q == NULL)
    return;

  pthread_rwlock_wrlock(&q->q_lock);
  if (q->head == NULL)
  {
    j->prev = j;
    j->next = j;
    q->head = j;
    q->tail = j;
  }
  else
  {
    j->prev = q->tail;
    j->next = q->head;
    q->tail->next = j;
    q->head->prev = j;
  }
  pthread_rwlock_unlock(&q->q_lock);
}

/*queue pop*/
void job_pop(struct queue *q)
{
  if (q == NULL || q->head == NULL)
    return;

  pthread_rwlock_wrlock(&q->q_lock);
  struct job *tmp = q->head;

  if (tmp->next != tmp)
  {
    tmp->next->prev = tmp->prev;
    tmp->prev->next = tmp->next;
    q->head = tmp->next;
  }
  else
  {
    q->head = NULL;
    q->tail = NULL;
  }
  free(tmp);
  pthread_rwlock_unlock(&q->q_lock);
}

/*find a job for given id*/

int main()
{

  return 0;
}
