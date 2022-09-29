#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
using std::string;

int main()
{
    string str = "abcdefghi";
    char *p = &str[0];

    while (*p != '\0')
    {
        putchar(*p);
        ++p;
    }
    putchar(*p);
    putchar('\n');
    printf("%s\n", &str[0]);
}