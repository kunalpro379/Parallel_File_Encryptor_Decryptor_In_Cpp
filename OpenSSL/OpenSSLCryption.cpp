#include <openssl/evp.h>
#include <openssl/aes.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <ctime>
#include "OpenSSLCryption.hpp"
#include "../processes/Task.hpp"
#include "../file_handling/ReadEnv.cpp"
using namespace std;

std::vector<unsigned char> OpenSSL_Cryption::encrypt(const std::string &data, const std::string &key) {
        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        std::vector<unsigned char> ciphertext(data.size() + AES_BLOCK_SIZE);
        int len;

        EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char *)key.c_str(), NULL);
        EVP_EncryptUpdate(ctx, ciphertext.data(), &len, (unsigned char *)data.c_str(), data.size());
        int ciphertext_len = len;
        EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len);
        ciphertext_len += len;

        EVP_CIPHER_CTX_free(ctx);
        ciphertext.resize(ciphertext_len);
        return ciphertext;
    }

std::string OpenSSL_Cryption::decrypt(const std::vector<unsigned char> &ciphertext, const std::string &key) {
        EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
        std::vector<unsigned char> plaintext(ciphertext.size());
        int len;

        EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (unsigned char *)key.c_str(), NULL);
        EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size());
        int plaintext_len = len;
        EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len);
        plaintext_len += len;

        EVP_CIPHER_CTX_free(ctx);
        plaintext.resize(plaintext_len);
        return std::string(plaintext.begin(), plaintext.end());
    }


int executionCryption(const std::string &taskData) {
    Task task = Task::fromString(taskData); // Deserialize task data
    ReadEnv env;
    std::string envKey = env.getenv();
    std::cout << "Environment Key: " << envKey << std::endl; // Debugging output

    if (task.action == Action::ENCRYPT) {
        // Read the entire file content
        std::string data((std::istreambuf_iterator<char>(task.f_stream)), std::istreambuf_iterator<char>());
        
        // Encrypt the data
        std::vector<unsigned char> encryptedData = OpenSSL_Cryption::encrypt(data, envKey);

        // Write back the encrypted data
        task.f_stream.seekp(0);
        task.f_stream.write((char *)encryptedData.data(), encryptedData.size());
    } else { // Decrypt
        // Read the entire file content
        std::vector<unsigned char> encryptedData((std::istreambuf_iterator<char>(task.f_stream)), std::istreambuf_iterator<char>());

        // Decrypt the data
        std::string decryptedData = OpenSSL_Cryption::decrypt(encryptedData, envKey);

        // Write back the decrypted data
        task.f_stream.seekp(0);
        task.f_stream.write(decryptedData.data(), decryptedData.size());
    }

    auto t = std::time(nullptr);
    struct tm now;
    localtime_s(&now, &t);
    std::cout << "Exiting the encryption/decryption at: "
              << std::put_time(&now, "%Y-%m-%d %H:%M:%S") << std::endl;

    return 0;
}
