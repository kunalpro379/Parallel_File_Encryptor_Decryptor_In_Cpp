#ifndef OPENSSLCRYPTION_HPP
#define OPENSSLCRYPTION_HPP

#include <vector>
#include <string>

class OpenSSL_Cryption {
public:
    static std::vector<unsigned char> encrypt(const std::string &data, const std::string &key);
    static std::string decrypt(const std::vector<unsigned char> &ciphertext, const std::string &key);
};
#endif