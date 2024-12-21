#include<windows.h>
#undef byte
#include <iostream>
#include "ProcessManagement.hpp"
#include <condition_variable>
#include <mutex>


// #include <wait.h>
// #include<sys/wait.h>

// #include <sys/wait.h>
// #include <sys/fcntl.h>
#include "../EncryptDecrypt/cryption.hpp"
#include <cstddef> 
#include <thread>
#include <string>
#include <queue>
#include <memory>
#include "Task.hpp"
using namespace std;
// ProcessManagement.cpp
#include "../MultiThreading/ThreadPool.hpp"

// Add a member variable for the thread pool
ThreadPool threadPool(std::thread::hardware_concurrency());

ProcessManagement::ProcessManagement()
{
    // sem_t *itemsSemaphore = sem_open("/items_semaphore", O_CREAT, 0666, 0);
    // sem_t *emptySemaphore = sem_open("/empty_semaphore", O_CREAT, 0666, 1000);
    // Initialize semaphores
    itemsSemaphore = CreateSemaphore(NULL, 0, 1000, "items_semaphore");
    emptySemaph = CreateSemaphore(NULL, 1000, 1000, "empty_semaphore");
    /*
    shmFd = shm_open(SHM_NAME, OF_CREATE, O_RDWR, 0666);
    gtruncate(shmfd, sizeof(SharedMemory));
    sharedMem = static_cast<SharedMemory *>(map(nullptr, sizeof(), PROT_READ | PROT_WRITE, MAP_SHARED, shmFd, 0));
    sharedMem->front = 0;
    sharedMem->rear = 0;
    sharedMem->size.store(d : 0);
    */
//    shmFd = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SharedMemory), SHM_NAME);

// sharedMem = static_cast<SharedMemory *>(MapViewOfFile(shmFd, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedMemory)));    
// sharedMem->front = 0;
//     sharedMem->rear = 0;    
//     shmFd = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SharedMemory), SHM_NAME);

//     sharedMem->size.store(0);
    // Initialize shared memory
    shmFd = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SharedMemory), SHM_NAME);
    sharedMem = static_cast<SharedMemory *>(MapViewOfFile(shmFd, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedMemory)));    
    sharedMem->front = 0;
    sharedMem->rear = 0;
    sharedMem->size.store(0);
  
}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task)
{
    // sem_wait(emptySemaph);
     WaitForSingleObject(emptySemaph, INFINITE);
    std::unique_lock<std::mutex> lock(QueuLock);
    // taskQueue.push(std::move(task));
    if (sharedMem->size.load() == 1000) // tasks queue full
    {
        return false;
    }
    strcpy(sharedMem->tasks[sharedMem->rear], task->toString().c_str());
    sharedMem->rear = (sharedMem->rear + 1) % 1000;             // circular Que
    sharedMem->size.fetch_add(1);

    //Create New Process
    STARTUPINFO si={sizeof(si)};
    PROCESS_INFORMATION pi;
    /* below  wala was code for linux
     int pid = fork();

    lock.unlock();
    sem_post(itemsSemaphore);

    if (pid < 0)
        return false;
    else if (pid > 0)
        cout << "Entering the parent process" << endl;
    else
    {
        cout << "Entering the child process" << endl;
        executeTasks();
        cout << "Exiting the child process" << endl;
    }
    */
//  if (!CreateProcess(NULL,
//                    (LPSTR)"child_process.exe", // Process to execute
//                    NULL,
//                    NULL,
//                    FALSE,
//                    0,
//                    NULL,
//                    NULL,
//                    &si,
//                    &pi))
// {
//     return false;
// }

    lock.unlock();
    ReleaseSemaphore(itemsSemaphore, 1, NULL); // Signal that an item is available

    return true;
}
void ProcessManagement::executeTasks()

{
    while(sharedMem->size.load() > 0) { // Change this line
        WaitForSingleObject(itemsSemaphore, INFINITE); // Wait for items semaphore
        unique_lock<mutex> lock(QueuLock);
        
        char taskStr[256];
        strcpy(taskStr, sharedMem->tasks[sharedMem->front]); // Copy task string
        sharedMem->front = (sharedMem->front + 1) % 1000; // Circular queue
        sharedMem->size.fetch_sub(1); // Decrement task count

        lock.unlock();
        ReleaseSemaphore(emptySemaph, 1, NULL); // Signal that an empty slot is available
        
        std::cout << "Executing task: " << taskStr << std::endl;
        try {
            executionCryption(taskStr);
            std::cout << "Task completed successfully: " << taskStr << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Error executing task: " << e.what() << std::endl;
        }
    }


    /*
        sem_wait(itemsSemaphore);
    unique_lock<mutex> lock(QueueLock);

    char taskStr[256];

    strcpy(askStr, sharedMem->tasks[sharedMem->front]);
    // lock.unlock();
    sharedMem->front = (sharedMem->front + 1) % 1000;
    sharedMem->size.fetch_sub(op : 1);

    lock.unlock();
    sem_post(emptySemaphore);
    
    cout << "Executing CHILD process" << taskStr << endl;
    executeCryptio(taskStr);
    
    */
    // while (!taskQueue.empty())
    // {

    //     unique_ptr<Task> task = move(taskQueue.front());
    //     taskQueue.pop();
    //     cout << "Task Execution-" << taskToExecute->toString() << endl;
    //     executeCryption(taskToExecute->toString());
    // }

}
void ProcessManagement::executeTasksWithThreading() {
    std::cout << "Starting threaded task execution..." << std::endl;
    while (sharedMem->size.load() > 0) {
        WaitForSingleObject(itemsSemaphore, INFINITE);
        std::unique_lock<std::mutex> lock(QueuLock);

        char taskStr[256];
        strcpy(taskStr, sharedMem->tasks[sharedMem->front]);
        sharedMem->front = (sharedMem->front + 1) % 1000;
        sharedMem->size.fetch_sub(1);

        lock.unlock();
        ReleaseSemaphore(emptySemaph, 1, NULL);

        std::cout << "Submitting task to thread pool: " << taskStr << std::endl;
        threadPool.enqueue([taskStr] {
            try {
                executionCryption(taskStr);
                std::cout << "Task completed successfully: " << taskStr << std::endl;
            } catch (const std::exception& e) {
                std::cout << "Error executing task: " << e.what() << std::endl;
            }
        });
    }
    std::cout << "Threaded task execution completed." << std::endl;
}
ProcessManagement::~ProcessManagement() {
    // Clean up resources if necessary
    if (itemsSemaphore) {
        CloseHandle(itemsSemaphore);
    }
    if (emptySemaph) {
        CloseHandle(emptySemaph);
    }
    if (shmFd) {
        CloseHandle(shmFd);
    }
    if (sharedMem) {
        UnmapViewOfFile(sharedMem);
    }
}