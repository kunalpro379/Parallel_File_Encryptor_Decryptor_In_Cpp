#include <windows.h>
#undef byte

#include "Cryption.hpp"
#include "../processes/Task.hpp"
#include "../file_handling/ReadEnv.cpp"
#include <ctime>
#include <iomanip>
#include <iostream>

int executionCryption(const std::string &taskData)
{
    Task task = Task::fromString(taskData); // Deserialize task data
    ReadEnv env;
    std::string envKey = env.getenv();
    std::cout << "Environment Key: " << envKey << std::endl; // Debugging output

    int key =123;// Convert environment key to integer
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

    auto t = std::time(nullptr);
    struct tm now;
    localtime_s(&now, &t);
    std::cout << "Exiting the encryption/decryption at: "
              << std::put_time(&now, "%Y-%m-%d %H:%M:%S") << std::endl;

    return 0;
}
