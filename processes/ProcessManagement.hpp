#ifndef PROCESS_MANAGEMENT_HPP
#define PROCESS_MANAGEMENT_HPP
#include <windows.h>
#undef byte
#include <iostream>
#include "Task.hpp"
#include <thread>
#include <vector>
#include <queue>
#include <memory>
#include <semaphore>
#include <cstddef>
#include <atomic>
#include <mutex>
using namespace std;

class ProcessManagement
{
    // sem_t *itemsSemaphore;
    // sem_t *emptySemaph;
    HANDLE itemsSemaphore;
    HANDLE emptySemaph;

public:
    ProcessManagement();
    ~ProcessManagement(); // Free up shared mem;
    bool submitToQueue(unique_ptr<Task> task);
    void executeTasks();
    void executeTasksWithThreading();

private:
    // queue<unique_ptr<Task>> taskQueue;
    // shared memory
    struct SharedMemory
    {
        atomic<int> size;
        char tasks[1000][256];
        int front;
        int rear;
        void printSharedMemory()
        {
            cout << size << endl;
        }
    };

    struct SharedMemory *sharedMem;
    HANDLE shmFd;
    const char *SHM_NAME = "/MyQueue";

    mutex QueuLock;
};

#endif // PROCESS_MANAGEMENT_HPP