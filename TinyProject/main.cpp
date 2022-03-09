#include "Person.h"
#include "Army.h"
#include "Politician.h"
#include "Business.h"
#include "Call.cpp"
#include "ShowInfo.cpp"

int main(int argc, char const *argv[])
{

    Person *res = new Politician();
    showinfo(res);
    delete res;
    res = new Army();
    showinfo(res);
    delete res;
    res = new Business();
    showinfo(res);
    delete res;
    res = nullptr;

    return 0;
}
