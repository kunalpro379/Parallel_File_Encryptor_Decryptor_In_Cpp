#include <iostream>
#include <cstddef> 
#include <filesystem>
#include "processes/ProcessManagement.hpp"
#include "EncryptDecrypt/cryption.hpp"
#include <algorithm> 

namespace fs = std::filesystem;
using namespace std;

int main(int argc, char *argv[])
{
    ProcessManagement processManagement;
    string directory;
    string action;

    cout << "Enter the directory path: ";
    getline(cin, directory);
    cout << "Checking directory: " << directory << endl;

    cout << "Enter the action to perform (Encrypt/Decrypt): ";
    getline(cin, action);
    cout << "Action entered: " << action << endl;

    // Convert action to lowercase for case-insensitive comparison
    std::transform(action.begin(), action.end(), action.begin(), ::tolower);
    cout << "Converted action to lowercase: " << action << endl;

    try {
        // Check if the directory exists and is indeed a directory
        if (fs::exists(directory) && fs::is_directory(directory)) {
            cout << "Valid directory path!" << endl;
            // ProcessManagement processManagement;

            // Iterate over all entries in the directory
            for (const auto& entry : fs::recursive_directory_iterator(directory)) {
                cout << "Found entry: " << entry.path().string() << endl; // Debugging output
                if (entry.is_regular_file()) {
                    std::string filePath = entry.path().string();
                    cout << "Processing file: " << filePath << endl;

                    IO io(filePath);
                    std::fstream f_stream = std::move(io.getFileStream());

                    if (f_stream.is_open()) {
                        cout << "File opened successfully: " << filePath << endl;
                        Action taskAction = (action == "encrypt") ? Action::ENCRYPT : Action::DECRYPT;
                        cout << "Task action determined: " << (taskAction == Action::ENCRYPT ? "Encrypt" : "Decrypt") << endl;

                        auto task = std::make_unique<Task>(std::move(f_stream), taskAction, filePath);
                        processManagement.submitToQueue(std::move(task));
                        cout << "Task submitted to queue for: " << filePath << endl;
                    } else {
                        cout << "Unable to open file: " << filePath << endl;
                    }
                } else {
                    cout << "Entry is not a regular file: " << entry.path().string() << endl;
                }
            }

            // Execute all tasks in the queue
            cout << "Executing tasks..." << endl;
            processManagement.executeTasks();
            cout << "Tasks execution completed." << endl;
        } else {
            cout << "Invalid directory path!" << endl;
        }
    } catch (const fs::filesystem_error& ex) {
        cout << "Filesystem error: " << ex.what() << endl;
    }

    return 0;
}