﻿

---

# 🛡️ Parallel File Encryptor/Decryptor in C++

This project is a C++ application designed to efficiently encrypt and decrypt files in parallel. By utilizing multithreading, shared memory, and task queues, the system ensures high performance and scalability. The encryption and decryption processes are secured using **AES encryption** from OpenSSL, making this a robust solution for secure file processing. 🚀

---
---



Here is the architectural diagram for better understanding:
![image](https://github.com/user-attachments/assets/b61b52be-143e-4cbb-914a-c1a74bbe7869)


---
## 📌 Features

- **🔄 Parallel Processing**: Encrypts and decrypts files concurrently using multiple threads.
- **🔒 Advanced Encryption**: Implements AES encryption/decryption via OpenSSL.
- **📂 Shared Memory**: Enables inter-process communication with efficient shared memory usage.
- **📋 Task Queue**: Tasks (encrypt/decrypt operations) are managed in a circular buffer queue.
- **⚙️ Semaphore-Based Synchronization**: Ensures safe access to shared resources across processes.
- **🔧 Scalability**: Leverages thread pooling to optimize resource usage.

---

## 🛠️ System Overview

The application architecture is designed around key operating system concepts like shared memory and semaphores. 

### **How It Works**

1. **File Input**: The user provides files for encryption or decryption.
2. **Task Submission**: Tasks are submitted to a shared memory-based task queue.
3. **Task Queue Processing**: A circular buffer holds the tasks for efficient memory use.
4. **Thread Pool Execution**: Worker threads pick tasks and process them using OpenSSL for encryption/decryption.
5. **Synchronization**: Semaphores manage task access, ensuring no race conditions occur.

---

## 🧩 Core Concepts and Implementation

### **1. Shared Memory**

#### 📖 **Concept**
Shared memory is a region of memory accessible by multiple processes. It provides efficient inter-process communication (IPC) by avoiding the overhead of message passing.

#### 🛠️ **Implementation**
The project uses Windows APIs like `CreateFileMapping` and `MapViewOfFile` to create and manage shared memory. The shared memory holds the **circular buffer queue** of tasks.

```cpp
shmFd = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(SharedMemory), SHM_NAME);
sharedMem = static_cast<SharedMemory *>(MapViewOfFile(shmFd, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(SharedMemory)));
sharedMem->front = 0;
sharedMem->rear = 0;
sharedMem->size.store(0);
```

---

### **2. Semaphores**

#### 📖 **Concept**
Semaphores are synchronization primitives that control access to shared resources by multiple threads or processes. They prevent race conditions by allowing only a limited number of processes to access a resource simultaneously.

#### 🛠️ **Implementation**
Two semaphores are used:
- `itemsSemaphore`: Tracks the number of available tasks in the queue.
- `emptySemaphore`: Tracks available slots in the queue.

```cpp
itemsSemaphore = CreateSemaphore(NULL, 0, 1000, "items_semaphore");
emptySemaphore = CreateSemaphore(NULL, 1000, 1000, "empty_semaphore");
```

---

### **3. Task Queue**

#### 📖 **Concept**
A **task queue** organizes and stores tasks in a **first-in, first-out (FIFO)** order. In this project, tasks represent encryption or decryption operations.

#### 🛠️ **Implementation**
The task queue is implemented as a **circular buffer** in shared memory. Tasks are added to the rear and removed from the front of the queue. Atomic operations are used to maintain thread safety.

```cpp
bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task) {
    WaitForSingleObject(emptySemaphore, INFINITE);
    std::unique_lock<std::mutex> lock(QueueLock);

    if (sharedMem->size.load() == 1000) {
        return false;
    }

    strcpy(sharedMem->tasks[sharedMem->rear], task->toString().c_str());
    sharedMem->rear = (sharedMem->rear + 1) % 1000;
    sharedMem->size.fetch_add(1);

    lock.unlock();
    ReleaseSemaphore(itemsSemaphore, 1, NULL);
    return true;
}
```

---

### **4. Thread Pool**

#### 📖 **Concept**
A **thread pool** is a collection of pre-instantiated, reusable threads. It avoids the overhead of creating and destroying threads for each task, thereby improving efficiency.

#### 🛠️ **Implementation**
The thread pool is initialized based on the **hardware concurrency** (number of CPU cores). Tasks are assigned to threads in the pool for concurrent processing.

```cpp
ThreadPool threadPool(std::thread::hardware_concurrency());
```

---

### **5. Encryption/Decryption**

#### 📖 **Concept**
The project uses **AES encryption** from OpenSSL for secure file processing.

#### 🛠️ **Implementation**
Encryption and decryption are performed using OpenSSL’s library functions. Files are read and written in chunks to handle large file sizes efficiently.

---

## 🖥️ Prerequisites

To build and run this project, you need the following:

- **OpenSSL**: Ensure OpenSSL is installed and linked properly.
- **C++17**: A C++17 compatible compiler is required.
- **Windows OS**: The project uses Windows-specific APIs for shared memory and synchronization.

---

## ⚙️ Build and Run

Follow these steps to build and run the project:

1. Install the necessary libraries:
   - **OpenSSL**: Install via a package manager like [vcpkg](https://vcpkg.io/).
   - Ensure OpenSSL headers and libraries are linked in your environment.

2. Compile the code:
   ```bash
   g++ -std=c++17 -o ParallelFileEncryptor.exe \
   main.cpp \
   processes/ProcessManagement.cpp \
   EncryptDecrypt/cryption.cpp \
   file_handling/IO.cpp \
   file_handling/ReadEnv.cpp \
   FileCompression/CompresseFile.cpp \
   MultiThreading/ThreadPool.cpp \
   OpenSSL/OpenSSLCryption.cpp \
   -I./processes -I./EncryptDecrypt -I./file_handling -I./FileCompression -I./OpenSSL \
   -I"C:/vcpkg/installed/x64-windows/include" \
   -L"C:/vcpkg/installed/x64-windows/lib" -lssl -lcrypto -lstdc++fs -lz
   ```

3. Run the executable:
   ```bash
   ./ParallelFileEncryptor.exe
   ```

---

## 🚀 Conclusion

This project demonstrates the integration of multithreading, shared memory, semaphores, and advanced encryption in a single application. It serves as a practical example of secure and efficient file processing using C++.

Feel free to contribute, raise issues, or suggest improvements! ✨



Enjoy using the **Parallel File Encryptor/Decryptor**! 🔐✨
