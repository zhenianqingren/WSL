long mult2(long, long);

void multstore(long x, long y, long *dst)
{
    long t = mult2(x, y);
    *dst = t;
}

// gcc -Og -S multstore.c
// gcc -Og -c multstore.c
// objdump -d multstore.o

