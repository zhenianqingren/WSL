#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <pthread.h>

class locker
{
private:
    pthread_mutex_t p_mutex;

public:
    locker()
    {
        if (pthread_mutex_init(&p_mutex, NULL) != 0)
            throw std::exception();
    }
    ~locker()
    {
        pthread_mutex_destroy(&p_mutex);
    }
    bool lock()
    {
        return pthread_mutex_lock(&p_mutex) == 0;
    }
    bool unlock()
    {
        return pthread_mutex_unlock(&p_mutex) == 0;
    }
};

class cond
{
private:
    pthread_cond_t p_cond;
    pthread_mutex_t p_mutex;

public:
    cond()
    {
        if (pthread_mutex_init(&p_mutex, NULL) != 0)
            throw std::exception();
        if (pthread_cond_init(&p_cond, NULL) != 0)
        {
            pthread_mutex_destroy(&p_mutex);
            throw std::exception();
        }
    }
    ~cond()
    {
        pthread_mutex_destroy(&p_mutex);
        pthread_cond_destroy(&p_cond);
    }
    bool wait()
    {
        int ret;
        pthread_mutex_lock(&p_mutex);
        ret = pthread_cond_wait(&p_cond, &p_mutex);
        pthread_mutex_unlock(&p_mutex);
        return ret == 0;
    }
    bool signal()
    {
        return pthread_cond_signal(&p_cond) == 0;
    }
};

#endif