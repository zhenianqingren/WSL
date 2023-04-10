#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

// single thread can exit without the whole process exit
// 1. return from where it starts
// 2. canceled by other threads
// 3. call the pthread_exit

// void pthread_exit(void *rval_ptr)

// int pthread_join(pthread_t thread , void **rval_ptr)
// wait thread untile it exit or be canceled
// if thread normally exit , rval_ptr includes return value
// otherwise , the memory zone pointed by rval_ptr will be set to PTHREAD_CANCELED

void *th_fn1(void *arg)
{
    printf("thread: %lu returning\n", (unsigned long)pthread_self());
    return ((void *)1);
}

void *th_fn2(void *arg)
{
    printf("thread: %lu returning\n", (unsigned long)pthread_self());
    return ((void *)2);
}

int main()
{
    int err;
    pthread_t tid1, tid2;
    void *tret;

    if ((err = pthread_create(&tid1, NULL, th_fn1, NULL)) != 0)
        fprintf(stderr, "create thread1 error\n");
    if ((err = pthread_create(&tid2, NULL, th_fn2, NULL)) != 0)
        fprintf(stderr, "create thread2 error\n");

    if ((err = pthread_join(tid1, &tret)) != 0)
        fprintf(stderr, "join thread1 error\n");

    printf("thread1 exit code: %ld\n", (long)tret);

    if ((err = pthread_join(tid2, &tret)) != 0)
        fprintf(stderr, "join thread2 error\n");

    printf("thread2 exit code: %ld\n", (long)tret);

    exit(0);
}
