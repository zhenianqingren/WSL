#ifndef MINHEAP
#define MINHEAP

#include <iostream>
#include <netinet/in.h>
#include <time.h>
using std::exception;

#define BUF 64

class heap_timer;

struct client_dat
{
    sockaddr_in address;
    int sockfd;
    char buf[BUF];
    timer *_timer;
};

class timer
{
public:
    time_t expire;
    void (*cb_func)(client_dat *);
    client_dat *usr_dat;

    timer(int delay) : expire(time(NULL) + delay){};
};

class heap
{
private:
    timer **array;
    int capacity;
    int size;

    void sink(int hole)
    {
        timer *tmp = array[hole];
        int child;
        for (; hole * 2 + 1 < size; hole = child)
        {
            child = hole * 2 + 1;
            if ((child < size - 1) && (array[child]->expire < array[child + 1]->expire))
                ++child;
            if (array[child]->expire > array[hole]->expire)
                break;
            array[hole] = array[child];
        }
        array[hole] = tmp;
    }

    void resize()
    {
        timer **tmp = new timer *[2 * capacity];
        if (!tmp)
            throw std::exception();
        for (int i = 0; i < 2 * capacity; ++i)
            tmp[i] = NULL;
        capacity *= 2;
        for (int i = 0; i < size; ++i)
            tmp[i] = array[i];
        delete[] array;
        array = tmp;
    }

public:
    heap(int _capacity) throw(std::exception) : capacity(_capacity), size(0)
    {
        array = new timer *[capacity];
        if (!array)
            throw std::exception();
        for (int i = 0; i < capacity; ++i)
            array[i] = NULL;
    }
    heap(timer **_array, int _size, int _capacity) throw(std::exception) : capacity(_capacity), size(_size)
    {
        if (capacity < size)
            throw std::exception();
        array = new timer *[capacity];
        if (!array)
            throw std::exception();
        if (size != 0)
        {
            for (int i = 0; i < size; ++i)
                array[i] = _array[i];
            for (int i = size / 2 - 1; i >= 0; --i)
                sink(i);
        }
    }
    ~heap()
    {
        for (int i = 0; i < size; ++i)
            delete array[i];
        delete[] array;
    }

    bool empty() const
    {
        return size == 0;
    }

    void add_timer(timer *_timer) throw(std::exception)
    {
        if (!_timer)
            return;
        if (size >= capacity)
            resize();
        int hole = size++;
        int parent = 0;
        for (; hole > 0; hole = parent)
        {
            parent = (hole - 1) >> 1;
            if (array[parent]->expire <= _timer->expire)
                break;
            array[hole] = array[parent];
        }
        array[hole] = _timer;
    }

    void del_timer(timer *_timer)
    {
        if (!_timer)
            return;
        _timer->cb_func = NULL;
    }

    timer *top() const
    {
        if (empty())
            return NULL;
        return array[0];
    }

    void pop_timer()
    {
        if (empty())
            return;
        if (array[0])
        {
            delete array[0];
            array[0] = array[--size];
            sink(0);
        }
    }

    void tick()
    {
        timer *tmp = array[0];
        time_t cur = time(NULL);
        while (!empty())
        {
            if (!tmp)
                break;
            if (tmp->expire > cur)
                break;
            if (array[0]->cb_func)
                array[0]->cb_func(array[0]->usr_dat);
            pop_timer();
            tmp = array[0];
        }
    }
};

#endif