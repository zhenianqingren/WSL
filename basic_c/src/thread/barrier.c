// 屏障是用户协调多个线程并行工作的同步机制
// 屏障允许每个线程等待，直到所有的合作线程都到达某一点，然后从该点继续执行。
// 允许任意数量的线程等待，直到所有的线程完成工作，而线程不需要退出。
// 所有线程到达屏障后继续工作。

#include <limits.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define NTHR 8               /*number of threads*/
#define NUMNUM 8000000L      /*number of numbers to sort*/
#define TNUM (NUMNUM / NTHR) /*number to sort per thread*/

#define TRAV(p1, p2, count)                                                    \
  {                                                                            \
    for (int i = 0; i < count; ++i) {                                          \
      *p1 = (*p1) ^ (*p2);                                                     \
      *p2 = (*p1) ^ (*p2);                                                     \
      *p1 = (*p1) ^ (*p2);                                                     \
      ++p1;                                                                    \
      ++p2;                                                                    \
    }                                                                          \
  }

#define MOVEBYTES(px, py, count)                                               \
  {                                                                            \
    char *p1 = (char *)(px);                                                   \
    char *p2 = (char *)(py);                                                   \
    for (int i = 0; i < count; ++i) {                                          \
      *p1 = *p2;                                                               \
      ++p1;                                                                    \
      ++p2;                                                                    \
    }                                                                          \
  }

#define SWAP(px, py, count)                                                    \
  {                                                                            \
    char *p1 = (char *)(px);                                                   \
    char *p2 = (char *)(py);                                                   \
    TRAV(p1, p2, count)                                                        \
  }

#define OFFSET(ptr, offset) ((void *)((char *)ptr + sizeof(void *) * (offset)))

long nums[NUMNUM];
long snums[NUMNUM];

pthread_barrier_t b;

int cmpl(const void *arg1, const void *arg2) {
  long l1 = *(long *)arg1;
  long l2 = *(long *)arg2;
  return l1 > l2 ? 1 : 0;
}

void heapify(void *seq, size_t root, size_t esiz, size_t end,
             int (*cmp)(const void *, const void *)) {
  void *tmp = malloc(esiz);
  MOVEBYTES(tmp, OFFSET(seq, root), esiz);
  while ((root << 1) + 1 < end) {
    size_t lchild = (root << 1) + 1;
    size_t grt = lchild + 1 < end &&
                         cmpl(OFFSET(seq, lchild + 1), OFFSET(seq, lchild)) == 1
                     ? lchild + 1
                     : lchild;
    if (cmpl(tmp, OFFSET(seq, grt)))
      break;
    MOVEBYTES(OFFSET(seq, root), OFFSET(seq, grt), esiz);
    root = grt;
  }
  MOVEBYTES(OFFSET(seq, root), tmp, esiz);
  free(tmp);
}

void heapsort(void *seq, size_t siz, size_t esiz,
              int (*cmp)(const void *, const void *)) {

  for (size_t k = (siz >> 1) - 1; k < siz; --k)
    heapify(seq, k, esiz, siz, cmp);

  for (size_t i = 1; i < siz; ++i) {
    SWAP(seq, OFFSET(seq, siz - i), esiz);
    heapify(seq, 0, esiz, siz - i, cmp);
  }
}

void *thfn(void *arg) {
  long idx = (long)arg;
  heapsort((void *)&nums[idx], TNUM, sizeof(long), cmpl);
  pthread_barrier_wait(&b);
  return (void *)0;
}

void merge() {
  long idx[NTHR];
  long i, minidx, sidx, num;
  for (i = 0; i < NTHR; ++i)
    idx[i] = i * TNUM;
  for (sidx = 0; sidx < NUMNUM; ++sidx) {
    num = LONG_MAX;
    for (i = 0; i < NTHR; ++i) {
      if ((idx[i] < (i + 1) * TNUM) && (nums[idx[i]] < num)) {
        num = nums[idx[i]];
        minidx = i;
      }
    }
    snums[sidx] = nums[idx[minidx]];
    ++idx[minidx];
  }
}

int main() {
  unsigned long i;
  struct timeval start, end;
  long long startusec, endusec;
  double elapsed;
  int err;
  pthread_t tid;

  /*Create the initial set of numbers to sort*/
  srand(1);

  for (i = 0; i < NUMNUM; ++i)
    nums[i] = rand();

  /*Create 8 threads to sort the numbers*/
  gettimeofday(&start, NULL);
  pthread_barrier_init(
      &b, NULL, NTHR + 1); // why +1? because the main thread is accounted
  for (i = 0; i < NTHR; ++i) {
    if ((err = pthread_create(&tid, NULL, thfn, (void *)(i * TNUM))) != 0)
      printf("thread create fail\n");
  }

  pthread_barrier_wait(&b);
  merge();
  gettimeofday(&end, NULL);

  /*print the sorted list*/
  startusec = start.tv_sec * 1000000 + start.tv_usec;
  endusec = end.tv_sec * 1000000 + end.tv_usec;
  elapsed = (double)(endusec - startusec) / 1000000.0;
  printf("sort took %.4f seconds\n", elapsed);
  // for (i = 0; i < NUMNUM; ++i)
  //   printf("%ld\n", snums[i]);

  return 0;
}