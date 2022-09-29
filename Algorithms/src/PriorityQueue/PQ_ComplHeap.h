#include "./PQ.h"
#include <vector>

using rank = int;
using std::vector;

template <typename T>
class PQ_ComplHeap : public PQ<T>
{
protected:
    vector<T> elem;

    rank percolateDown(rank i, rank n); //下滤
    rank percolateUp(rank);             //上滤
    void heapify(rank);                 // Floyd建堆?
public:
    PQ_ComplHeap(T *A, rank n);
    void insert(T &);
    T max() { return elem[0]; };
    T delmax();
};

template <typename T>
PQ_ComplHeap<T>::PQ_ComplHeap(T *A, rank n)
{
    elem.resize(n);
    rank i;
    for (T *t = A; t != nullptr; ++t, ++i)
        elem[i] = *t;
    heapify(n);
}

template <typename T>
void PQ_ComplHeap<T>::heapify(rank n)
{
    for (rank i = (n - 1) >> 1; i >= 0; --i)
        percolateDown(i, n);
}

template <typename T>
void PQ_ComplHeap<T>::insert(T &e)
{
    elem.emplace_back(e);
    percolateUp(elem.size() - 1);
}

template <typename T>
rank PQ_ComplHeap<T>::percolateUp(rank i)
{
    rank j;
    T temp = std::move(elem[i]);
    while (ParentValid(i))
    {
        j = Parent(i);
        if (temp > elem[j])
            elem[i] = elem[j];
        else
            break;
        i = j;
    }
    elem[i] = temp;
    return i;
}

template <typename T>
T PQ_ComplHeap<T>::delmax()
{
    T maxElem = std::move(elem[0]);
    elem[0] = elem[elem.size() - 1];
    elem.pop_back();

    return maxElem;
}

template <typename T>
rank PQ_ComplHeap<T>::percolateDown(rank i, rank n)
{
    rank j;
    T temp = std::move(elem[i]);
    while ((i << 1) + 1 <= n)
    {
        j = (i << 1) + 2 <= n && elem[(i << 1) + 1] < elem[(i + 1) << 1] ? (i + 1) << 1 : (i << 1) + 1;
        if (temp < elem[j])
            elem[i] = elem[j];
        else
            break;
        i = j;
    }
    elem[i] = temp;
    return i;
}