#include <stdio.h>

void addvec(int *x, int *y, int *z, int n);
void multvec(int *x, int *y, int *z, int n);

int x[2] = {1, 2};
int y[2] = {3, 4};
int z[2];

int addcnt;
int multcnt;

int main()
{
    addvec(x, y, z, 2);
    printf("z = ( %d , %d )\n", z[0], z[1]);
    multvec(x, y, z, 2);
    printf("z = ( %d , %d )\n", z[0], z[1]);
    printf("addcnt: %d   multcnt: %d\n", addcnt, multcnt);
    return 0;
}

// gcc -c main2.c
// gcc -static -o prog2 main2.o -lvec -L.
// -L. indicates that libvec.a locates on directory .
