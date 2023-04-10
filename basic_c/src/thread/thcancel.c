#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

// int pthread_cancel(pthread_t tid)
// 在默认情况下 pthread_cancel函数会使得由tid标识的线程行为表现如同调用
// 了参数为PTHREAD_CANCELED的pthread_exit函数，但是线程可以选择忽略被取消
// 或者选择如何被取消

// void pthread_cleanup_push(void (*rtn)(void *),void *arg)
// void pthread_cleanup_pop(int execute)
// the two function just like atexit()

// when thread behave like following , rtn is dispatched by pthread_cleanup_push
// with only one arg
// 1. call pthread_exit
// 2. respond to cancel request
// 3. call pthread_cleanup_pop with no zero execute
// but if execute is set to zero , no matter which conditions
// rtn won't be called


// in default condition , the exit status will be remained until
// pthread_join is called. if thread is detached, pthread_join can't
// catch the exit status, because undefined behavior will occur
// pthread_detach is used to detach one thread
void clean(void *arg)
{
    printf("clean up: %s\n", (char *)arg);
}

void *th_fn1(void *arg)
{
    printf("thread1 start\n");
    pthread_cleanup_push(clean, "thread1 first handler");
    pthread_cleanup_push(clean, "thread1 second handler");
    printf("thread1 cleanup push completed\n");
    if (arg)
        return ((void *)1);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    return ((void *)1);
}

void *th_fn2(void *arg)
{
    printf("thread2 start\n");
    pthread_cleanup_push(clean, "thread2 first handler");
    pthread_cleanup_push(clean, "thread2 second handler");
    printf("thread2 cleanup push completed\n");
    if (arg)
        pthread_exit((void *)2);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    pthread_exit((void *)2);
}

int main()
{
    int err;
    pthread_t tid1, tid2;
    void *tret;
    if ((err = pthread_create(&tid1, NULL, th_fn1, (void *)1)) != 0)
        fprintf(stderr, "thread1 create error\n");
    if ((err = pthread_create(&tid2, NULL, th_fn2, (void *)2)) != 0)
        fprintf(stderr, "thread2 create error\n");

    if ((err = pthread_join(tid1, &tret)) != 0)
        fprintf(stderr, "thread1 join error\n");
    printf("thread1 exit code: %ld\n", (long)tret);

    if ((err = pthread_join(tid2, &tret)) != 0)
        fprintf(stderr, "thread2 join error\n");
    printf("thread2 exit code: %ld\n", (long)tret);
    exit(0);
}