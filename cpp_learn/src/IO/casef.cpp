#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main(int argc, char const *argv[])
{
    ofstream output("/home/mice/cpp_learn/src/IO/file/test1");
    ifstream input("/home/mice/cpp_learn/src/IO/file/source");
    ofstream output_("/home/mice/cpp_learn/src/IO/file/test2");
    ifstream input_("/home/mice/cpp_learn/src/IO/file/source");
    string temp;
    string another;
    vector<string> strs;
    if (input.is_open())
        cout << "input_open\n";
    if (output.is_open())
        cout << "output_open\n";
    while (input >> temp)
    {
        strs.push_back(temp);
        output << temp;
    }
    for (auto &e : strs)
        cout << e << endl;
    while (getline(input_, another))
        output_ << another << endl;

    return 0;
}
