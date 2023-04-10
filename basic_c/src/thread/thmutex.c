#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define THREAD_COUNT 32

// mutex variable is expressed as pthread_mutex_t type
// before using it , must initialize it to const PTHREAD_MUTEX_INITIALIZER
// or call pthread_mutex_init (just for static allocate mutex variable)

// if dynamic allocate mutext variable such as call malloc
// before free it you should call pthread_mutex_destroy

// int pthread_mutex_init(pthread_mutex_t *restrict mutex,
//                        const pthread_mutexattr_t *restrict attr)
// int pthread_mutex_destroy(pthread_mutex_t *mutex)
// to initialize mutext defaultly , just set attr to NULL

// int pthread_mutex_lock(pthread_mutex_t *mutex)
// int pthread_mutex_trylock(pthread_mutex_t *mutex)
// int pthread_mutext_unlock(pthread_mutex_t *mutex)

// if thread won't expect to be blocked , call *trylock to lock mutex
// if it is unlocked when call this , trylock will lock the mutext and
// won't be locked , return 0. otherwise fail , return EBUSY

struct foo
{
    int f_count;
    pthread_mutex_t f_lock;
    int f_id;
};

void *fooalloc(void *id)
{
    struct foo *fp;
    if ((fp = malloc(sizeof(struct foo))) != NULL)
    {
        fp->f_count = 1;
        fp->f_id = (int)id;
        if (pthread_mutex_init(&fp->f_lock, NULL) != 0)
        {
            free(fp);
            return NULL;
        }
    }

    return (void *)fp;
}

void *foo_hold(void *fp)
{
    pthread_mutex_lock(&((struct foo *)fp)->f_lock);
    ++((struct foo *)fp)->f_count;
    pthread_mutex_unlock(&((struct foo *)fp)->f_lock);
    return ((void *)pthread_self());
}

void *foo_rele(void *fp)
{
    pthread_mutex_lock(&((struct foo *)fp)->f_lock);
    if (--((struct foo *)fp)->f_count == 0)
    {
        pthread_mutex_unlock(&((struct foo *)fp)->f_lock);
        pthread_mutex_destroy(&((struct foo *)fp)->f_lock);
        free(fp);
        printf("END\n");
    }
    else
        pthread_mutex_unlock(&((struct foo *)fp)->f_lock);

    return ((void *)pthread_self());
}

int main(int argc, const char *argv[])
{
    int err;
    pthread_t cre_tid, com_tid;
    pthread_t th_arr[THREAD_COUNT + 1];

    if ((err = pthread_create(&cre_tid, NULL, fooalloc, (void *)0)) != 0)
        fprintf(stderr, "thread first create error\n");

    void *tret, *fp;

    if ((err = pthread_join(cre_tid, &fp)) != 0)
        fprintf(stderr, "thread first join error\n");

    printf("thread: %lu initial foo\n", (unsigned long)cre_tid);
    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        if ((err = pthread_create(&com_tid, NULL, foo_hold, fp)) != 0)
            fprintf(stderr, "thread%d create error\n", i);
        th_arr[i] = com_tid;
    }

    for (int i = 0; i < THREAD_COUNT; ++i)
    {
        if ((err = pthread_join(th_arr[i], &tret)) != 0)
            fprintf(stderr, "thread%d join error\n", i);

        printf("thread: %lu join  current fp reference:%d\n", (unsigned long)tret, ((struct foo *)fp)->f_count);
    }

    for (int i = 0; i < THREAD_COUNT + 1; ++i)
    {
        if ((err = pthread_create(&com_tid, NULL, foo_rele, fp)) != 0)
            fprintf(stderr, "thread%d create error\n", i);

        th_arr[i] = com_tid;
    }

    for (int i = 0; i < THREAD_COUNT + 1; ++i)
    {
        if ((err = pthread_join(th_arr[i], &tret)) != 0)
            fprintf(stderr, "thread%d join error\n", i);

        printf("thread: %lu join  current fp reference:%d\n", (unsigned long)tret, ((struct foo *)fp)->f_count);
    }
    exit(0);
}