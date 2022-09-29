#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
using namespace std;

int main(int argc, char const *argv[])
{
    ifstream ifs("/home/mice/cpp_learn/bin/hello_world", ios::binary);
    ofstream ofs("/home/mice/cpp_learn/bin/hello_world_copy", ios::binary);
    char *buffer = new char[64];
    while (ifs.read(buffer,64))
    {
        ofs.write(buffer,strlen(buffer));
        delete buffer;
        buffer = new char[64];
    }
    ofs.close();
    ifs.close();
    return 0;
}
