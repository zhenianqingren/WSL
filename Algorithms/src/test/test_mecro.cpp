#include "mecro.cpp"
#include <iostream>
using namespace std;
#define M(T) mecro<T> *
template <typename T>
static M(T) func(T data)
{
    M(T)
    res = new mecro<T>();
    res->data = data;
    return res;
}
int main(int argc, char const *argv[])
{
    cout << func(5) << endl;
    return 0;
}
