#include <windows.h>
#undef byte
#include "../FileCompression/CompresseFile.hpp"
#include "Cryption.hpp"
#include "../processes/Task.hpp"
#include "../file_handling/ReadEnv.cpp"
#include <ctime>
#include <iomanip>
#include <iostream>
#include "../OpenSSL/OpenSSLCryption.hpp"


int executionCryption(const std::string &taskData)
{
    Task task = Task::fromString(taskData); // Deserialize task data
    ReadEnv env;
    std::string envKey = env.getenv();
    std::cout << "Environment Key: " << envKey << std::endl; // Debugging output

    int key =123;// Convert environment key to integer
    FileCompression fileCompression;
    if (task.action == Action::ENCRYPT)
    {
        // Read the entire file content
        std::string data((std::istreambuf_iterator<char>(task.f_stream)), std::istreambuf_iterator<char>());
        
        // Compress the data
std::cout << "Original size: " << data.size() << " bytes" << std::endl;
std::vector<char> compressedData = fileCompression.compress(data);
std::cout << "Compressed size: " << compressedData.size() << " bytes" << std::endl;
        // Encrypt the compressed data

        /*
         for (char &ch : compressedData)
        {
            ch = (ch + key) % 256; // Encrypt character
        }
        
        */
       
        std::vector<unsigned char> encryptedData = OpenSSL_Cryption::encrypt(data, envKey);

        // Write back the encrypted data
        task.f_stream.seekp(0);
        task.f_stream.write((char *)encryptedData.data(), encryptedData.size());
  
    }
    else // Decrypt
    {
        // Read the entire file content
        std::vector<char> encryptedData((std::istreambuf_iterator<char>(task.f_stream)), std::istreambuf_iterator<char>());
/*
        // Decrypt the data
        for (char &ch : encryptedData)
        {
            ch = (ch - key + 256) % 256; // Decrypt character
        }
*/

 // Read the entire file content

        // Decrypt the data
        std::string decryptedData = OpenSSL_Cryption::decrypt(std::vector<unsigned char>(encryptedData.begin(), encryptedData.end()), envKey);

        // Decompress the decrypted data
        std::string decompressedData = fileCompression.decompress(std::vector<char>(decryptedData.begin(), decryptedData.end()));

        // Decompress the decrypted data
        //std::string decompressedData = fileCompression.decompress(encryptedData);

        // Decrypt the data
        // std::string decryptedData = OpenSSL_Cryption::decrypt(decompressedData, envKey);

        // Write back the decrypted data
        task.f_stream.seekp(0);
        task.f_stream.write(decryptedData.data(), decryptedData.size());
    }



    /*
    FileCompression fileCompression; 

    char ch;
    while (task.f_stream.get(ch))
    {
        if (task.action == Action::ENCRYPT)
        {
            ch = (ch + key) % 256; // Encrypt character
        }
        else // Decrypt
        {
            ch = (ch - key + 256) % 256; // Decrypt character
        }
        task.f_stream.seekp(-1, std::ios::cur); // Move back 1 position
        task.f_stream.put(ch);
    }

*/
    auto t = std::time(nullptr);
    struct tm now;
    localtime_s(&now, &t);
    std::cout << "Exiting the encryption/decryption at: "
              << std::put_time(&now, "%Y-%m-%d %H:%M:%S") << std::endl;

    return 0;
}

