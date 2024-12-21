#include <iostream>
#include <fstream>
#include <cstddef> 
#include "IO.hpp"
using namespace std;
IO::IO(const string &file_path)
{
    file_stream.open(file_path, ios::in | ios::out | ios::binary);
    if (!file_stream.is_open())
        cout << "unable to open the file" << file_path << endl;
}
fstream IO::getFileStream()
{
    return move(file_stream);
}
IO::~IO()
{
    if (file_stream.is_open())
        file_stream.close();
}