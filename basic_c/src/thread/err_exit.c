#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

struct foo
{
    int a;
    int b;
    int c;
    int d;
};

void pfoo(const char *s, const struct foo *fp)
{
    printf("%s\n", s);
    printf("structure at 0x%lx\n", (unsigned long)fp);
    printf("foo.a = %d\n", fp->a);
    printf("foo.b = %d\n", fp->b);
    printf("foo.c = %d\n", fp->c);
    printf("foo.d = %d\n", fp->d);
}

void *th_fn1(void *arg)
{
    struct foo obj1 = {1, 2, 3, 4};
    pfoo("thread1:", &obj1);
    pthread_exit((void *)&obj1);
}

void *th_fn2(void *arg)
{
    printf("thread2 with ID: %lu\n", (unsigned long)pthread_self());
    pthread_exit((void *)0);
}

int main()
{
    int err;
    pthread_t tid1, tid2;
    struct foo *fp;

    if ((err = pthread_create(&tid1, NULL, th_fn1, NULL)) != 0)
        fprintf(stderr, "thread1 create error\n");
    if ((err = pthread_join(tid1, (void **)&fp)) != 0)
        fprintf(stderr, "thread1 join error\n");

    sleep(2);
    printf("second thread\n");
    if ((err = pthread_create(&tid2, NULL, th_fn2, NULL)) != 0)
        fprintf(stderr, "thread2 create error\n");
    sleep(2);
    pfoo("parent:", fp);
    exit(0);
}