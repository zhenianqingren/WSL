#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <unistd.h>
#include <errno.h>

using std::string;

char *value()
{
    char *res = "abcd";
    printf("%s\n", res);
    return res;
}

int main()
{
    string str = "abcdefg";

    const char *ptr = str.c_str();
    string another = ptr;
    printf("%s\n", another.c_str());
    unsigned long pos1 = (unsigned long)ptr;
    unsigned long pos2 = (unsigned long)another.c_str();
    printf("%lu\n", pos1);
    printf("%lu\n", pos2);
    string num1 = "123456789";
    string num2 = "3.1415926";
    long show1 = atol(num1.c_str());
    double show2 = atof(num2.c_str());
    printf("%ld \n", show1);
    printf("%f \n", show2);

    char *heap_v = value();
    printf("%s\n", heap_v);
    // free(heap_v);

    char tt[1024];
    const char *ss = "123456789";
    strcpy(tt, ss);

    printf("%d\t%s\n", strlen(tt), tt);
    alarm(3);
    while (1)
        printf("%d\n", errno);
    return 0;
}