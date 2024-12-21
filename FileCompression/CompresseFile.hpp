#include<iostream>
#ifndef FILE_COMPRESSION_HPP
#define FILE_COMPRESSION_HPP
#include <string>
#include <vector>
#include <stdexcept>

class FileCompression {
public:
    static std::vector<char> compress(const std::string& data);
    static std::string decompress(const std::vector<char>& data);
};
#endif