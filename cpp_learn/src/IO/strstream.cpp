#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char const *argv[])
{
    string line, word;
    ifstream ifs("/home/mice/cpp_learn/src/IO/file/source");
    ostringstream ostr;
    while (getline(ifs, line))
    {
        istringstream istr(line);
        while (istr >> word)
        {
            // cout << word << endl;
            ostr << word << " ";
        }
        ostr << "\n";
    }
    cout << ostr.str() << endl;
    ifs.close();
    return 0;
}
