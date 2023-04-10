#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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

int main() {

  long arr[9];

  for (int i = 0; i < 9; ++i)
    arr[i] = 9 - i;

  heapsort((void *)arr, 9, sizeof(long), cmpl);

  for (int i = 0; i < 9; ++i)
    printf("%ld ", arr[i]);
  printf("\n");

  return 0;
}