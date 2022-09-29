#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define SIZE 512

int main()
{
    char *str = (char *)malloc(sizeof(char) * SIZE);
    double d = 3.14159;
    char output[] = "1.2345";

    float res = atof(output);
    printf("%f\n", res);
    sprintf(str, "Pai is %f", d);
    printf("%s\n", str);
    exit(0);
}