#include <pthread.h>
#define MAXLEN 128

struct msg {
  char info[MAXLEN];
  struct msg *next;
};

struct msg *workq;

pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

void process_msg(void) {
  struct msg *mq;
  for (;;) {
    pthread_mutex_lock(&qlock);
    while (workq == NULL)
      pthread_cond_wait(&qready, &qlock);
    mq = workq;
    workq = mq->next;
    pthread_mutex_unlock(&qlock);
  }
}

void enqueue_msg(struct msg *mq) {
  pthread_mutex_lock(&qlock);
  mq->next = workq;
  workq = mq;
  pthread_mutex_unlock(&qlock);
  pthread_cond_signal(&qready);
}