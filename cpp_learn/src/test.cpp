#include <stdio.h>

static int p = 3;

int &val()
{
    int x = 10;
    return x;
}

int &_val()
{
    return p;
}

int main(int argc, char const *argv[])
{
    // int a = val();
    // printf("%d\n", a);
    // int &b = val();
    // ++b;
    // printf("%d\n", b);
    int f = _val();
    ++f;
    printf("%d\t%d\n", f, p);
    int &g = _val();
    ++g;
    printf("%d\t%d\n", g, p);
    return 0;
}
